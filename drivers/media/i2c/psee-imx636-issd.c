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

/* ========================================================================================================================================== */
/*                                                             IMX636 ISSDS                                                                    */
/* ========================================================================================================================================== */

/* ------------------------------------------------------------------------------------------------------------------- */
/*                                          STREAMING SEQUENCES                                                         */
/* ------------------------------------------------------------------------------------------------------------------- */
static const struct reg_op imx636_issd_stream_init_seq[] = {
  // While loop
  // Read and check add=0x00400010, rdata=0xCAFEBABE
  //{.op = PSEE_WRITE, .args = {.write = {0x0000001C, 0x00000001}}},
  //{.op = PSEE_DELAY, .args = {.delay = {1000000}}},
  {.op = PSEE_READ, .args = {.read = {0x00400010, 0xCAFEBABE, 0xFFFFFFFF}}},
  {.op = PSEE_READ, .args = {.read = {0x0000B01C, 0x0000000, 0x0}}},

//{.op = PSEE_WRITE, .args = {.write = {0x00400004, 0x00000001}}},
//{.op = PSEE_DELAY, .args = {.delay = {500000}}},
//{.op = PSEE_WRITE, .args = {.write = {0x00400004, 0x00000000}}},
//{.op = PSEE_DELAY, .args = {.delay = {1000000}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x00000158}}},
//{.op = PSEE_DELAY, .args = {.delay = {300}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B044, 0x00000000}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B004, 0x0000000A}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x00000000}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B0C8, 0x00000000}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x00000000}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x00000000}}},
  {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x4F006442}}},
  {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x0F006442}}},
  {.op = PSEE_WRITE, .args = {.write = {0x000000B8, 0x00000400}}},
  {.op = PSEE_WRITE, .args = {.write = {0x000000B8, 0x00000400}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B07C, 0x00000000}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B074, 0x00000002}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B078, 0x000000A0}}},
  {.op = PSEE_WRITE, .args = {.write = {0x000000C0, 0x00000110}}},
  {.op = PSEE_WRITE, .args = {.write = {0x000000C0, 0x00000210}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B120, 0x00000001}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000E120, 0x00000000}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B068, 0x00000004}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B07C, 0x00000001}}},
  {.op = PSEE_DELAY, .args = {.delay = {10}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B07C, 0x00000003}}},
  {.op = PSEE_DELAY, .args = {.delay = {1000}}},
  {.op = PSEE_WRITE, .args = {.write = {0x000000B8, 0x00000401}}},
  {.op = PSEE_WRITE, .args = {.write = {0x000000B8, 0x00000409}}},
  {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x4F006442}}},
  {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x4F00644A}}}, // 100MHz sys_clk = 100MHz
  {.op = PSEE_WRITE, .args = {.write = {0x0000B080, 0x00000077}}}, // mipi global operation TCLK POST time control
  {.op = PSEE_WRITE, .args = {.write = {0x0000B084, 0x0000000F}}}, // mipi global operation TCLK PRE time control 
  {.op = PSEE_WRITE, .args = {.write = {0x0000B088, 0x00000037}}}, // mipi global operation TCLK PREPARE time control 
  {.op = PSEE_WRITE, .args = {.write = {0x0000B08C, 0x00000037}}}, // mipi global operation TCLK TRAIL time control 
  {.op = PSEE_WRITE, .args = {.write = {0x0000B090, 0x000000DF}}}, //** mipi global operation TCLK ZERO time control
  {.op = PSEE_WRITE, .args = {.write = {0x0000B094, 0x00000057}}}, // mipi global operation TCLK EXIT time control
  {.op = PSEE_WRITE, .args = {.write = {0x0000B098, 0x00000037}}}, // mipi global operation THS PREPARE time control  
  {.op = PSEE_WRITE, .args = {.write = {0x0000B09C, 0x00000067}}}, // **mipi global operation THS ZERO time control
  {.op = PSEE_WRITE, .args = {.write = {0x0000B0A0, 0x00000037}}}, // mipi global operation THS TRAIL time control 
  {.op = PSEE_WRITE, .args = {.write = {0x0000B0A4, 0x0000002F}}}, // mipi global operation TLPX time control 
  {.op = PSEE_WRITE, .args = {.write = {0x0000B0AC, 0x00000028}}}, // ** TXCLKESC freq
  {.op = PSEE_WRITE, .args = {.write = {0x0000B0CC, 0x00000001}}}, // DPHY clock divider: 2
  //{.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x000003D8}}}, // line + frame blanking, clock lane in LPS for line and frame blanking, Data lane 1 & 2
  {.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x000003C8}}}, // line + frame blanking, clock lane in LPS for line and frame blanking, Data lane 1
  {.op = PSEE_WRITE, .args = {.write = {0x0000B004, 0x0000008A}}}, // escape clock = system clock / 10
  {.op = PSEE_WRITE, .args = {.write = {0x0000B01C, 0x0000005E}}}, // *** Data Itentifier: Modified for i.MX8 (0x30 by default)
  {.op = PSEE_WRITE, .args = {.write = {0x0000B020, 0x00001000}}}, // Packet size Max: 16KB 0x00004000
