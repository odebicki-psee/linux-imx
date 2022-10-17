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
#include "psee-issd.h"
#include "psee-gen4-ops.h"

#define CCAM5_FMT32_DESC "Prophesee 32-bit Event raw format"
#define CCAM5_FMT16_DESC "Prophesee 16-bit Event raw format"
#define CCAM5_FMT8_DESC "Prophesee 8-bit Event raw format"
#define MEDIA_BUS_FMT_YUYV8_2X8                 0x2008
#define MEDIA_BUS_FMT_AYUV8_1X32                0x2017
#define MEDIA_BUS_FMT_SBGGR8_1X8                0x3001
#define MEDIA_BUS_FMT_PSEE8                     0x5002
#define MEDIA_BUS_FMT_PSEE16                    0x5003
#define MEDIA_BUS_FMT_PSEE32                    0x5004



/* Aliases to get access to sensor_dev. */
static inline struct sensor_dev *sd_to_sensor_dev(struct v4l2_subdev *sd)
{
	return container_of(sd, struct sensor_dev, sd);
}
static inline struct sensor_dev *ctrl_to_sensor_dev(struct v4l2_ctrl_handler *hdl)
{
	return container_of(hdl, struct sensor_dev, ctrl_handler);
}
static inline const struct issd *sensor_issd(struct sensor_dev *sensor)
{
    return sensor->issd[sensor->issd_idx];
}

int sensor_set_power(struct sensor_dev *sensor, int on) {
	struct i2c_client *client = v4l2_get_subdevdata(&sensor->sd);

	if (!sensor->reset_gpio || !sensor->pwdn_gpio) {
	    dev_err(&client->dev, "%s: Sensor not initialized.", __func__);
		return -1;
	}

	if (on) {
		if (sensor->is_powered == true) {
    	    dev_info(&client->dev, "%s: Sensor already powered.", __func__);
			return 0;
		}

   	    dev_info(&client->dev, "%s: Sensor power on.", __func__);

//		gpiod_set_value(sensor->reset_gpio, 0);
//		gpiod_set_value(sensor->pwdn_gpio, 0);
//		msleep(5);

		gpiod_set_value(sensor->pwdn_gpio, 1);
		msleep(2000);

		gpiod_set_value(sensor->reset_gpio, 1);
		msleep(50);
		sensor->is_powered = true;
	} else {
   	    dev_info(&client->dev, "%s: Sensor power off.", __func__);

		gpiod_set_value(sensor->reset_gpio, 0);
		gpiod_set_value(sensor->pwdn_gpio, 0);
		sensor->is_powered = false;
		msleep(5);
	}
	return 0;
}

/* ISSD initialization */

/* Some sensor registers values can be modified through v4l2_ctrl interface.
   sensor_update_issd_op returns the most recent register value. */
static const struct v4l2_ctrl_ops sensor_reg_ctrl_ops;
static u32 sensor_update_write_op(const struct sensor_dev *sensor, const struct write_op *op){
	struct i2c_client *client = v4l2_get_subdevdata(&sensor->sd);
    struct v4l2_ctrl *ctrl;
    u32 addr;
    unsigned int i;
    u32 val = op->data;

	for (i = 0; i < sensor->nb_ctrls; ++ i) {
        ctrl = sensor->ctrls[i];
        if  ( ctrl->ops == &sensor_reg_ctrl_ops ) {
            addr = (u32)(size_t)ctrl->priv;
            if (addr == op->addr){
                val = ctrl->cur.val;
                dev_info(&client->dev, "%s:%d Update register 0x%08x = 0x%08x\n",
                    __func__, __LINE__,
                    addr, val);
                break;
            }
        }
    }
    return val;
}

