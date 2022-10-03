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
#include "psee-issd.h"

static const struct reg_op gen41_issd_stream_init_seq[] = {
    //    {.op = PSEE_WRITE, .args = {.write = {0x0000001CUL, 0x00000001}}},
    // Digital init
    //    {.op = PSEE_WRITE, .args = {.write = {0x00000064CUL, 0x00000001}}},
    // Config PLL: pll internal clock = 10 MHz, pll output clock 1600 MHz
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B074UL, 0x00000002}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_5)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B078UL, 0x000000A0}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_6)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x10006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    // Start_PLL
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B068UL, 0x00000004}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_2)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B07CUL, 0x00000003}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_7)
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},  // 1ms delay
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x50006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x5000640A}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    // Start_MIPI @ 800MHz
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B080UL, 0x00000067}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkpost)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B084UL, 0x0000000F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkpre)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B088UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkprepare)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B08CUL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclktrail)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B090UL, 0x000000DF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkzero)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B094UL, 0x00000057}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thsexit)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B098UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thsprepare)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B09CUL, 0x00000067}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thszero)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0A0UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thstrail)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B0A4UL, 0x0000002F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tlpx)

    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0ACUL, 0x00000028}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_txclkesc_freq)
                                                     //   default is 0x28 = 10.0Mhz (the 2 lsb are the decimal part)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0CCUL, 0x00000001}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_pll_div)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B02CUL, 0x000000FF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_line_blanking)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B030UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_blanking)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B034UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_start_delay)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B038UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_end_delay)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B028UL, 0x00004E20}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_period): Frame
                                                     // period in us, default is 0x7d0 (2000), 0x4e20 = 20ms
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL, 0x000003D8}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control)
                                                     //   bit 0: mipi_csi_enable = 0 (set to 1 to start streaming)
                                                     //   bit 1: mipi_sram_powerdown = 0
                                                     //   bit 2: mipi_csi_empty = 0
                                                     //   bit 3: data lane 0 = 1
                                                     //   bit 4: data lane 1 = 1
                                                     //   bit 5 : mipi_packet_timeout_enable = 0
                                                     //   bit 6: line_blanking_ck_lane_lps_enable = 1
                                                     //   bit 7: frame_blanking_ck_lane_lps_enable = 1
                                                     //   bit 8: line_blanking_en = 1
                                                     //   bit 9: frame_blanking_en = 1
                                                     //   bit 10: lane_switch = 0
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000008A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
                                                     //   divider = 0x0a
                                                     //   enable = 1
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B01CUL, 0x0000005E}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_data_identifier)
                                                     //   bits 0-5: data type (default is 0x30: user-defined)
                                                     //   bits 6-7: virtual channel
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B020UL, 0x00004000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_packet_size)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000007}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x00000101}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00002405}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_ana)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x00000101}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg2)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000170}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_cc)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x0000000F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000008A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},   // 200us delay
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0C8UL, 0x00000003}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_power_ctrl)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},   // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000B044UL, 0x00000001}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_stream)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL, 0x000003D9}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control)
                                                     //   bit 0: mipi_csi_enable = 1
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x00000640}}}, // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                        //   bit 0: time_base_enable = 0
                                                                        //   bit 1: time_base_mode = 0
                                                                        //   bit 2: external_mode = 0
                                                                        //   bit 3: external_mode_enable = 0
                                                                        //   bit 4-10: us_counter_max = 0x64
    // Start_Sensor_clock
    //    {.op = PSEE_WRITE, .args = {.write = {0x00008000UL, 0x0001E400}}}, //
    //    (PSEE/SENSOR_IF/HUAHINE/eoi/cfg_mode_control)
    //   bit 10: cfg_clk_out_en = 1
    //   bit 13-20: cfg_clk_timeout = 0xF
    // Configure sensor digital pipeline
    {.op = PSEE_WRITE, .args = {.write = {0x00008000UL, 0x0001E005}}}, // (PSEE/SENSOR_IF/HUAHINE/eoi/cfg_mode_control)
                                                                        //   bit 0: cfg_control_enable = 1
                                                                        //   bit 2: cfg_bypass_enable = 1
                                                                        //   bit 3: cfg_output_fifo_bypass = 0
                                                                        //   bit 10: cfg_clk_out_en = 0
                                                                        //   bit 13-20: cfg_clk_timeout = 0xF
    {.op = PSEE_WRITE, .args = {.write = {0x00007008UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/edf/control)
    {.op = PSEE_WRITE, .args = {.write = {0x00007000UL, 0x00070001}}}, // (PSEE/SENSOR_IF/HUAHINE/edf/pipeline_control)
    // ERC
    {.op = PSEE_WRITE, .args = {.write = {0x00006008UL, 0x000000C8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000600CUL, 0x00000FA0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006004UL, 0x00000001}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000602CUL, 0x00000001}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006800UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006804UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006808UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000680CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006810UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006814UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006818UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000681CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006820UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006824UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006828UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000682CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006830UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006834UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006838UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000683CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006840UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006844UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006848UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000684CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006850UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006854UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006858UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000685CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006860UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006864UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006868UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000686CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006870UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006874UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006878UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000687CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006880UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006884UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006888UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000688CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006890UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006894UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006898UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000689CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068A0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068A4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068A8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068ACUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068B0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068B4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068B8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068BCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068C0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068C4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068C8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068CCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068D0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068D4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068D8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068DCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068E0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068E4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068E8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068ECUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068F0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068F4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068F8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068FCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006900UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006904UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006908UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000690CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006910UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006914UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006918UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000691CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006920UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006924UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006928UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000692CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006930UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006934UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006938UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000693CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006940UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006944UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006948UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000694CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006950UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006954UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006958UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000695CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006960UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006964UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006968UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000696CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006970UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006974UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006978UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000697CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006980UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006984UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006988UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000698CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006990UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006994UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006998UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000699CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069A0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069A4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069A8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069ACUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069B0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069B4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069B8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069BCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069C0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069C4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069C8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069CCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069D0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069D4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069D8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069DCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069E0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069E4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069E8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069ECUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069F0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069F4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069F8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069FCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A00UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A04UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A08UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A0CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A10UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A14UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A18UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A1CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A20UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A24UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A28UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A2CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A30UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A34UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A38UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A3CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A40UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A44UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A48UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A4CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A50UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A54UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A58UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A5CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A60UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A64UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A68UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A6CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A70UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A74UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A78UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A7CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A80UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A84UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A88UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A8CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A90UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A94UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A98UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A9CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AA0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AA4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AA8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AACUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AB0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AB4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AB8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006ABCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AC0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AC4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AC8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006ACCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AD0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AD4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AD8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006ADCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AE0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AE4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AE8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AECUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AF0UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AF4UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AF8UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AFCUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B00UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B04UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B08UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B0CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B10UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B14UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B18UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B1CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B20UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B24UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B28UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B2CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B30UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B34UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B38UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B3CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B40UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B44UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B48UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B4CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B50UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B54UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B58UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B5CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B60UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B64UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B68UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B6CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B70UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B74UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B78UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B7CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B80UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B84UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B88UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B8CUL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B90UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B94UL, 0x08080808}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000602CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006800UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006804UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006808UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000680CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006810UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006814UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006818UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000681CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006820UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006824UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006828UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000682CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006830UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006834UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006838UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000683CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006840UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006844UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006848UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000684CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006850UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006854UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006858UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000685CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006860UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006864UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006868UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000686CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006870UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006874UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006878UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000687CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006880UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006884UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006888UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000688CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006890UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006894UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006898UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000689CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068A0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068A4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068A8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068ACUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068B0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068B4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068B8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068BCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068C0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068C4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068C8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068CCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068D0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068D4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068D8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068DCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068E0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068E4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068E8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068ECUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068F0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068F4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068F8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000068FCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006900UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006904UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006908UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000690CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006910UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006914UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006918UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000691CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006920UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006924UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006928UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000692CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006930UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006934UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006938UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000693CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006940UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006944UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006948UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006950UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000694CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006954UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006958UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000695CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006960UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006964UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006968UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000696CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006970UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006974UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006978UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000697CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006980UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006984UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006988UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000698CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006990UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006994UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006998UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000699CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069A0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069A4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069A8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069ACUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069B0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069B4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069B8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069BCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069C0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069C4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069C8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069CCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069D0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069D4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069D8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069DCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069E0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069E4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069E8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069ECUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069F0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069F4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069F8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000069FCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A00UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A04UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A08UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A0CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A10UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A14UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A18UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A1CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A20UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A24UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A28UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A2CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A30UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A34UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A38UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A3CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A40UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A44UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A48UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A4CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A50UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A54UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A58UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A5CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A60UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A64UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A68UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A6CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A70UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A74UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A78UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A7CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A80UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A84UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A88UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A8CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A90UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A94UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A98UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006A9CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AA0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AA4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AA8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AACUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AB0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AB4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AB8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006ABCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AC0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AC4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AC8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006ACCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AD0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AD4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AD8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006ADCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AE0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AE4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AE8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AECUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AF0UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AF4UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AF8UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006AFCUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B00UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B04UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B08UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B0CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B10UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B14UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B18UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B1CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B20UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B24UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B28UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B2CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B30UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B34UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B38UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B3CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B40UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B44UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B48UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B4CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B50UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B54UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B58UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B5CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B60UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B64UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B68UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B6CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B70UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B74UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B78UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B7CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B80UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B84UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B88UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B8CUL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B90UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006B94UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006400UL, 0x00010000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006404UL, 0x00030002}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006408UL, 0x00050004}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000640CUL, 0x00070006}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006410UL, 0x00090008}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006414UL, 0x000B000A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006418UL, 0x000D000C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000641CUL, 0x000F000E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006420UL, 0x00110010}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006424UL, 0x00130012}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006428UL, 0x00150014}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000642CUL, 0x00170016}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006430UL, 0x00190018}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006434UL, 0x001B001A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006438UL, 0x001D001C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000643CUL, 0x001F001E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006440UL, 0x00210020}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006444UL, 0x00230022}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006448UL, 0x00250024}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000644CUL, 0x00270026}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006450UL, 0x00290028}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006454UL, 0x002B002A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006458UL, 0x002D002C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000645CUL, 0x002F002E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006460UL, 0x00310030}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006464UL, 0x00330032}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006468UL, 0x00350034}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000646CUL, 0x00370036}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006470UL, 0x00390038}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006474UL, 0x003B003A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006478UL, 0x003D003C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000647CUL, 0x003F003E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006480UL, 0x00410040}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006484UL, 0x00430042}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006488UL, 0x00450044}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000648CUL, 0x00470046}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006490UL, 0x00490048}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006494UL, 0x004B004A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006498UL, 0x004D004C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000649CUL, 0x004F004E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064A0UL, 0x00510050}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064A4UL, 0x00530052}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064A8UL, 0x00550054}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064ACUL, 0x00570056}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064B0UL, 0x00590058}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064B4UL, 0x005B005A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064B8UL, 0x005D005C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064BCUL, 0x005F005E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064C0UL, 0x00610060}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064C4UL, 0x00630062}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064C8UL, 0x00650064}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064CCUL, 0x00670066}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064D0UL, 0x00690068}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064D4UL, 0x006B006A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064D8UL, 0x006D006C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064DCUL, 0x006F006E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064E0UL, 0x00710070}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064E4UL, 0x00730072}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064E8UL, 0x00750074}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064ECUL, 0x00770076}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064F0UL, 0x00790078}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064F4UL, 0x007B007A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064F8UL, 0x007D007C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000064FCUL, 0x007F007E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006500UL, 0x00810080}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006504UL, 0x00830082}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006508UL, 0x00850084}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000650CUL, 0x00870086}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006510UL, 0x00890088}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006514UL, 0x008B008A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006518UL, 0x008D008C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000651CUL, 0x008F008E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006520UL, 0x00910090}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006524UL, 0x00930092}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006528UL, 0x00950094}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000652CUL, 0x00970096}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006530UL, 0x00990098}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006534UL, 0x009B009A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006538UL, 0x009D009C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000653CUL, 0x009F009E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006540UL, 0x00A100A0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006544UL, 0x00A300A2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006548UL, 0x00A500A4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000654CUL, 0x00A700A6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006550UL, 0x00A900A8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006554UL, 0x00AB00AA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006558UL, 0x00AD00AC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000655CUL, 0x00AF00AE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006560UL, 0x00B100B0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006564UL, 0x00B300B2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006568UL, 0x00B500B4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000656CUL, 0x00B700B6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006570UL, 0x00B900B8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006574UL, 0x00BB00BA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006578UL, 0x00BD00BC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000657CUL, 0x00BF00BE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006580UL, 0x00C100C0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006584UL, 0x00C300C2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006588UL, 0x00C500C4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000658CUL, 0x00C700C6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006590UL, 0x00C900C8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006594UL, 0x00CB00CA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006598UL, 0x00CD00CC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000659CUL, 0x00CF00CE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065A0UL, 0x00D100D0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065A4UL, 0x00D300D2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065A8UL, 0x00D500D4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065ACUL, 0x00D700D6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065B0UL, 0x00D900D8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065B4UL, 0x00DB00DA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065B8UL, 0x00DD00DC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065BCUL, 0x00DF00DE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065C0UL, 0x00E100E0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065C4UL, 0x00E300E2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065C8UL, 0x00E500E4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065CCUL, 0x00E700E6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065D0UL, 0x00E900E8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065D4UL, 0x00EB00EA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065D8UL, 0x00ED00EC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065DCUL, 0x00EF00EE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065E0UL, 0x00F100F0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065E4UL, 0x00F300F2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065E8UL, 0x00F500F4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065ECUL, 0x00F700F6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065F0UL, 0x00F900F8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065F4UL, 0x00FB00FA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065F8UL, 0x00FD00FC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000065FCUL, 0x00FF00FE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006600UL, 0x01010100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006604UL, 0x01030102}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006608UL, 0x01050104}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000660CUL, 0x01070106}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006610UL, 0x01090108}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006614UL, 0x010B010A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006618UL, 0x010D010C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000661CUL, 0x010F010E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006620UL, 0x01110110}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006624UL, 0x01130112}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006628UL, 0x01150114}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000662CUL, 0x01170116}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006630UL, 0x01190118}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006634UL, 0x011B011A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006638UL, 0x011D011C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000663CUL, 0x011F011E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006640UL, 0x01210120}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006644UL, 0x01230122}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006648UL, 0x01250124}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000664CUL, 0x01270126}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006650UL, 0x01290128}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006654UL, 0x012B012A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006658UL, 0x012D012C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000665CUL, 0x012F012E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006660UL, 0x01310130}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006664UL, 0x01330132}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006668UL, 0x01350134}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000666CUL, 0x01370136}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006670UL, 0x01390138}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006674UL, 0x013B013A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006678UL, 0x013D013C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000667CUL, 0x013F013E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006680UL, 0x01410140}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006684UL, 0x01430142}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006688UL, 0x01450144}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000668CUL, 0x01470146}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006690UL, 0x01490148}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006694UL, 0x014B014A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006698UL, 0x014D014C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000669CUL, 0x014F014E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066A0UL, 0x01510150}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066A4UL, 0x01530152}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066A8UL, 0x01550154}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066ACUL, 0x01570156}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066B0UL, 0x01590158}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066B4UL, 0x015B015A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066B8UL, 0x015D015C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066BCUL, 0x015F015E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066C0UL, 0x01610160}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066C4UL, 0x01630162}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066C8UL, 0x01650164}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066CCUL, 0x01670166}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066D0UL, 0x01690168}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066D4UL, 0x016B016A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066D8UL, 0x016D016C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066DCUL, 0x016F016E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066E0UL, 0x01710170}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066E4UL, 0x01730172}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066E8UL, 0x01750174}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066ECUL, 0x01770176}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066F0UL, 0x01790178}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066F4UL, 0x017B017A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066F8UL, 0x017D017C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000066FCUL, 0x017F017E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006700UL, 0x01810180}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006704UL, 0x01830182}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006708UL, 0x01850184}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000670CUL, 0x01870186}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006710UL, 0x01890188}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006714UL, 0x018B018A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006718UL, 0x018D018C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000671CUL, 0x018F018E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006720UL, 0x01910190}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006724UL, 0x01930192}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006728UL, 0x01950194}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000672CUL, 0x01970196}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006730UL, 0x01990198}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006734UL, 0x019B019A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006738UL, 0x019D019C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000673CUL, 0x019F019E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006740UL, 0x01A101A0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006744UL, 0x01A301A2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006748UL, 0x01A501A4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000674CUL, 0x01A701A6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006750UL, 0x01A901A8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006754UL, 0x01AB01AA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006758UL, 0x01AD01AC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000675CUL, 0x01AF01AE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006760UL, 0x01B101B0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006764UL, 0x01B301B2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006768UL, 0x01B501B4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000676CUL, 0x01B701B6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006770UL, 0x01B901B8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006774UL, 0x01BB01BA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006778UL, 0x01BD01BC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000677CUL, 0x01BF01BE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006780UL, 0x01C101C0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006784UL, 0x01C301C2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006788UL, 0x01C501C4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000678CUL, 0x01C701C6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006790UL, 0x01C901C8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006794UL, 0x01CB01CA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006798UL, 0x01CD01CC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000679CUL, 0x01CF01CE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067A0UL, 0x01D101D0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067A4UL, 0x01D301D2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067A8UL, 0x01D501D4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067ACUL, 0x01D701D6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067B0UL, 0x01D901D8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067B4UL, 0x01DB01DA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067B8UL, 0x01DD01DC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067BCUL, 0x01DF01DE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067C0UL, 0x01E101E0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067C4UL, 0x01E301E2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067C8UL, 0x01E501E4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067CCUL, 0x01E701E6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067D0UL, 0x01E901E8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067D4UL, 0x01EB01EA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067D8UL, 0x01ED01EC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067DCUL, 0x01EF01EE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067E0UL, 0x01F101F0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067E4UL, 0x01F301F2}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067E8UL, 0x01F501F4}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067ECUL, 0x01F701F6}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067F0UL, 0x01F901F8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067F4UL, 0x01FB01FA}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067F8UL, 0x01FD01FC}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000067FCUL, 0x01FF01FE}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006080UL, 0x03020100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006084UL, 0x07060504}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006088UL, 0x0B0A0908}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000608CUL, 0x0F0E0D0C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006090UL, 0x13121110}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006094UL, 0x17161514}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006098UL, 0x1B1A1918}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000609CUL, 0x1F1E1D1C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060C0UL, 0x03020100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060C4UL, 0x07060504}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060C8UL, 0x0B0A0908}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060CCUL, 0x0F0E0D0C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060D0UL, 0x13121110}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060D4UL, 0x17161514}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060D8UL, 0x1B1A1918}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000060DCUL, 0x1F1E1D1C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006028UL, 0x00000003}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006050UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006060UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006070UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00006000UL, 0x00155401}}},
    // STC
    {.op = PSEE_WRITE, .args = {.write = {0x0000D000UL, 0x00000005}}}, // (PSEE/SENSOR_IF/HUAHINE/stc/pipe_control)
                                                                        //   bit 0: enable = 1
                                                                        //   bit 2: bypass = 1
    // AFK
    {.op = PSEE_WRITE, .args = {.write = {0x0000C000UL, 0x00000005}}}, // (PSEE/SENSOR_IF/HUAHINE/afk/pipe_control)
                                                                        //   bit 0: enable = 1
                                                                        //   bit 2: bypass = 1
    //
    {.op = PSEE_WRITE, .args = {.write = {0x00009000UL, 0x00000208}}}, // (PSEE/SENSOR_IF/HUAHINE/ro/readout_ctrl)
                                                                        //   bit 1: ro_td_self_test_en = 0
                                                                        //   bit 3: ro_analog_pipe_en = 1
                                                                        //   bit 9: ro_digital_pipe_en = 1
    // EDF
    {.op = PSEE_WRITE, .args = {.write = {0x00007008UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/edf/control)
                                                                        //   bit 0: format = 0
    {.op = PSEE_WRITE, .args = {.write = {0x00007000UL, 0x00070001}}}, // (PSEE/SENSOR_IF/HUAHINE/edf/pipeline_control)
                                                                        //   bit 0: enable = 1
                                                                        //   bit 16-31: timeout
    {.op = PSEE_WRITE, .args = {.write = {0x00008000UL, 0x0001E085}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x00000644}}}, // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                        //   bit 0: time_base_enable = 0
                                                                        //   bit 1: time_base_mode = 0
                                                                        //   bit 2: external_mode = 1
                                                                        //   bit 3: external_mode_enable = 0
                                                                        //   bit 4-10: us_counter_max = 0x64
    {.op = PSEE_WRITE, .args = {.write = {0x00000004UL, 0xF0005442}}},
    // Analog INIT
    {.op = PSEE_WRITE, .args = {.write = {0x00000004UL, 0xF0005042}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000002CUL, 0x00200224}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x00000101}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x000000A1}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    // RegisterOperation::Read(0x0000A000, 0x000002A1, 0x00000200),
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00002405}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    // RegisterOperation::Read(0x0000A008, 0x00082405, 0x00080000),
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x00000101}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x000000A1}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    // RegisterOperation::Read(0x0000A004, 0x000002A1, 0x00000200),
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000170}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000160}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00082401}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000004CUL, 0x00007141}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000054UL, 0x00000210}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000008UL, 0x60000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001104UL, 0x00000001}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A010UL, 0x0000A06B}}},
    {.op = PSEE_DELAY, .args = {.delay = {300}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001100UL, 0x00000004}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    // RegisterOperation::Read(0x0000A010, 0x0180A06B, 0x00800000),
    {.op = PSEE_WRITE, .args = {.write = {0x0000A010UL, 0x0180A063}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A00CUL, 0x00000404}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A00CUL, 0x00000405}}},
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A00CUL, 0x00000401}}},
    {.op = PSEE_DELAY, .args = {.delay = {300}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000070UL, 0x00400000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000006CUL, 0x0EE47117}}},
    {.op = PSEE_DELAY, .args = {.delay = {100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000006CUL, 0x0EE4711F}}},
    {.op = PSEE_DELAY, .args = {.delay = {100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000070UL, 0x00480000}}},
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001044UL, 0x01A98A7C}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001040UL, 0x01A98A94}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001048UL, 0x01A98B94}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000101CUL, 0x01A1575B}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001050UL, 0x01A1B251}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001020UL, 0x01A9CD44}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000100CUL, 0x01A1FF00}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001018UL, 0x01A13734}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001010UL, 0x01A16373}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001014UL, 0x01A15050}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001004UL, 0x01A1E84A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001008UL, 0x01210000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001000UL, 0x01A1C469}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000104CUL, 0x01A19278}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001100UL, 0x00000005}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000002CUL, 0x0022C724}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000018UL, 0x00000200}}}
    // RegisterOperation::Read(0x00000014, 0xA0301003, 0xFFFFFFFF)
};

