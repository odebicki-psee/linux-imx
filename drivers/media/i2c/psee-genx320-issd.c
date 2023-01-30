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
/* =================================================================================================================== */
/*                                          STREAMING SEQUENCE                                                         */
/* =================================================================================================================== */
static const struct reg_op issd_stream_init_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000320}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00010320}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0200UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000004A0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000004A1}} },
	 
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008E74}} }, // 0x00008E74
	 {.op = PSEE_WRITE, .args = {.write = {0x020CUL, 0x00000033}} },
	 {.op = PSEE_READ, .args = {.read = {0x0214UL, 0x000184A1UL, 0x00010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00000E75}} }, // 0x00000E75
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00000E77}} }, // 0x00000E77
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000C00}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x00000002}} }, // 0x00000002
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000190}} },

/*
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008214}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x020CUL, 0x000000C9}} },
	 {.op = PSEE_READ, .args = {.read = {0x0214, 0x000184A1, 0x00010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008215}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008217}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000C0B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x0000000A}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000320}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00209C40}} },
*/
/* It works at 40 ms with probes and James aside
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008214}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x020CUL, 0x000000C9}} },
	 {.op = PSEE_READ, .args = {.read = {0x0214UL, 0x000190A1UL, 0x00010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008215}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008217}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000C13}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x0000000A}} },
*/ 
	 {.op = PSEE_READ, .args = {.read = {0x0064UL, 0x03E80000UL, 0x00000002}} },
	 {.op = PSEE_READ, .args = {.read = {0x0600UL, 0x6739CE73UL, 0x40000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA000UL, 0x00000406}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007641F}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E22}} },
	 {.op = PSEE_DELAY, .args = {.delay = {100}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007E41F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {300}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x00100000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E20}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} }, // Pkt size: 4096
	                                                                 // Data Type: 0x1E vchannel: 1
	 {.op = PSEE_READ, .args = {.read = {0x0064UL, 0x03E80000UL, 0x00000002}} },
	 {.op = PSEE_READ, .args = {.read = {0x0600UL, 0x6739CE73UL, 0x40000000}} },
	 {.op = PSEE_READ, .args = {.read = {0x0064UL, 0x03E80000UL, 0x00000008}} },
	 {.op = PSEE_READ, .args = {.read = {0x0600UL, 0x6739CE73UL, 0x80000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x00000F02}} }, // 0x00000F02
	                                                                 // Escape clock fivide by 2 the sysclock
	                                                                 // Enable escape clock, STBus, 
	                                                                 //Clock Lane Escape Clock, Data Lane Escape Clock
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x0000001F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB404UL, 0x00000068}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB470UL, 0x0000000D}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB45CUL, 0x00000030}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4E4UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB434UL, 0x0000000C}} }, // 0x0000000C
	 {.op = PSEE_WRITE, .args = {.write = {0xB4A0UL, 0x0000000C}} }, // 0x0000000C
	 {.op = PSEE_WRITE, .args = {.write = {0xB010UL, 0x0000001E}} }, // pkt_timeout disabled
	                                                                 // pkt_fix_rate_en pkt_fix_size_en enabled
	                                                                 // frame_fix_rate_en frame_fix_size_en enabled