/* Sensor register access through I2C subdev interface */
static int sensor_write_reg(struct i2c_client *client, const u32 reg, const u32 val)
{
	struct i2c_msg msg;
	u8 buf[8] = {0};
	int ret;

	dev_info(&client->dev, "Writing register 0x%08x = 0x%08x\n", reg, val);

	// Write reg addr
	buf[0] = (uint8_t)((reg & 0xff000000) >> 24);
	buf[1] = (uint8_t)((reg & 0x00ff0000) >> 16);
	buf[2] = (uint8_t)((reg & 0x0000ff00) >> 8);
	buf[3] = (uint8_t)(reg & 0x000000ff);

	// Write value
	buf[4] = (uint8_t)((val & 0xff000000) >> 24);
	buf[5] = (uint8_t)((val & 0x00ff0000) >> 16);
	buf[6] = (uint8_t)((val & 0x0000ff00) >> 8);
	buf[7] = (uint8_t)(val & 0x000000ff);


	msg.addr = client->addr;
	msg.flags = client->flags;
	msg.buf = buf;
	msg.len = sizeof(buf);

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret < 0) {
		dev_err(&client->dev, "%s error writing reg=0x%08x, val=0x%x, err = %d\n",
			__func__, reg, val, ret);
		return ret;
	}

	return 0;
}
static int sensor_read_reg(struct i2c_client *client, u32 reg, u32 *val)
{
	struct i2c_msg msg;
	u8 buf[4] = {0};
	int ret;

	msg.addr = client->addr;
	msg.flags = client->flags;
	msg.buf = buf;
	msg.len = sizeof(buf);

	buf[0] = (uint8_t)((reg & 0xff000000) >> 24);
	buf[1] = (uint8_t)((reg & 0x00ff0000) >> 16);
	buf[2] = (uint8_t)((reg & 0x0000ff00) >> 8);
	buf[3] = (uint8_t)(reg & 0x000000ff);

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret < 0)
		goto err;

	usleep_range(100, 200);	// delay 50us

	msg.flags = I2C_M_RD;
	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret < 0)
		goto err;

	*val = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];

	dev_info(&client->dev, "%s: Reading register 0x%08x = 0x%08x\n", __func__, reg, *val);

	return 0;

err:
	dev_err(&client->dev, "%s: Error reading register reg=0x%08x (%d)\n", __func__, reg, ret);
	return ret;
}
static int sensor_exec(struct sensor_dev *sensor, const struct sequence *seq)
{
	struct i2c_client *client = v4l2_get_subdevdata(&sensor->sd);
	const struct reg_op *op = seq->ops;
	int ret = 0;
	size_t i;
	u32 val;
	s32 timeout;

	dev_info(&client->dev, "%s: Executing sequence %s\n", __func__, seq->name);

	for (i = 0; i < seq->len; ++i) {
		if (op->op == PSEE_READ) {
			timeout = 100;
			while (timeout--) {
				ret = sensor_read_reg(client,
				op->args.read.addr, &val);
				if (val == op->args.read.data)
					break;
			}
			if (timeout == 0)
				return -ENODEV;
		}
		else if (op->op == PSEE_WRITE) {
			val = sensor_update_write_op(sensor, &op->args.write);
			ret = sensor_write_reg(client,
				op->args.write.addr,
				val);
		}
		else if (op->op == PSEE_DELAY) {
			usleep_range(op->args.delay.us, op->args.delay.us * 2);
			dev_info(&client->dev, "Delay for %d usec\n", op->args.delay.us);
		}
		else
			return -ERANGE;
		if (ret)
			return ret;
		++op;
	}

//	dev_info(&client->dev, "%s: Done!\n", __func__);

	return 0;
}