static const struct reg_op gen41_issd_stream_start_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x0000B000UL, 0x000002D9}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00009028UL, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x00000645}}}, // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                        //   bit 0: time_base_enable = 1
                                                                        //   bit 1: time_base_mode = 0
                                                                        //   bit 2: external_mode = 1
                                                                        //   bit 3: external_mode_enable = 0
                                                                        //   bit 4-10: us_counter_max = 0x64
    // Analog start
    {.op = PSEE_WRITE, .args = {.write = {0x0000002CUL, 0x0022C724}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000004UL, 0xF0005442}}}};

static const struct reg_op gen41_issd_stream_stop_seq[] = {
    // Analog stop
    {.op = PSEE_WRITE, .args = {.write = {0x00000004UL, 0xF0005042}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000002CUL, 0x0022C324}}},
    // Digital stop
    // RegisterOperation::Read(0x0000C000, 0x00000005, 0x00000007),
    {.op = PSEE_WRITE, .args = {.write = {0x0000C000UL, 0x00000002}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00009028UL, 0x00000002}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000C000UL, 0x00000005}}},
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x00000644}}}, // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                        //   bit 0: time_base_enable = 0
    {.op = PSEE_WRITE, .args = {.write = {0x0000B000UL, 0x000002D8}}},
    {.op = PSEE_DELAY, .args = {.delay = {300}}}};