//{.op = PSEE_WRITE, .args = {.write = {0x0000B02C, 0x000000FF}}},
//{.op = PSEE_WRITE, .args = {.write = {0x0000B030, 0x00003E80}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B028, 0x00004E20}}}, // Frame period in us, default is 0x7d0 (2000), 0x4e20 = 20ms
/*  
  {.op = PSEE_WRITE, .args = {.write = {0x0000A000, 0x000B0501}}},
  {.op = PSEE_DELAY, .args = {.delay = {200}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000A008, 0x00002405}}},
  {.op = PSEE_DELAY, .args = {.delay = {200}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000A004, 0x000B0501}}},
  {.op = PSEE_DELAY, .args = {.delay = {200}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000A020, 0x00000150}}},
  {.op = PSEE_DELAY, .args = {.delay = {200}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x00000007}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B064, 0x00000006}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x0000000F}}},
  {.op = PSEE_DELAY, .args = {.delay = {100}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B004, 0x0000008A}}},
  {.op = PSEE_DELAY, .args = {.delay = {200}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000B0C8, 0x00000003}}},
  {.op = PSEE_DELAY, .args = {.delay = {200}}},
*/
  {.op = PSEE_WRITE, .args = {.write = {0x0000B044, 0x00000001}}}, // DPHY Startup signal
  //{.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x000003D9}}}, // csi enable, line + frame blanking, clock lane in LPS for line and frame blanking, Data lane 1 & 2
  {.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x000003C9}}}, // csi enable, line + frame blanking, clock lane in LPS for line and frame blanking, Data lane 1
  {.op = PSEE_WRITE, .args = {.write = {0x00009008, 0x00000644}}}, // time base is enabled 1us tick = pll / 100
  {.op = PSEE_WRITE, .args = {.write = {0x00000004, 0xF0005042}}}, // ??? ROI programming: ROI slope 10ns
  {.op = PSEE_WRITE, .args = {.write = {0x00000018, 0x00000200}}}, // ??? Spare control
  {.op = PSEE_WRITE, .args = {.write = {0x00001014, 0x11A1504D}}}, // load immediate, idac, mux, vdac, buf
  {.op = PSEE_WRITE, .args = {.write = {0x00009004, 0x00000000}}}, // Delay to sample a new line
  {.op = PSEE_DELAY, .args = {.delay = {1000}}},

  // Enable MIPI meta data at the end of the frame
  {.op = PSEE_WRITE, .args = {.write = {0x00008000, 0x1E085 | (1 << 22) |(1 << 21)}}},
  {.op = PSEE_WRITE, .args = {.write = {0x00008018, 0xE5A55A5A}}},
  {.op = PSEE_WRITE, .args = {.write = {0x0000801c, 0xEA55A5A5}}},

};

static const struct reg_op imx636_issd_stream_start_seq[] = {
  {.op = PSEE_WRITE, .args = {.write = {0x0000B100UL, 0x000001}}}, // Enable stat counter 
//  {.op = PSEE_WRITE, .args = {.write = {0x00008010, 0x00000002}}}, // Enable the MIPI pattern generator
  {.op = PSEE_WRITE, .args = {.write = {0x00009000UL, 0x00000208}}}, 
  //{.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x000003D9}}},                        // 0x119 in SSS product spec
  {.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x000003C9}}},                        // 0x119 in SSS product spec
  {.op = PSEE_WRITE, .args = {.write = {0x00009028, 0x00000000}}},
  {.op = PSEE_WRITE, .args = {.write = {0x00009008, 0x00000645}}}, //RegisterOperation::WriteField(0x00009008, 0x645, 0x00000001)}}}
  // Analog START
  {.op = PSEE_WRITE, .args = {.write = {0x0000002C, 0x0022C724}}},
  {.op = PSEE_WRITE, .args = {.write = {0x00000004, 0xF0005442}}},

};

