/* Copyright 2024 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef PTLRVP_MCHP_BOARD_H_
#define PTLRVP_MCHP_BOARD_H_

/* Power Signals */
#define PWR_EN_PP3300_S5 MCHP_GPIO_DECODE_226
#define PWR_EN_PP5000_S5 MCHP_GPIO_DECODE_244
#define PWR_RSMRST_PWRGD MCHP_GPIO_DECODE_227
#define PWR_EC_PCH_RSMRST MCHP_GPIO_DECODE_055
#define PWR_SLP_S0 MCHP_GPIO_DECODE_052
#define PWR_PCH_PWROK MCHP_GPIO_DECODE_100
#define PWR_ALL_SYS_PWRGD MCHP_GPIO_DECODE_057
#define PWR_SYS_RST MCHP_GPIO_DECODE_224

/* I2C Ports */
/* TODO: Eliminate the I2C macros b/384773566 */
#define CHARGER_I2C i2c_smb_0
#define PD_POW_I2C i2c_smb_1

/* PD Interrupts */
#define PD_POW_IRQ_GPIO MCHP_GPIO_DECODE_143

#endif /* PTLRVP_MCHP_BOARD_H_ */