static const struct reg_op gen41_issd_stream_destroy_seq[] = {
    // Analog destroy
    {.op = PSEE_WRITE, .args = {.write = {0x00000070UL, 0x00400000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000006CUL, 0x0EE47114}}},
    {.op = PSEE_DELAY, .args = {.delay = {500}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A00CUL, 0x00000400}}},
    {.op = PSEE_DELAY, .args = {.delay = {500}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A010UL, 0x00008068}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001104UL, 0x00000000}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000060}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x000002A0}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00002400}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x000002A0}}},
    // Digital destroy
    {.op = PSEE_WRITE, .args = {.write = {0x0000B044UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_stream)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000000A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x0000000E}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0C8UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_power_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000006}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000004}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x50006442}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x10006442}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B07CUL, 0x00000000}}} // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_7)
};

static const struct issd gen41_issd_stream = {
    .init    = {.ops  = gen41_issd_stream_init_seq,
             .len  = ARRAY_SIZE(gen41_issd_stream_init_seq),
             .name = "gen41_issd_stream_init_seq"},
    .start   = {.ops  = gen41_issd_stream_start_seq,
              .len  = ARRAY_SIZE(gen41_issd_stream_start_seq),
              .name = "gen41_issd_stream_start_seq"},
    .stop    = {.ops  = gen41_issd_stream_stop_seq,
             .len  = ARRAY_SIZE(gen41_issd_stream_stop_seq),
             .name = "gen41_issd_stream_stop_seq"},
    .destroy = {.ops  = gen41_issd_stream_destroy_seq,
                .len  = ARRAY_SIZE(gen41_issd_stream_destroy_seq),
                .name = "gen41_issd_stream_destroy_seq"},
};