/* =============== subdev control operations =============== */
/* Control interface: control the bias settings. */
static int sensor_g_reg_ctrl(struct v4l2_ctrl *ctrl)
{
    struct sensor_dev *sensor = ctrl_to_sensor_dev(ctrl->handler);
	struct i2c_client *client = v4l2_get_subdevdata(&sensor->sd);
    u32 reg_addr;
    int ret = 0;

    if (sensor->is_powered == 0)
        return ret;

    reg_addr = (u32)(size_t)ctrl->priv;
    dev_info(&client->dev, "%s:%d Reading register 0x%08x = 0x%08x\n",
        __func__, __LINE__,
        reg_addr, ctrl->val);
    mutex_lock (&sensor->mutex);
    ret = sensor_read_reg(client, reg_addr, &ctrl->val);
    mutex_unlock (&sensor->mutex);
    return ret;
}
static int sensor_s_reg_ctrl(struct v4l2_ctrl *ctrl)
{
    struct sensor_dev *sensor = ctrl_to_sensor_dev(ctrl->handler);
	struct i2c_client *client = v4l2_get_subdevdata(&sensor->sd);
    u32 reg_addr;
    int ret = 0;

    if (sensor->is_powered == 0)
        return ret;

    reg_addr = (u32)(size_t)ctrl->priv;
    dev_info(&client->dev, "%s:%d Writing register 1 0x%08x = 0x%08x\n", __func__, __LINE__, reg_addr, ctrl->val);
    mutex_lock (&sensor->mutex);
    ret = sensor_write_reg(client, reg_addr, ctrl->val);
    mutex_unlock (&sensor->mutex);
    return ret;
}
static const struct v4l2_ctrl_ops sensor_reg_ctrl_ops = {
    .g_volatile_ctrl = sensor_g_reg_ctrl,
	.s_ctrl = sensor_s_reg_ctrl,
};

/* Control interface: get/set the active issd set of sequences. */
static int gen41_g_issd_ctrl(struct v4l2_ctrl *ctrl)
{
    struct sensor_dev *sensor = ctrl_to_sensor_dev(ctrl->handler);
    mutex_lock (&sensor->mutex);
    ctrl->val = sensor->issd_idx;
    mutex_unlock (&sensor->mutex);
    return 0;
}
static int gen41_s_issd_ctrl(struct v4l2_ctrl *ctrl)
{
    struct sensor_dev *sensor = ctrl_to_sensor_dev(ctrl->handler);
    mutex_lock (&sensor->mutex);
    if (ctrl->val < sensor->issd_size)
    	sensor->issd_idx = ctrl->val;
    mutex_unlock (&sensor->mutex);
    return 0;
}
static const struct v4l2_ctrl_ops gen41_issd_ctrl_ops = {
    .g_volatile_ctrl = gen41_g_issd_ctrl,
	.s_ctrl = gen41_s_issd_ctrl,
};

/* Control interface: init device controls. */
#define sensor_ctrl_cfg(i_id, i_ops, i_name, i_min, i_max, i_def) {\
    	.ops = i_ops, \
        .id = (V4L2_CID_USER_BASE + i_id), \
        .name = i_name, \
        .type = V4L2_CTRL_TYPE_INTEGER, \
        .flags = V4L2_CTRL_FLAG_SLIDER, \
        .min = (i_min), \
        .max = (i_max), \
		.def = (i_def), \
        .step = 1, \
	}
struct sensor_ctrl_desc {
    const struct v4l2_ctrl_config cfg;
    void *priv;
};
static int sensor_init_controls(
	struct sensor_dev *sensor,
	const struct sensor_ctrl_desc *ctrls)
{
	struct v4l2_ctrl_handler *hdl = &sensor->ctrl_handler;
    const struct v4l2_ctrl_config *cfg;
	unsigned int i;
	int rc = 0;

	v4l2_ctrl_handler_init(hdl, sensor->nb_ctrls);

	for (i = 0; i < sensor->nb_ctrls; ++ i) {
        cfg = &ctrls[i].cfg;
		dev_info(sensor->sd.dev, "%s  Add control %d - %d, %s\n", __func__, i, cfg->id, cfg->name);

		sensor->ctrls[i] = v4l2_ctrl_new_custom(hdl, cfg, ctrls[i].priv);
		if ((sensor->ctrls[i] == NULL) || hdl->error) {
			rc = hdl->error;
			dev_info(sensor->sd.dev, "%s invoke v4l2_ctrl_new_custom %d error: %d\n", __func__, i, rc);
			break;
		}
        //if (ctrls[i].cfg.ops == &sensor_reg_ctrl_ops)
        //    sensor->ctrls[i]->flags |= V4L2_CTRL_FLAG_VOLATILE;
	}
	if (rc) {
        dev_err(sensor->sd.dev, "%s failed to init subdev controls.\n", __func__);
        v4l2_ctrl_handler_free(hdl);
	}
	sensor->sd.ctrl_handler = hdl;
	return rc;
}


