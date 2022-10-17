/********************************************************************************
 * Copyright (c) Prophesee S.A. *
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); * you may not
 * use this file except in compliance with the License. * You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0 * Unless required
 * by applicable law or agreed to in writing, software distributed under the
 * License is distributed   * on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations
 * under the License.
 *
 ********************************************************************************/
#ifndef _PSEE_GEN4_OPS_H_
#define _PSEE_GEN4_OPS_H_

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fwnode.h>
#include <media/v4l2-subdev.h>

struct sensor_dev {
	struct v4l2_subdev sd;
	struct media_pad pad;

	struct v4l2_ctrl_handler ctrl_handler;
	struct v4l2_ctrl **ctrls;
	unsigned int nb_ctrls;
	// Several ISSD sequences may be available for a sensor.
	// The first one is the default, others corresponds to test
	// pattern activation.
	const struct issd **issd;
	int issd_size; /* Number of ISSD sequences available. */
	unsigned int issd_idx; /* Current sequence index */

	struct v4l2_ctrl *reg_ctrl;

	struct gpio_desc *reset_gpio;
	struct gpio_desc *pwdn_gpio;

	bool is_powered;

	struct mutex mutex;
};

extern int gen4_init_sensor_dev(struct sensor_dev *sensor,
				struct i2c_client *client);
extern int gen4_get_camera_id(struct sensor_dev *sensor, u32 *sensor_id);
extern int gen4_ctl_init_controls(struct sensor_dev *sensor);
extern int gen4_sensor_remove(struct i2c_client *client);

#endif /* _PSEE_GEN4_OPS_H_ */