static const struct reg_op gen41_issd_timebases_sync_init_seq[] = {
    // Config PLL: pll internal clock = 10 MHz, pll output clock 1600 MHz
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B074UL, 0x00000002}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_5)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B078UL, 0x000000A0}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_6)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x10006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    // Start_PLL
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B068UL, 0x00000004}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_2)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B07CUL, 0x00000003}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_7)
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},  // 1ms delay
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x50006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x5000640A}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    // Start_MIPI @ 800MHz
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B080UL, 0x00000067}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkpost)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B084UL, 0x0000000F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkpre)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B088UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkprepare)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B08CUL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclktrail)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B090UL, 0x000000DF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkzero)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B094UL, 0x00000057}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thsexit)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B098UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thsprepare)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B09CUL, 0x00000067}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thszero)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0A0UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thstrail)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B0A4UL, 0x0000002F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tlpx)

    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0ACUL, 0x00000028}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_txclkesc_freq)
                                                     //   default is 0x28 = 10.0Mhz (the 2 lsb are the decimal part)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0CCUL, 0x00000001}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_pll_div)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B02CUL, 0x000000FF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_line_blanking)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B030UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_blanking)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B034UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_start_delay)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B038UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_end_delay)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B028UL, 0x00004E20}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_period): Frame
                                                     // period in us, default is 0x7d0 (2000), 0x4e20 = 20ms
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL, 0x000003D8}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control)
                                                     //   bit 0: mipi_csi_enable = 0 (set to 1 to start streaming)
                                                     //   bit 1: mipi_sram_powerdown = 0
                                                     //   bit 2: mipi_csi_empty = 0
                                                     //   bit 3: data lane 0 = 1
                                                     //   bit 4: data lane 1 = 1
                                                     //   bit 5 : mipi_packet_timeout_enable = 0
                                                     //   bit 6: line_blanking_ck_lane_lps_enable = 1
                                                     //   bit 7: frame_blanking_ck_lane_lps_enable = 1
                                                     //   bit 8: line_blanking_en = 1
                                                     //   bit 9: frame_blanking_en = 1
                                                     //   bit 10: lane_switch = 0
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000008A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
                                                     //   divider = 0x0a
                                                     //   enable = 1
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B01CUL, 0x0000005E}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_data_identifier)
                                                     //   bits 0-5: data type (default is 0x30: user-defined)
                                                     //   bits 6-7: virtual channel
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B020UL, 0x00004000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_packet_size)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000007}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x00000101}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00002405}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_ana)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x00000101}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg2)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000170}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_cc)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x0000000F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000008A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},   // 200us delay
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0C8UL, 0x00000003}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_power_ctrl)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},   // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000B044UL, 0x00000001}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_stream)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL, 0x000003D9}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control)
                                                     //   'mipi_csi_enable' = '0x00000001'
    // Start_Sensor_clock
    {.op = PSEE_WRITE, .args = {.write = {0x00008000UL, 0x0001E400}}}, // (PSEE/SENSOR_IF/HUAHINE/eoi/cfg_mode_control)
                                                                        //   bit 10: cfg_clk_out_en = 1
                                                                        //   bit 13-20: cfg_clk_timeout = 0xF
    // Configure sensor digital pipeline
    {.op = PSEE_WRITE, .args = {.write = {0x00008000UL, 0x0001E00D}}}, // (PSEE/SENSOR_IF/HUAHINE/eoi/cfg_mode_control)
                                                                        //   bit 0: cfg_control_enable = 1
                                                                        //   bit 2: cfg_bypass_enable = 1
                                                                        //   bit 3: cfg_output_fifo_bypass = 1
                                                                        //   bit 10: cfg_clk_out_en = 0
                                                                        //   bit 13-20: cfg_clk_timeout = 0xF
    {.op = PSEE_WRITE, .args = {.write = {0x00007008UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/edf/control)
    {.op = PSEE_WRITE, .args = {.write = {0x00007000UL, 0x00070001}}}, // (PSEE/SENSOR_IF/HUAHINE/edf/pipeline_control)
    {.op = PSEE_WRITE, .args = {.write = {0x00006000UL, 0x0015541E}}}, // (PSEE/SENSOR_IF/HUAHINE/erc/pipeline_control)
    {.op = PSEE_WRITE, .args = {.write = {0x00006000UL, 0x0015541F}}}, // (PSEE/SENSOR_IF/HUAHINE/erc/pipeline_control)
    {.op = PSEE_WRITE, .args = {.write = {0x0000D000UL, 0x00000005}}}, // (PSEE/SENSOR_IF/HUAHINE/stc/pipeline_control)
    {.op = PSEE_WRITE, .args = {.write = {0x0000C000UL, 0x00000005}}}, // (PSEE/SENSOR_IF/HUAHINE/afk/pipeline_control)
    {.op = PSEE_WRITE, .args = {.write = {0x00009000UL, 0x0000020A}}}, // (PSEE/SENSOR_IF/HUAHINE/ro/readout_ctrl)
                                                                        //    bit 1: ro_td_self_test_en = 1
                                                                        //    bit 3: ro_analog_pipe_en = 1
                                                                        //    bit 9: ro_digital_pipe_en = 1
    {.op = PSEE_WRITE, .args = {.write = {0x00000044UL, 0xCCFFFCCF}}}, // (PSEE/SENSOR_IF/HUAHINE/dig_pad2_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x0000064E}}}, // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                        //   bit 0: time_base_enable = 0
                                                                        //   bit 1: time_base_mode = 1
                                                                        //   bit 2: external_mode = 1
                                                                        //   bit 3: external_mode_enable = 1
                                                                        //   bit 4-10: us_counter_max = 0x64
    {.op = PSEE_WRITE, .args = {.write = {0x00000044UL, 0xCCFCFCCF}}}  // (PSEE/SENSOR_IF/HUAHINE/dig_pad2_ctrl)
};

static const struct reg_op gen41_issd_timebases_sync_start_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x0000064F}}} // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                       //   bit 0: time_base_enable = 1
};