/* Control interface: Defines active controls for GEN4.1 */
const struct sensor_ctrl_desc  gen41_ctrl_desc[] = {
	/*           i_id,  i_ops,               i_name,     i_min,  i_max, i_def) */
//	{sensor_ctrl_cfg(0, &sensor_reg_ctrl_ops,  "pr",           0,    100,    50), .priv = (void *)(0x00001000UL + sizeof(u32) * 0)},
//	{sensor_ctrl_cfg(1, &sensor_reg_ctrl_ops,  "fo_n",        10,     40,    30), .priv = (void *)(0x00001000UL + sizeof(u32) * 1)},
//	{sensor_ctrl_cfg(2, &sensor_reg_ctrl_ops,  "hpf",          0,    160,   120), .priv = (void *)(0x00001000UL + sizeof(u32) * 2)},
//	{sensor_ctrl_cfg(3, &sensor_reg_ctrl_ops,  "diff_on",     90,    150,   100), .priv = (void *)(0x00001000UL + sizeof(u32) * 3)},
//	{sensor_ctrl_cfg(4, &sensor_reg_ctrl_ops,  "diff",         0,    150,    90), .priv = (void *)(0x00001000UL + sizeof(u32) * 4)},
//	{sensor_ctrl_cfg(5, &sensor_reg_ctrl_ops,  "diff_off",     0,     60,    30), .priv = (void *)(0x00001000UL + sizeof(u32) * 5)},
//	{sensor_ctrl_cfg(6, &sensor_reg_ctrl_ops,  "refr",         0,    120,    10), .priv = (void *)(0x00001000UL + sizeof(u32) * 6)},
//	{sensor_ctrl_cfg(7, &sensor_reg_ctrl_ops,  "frame_period", 0, 0xffff, 20000), .priv = (void *)(0x00001508UL)},
//	{sensor_ctrl_cfg(8, &sensor_reg_ctrl_ops,  "speed",        1,      1,     1), .priv = (void *)(0x0000023CUL)},
	{sensor_ctrl_cfg(9, &gen41_issd_ctrl_ops, "issd",         0,      100, 0), .priv = (void *)0},
};
int gen4_ctl_init_controls(struct sensor_dev *sensor)
{
    sensor->nb_ctrls = ARRAY_SIZE(gen41_ctrl_desc);
	sensor->ctrls = devm_kzalloc(sensor->sd.dev, ARRAY_SIZE(gen41_ctrl_desc) * sizeof(struct v4l2_ctrl *), GFP_KERNEL);
	return sensor_init_controls(sensor, gen41_ctrl_desc);
}


/* =============== subdev media operations =============== */


static int sensor_link_setup(struct media_entity *entity,
			   const struct media_pad *local,
			   const struct media_pad *remote, u32 flags)
{
	return 0;
}
static const struct media_entity_operations sensor_sd_media_ops = {
	.link_setup = sensor_link_setup,
};




/* =============== subdev core operations =============== */
static int sensor_s_power (struct v4l2_subdev *sd, int on)
{
	struct sensor_dev *sensor = sd_to_sensor_dev(sd);
	const struct issd *issd = sensor_issd(sensor);
    int ret = 0;

	dev_info(sd->dev, "%s: sensor_s_power %d (%d)\n", __func__, __LINE__, on);
    mutex_lock(&sensor->mutex);
	if (on) {
		sensor_set_power(sensor, on);
		ret = sensor_exec(sensor, &issd->init);
	}
	else {
		ret = sensor_exec(sensor, &issd->destroy);
		sensor_set_power(sensor, on);
	}
    mutex_unlock(&sensor->mutex);

	if (on && ret == 0) {
		ret = v4l2_ctrl_handler_setup(&sensor->ctrl_handler);
		if (ret < 0)
			return ret;
    }

    return ret;
}

static struct v4l2_subdev_core_ops sensor_core_ops = {
    .s_power = &sensor_s_power,
	.log_status = v4l2_ctrl_subdev_log_status,
	.subscribe_event = v4l2_ctrl_subdev_subscribe_event,
	.unsubscribe_event = v4l2_event_subdev_unsubscribe,
};