static const struct reg_op imx636_issd_stream_stop_seq[] = {
    
    // Analog STOP
    {.op = PSEE_WRITE, .args = {.write = {0x00000004, 0xF0005042}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000002C, 0x0022C324}}},
/*
    // Digital STOP
    {.op = PSEE_WRITE, .args = {.write = {0x00009028, 0x00000002}}},
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},
*/    
    {.op = PSEE_WRITE, .args = {.write = {0x00009008, 0x00000644}}}, // RegisterOperation::WriteField(0x00009008, 0x00000644, 0x0000}}}
/*    
    {.op = PSEE_WRITE, .args = {.write = {0x0000B000, 0x000002F8}}},
    */
    {.op = PSEE_DELAY, .args = {.delay = {300}}},
      {.op = PSEE_WRITE, .args = {.write = {0x0000B100UL, 0x000004}}}, // Disable stat counter 

};

static const struct reg_op imx636_issd_stream_destroy_seq[] = {
/*    
    // Analog DESTROY
    {.op = PSEE_WRITE, .args = {.write = {0x00000070, 0x00400008}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000006C, 0x0EE47114}}},
    {.op = PSEE_DELAY, .args = {.delay = {500}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A00C, 0x00020400}}},
    {.op = PSEE_DELAY, .args = {.delay = {500}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A010, 0x00008068}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00001104, 0x00000000}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A020, 0x00000050}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A004, 0x000B0500}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A008, 0x00002404}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000A000, 0x000B0500}}},
    // Digital DESTROY
    {.op = PSEE_WRITE, .args = {.write = {0x0000B044, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000B004, 0x0000000A}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x0000000E}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000B0C8, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x00000006}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000B040, 0x00000004}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x4F006442}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x0F006442}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000B8, 0x00000401}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000B8, 0x00000400}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000B07C, 0x00000000}}}
    */
};

static const struct issd imx636_issd_stream = {
    .init    = {.ops  = imx636_issd_stream_init_seq,
             .len  = ARRAY_SIZE(imx636_issd_stream_init_seq),
             .name = "imx636_issd_stream_init_seq"},
    .start   = {.ops  = imx636_issd_stream_start_seq,
              .len  = ARRAY_SIZE(imx636_issd_stream_start_seq),
              .name = "imx636_issd_stream_start_seq"},
    .stop    = {.ops  = imx636_issd_stream_stop_seq,
             .len  = ARRAY_SIZE(imx636_issd_stream_stop_seq),
             .name = "imx636_issd_stream_stop_seq"},
    .destroy = {.ops  = imx636_issd_stream_destroy_seq,
                .len  = ARRAY_SIZE(imx636_issd_stream_destroy_seq),
                .name = "imx636_issd_stream_destroy_seq"},
};


/* ------------------------------------------------------------------------------------------------------------------- */
/*                                              ??? SEQUENCES                                                          */
/* ------------------------------------------------------------------------------------------------------------------- */
static const struct reg_op imx636_issd_timebases_sync_init_seq[] = {
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

static const struct reg_op imx636_issd_timebases_sync_start_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x0000064F}}} // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                       //   bit 0: time_base_enable = 1
};

static const struct reg_op imx636_issd_timebases_sync_stop_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x0000064E}}} // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                       //   bit 0: time_base_enable = 0
};