static const struct reg_op gen41_issd_timebases_sync_stop_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x0000064E}}} // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                       //   bit 0: time_base_enable = 0
};

static const struct reg_op gen41_issd_timebases_sync_destroy_seq[] = {
    // Sensor Destroy
    {.op = PSEE_WRITE, .args = {.write = {0x00000070UL, 0x0040002E}}}, // (PSEE/SENSOR_IF/HUAHINE/cp_ctrl2)
    {.op = PSEE_WRITE, .args = {.write = {0x0000006CUL, 0x0EE47114}}}, // (PSEE/SENSOR_IF/HUAHINE/cp_ctrl1)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A00CUL, 0x00000454}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_pix)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A010UL, 0x00008068}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_dig)
    {.op = PSEE_WRITE, .args = {.write = {0x00001104UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/bias/bgen_spr)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000070}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_cc)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x00000100}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg2)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00002404}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_ana)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x00000100}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B044UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_stream)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000000A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x0000000E}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0C8UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_power_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000006}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000004}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x50006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x10006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B07CUL, 0x00000000}}} // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_7)
};

static const struct issd gen41_issd_timebases_sync = {
    .init    = {.ops  = gen41_issd_timebases_sync_init_seq,
             .len  = ARRAY_SIZE(gen41_issd_timebases_sync_init_seq),
             .name = "gen41_issd_timebases_sync_init_seq"},
    .start   = {.ops  = gen41_issd_timebases_sync_start_seq,
              .len  = ARRAY_SIZE(gen41_issd_timebases_sync_start_seq),
              .name = "gen41_issd_timebases_sync_start_seq"},
    .stop    = {.ops  = gen41_issd_timebases_sync_stop_seq,
             .len  = ARRAY_SIZE(gen41_issd_timebases_sync_stop_seq),
             .name = "gen41_issd_timebases_sync_stop_seq"},
    .destroy = {.ops  = gen41_issd_timebases_sync_destroy_seq,
                .len  = ARRAY_SIZE(gen41_issd_timebases_sync_destroy_seq),
                .name = "gen41_issd_timebases_sync_destroy_seq"},
};

