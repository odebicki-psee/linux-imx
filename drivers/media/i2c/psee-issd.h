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

#ifndef _PSEE_ISSD_H_
#define _PSEE_ISSD_H_
#include <linux/kernel.h>
#include <linux/types.h>

enum op_id { PSEE_READ, PSEE_WRITE, PSEE_DELAY };

struct read_op {
    u32 addr;
    u32 data;
    u32 mask;
};

struct write_op {
    u32 addr;
    u32 data;
};

struct delay_op {
    u32 us;
};

struct reg_op {
    enum op_id op;
    union {
        struct read_op read;
        struct write_op write;
        struct delay_op delay;
    } args;
};

struct sequence {
    const struct reg_op *ops;
    size_t len;
    char *name;
};

struct issd {
    const struct sequence init;
    const struct sequence start;
    const struct sequence stop;
    const struct sequence destroy;
};

extern unsigned int gen41_issd(const struct issd **[]);
extern unsigned int imx636_issd(const struct issd **[]);

#endif