static const struct reg_op imx636_issd_timebases_sync_destroy_seq[] = {
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

static const struct issd imx636_issd_timebases_sync = {
    .init    = {.ops  = imx636_issd_timebases_sync_init_seq,
             .len  = ARRAY_SIZE(imx636_issd_timebases_sync_init_seq),
             .name = "imx636_issd_timebases_sync_init_seq"},
    .start   = {.ops  = imx636_issd_timebases_sync_start_seq,
              .len  = ARRAY_SIZE(imx636_issd_timebases_sync_start_seq),
              .name = "imx636_issd_timebases_sync_start_seq"},
    .stop    = {.ops  = imx636_issd_timebases_sync_stop_seq,
             .len  = ARRAY_SIZE(imx636_issd_timebases_sync_stop_seq),
             .name = "imx636_issd_timebases_sync_stop_seq"},
    .destroy = {.ops  = imx636_issd_timebases_sync_destroy_seq,
                .len  = ARRAY_SIZE(imx636_issd_timebases_sync_destroy_seq),
                .name = "imx636_issd_timebases_sync_destroy_seq"},
};

/* ------------------------------------------------------------------------------------------------------------------- */
/*                                              TEST SEQUENCES                                                          */
/* ------------------------------------------------------------------------------------------------------------------- */
static const struct reg_op imx636_issd_test_pattern_init_seq[] = {
    /* Pll power down and switch */
    {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x4f006402}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000b8, 0x00000400}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000b8, 0x00000400}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b07c, 0x00000000}}},
    /* Init */
    {.op = PSEE_WRITE, .args = {.write = {0x0000b074, 0x00000002}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b078, 0x000000a0}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000c0, 0x00000120}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000c0, 0x00000120}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b120, 0x00000001}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000e120, 0x00000000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b068, 0x00000004}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b07c, 0x00000001}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b07c, 0x00000003}}},
    {.op = PSEE_DELAY, .args = {.delay = {1000}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000b8, 0x00000401}}},
    {.op = PSEE_WRITE, .args = {.write = {0x000000b8, 0x00000409}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x4f006402}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00000000, 0x4f00640a}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b080, 0x00000077}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b084, 0x0000000f}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b088, 0x00000037}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b08c, 0x00000037}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b090, 0x000000df}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b094, 0x00000057}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b098, 0x00000037}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b09c, 0x00000067}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b0a0, 0x00000037}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b0a4, 0x0000002f}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b0ac, 0x00000028}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b0cc, 0x00000001}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b000, 0x000003f8}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b004, 0x0000008a}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b01c, 0x0000005e}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b020, 0x00004000}}},
    {.op = PSEE_DELAY, .args = {.delay = {100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000a000, 0x000b0501}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000a008, 0x00002405}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000a004, 0x000b0501}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000a020, 0x00000150}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b040, 0x00000007}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b064, 0x00000006}}},
    {.op = PSEE_DELAY, .args = {.delay = {100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b040, 0x0000000f}}},
    {.op = PSEE_DELAY, .args = {.delay = {100}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b004, 0x0000008a}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b0c8, 0x00000003}}},
    {.op = PSEE_DELAY, .args = {.delay = {200}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b044, 0x00000001}}},
    {.op = PSEE_WRITE, .args = {.write = {0x0000b000, 0x000003f9}}},
    {.op = PSEE_WRITE, .args = {.write = {0x00008010, 0x00000002}}}
};

static const struct reg_op imx636_issd_test_pattern_start_seq[] = {
    {.op = PSEE_WRITE, .args = {.write = {0x0000B100UL, 0x000001}}}, // Enable stat counter 

    {.op = PSEE_WRITE, .args = {.write = {0x00009008UL, 0x00000641}}} // (PSEE/SENSOR_IF/HUAHINE/ro/time_base_ctrl)
                                                                       //   bit 0: time_base_enable = 1
                                                                       //   bit 1: time_base_mode = 0
                                                                       //   bit 2: external_mode = 0
                                                                       //   bit 3: external_mode_enable = 0
                                                                       //   bit 4-10: us_counter_max = 0x64
};

static const struct reg_op imx636_issd_test_pattern_stop_seq[] = {
    {.op   = PSEE_WRITE,
     .args = {.write = {0x0000B000UL, 0x000003D8}}}, // (PSEE/SENSOR_IF/HUAHINE/mipi_csi/mipi_control)
                                                     //   bit 0: time_base_enable = 0
    {.op = PSEE_DELAY, .args = {.delay = {300}}},   // 300us delay
};

static const struct reg_op imx636_issd_test_pattern_destroy_seq[] = {
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

static const struct issd imx636_issd_test_pattern = {
    .init    = {.ops  = imx636_issd_test_pattern_init_seq,
             .len  = ARRAY_SIZE(imx636_issd_test_pattern_init_seq),
             .name = "imx636_issd_test_pattern_init_seq"},
    .start   = {.ops  = imx636_issd_test_pattern_start_seq,
              .len  = ARRAY_SIZE(imx636_issd_test_pattern_start_seq),
              .name = "imx636_issd_test_pattern_start_seq"},
    .stop    = {.ops  = imx636_issd_test_pattern_stop_seq,
             .len  = ARRAY_SIZE(imx636_issd_test_pattern_stop_seq),
             .name = "imx636_issd_test_pattern_stop_seq"},
    .destroy = {.ops  = imx636_issd_test_pattern_destroy_seq,
                .len  = ARRAY_SIZE(imx636_issd_test_pattern_destroy_seq),
                .name = "imx636_issd_test_pattern_destroy_seq"},
};

static const struct issd *imx636_issds[] = {
    &imx636_issd_stream,
    &imx636_issd_timebases_sync,
    &imx636_issd_test_pattern,
};

unsigned int imx636_issd(const struct issd **issd_array[])
{
    *issd_array = imx636_issds;
    return ARRAY_SIZE(imx636_issds);
}