static const struct reg_op gen41_issd_test_pattern_init_seq[] = {
    // Config PLL: pll internal clock = 10 MHz, pll output clock 1600 MHz
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B074UL, 0x00000002}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_5)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B078UL, 0x000000A0}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_6)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x10006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    // Start_PLL
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B068UL, 0x00000004}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_2)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B07CUL, 0x00000003}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_7)
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},  // 1ms delay
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x50006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x5000640A}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    // Start_MIPI @ 800MHz
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B080UL, 0x00000067}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkpost)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B084UL, 0x0000000F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkpre)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B088UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkprepare)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B08CUL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclktrail)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B090UL, 0x000000DF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tclkzero)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B094UL, 0x00000057}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thsexit)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B098UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thsprepare)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B09CUL, 0x00000067}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thszero)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0A0UL, 0x00000037}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_thstrail)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B0A4UL, 0x0000002F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_tlpx)

    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0ACUL, 0x00000028}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_txclkesc_freq)
                                                     //   default is 0x28 = 10.0Mhz (the 2 lsb are the decimal part)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0CCUL, 0x00000001}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_pll_div)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B02CUL, 0x000000FF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_line_blanking):
                                                                        // Line blanking period in system clk cycles
    {.op = PSEE_WRITE, .args = {.write = {0x0000B030UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_blanking):
                                                                        // Frame blanking period in system clk cycles
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B034UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_start_delay): Frame
                                                     // Start packet to first data packet spacing in system clk cycles
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B038UL, 0x0000FFFF}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_end_delay): Last
                                                     // data packet to Frame End packet spacing in system clk cycles
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B028UL, 0x00004E20}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_frame_period): Frame
                                                     // period in us, default is 0x7d0 (2000), 0x4e20 = 20ms
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL, 0x000003D8}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control)
                                                     //   bit 0: mipi_csi_enable = 0 (set to 1 to start streaming)
                                                     //   bit 1: mipi_sram_powerdown = 0
                                                     //   bit 2: mipi_csi_empty = 0
                                                     //   bit 3: data lane 0 = 1
                                                     //   bit 4: data lane 1 = 1
                                                     //   bit 5 : mipi_packet_timeout_enable = 0
                                                     //   bit 6: line_blanking_ck_lane_lps_enable = 1
                                                     //   bit 7: frame_blanking_ck_lane_lps_enable = 1
                                                     //   bit 8: line_blanking_en = 1
                                                     //   bit 9: frame_blanking_en = 1
                                                     //   bit 10: lane_switch = 0
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000008A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
                                                     //   divider = 0x0a
                                                     //   enable = 1
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B01CUL, 0x0000005E}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_data_identifier)
                                                     //   bits 0-5: data type (default is 0x30: user-defined)
                                                     //   bits 6-7: virtual channel
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B020UL, 0x00004000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_packet_size)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000007}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x00000101}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00002405}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_ana)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x00000101}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg2)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000170}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_cc)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},                      // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x0000000F}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000008A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},   // 200us delay
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0C8UL, 0x00000003}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_power_ctrl)
    {.op = PSEE_DELAY, .args = {.delay = {200}}},   // 200us delay
    {.op = PSEE_WRITE, .args = {.write = {0x0000B044UL, 0x00000001}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_stream)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL,
                        0x000003D9}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control) - bit 0: mipi_csi_enable
                                       //   bit 0: mipi_csi_enable = 1
    // Test pattern
    {.op = PSEE_WRITE, .args = {.write = {0x00008010UL, 0x00000002}}} // (PSEE/SENSOR_IF/HUAHINE/eoi/cfg_csi_self_test)
};