/* =============== subdev video operations =============== */
static int sensor_video_querystd(struct v4l2_subdev *sd, v4l2_std_id *a)
{
	dev_info(sd->dev, "%s:%d, std_id=0x%08x (V4L2_STD_NTSC)\n",
		__func__, __LINE__, (unsigned int) V4L2_STD_NTSC);
	*a = V4L2_STD_NTSC;
	return 0;
}

static int sensor_video_s_stream (struct v4l2_subdev *sd, int enable)
{
	struct sensor_dev *sensor = sd_to_sensor_dev(sd);
   	struct i2c_client *client = v4l2_get_subdevdata(&sensor->sd);

	const struct issd *issd = sensor_issd(sensor);
	int ret = 0;
    	//int mipi_stat_frame_cnt = 0;
    	//int mipi_stat_long_pkt_cnt, mipi_stat_pkt_timeout_cnt, mipi_stat_data_cnt;
    	int pipeline_control;

	dev_info(sd->dev, "%s: sensor_s_stream (%d)\n", __func__, enable);

	mutex_lock(&sensor->mutex);
    ret = sensor_read_reg(client, 0x00007000, &pipeline_control);
    dev_info(sd->dev, "%s: pipeline_control = 0x%08x", __func__, pipeline_control);

	ret = sensor_exec(sensor, (enable) ? &issd->start : &issd->stop);
    if (enable) {
        /*
        while (mipi_stat_frame_cnt < 20) {
            ret = sensor_read_reg(client, 0x0000b104, &mipi_stat_frame_cnt);
            ret = sensor_read_reg(client, 0x0000b108, &mipi_stat_long_pkt_cnt);
            ret = sensor_read_reg(client, 0x0000b10c, &mipi_stat_pkt_timeout_cnt);
            ret = sensor_read_reg(client, 0x0000b110, &mipi_stat_data_cnt);
            dev_info(sd->dev, "%s: mipi_stat_frame_cnt = 0x%08x", __func__, mipi_stat_frame_cnt);
            dev_info(sd->dev, "%s: mipi_stat_long_pkt_cnt = 0x%08x", __func__, mipi_stat_long_pkt_cnt);
            dev_info(sd->dev, "%s: mipi_stat_pkt_timeout_cnt = 0x%08x", __func__, mipi_stat_pkt_timeout_cnt);
            dev_info(sd->dev, "%s: mipi_stat_data_cnt = 0x%08x", __func__, mipi_stat_data_cnt);
        }
        */
    }    
	mutex_unlock(&sensor->mutex);
	dev_info(sd->dev, "%s: sensor_s_stream %d (%d) successfull\n", __func__, __LINE__, enable);

	return ret;
}
static const struct v4l2_subdev_video_ops sensor_video_ops = {
	.querystd = &sensor_video_querystd,
	.s_stream = &sensor_video_s_stream,
};

/* =============== subdev pad operations =============== */
/*
 * sensor_enum_mbus_code -
 * return the supported formats for the current imager
 */
static int sensor_pad_enum_mbus_code (
    struct v4l2_subdev *sd,
    struct v4l2_subdev_pad_config *cfg,
    struct v4l2_subdev_mbus_code_enum *code)
{
	// Application send pad, which and index
	dev_info(sd->dev, "%s: pad=%u, which=%u, index=%u\n", __func__, code->pad, code->which, code->index);

	/* if (code->index == 0) {
		code->code = MEDIA_BUS_FMT_Y8_1X8;
		dev_info(sd->dev, "%s: code = MEDIA_BUS_FMT_Y8_1X8\n", __func__);
	} */
	if (code->index == 0) {
		code->code = MEDIA_BUS_FMT_SBGGR8_1X8;
		dev_info(sd->dev, "%s: code = MEDIA_BUS_FMT_SBGGR8_1X8\n", __func__);
	}
	else if (code->index == 1) {
		code->code = MEDIA_BUS_FMT_YUYV8_2X8;
		dev_info(sd->dev, "%s: code = MEDIA_BUS_FMT_YUYV8_2X8\n", __func__);
	}
	else if (code->index == 2) {
		code->code = MEDIA_BUS_FMT_AYUV8_1X32;
		dev_info(sd->dev, "%s: code = MEDIA_BUS_FMT_AYUV8_1X32\n", __func__);
	}
	else if (code->index == 3) {
		code->code = MEDIA_BUS_FMT_PSEE8;
		dev_info(sd->dev, "%s: code = MEDIA_BUS_FMT_PSEE8\n", __func__);
	}
	else if (code->index == 4) {
		code->code = MEDIA_BUS_FMT_PSEE16;
		dev_info(sd->dev, "%s: code = MEDIA_BUS_FMT_PSEE16\n", __func__);
	}
	else if (code->index == 5) {
		code->code = MEDIA_BUS_FMT_PSEE32;
		dev_info(sd->dev, "%s: code = MEDIA_BUS_FMT_PSEE32\n", __func__);
	}
	else
		// pad or index are invalid
		return -EINVAL;

