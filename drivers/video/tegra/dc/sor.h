
/*
 * drivers/video/tegra/dc/sor.h
 *
 * Copyright (c) 2011, NVIDIA Corporation.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __DRIVERS_VIDEO_TEGRA_DC_SOR_H__
#define __DRIVERS_VIDEO_TEGRA_DC_SOR_H__


enum {
	trainingPattern_Disabled	= 0,
	trainingPattern_1		= 1,
	trainingPattern_2		= 2,
	trainingPattern_3		= 3,
	trainingPattern_None		= 0xff
};

struct tegra_dc_dp_link_config {
	bool	is_valid;

	/* Supported configuration */
	u8	max_link_bw;
	u8	max_lane_count;
	bool	downspread;
	bool	support_enhanced_framing;
	u32	bytes_per_pixel;
	bool	alt_scramber_reset_cap; /* true for eDP */
	bool	only_enhanced_framing;	/* enhanced_frame_en ignored */

	/* Actual configuration */
	u8	link_bw;
	u8	lane_count;
	bool	enhanced_framing;
	bool	scramble_ena;

	u32	activepolarity;
	u32	active_count;
	u32	tu_size;
	u32	active_frac;
	u32	watermark;

	u32	hblank_sym;
	u32	vblank_sym;
};


struct tegra_dc_sor_data {
	struct tegra_dc	*dc;

	void __iomem	*base;
	struct resource	*base_res;
	struct clk	*sor_clk;

	u8					 portnum;	/* 0 or 1 */
	const struct tegra_dc_dp_link_config	*link_cfg;
};

#define TEGRA_SOR_TIMEOUT_MS  1000

#define CHECK_RET(x)			\
	do {				\
		ret = (x);		\
		if (ret != 0)		\
			return ret;	\
	} while (0)


static inline void usleep(unsigned int usecs)
{
	unsigned long timeout = usecs_to_jiffies(usecs) + 1;

	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_timeout_uninterruptible(timeout);
}

struct tegra_dc_sor_data *tegra_dc_sor_init(struct tegra_dc *dc,
	const struct tegra_dc_dp_link_config *cfg);

void tegra_dc_sor_destroy(struct tegra_dc_sor_data *sor);
void tegra_dc_sor_enable(struct tegra_dc_sor_data *sor);
void tegra_dc_sor_disable(struct tegra_dc_sor_data *sor);

void tegra_dc_sor_set_internal_panel(struct tegra_dc_sor_data *sor,
	bool is_int);
void tegra_dc_sor_read_link_config(struct tegra_dc_sor_data *sor,
	u8 *link_bw, u8 *lane_count);
void tegra_dc_sor_set_link_bandwidth(struct tegra_dc_sor_data *sor,
	u8 link_bw);
void tegra_dc_sor_set_lane_count(struct tegra_dc_sor_data *sor, u8 lane_count);
void tegra_dc_sor_set_panel_power(struct tegra_dc_sor_data *sor,
	bool power_up);
void tegra_dc_sor_set_pwm(struct tegra_dc_sor_data *sor, u32 pwm_div,
	u32 pwm_dutycycle, u32 pwm_clksrc);
int  tegra_dc_sor_powerdown_dplanes(struct tegra_dc_sor_data *sor,
	u32 lane_count);
void tegra_dc_sor_set_dp_lanedata(struct tegra_dc_sor_data *sor,
	u32 lane, u32 pre_emphasis, u32 drive_current, u32 tx_pu);
void tegra_dc_sor_set_dp_linkctl(struct tegra_dc_sor_data *sor, bool ena,
	u8 training_pattern, const struct tegra_dc_dp_link_config *cfg,
	bool use_scramble);

#endif