static const struct reg_op gen41_issd_test_pattern_start_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x00000641}}} // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                       //   bit 0: time_base_enable = 1
                                                                       //   bit 1: time_base_mode = 0
                                                                       //   bit 2: external_mode = 0
                                                                       //   bit 3: external_mode_enable = 0
                                                                       //   bit 4-10: us_counter_max = 0x64
};

static const struct reg_op gen41_issd_test_pattern_stop_seq[] = {
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL, 0x000003D8}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control)
                                                     //   bit 0: time_base_enable = 0
    {.op = PSEE_DELAY, .args = {.delay = {300}}},   // 300us delay
};

static const struct reg_op gen41_issd_test_pattern_destroy_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x00000070UL, 0x0040002E}}}, // (PSEE/SENSOR_IF/HUAHINE/cp_ctrl2)
    {.op = PSEE_WRITE, .args = {.write = {0x0000006CUL, 0x0EE47114}}}, // (PSEE/SENSOR_IF/HUAHINE/cp_ctrl1)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A00CUL, 0x00000454}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_pix)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A010UL, 0x00008068}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_dig)
    {.op = PSEE_WRITE, .args = {.write = {0x00001104UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/bias/bgen_spr)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020UL, 0x00000070}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_cc)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004UL, 0x00000100}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg2)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008UL, 0x00002404}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_ana)
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000UL, 0x00000100}}}, // (PSEE/SENSOR_IF/HUAHINE/ldo/ldo_bg)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B044UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_stream)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B004UL, 0x0000000A}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_escape_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x0000000E}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B0C8UL, 0x00000000}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_dphy_power_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000006}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040UL, 0x00000004}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_power)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x50006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x00000000UL, 0x10006402}}}, // (PSEE/SENSOR_IF/HUAHINE/global_ctrl)
    {.op = PSEE_WRITE, .args = {.write = {0x0000B07CUL, 0x00000000}}} // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_pl_rg_7)
};

static const struct issd gen41_issd_test_pattern = {
    .init    = {.ops  = gen41_issd_test_pattern_init_seq,
             .len  = ARRAY_SIZE(gen41_issd_test_pattern_init_seq),
             .name = "gen41_issd_test_pattern_init_seq"},
    .start   = {.ops  = gen41_issd_test_pattern_start_seq,
              .len  = ARRAY_SIZE(gen41_issd_test_pattern_start_seq),
              .name = "gen41_issd_test_pattern_start_seq"},
    .stop    = {.ops  = gen41_issd_test_pattern_stop_seq,
             .len  = ARRAY_SIZE(gen41_issd_test_pattern_stop_seq),
             .name = "gen41_issd_test_pattern_stop_seq"},
    .destroy = {.ops  = gen41_issd_test_pattern_destroy_seq,
                .len  = ARRAY_SIZE(gen41_issd_test_pattern_destroy_seq),
                .name = "gen41_issd_test_pattern_destroy_seq"},
};