/*	 {.op = PSEE_WRITE, .args = {.write = {0xB010UL, 0x0000001A}} }, // pkt_timeout disabled
	                                                                 // pkt_fix_rate_en pkt_fix_size_en disabled
	                                                                 // frame_fix_rate_en frame_fix_size_en enabled
                                                                     */
	 {.op = PSEE_WRITE, .args = {.write = {0xB014UL, 0x00EB104E}} }, // Frame Period: 0x00EB104E = 4174 (fixed packet rate period)
	                                                                 // Frame Size (Nb packets): 235(0xEB)
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB01CUL, 0x0000041D}} }, // Manufacturer ID: Prophesee ID
	 {.op = PSEE_WRITE, .args = {.write = {0x704CUL, 0x0000FFF0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7100UL, 0x00010104}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00B8UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00C0UL, 0x000000B8}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB020UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB024UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007901}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xE000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xC000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xD000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x6000UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x6000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x60A0UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7044UL, 0x00000002}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7048UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7000UL, 0x00000001}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000194}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9000UL, 0x00000200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1218UL, 0x00000111}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1218UL, 0x00000111}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1218UL, 0x00000111}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1220UL, 0x00000001}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA008UL, 0x00008085}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA004UL, 0x00008025}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0070UL, 0x0000055F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1220UL, 0x00000003}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1208UL, 0x00000030}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000420}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000400}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000822}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3000UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3004UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3008UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x300CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3010UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3014UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3018UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x301CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3020UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3024UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3028UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000822}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2000UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2004UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2008UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x200CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2010UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2014UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2018UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x201CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2020UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2024UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3000UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3004UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3008UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x300CUL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3010UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3014UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3018UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x301CUL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3020UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3024UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000822}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1000UL, 0x0301003D}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1004UL, (0x03010000 + 19)}} }, // FO: 0x03010027
	 {.op = PSEE_WRITE, .args = {.write = {0x1008UL, 0x0101003F}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1100UL, 0x03010000}} }, // HPF: 0x03010000
	 {.op = PSEE_WRITE, .args = {.write = {0x1104UL, 0x01090018}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1108UL, 0x01010033}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x110CUL, (0x01010000 + 41)}} }, // DIFF: 0x01090013
	 {.op = PSEE_WRITE, .args = {.write = {0x1110UL, 0x01010039}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1114UL, 0x03010052}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1118UL, 0x03010042}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x111CUL, 0x03000074}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1120UL, 0x010000A4}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1128UL, (0x01090018 + 20)}} }, // DIFF_ON: 0x010000A4
	 {.op = PSEE_WRITE, .args = {.write = {0x1130UL, (0x01090013 + 20)}} }, // DIFF_OFF: 0x010000A4
	 {.op = PSEE_WRITE, .args = {.write = {0x1208UL, 0x00000035}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
};
static const struct reg_op issd_stream_start_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0xB080UL, 0x00000001}} }, // Enable Stats
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007901}} }, // Pkt Size: 4096
	                                                                 // vchannel: 1
	                                                                 // Data Type: 0x1e
	                                                                 // Enable
																	 // frame_sync_en and line_sync_en are disabled
	 {.op = PSEE_WRITE, .args = {.write = {0x9028UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000195}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x002CUL, 0x0022C724}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000C02}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1}} },
};
static const struct reg_op issd_stream_stop_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x002CUL, 0x00200624}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9028UL, 0x00000002}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000194}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_READ, .args = {.read = {0xB000UL, 0x10007906UL, 0x00000004}} },
	 {.op = PSEE_DELAY, .args = {.delay = {250}} },
};
static const struct reg_op issd_stream_destroy_seq[] = {

};
static const struct issd issd_stream = {
	.init = {
		.ops = issd_stream_init_seq,
		.len = ARRAY_SIZE(issd_stream_init_seq),
		.name = "genx320_issd_stream_init_seq"},
	.start = {
		.ops = issd_stream_start_seq,
		.len = ARRAY_SIZE(issd_stream_start_seq),
		.name = "genx320_issd_stream_start_seq"},
	.stop = {
		.ops = issd_stream_stop_seq,
		.len = ARRAY_SIZE(issd_stream_stop_seq),
		.name = "genx320_issd_stream_stop_seq"},
	.destroy = {
		.ops = issd_stream_destroy_seq,
		.len = ARRAY_SIZE(issd_stream_destroy_seq),
		.name = "genx320_issd_stream_destroy_seq"},
};
/* =================================================================================================================== */
/*                                          SPG200                                                         */
/* =================================================================================================================== */
static const struct reg_op spg_200_init_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x0200UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008214}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x020CUL, 0x000000C9}} },
	 {.op = PSEE_READ, .args = {.read = {0x0214UL, 0x000190A1UL, 0x00010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008215}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008217}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000C13}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x0000000A}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x00000F0A}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA000UL, 0x00000406}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007641F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E22}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007E41F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {600}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x00100000}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x000F0000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E20}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x0000001F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB404UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB470UL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB45CUL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4E4UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB434UL, 0x0000000E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4A0UL, 0x0000000E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB020UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB024UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00B8UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00C0UL, 0x000000B8}} }
};
static const struct reg_op spg_200_start_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0xB080UL, 0x00000001}} }, // Enable Stats
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007901}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB0C0UL, 0x00000002}} }
};
static const struct reg_op spg_200_stop_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0xB0C0UL, 0x00000000}} }
};
static const struct reg_op spg_200_destroy_seq[] = {

};
static const struct issd spg_200 = {
	.init = {
		.ops = spg_200_init_seq,
		.len = ARRAY_SIZE(spg_200_init_seq),
		.name = "spg_200_init_seq"},
	.start = {
		.ops = spg_200_start_seq,
		.len = ARRAY_SIZE(spg_200_start_seq),
		.name = "spg_200_start_seq"},
	.stop = {
		.ops = spg_200_stop_seq,
		.len = ARRAY_SIZE(spg_200_stop_seq),
		.name = "spg_200_stop_seq"},
	.destroy = {
		.ops = spg_200_destroy_seq,
		.len = ARRAY_SIZE(spg_200_destroy_seq),
		.name = "spg_200_destroy_seq"},
};
/* =================================================================================================================== */
/*                                          SPG400                                                         */
/* =================================================================================================================== */
static const struct reg_op spg_400_init_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x0200UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008214}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x020CUL, 0x000000C9}} },
	 {.op = PSEE_READ, .args = {.read = {0x0214UL, 0x000190A1UL, 0x00010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008215}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008217}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000C13}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x0000000A}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x00000F0A}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA000UL, 0x00000406}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007641F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E22}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007E41F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {600}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x00100000}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x000F0000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E20}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x0000001F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB404UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB470UL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB45CUL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4E4UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB434UL, 0x0000000E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4A0UL, 0x0000000E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB020UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB024UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00B8UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00C0UL, 0x000000B8}} }
};
static const struct reg_op spg_400_start_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0xB080UL, 0x00000001}} }, // Enable Stats
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007901}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB0C0UL, 0x00000002}} }
};
static const struct reg_op spg_400_stop_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0xB0C0UL, 0x00000000}} }
};
static const struct reg_op spg_400_destroy_seq[] = {

};
static const struct issd spg_400 = {
	.init = {
		.ops = spg_400_init_seq,
		.len = ARRAY_SIZE(spg_400_init_seq),
		.name = "spg_400_init_seq"},
	.start = {
		.ops = spg_400_start_seq,
		.len = ARRAY_SIZE(spg_400_start_seq),
		.name = "spg_400_start_seq"},
	.stop = {
		.ops = spg_400_stop_seq,
		.len = ARRAY_SIZE(spg_400_stop_seq),
		.name = "spg_400_stop_seq"},
	.destroy = {
		.ops = spg_400_destroy_seq,
		.len = ARRAY_SIZE(spg_400_destroy_seq),
		.name = "spg_400_destroy_seq"},
};
/* =================================================================================================================== */
/*                                          SPG800                                                         */
/* =================================================================================================================== */
static const struct reg_op spg_800_init_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x0200UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008214}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000010A1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000009}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x0000000B}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x020CUL, 0x000000C9}} },
	 {.op = PSEE_READ, .args = {.read = {0x0214UL, 0x000190A1UL, 0x00010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008215}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008217}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0208UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000C13}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x0000000A}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x00000F0A}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA000UL, 0x00000406}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007641F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E22}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007E41F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {600}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x00100000}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x000F0000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E20}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x0000001F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB404UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB470UL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB45CUL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4E4UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB434UL, 0x0000000E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4A0UL, 0x0000000E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB020UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB024UL, 0xC0000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00B8UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00C0UL, 0x000000B8}} }
};
static const struct reg_op spg_800_start_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0xB080UL, 0x00000001}} }, // Enable Stats
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007901}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB0C0UL, 0x00000002}} }
};
static const struct reg_op spg_800_stop_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0xB0C0UL, 0x00000000}} }
};
static const struct reg_op spg_800_destroy_seq[] = {

};
static const struct issd spg_800 = {
	.init = {
		.ops = spg_800_init_seq,
		.len = ARRAY_SIZE(spg_800_init_seq),
		.name = "spg_800_init_seq"},
	.start = {
		.ops = spg_800_start_seq,
		.len = ARRAY_SIZE(spg_800_start_seq),
		.name = "spg_800_start_seq"},
	.stop = {
		.ops = spg_800_stop_seq,
		.len = ARRAY_SIZE(spg_800_stop_seq),
		.name = "spg_800_stop_seq"},
	.destroy = {
		.ops = spg_800_destroy_seq,
		.len = ARRAY_SIZE(spg_800_destroy_seq),
		.name = "spg_800_destroy_seq"},
};
/* =================================================================================================================== */
/*                                              TEST PATTERNS                                                          */
/* =================================================================================================================== */
static const struct reg_op issd_ro_spg_init_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000320}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00010320}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0200UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000004A0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0214UL, 0x000004A1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00008E74}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x020CUL, 0x00000033}} },
	 {.op = PSEE_READ, .args = {.read = {0x0214UL, 0x000184A1UL, 0x00010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00000E75}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0204UL, 0x00000E77}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0210UL, 0x00000C00}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x00000002}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000190}} },
	 {.op = PSEE_READ, .args = {.read = {0x0064UL, 0x03E80000UL, 0x00000002}} },
	 {.op = PSEE_READ, .args = {.read = {0x0600UL, 0x6739CE73UL, 0x40000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA000UL, 0x00000406}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007641F}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000010}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E22}} },
	 {.op = PSEE_DELAY, .args = {.delay = {100}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA01CUL, 0x0007E41F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {300}} },
	 {.op = PSEE_READ, .args = {.read = {0xA01CUL, 0x0014E41FUL, 0x00100000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x004CUL, 0x00204E20}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_READ, .args = {.read = {0x0064UL, 0x03E80000UL, 0x00000002}} },
	 {.op = PSEE_READ, .args = {.read = {0x0600UL, 0x6739CE73UL, 0x40000000}} },
	 {.op = PSEE_READ, .args = {.read = {0x0064UL, 0x03E80000UL, 0x00000008}} },
	 {.op = PSEE_READ, .args = {.read = {0x0600UL, 0x6739CE73UL, 0x80000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB00CUL, 0x00000F02}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x00000013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB030UL, 0x0000001F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {199}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB404UL, 0x00000068}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB470UL, 0x0000000D}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB45CUL, 0x00000030}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4E4UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB434UL, 0x0000000C}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB4A0UL, 0x0000000C}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB010UL, 0x0000001E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB014UL, 0x00EB104E}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x704CUL, 0x0000FFF0}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7100UL, 0x00010104}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00B8UL, 0x00000020}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x00C0UL, 0x000000B8}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007901}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xE000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xC000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xD000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x6000UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x6000UL, 0x00000005}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x60A0UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7044UL, 0x00000002}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7048UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x7000UL, 0x00000001}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000194}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9000UL, 0x00000200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x001CUL, 0x00000004}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1218UL, 0x00000111}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1218UL, 0x00000111}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1218UL, 0x00000111}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1220UL, 0x00000001}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA008UL, 0x00008085}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xA004UL, 0x00008025}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0070UL, 0x0000055F}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1220UL, 0x00000003}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1208UL, 0x00000030}} },
	 {.op = PSEE_DELAY, .args = {.delay = {200}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000420}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000400}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000822}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3000UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3004UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3008UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x300CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3010UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3014UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3018UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x301CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3020UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3024UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3028UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000822}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2000UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2004UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2008UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x200CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2010UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2014UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2018UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x201CUL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2020UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x2024UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3000UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3004UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3008UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x300CUL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3010UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3014UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3018UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x301CUL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3020UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x3024UL, 0xFFFFFFFF}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000822}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1000UL, 0x0301003D}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1004UL, 0x03010027}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1008UL, 0x0101003F}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1100UL, 0x03010000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1104UL, 0x01090018}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1108UL, 0x01010033}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x110CUL, 0x01090013}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1110UL, 0x01010039}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1114UL, 0x03010052}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1118UL, 0x03010042}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x111CUL, 0x03000074}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1120UL, 0x010000A4}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x1208UL, 0x00000035}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
};
static const struct reg_op issd_ro_spg_start_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x9000UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007901}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9028UL, 0x00000000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000195}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x002CUL, 0x0022C724}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000C02}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9000UL, 0x00000202}} },
};
static const struct reg_op issd_ro_spg_stop_seq[] = {
	 {.op = PSEE_WRITE, .args = {.write = {0x0000UL, 0x00000802}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x002CUL, 0x00200624}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9028UL, 0x00000002}} },
	 {.op = PSEE_DELAY, .args = {.delay = {1000}} },
	 {.op = PSEE_WRITE, .args = {.write = {0x9008UL, 0x00000194}} },
	 {.op = PSEE_WRITE, .args = {.write = {0xB000UL, 0x10007900}} },
	 {.op = PSEE_READ, .args = {.read = {0xB000UL, 0x10007906UL, 0x00000004}} },
	 {.op = PSEE_DELAY, .args = {.delay = {250}} },
};
static const struct reg_op issd_ro_spg_destroy_seq[] = {

};
static const struct issd issd_ro_spg = {
	.init = {
		.ops = issd_ro_spg_init_seq,
		.len = ARRAY_SIZE(issd_ro_spg_init_seq),
		.name = "genx320_issd_ro_destroy_seq"},
	.start = {
		.ops = issd_ro_spg_start_seq,
		.len = ARRAY_SIZE(issd_ro_spg_start_seq),
		.name = "genx320_issd_ro_destroy_seq"},
	.stop = {
		.ops = issd_ro_spg_stop_seq,
		.len = ARRAY_SIZE(issd_ro_spg_stop_seq),
		.name = "genx320_issd_ro_destroy_seq"},
	.destroy = {
		.ops = issd_ro_spg_destroy_seq,
		.len = ARRAY_SIZE(issd_ro_spg_destroy_seq),
		.name = "genx320_issd_ro_destroy_seq"},
};
static const struct issd *issds[] = {
	&issd_stream,
	&issd_ro_spg,
};

unsigned int genx320_issd(const struct issd **issd_array[])
{
    *issd_array = issds;
    return ARRAY_SIZE(issds);
}