	return 0;
}

/*
 * sensor_set_format - set requested format from user
 * The image size return corresponds to the max number
 * of events that can fit into a frame.
 * It depends on the Frame Period and MIPI Tx capability
 */
#define MIPI_FREQ (800000000UL)
#define PSEE_MIN_BUFFER_PERIOD  (20) /* In ms */
#define NB_LANES (2)
// Gen4.1 mipi_packet_size = 0x2000 (8192 bytes in a line, this is the default value)​
// With the pattern generator, a frame is 32 lines.​
// 32 x 16384 = 524 288 bytes​
//#define PSEE_BUFFER_SIZE 524288UL
//#define PSEE_BUFFER_SIZE 1048576UL
#define PSEE_BUFFER_SIZE (2 * MIPI_FREQ / 8 / 1000 * PSEE_MIN_BUFFER_PERIOD * NB_LANES)

static int sensor_pad_set_format (
	struct v4l2_subdev *sd,
	struct v4l2_subdev_pad_config *cfg,
	struct v4l2_subdev_format *fmt)
{
	unsigned int buffsize;

	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int ret;
	u32 val;

	// Get the packet size from the sensor
	ret = sensor_read_reg(client, 0x0000B020UL, &val);
	if (ret) 
		return -EINVAL;
	dev_info(sd->dev, "%s: mipi_packet_size = %d\n", __func__, val);

	/* if (fmt->format.code == MEDIA_BUS_FMT_Y8_1X8) {
		dev_info(sd->dev, "%s:%d MEDIA_BUS_FMT_Y8_1X8\n", __func__, __LINE__);
		buffsize = PSEE_BUFFER_SIZE;
	} */
	if (fmt->format.code == MEDIA_BUS_FMT_SBGGR8_1X8) {
		dev_info(sd->dev, "%s:%d MEDIA_BUS_FMT_SBGGR8_1X8\n", __func__, __LINE__);
		buffsize = PSEE_BUFFER_SIZE;
	}
	else if (fmt->format.code == MEDIA_BUS_FMT_YUYV8_2X8) {
		dev_info(sd->dev, "%s:%d MEDIA_BUS_FMT_YUYV8_2X8\n", __func__, __LINE__);
		buffsize = PSEE_BUFFER_SIZE / 2;
	}
	else if (fmt->format.code == MEDIA_BUS_FMT_AYUV8_1X32) {
		dev_info(sd->dev, "%s:%d MEDIA_BUS_FMT_AYUV8_1X32\n", __func__, __LINE__);
		buffsize = PSEE_BUFFER_SIZE / 4;
	}
	else if (fmt->format.code == MEDIA_BUS_FMT_PSEE8) {
		dev_info(sd->dev, "%s:%d MEDIA_BUS_FMT_PSEE8\n", __func__, __LINE__);
		buffsize = PSEE_BUFFER_SIZE;
	}
	else if (fmt->format.code == MEDIA_BUS_FMT_PSEE32) {
		dev_info(sd->dev, "%s:%d MEDIA_BUS_FMT_PSEE32\n", __func__, __LINE__);
		buffsize = PSEE_BUFFER_SIZE / 4;
	}
	else if (fmt->format.code == MEDIA_BUS_FMT_PSEE16) {
		dev_info(sd->dev, "%s:%d MEDIA_BUS_FMT_PSEE16\n", __func__, __LINE__);
		buffsize = PSEE_BUFFER_SIZE / 2;
	}
	else {
			dev_err(sd->dev, "%s:%d Format (%d) not supported)\n", __func__, __LINE__, fmt->format.code);
		return -EINVAL;
	}

