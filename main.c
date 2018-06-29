/******************************************************************************
 * (c) Copyright 2012-2017 Xilinx, Inc. All rights reserved.
 *
 * This file contains confidential and proprietary information of Xilinx, Inc.
 * and is protected under U.S. and international copyright and other
 * intellectual property laws.
 *
 * DISCLAIMER
 * This disclaimer is not a license and does not grant any rights to the
 * materials distributed herewith. Except as otherwise provided in a valid
 * license issued to you by Xilinx, and to the maximum extent permitted by
 * applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
 * FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
 * IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
 * and (2) Xilinx shall not be liable (whether in contract or tort, including
 * negligence, or under any other theory of liability) for any loss or damage
 * of any kind or nature related to, arising under or in connection with these
 * materials, including for any direct, or any indirect, special, incidental,
 * or consequential loss or damage (including loss of data, profits, goodwill,
 * or any type of loss or damage suffered as a result of any action brought by
 * a third party) even if such damage or loss was reasonably foreseeable or
 * Xilinx had been advised of the possibility of the same.
 *
 * CRITICAL APPLICATIONS
 * Xilinx products are not designed or intended to be fail-safe, or for use in
 * any application requiring fail-safe performance, such as life-support or
 * safety devices or systems, Class III medical devices, nuclear facilities,
 * applications related to the deployment of airbags, or any other applications
 * that could lead to death, personal injury, or severe property or
 * environmental damage (individually and collectively, "Critical
 * Applications"). Customer assumes the sole risk and liability of any use of
 * Xilinx products in Critical Applications, subject only to applicable laws
 * and regulations governing limitations on product liability.
 *
 * THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
 * AT ALL TIMES.
 *******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <linux/videodev2.h>

#include "platform.h"
#include "video.h"

static struct {
	unsigned int flags;
} vcmd_cleanup_data;

static void vcmd_cleanup(void)
{
	vlib_pipeline_stop();
	if (!(vcmd_cleanup_data.flags & VLIB_CFG_FLAG_MULTI_INSTANCE)) {
		vlib_drm_set_layer0_state(1);
	}
	vlib_uninit();
}

static void vcmd_sig_handler(int signo)
{
	switch (signo) {
	case SIGINT:
	case SIGTERM:
		vcmd_cleanup();
		exit(0);
	default:
		break;
	}
}

static int getInput(void)
{
	int ch;
	int ret = -1;
	ch = getchar();
	if (ferror(stdin)) {
		perror("error reading input stream");
		abort();
	}
	if (ch >= '0' && ch <= '9')
		ret = ch - '0';
	while ((ch = getchar()) != '\n' && ch != EOF);
	return ret;
}

static struct option opts[] = {
	{ "dri-card", required_argument, NULL, 'd' },
	{ NULL, 0, NULL, 0 }
};

static void menu_loop(struct vlib_config *config, struct filter_tbl *ft,
		      unsigned int flags)
{
	int ret, choice = -1;
	struct filter_s *fs = NULL;

	/* Main control menu */
	do {
		struct vlib_config current_config;
		const char *t;
		/* start with menu index 1 since 0 is used for exit */
		size_t i, k, j = 1;

		printf("\nEnter '0' to EXIT : ");

		choice = getInput();

		current_config = *config;

		if (choice == 0) {
			/* exit application */
			vcmd_cleanup();
			exit(0);
		} else {
			printf("\n\n********* Invalid input, Please try Again ***********\n");
			continue;
		}

		/* Switch to selected video src, filter type, filter mode */
		ret = vlib_change_mode(config);
		if (ret) {
			printf("ERROR: %s\n", vlib_errstr);
			*config = current_config;
			vlib_change_mode(config);
		} else {
		}
	} while (choice);
}

static int parse_output_arg(char *arg, struct vlib_config_data *cfg)
{
	int ret;

	ret = sscanf(arg, "%ux%u", &cfg->width_out, &cfg->height_out);
	if (ret < 2) {
		return 1;
	}

	char *start = strpbrk(arg, "-@");
	if (!start) {
		return 0;
	}

	char *end = start + strlen(start);

	if (*start == '-') {
		cfg->vrefresh = strtol(start + 1, &end, 10);
		start = end;
	}

	if (*start == '@') {
		cfg->fmt_out = v4l2_fourcc(start[1],
				          start[2],
				          start[3],
				          start[4]);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int c, ret = 0;
	int interactive_mode = 1;
	unsigned int video_source = 2; 			////// set to being a Web-Camera input ///////
	char *sv_cam_params = NULL;
	struct filter_tbl ft = {};
	struct vlib_config_data cfg = {
		.ft = &ft,
		.fmt_in = V4L2_PIX_FMT_YUYV, .fmt_out = V4L2_PIX_FMT_YUYV,
	};
	struct vlib_config config = {};
	const struct sigaction sa_sigintterm = {
		.sa_handler = vcmd_sig_handler,
	};

	/* Parse command line arguments */
	while ((c = getopt_long(argc, argv, "d:hpi:o:f:uSLs:Mm:Il:P:b:", opts, NULL)) != -1) {
		switch (c) {
		case 'd':
			sscanf(optarg, "%u", &cfg.dri_card_id);
			break;
		}
	}

	/* Set input resolution equal to output resolution */
	if (!cfg.width_in) {
		cfg.width_in = cfg.width_out;
		cfg.height_in = cfg.height_out;
	}

	/* Initialize video library */
	ret = vlib_init(&cfg);
	if (ret) {
		fprintf(stderr, "ERROR: vlib_init failed: %s\n", vlib_errstr);
		return ret;
	}

	if (!(cfg.flags & VLIB_CFG_FLAG_MULTI_INSTANCE)) {
		vlib_drm_set_layer0_state(0); // Disable Layer0 (used for graphics only)
	}

	/* Print begin streaming status */
	printf("\n---------------------------------------------------------------\n");
	printf("----------------- BEGIN STREAMING FROM WEBCAM -----------------\n");
	printf("-----------------  THROUGH ZCU102 TO MONITOR  -----------------\n");
	printf("---------------------------------------------------------------\n");

	printf("\n[INFO]  Display monitor resolution: %dx%d\n", vlib_get_active_width(), vlib_get_active_height());

	if (video_source) {
		config.vsrc = video_source - 1;
	}

	/* Start default video src, filter type, filter mode */
	ret = vlib_change_mode(&config);
	if (ret) {
		printf("ERROR: %s\n", vlib_errstr);
	}

	ret = sigaction(SIGINT, &sa_sigintterm, NULL);
	if (ret) {
		fprintf(stderr, "WARN: cannot install handler for SIGINT\n");
	}
	ret = sigaction(SIGTERM, &sa_sigintterm, NULL);
	if (ret) {
		fprintf(stderr, "WARN: cannot install handler for SIGTERM\n");
	}

	menu_loop(&config, &ft, cfg.flags);

	return 0;
}