static const struct issd *gen41_issds[] = {
    &gen41_issd_stream,
    &gen41_issd_timebases_sync,
    &gen41_issd_test_pattern,
};

unsigned int gen41_issd(const struct issd **issd_array[])
{
    *issd_array = gen41_issds;
    return ARRAY_SIZE(gen41_issds);
}

#define CCAM5_SYS_ID     0x14

extern int gen4_ctl_init_controls(struct sensor_dev *sensor);
extern const struct media_entity_operations  gen4_sensor_sd_media_ops;
extern const struct v4l2_subdev_internal_ops gen4_sensor_internal_ops;

static int sensor_probe(struct i2c_client *client) {
	struct sensor_dev *sensor;
	u32 sensor_id, misc;
	int ret;

	sensor = devm_kzalloc(&client->dev, sizeof(*sensor), GFP_KERNEL);
	if (!sensor)
		return -ENOMEM;

	mutex_init(&sensor->mutex);

	/* Get pin information from the device tree. */
	sensor->pwdn_gpio = devm_gpiod_get(&client->dev, "pwdn",
						    GPIOD_OUT_HIGH);
	if (IS_ERR(sensor->pwdn_gpio)) {
		dev_err(&client->dev, "failed to request power down pin.");
		goto error_probe;
	}
	sensor->reset_gpio = devm_gpiod_get(&client->dev, "rst",
						     GPIOD_OUT_HIGH);
	if (IS_ERR(sensor->reset_gpio)) {
		dev_err(&client->dev, "failed to request power down pin.");
		goto error_probe;
	}

	/* Power Up and reset the camera. */
	ret = sensor_set_power(sensor, 1);
	if (ret) {
		dev_err(&client->dev, "failed to reset the camera.");
		goto error_probe;
	}

	/* Initialize subdev */
	v4l2_i2c_subdev_init(&sensor->sd, client, &sensor_subdev_ops);

	strscpy(sensor->sd.name, CCAM5_DRV_NAME, sizeof(sensor->sd.name));
	dev_info(&client->dev, "%s Subdev initialized\n", sensor->sd.name);

	sensor->sd.internal_ops = &sensor_internal_ops;
	sensor->sd.flags |= V4L2_SUBDEV_FL_IS_I2C;
	sensor->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;

	sensor->sd.grp_id = 678;
	sensor->sd.entity.ops = &sensor_sd_media_ops;
	sensor->sd.entity.function = MEDIA_ENT_F_CAM_SENSOR;

	/* Initialize source pad */
	sensor->pad.flags = MEDIA_PAD_FL_SOURCE;
	ret = media_entity_pads_init(&sensor->sd.entity, 1, &sensor->pad);
	if (ret) {
		dev_err(&client->dev, "failed to init entity pads: %d.", ret);
		goto error_handler_free;
	}

	/* Identify the camera */
	sensor->issd_idx = issd_idx;
	ret = sensor_read_reg(client, CCAM5_SYS_ID, &sensor_id);
	if (ret) {
		dev_err(&client->dev, "failed to identify the sensor.");
		goto error_probe;
	}

	if (sensor_id == 0xa0301002) {
		dev_info(&client->dev, "Prophesee gen41 ES (id: 0x%x) sensor detected", sensor_id);
		sensor->issd_size =  gen41_issd(&sensor->issd);
		ret = psee_ctl_init_controls(sensor);
	}
	else {
		dev_err(&client->dev, "Not a Prophesee gen41 ES sensor: %d.", sensor_id);
		ret = -ENODEV;
	}
	if (ret)
		goto error_probe;

	ret = v4l2_async_register_subdev(&sensor->sd);
	if (ret) {
		dev_err(&client->dev, "Failed to register subdev.\n");
		goto error_media_entity;
	}

	return 0;

error_media_entity:
	media_entity_cleanup(&sensor->sd.entity);

error_handler_free:
	v4l2_ctrl_handler_free(&sensor->ctrl_handler);

error_probe:
	mutex_destroy(&sensor->mutex);

	return ret;
}

static int sensor_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct sensor_dev *sensor = sd_to_sensor_dev(sd);

	v4l2_async_unregister_subdev(&sensor->sd);
	media_entity_cleanup(&sensor->sd.entity);
	v4l2_ctrl_handler_free(&sensor->ctrl_handler);

	mutex_destroy(&sensor->mutex);

	return 0;
}



static unsigned int issd_idx;
module_param(issd_idx, uint, 0444);
MODULE_PARM_DESC(issd_idx,
                 "PSEE Gen41 sensor driver: default issd_idx=0 for streaming");

#define GEN41_DRV_NAME "psee-gen41"
#define GEN41_DRV_DESC "PSEE Gen41 sensor driver"


static const struct i2c_device_id sensor_id[] = {
        {"psee_gen41", 0},
        {},
};
MODULE_DEVICE_TABLE(i2c, sensor_id);

static const struct of_device_id sensor_dt_of_match[] = {
        { .compatible = "psee,gen41" },
        {},
};
MODULE_DEVICE_TABLE(of, sensor_dt_of_match);

static struct i2c_driver sensor_i2c_driver = {
        .driver = {
                .owner = THIS_MODULE,
                .name  = GEN41_DRV_NAME,
                .of_match_table = of_match_ptr(sensor_dt_of_match),
        },
        .id_table = sensor_id,
        .probe_new = sensor_probe,
        .remove   = sensor_remove,
};

module_i2c_driver(sensor_i2c_driver);

MODULE_AUTHOR("Prophesee");
MODULE_DESCRIPTION(GEN41_DRV_DESC);
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");