	// minimum between buffsize and 65520
	fmt->format.width = min(buffsize, 0xfff0U);
	fmt->format.height = (buffsize % fmt->format.width) ?
		(buffsize / fmt->format.width) + 1:
		(buffsize / fmt->format.width);

//	fmt->format.height = 32;
//	fmt->format.width = buffsize / fmt->format.height;

	dev_info(sd->dev, "%s:%d width=%d height=%d buffsize=%d\n", __func__, __LINE__, 
			fmt->format.width, fmt->format.height, buffsize);

	return 0;
}

static const struct v4l2_subdev_pad_ops sensor_pad_ops = {
    //.get_fmt = fpga_camera_get_format,
    .set_fmt = &sensor_pad_set_format,
    //.enum_frame_size = fpga_enum_frame_size,
    .enum_mbus_code = &sensor_pad_enum_mbus_code,
};

/* =============== subdev operations =============== */

static const struct v4l2_subdev_ops sensor_subdev_ops = {
	.core  = &sensor_core_ops,
	.video = &sensor_video_ops,
	.pad = &sensor_pad_ops,
};

/* Open sub-device */
static int sensor_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct sensor_dev *sensor = sd_to_sensor_dev(sd);
	struct v4l2_mbus_framefmt *try_fmt =
		v4l2_subdev_get_try_format(sd, fh->pad, 0);

	dev_info(sd->dev, "%s:%d\n", __func__, __LINE__);

	mutex_lock(&sensor->mutex);

	/* Initialize try_fmt */
	try_fmt->width = 1600000000UL / 8 / 10; /* 10 frame per sec at 1.6Gpbs */
	try_fmt->height = 1;
	try_fmt->code = MEDIA_BUS_FMT_PSEE32;
	try_fmt->field = V4L2_FIELD_NONE;

//	524288 = 16384 * 32 
//	try_fmt->width = 16384;
//	try_fmt->height = 32;
//	try_fmt->code = MEDIA_BUS_FMT_Y8_1X8;

	mutex_unlock(&sensor->mutex);

	return 0;
}

static const struct v4l2_subdev_internal_ops sensor_internal_ops = {
	.open = sensor_open,
};

int gen4_init_sensor_dev(struct sensor_dev *sensor, struct i2c_client *client)
{
    int ret;

	v4l2_i2c_subdev_init(&sensor->sd, client, &sensor_subdev_ops);

	sensor->sd.internal_ops = &sensor_internal_ops;
	sensor->sd.flags |= V4L2_SUBDEV_FL_IS_I2C;
	sensor->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;

	sensor->sd.grp_id = 678;
	sensor->sd.entity.ops = &sensor_sd_media_ops;
	sensor->sd.entity.function = MEDIA_ENT_F_CAM_SENSOR;

	/* Initialize source pad */
	sensor->pad.flags = MEDIA_PAD_FL_SOURCE;
	ret = media_entity_pads_init(&sensor->sd.entity, 1, &sensor->pad);

    return ret;
}

/* System ID Register address */
#define CCAM5_SYS_ID 0x14

/* Identify the camera */
int gen4_get_camera_id(struct sensor_dev *sensor, u32 *sensor_id)
{
   	struct i2c_client *client = v4l2_get_subdevdata(&sensor->sd);
    int ret;

	/* Power Up and reset the camera. */
	ret = sensor_set_power(sensor, 1);
	if (ret) 
        return ret;

	ret = sensor_read_reg(client, CCAM5_SYS_ID, sensor_id);

    sensor_set_power(sensor, 0);
    return ret;

}

int gen4_sensor_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct sensor_dev *sensor = sd_to_sensor_dev(sd);

	v4l2_async_unregister_subdev(&sensor->sd);
	media_entity_cleanup(&sensor->sd.entity);
	v4l2_ctrl_handler_free(&sensor->ctrl_handler);

	mutex_destroy(&sensor->mutex);

	return 0;
}



