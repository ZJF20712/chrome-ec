/* Copyright 2019 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Kukui board configuration */

#ifndef __CROS_EC_BASEBOARD_H
#define __CROS_EC_BASEBOARD_H

/*
 * Variant battery defines, pick one:
 * VARIANT_KUKUI_BATTERY_MAX17055
 * VARIANT_KUKUI_BATTERY_MM8013
 * VARIANT_KUKUI_BATTERY_SMART
 */
#if defined(VARIANT_KUKUI_BATTERY_MAX17055)
#define CONFIG_BATTERY_MAX17055
#define CONFIG_BATTERY_MAX17055_ALERT
#define BATTERY_MAX17055_RSENSE             5     /* m-ohm */
#elif defined(VARIANT_KUKUI_BATTERY_MM8013)
#define CONFIG_BATTERY_MM8013
#elif defined(VARIANT_KUKUI_BATTERY_SMART)
#define CONFIG_BATTERY_SMART
#define CONFIG_BATTERY_FUEL_GAUGE
#else
#error Must define a VARIANT_KUKUI_BATTERY
#endif /* VARIANT_KUKUI_BATTERY */

/*
 * Variant charger defines, pick one:
 * VARIANT_KUKUI_CHARGER_MT6370
 * VARIANT_KUKUI_CHARGER_ISL9238
 */
#if defined(VARIANT_KUKUI_CHARGER_MT6370)
#define CONFIG_CHARGER_MT6370
#define CONFIG_CHARGER_MT6370_BC12_GPIO
#define CONFIG_CHARGE_RAMP_HW
#define CONFIG_CHARGER_OTG
#define CONFIG_CHARGER_PROFILE_OVERRIDE
#define CONFIG_USB_PD_TCPM_MT6370
#define CONFIG_USB_PD_TCPC_LOW_POWER
#define CONFIG_USB_PD_DISCHARGE_TCPC
#define CONFIG_USB_PD_DUAL_ROLE_AUTO_TOGGLE
/*
 * The Maximum input voltage is 13.5V, need another 5% tolerance.
 * 12.85V * 1.05 = 13.5V
 */
#define PD_MAX_VOLTAGE_MV 12850
#define CONFIG_USB_PD_PREFER_MV
#elif defined(VARIANT_KUKUI_CHARGER_ISL9238)
#define CONFIG_CHARGER_ISL9238
#define CONFIG_CHARGER_SENSE_RESISTOR_AC 20 /* BOARD_RS1 */
#define CONFIG_CHARGER_SENSE_RESISTOR 10 /* BOARD_RS2 */
#define CONFIG_CHARGER_OTG
#define CONFIG_CHARGE_RAMP_HW
#define PD_MAX_VOLTAGE_MV 20000
#else
#error Must define a VARIANT_KUKUI_CHARGER
#endif /* VARIANT_KUKUI_CHARGER */

/*
 * Variant pogo defines, if pick, VARIANT_KUKUI_POGO_KEYBOARD is mandatory
 * VARIANT_KUKUI_POGO_KEYBOARD
 * VARIANT_KUKUI_POGO_DOCK
 */
#ifdef VARIANT_KUKUI_POGO_DOCK
#ifndef VARIANT_KUKUI_POGO_KEYBOARD
#error VARIANT_KUKUI_POGO_KEYBOARD is mandatory if use dock
#endif /* !VARIANT_KUKUI_POGO_KEYBOARD */
#undef CONFIG_DEDICATED_CHARGE_PORT_COUNT
#define CONFIG_DEDICATED_CHARGE_PORT_COUNT 1
#define DEDICATED_CHARGE_PORT 1
#endif /* VARIANT_KUKUI_POGO_DOCK */

#ifdef VARIANT_KUKUI_POGO_KEYBOARD
#define CONFIG_DETACHABLE_BASE
#define CONFIG_BASE_ATTACHED_SWITCH
#endif

/*
 * Define this flag if board controls dp mux via gpio pins USB_C0_DP_OE_L and
 * USB_C0_DP_POLARITY.
 *
 * board must provide function board_set_dp_mux_control(output_enable, polarity)
 *
 * #define VARIANT_KUKUI_DP_MUX_GPIO
 */

/* Optional modules */
#define CONFIG_ADC
#undef  CONFIG_ADC_WATCHDOG
#define CONFIG_CHIPSET_MT8183
#define CONFIG_CMD_ACCELS
#define CONFIG_EMULATED_SYSRQ
#undef  CONFIG_HIBERNATE
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
#define CONFIG_I2C_VIRTUAL_BATTERY
#define CONFIG_I2C_PASSTHRU_RESTRICTED
#define CONFIG_LED_COMMON
#define CONFIG_LOW_POWER_IDLE
#define CONFIG_POWER_COMMON
#define CONFIG_SPI
#define CONFIG_SPI_MASTER
#define CONFIG_STM_HWTIMER32
#define CONFIG_SWITCH
#define CONFIG_WATCHDOG_HELP

#define CONFIG_SYSTEM_UNLOCKED /* Allow dangerous commands for testing */

#undef  CONFIG_UART_CONSOLE
#define CONFIG_UART_CONSOLE 1
#define CONFIG_UART_RX_DMA

/* Bootblock */
#ifdef SECTION_IS_RO
#define CONFIG_BOOTBLOCK

#define EMMC_SPI_PORT 2
#endif

/* Optional features */
#define CONFIG_BOARD_PRE_INIT
#define CONFIG_BOARD_VERSION_CUSTOM
#define CONFIG_BUTTON_TRIGGERED_RECOVERY
#define CONFIG_CHARGER_ILIM_PIN_DISABLED
#define CONFIG_FORCE_CONSOLE_RESUME
#define CONFIG_HOST_COMMAND_STATUS
#define CONFIG_CMD_AP_RESET_LOG
#define CONFIG_PRESERVE_LOGS

/* Required for FAFT */
#define CONFIG_CMD_BUTTON
#define CONFIG_CMD_CHARGEN

/* By default, set hcdebug to off */
#undef CONFIG_HOSTCMD_DEBUG_MODE
#define CONFIG_HOSTCMD_DEBUG_MODE HCDEBUG_OFF
#define CONFIG_SUPPRESSED_HOST_COMMANDS \
	EC_CMD_CONSOLE_SNAPSHOT, EC_CMD_CONSOLE_READ, EC_CMD_MOTION_SENSE_CMD

#define CONFIG_LTO
#define CONFIG_POWER_BUTTON
#define CONFIG_POWER_BUTTON_IGNORE_LID
#define CONFIG_POWER_TRACK_HOST_SLEEP_STATE
#define CONFIG_SOFTWARE_PANIC
#define CONFIG_VBOOT_HASH

#define CONFIG_CHARGER
#define CONFIG_CHARGER_INPUT_CURRENT 512
#define CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON 2
#define CONFIG_CHARGER_LIMIT_POWER_THRESH_BAT_PCT 2
#define CONFIG_CHARGER_LIMIT_POWER_THRESH_CHG_MW 15000
#define CONFIG_CHARGER_DISCHARGE_ON_AC
#define CONFIG_CHARGER_DISCHARGE_ON_AC_CUSTOM
#define CONFIG_USB_CHARGER

/* Increase tx buffer size, as we'd like to stream EC log to AP. */
#undef CONFIG_UART_TX_BUF_SIZE
#define CONFIG_UART_TX_BUF_SIZE 4096

/* To be able to indicate the device is in tablet mode. */
#define CONFIG_TABLET_MODE
#define GPIO_LID_OPEN GPIO_HALL_INT_L

#ifndef VARIANT_KUKUI_NO_SENSORS
#define CONFIG_ACCEL_FIFO
/* FIFO size is in power of 2. */
#define CONFIG_ACCEL_FIFO_SIZE 256
#define CONFIG_ACCEL_FIFO_THRES (CONFIG_ACCEL_FIFO_SIZE / 3)
#endif /* VARIANT_KUKUI_NO_SENSORS */

#ifndef VARIANT_KUKUI_TABLET_PWRBTN
#define POWERBTN_BOOT_DELAY 0
#endif

/* USB PD config */
#define CONFIG_CHARGE_MANAGER
#define CONFIG_USB_POWER_DELIVERY
#define CONFIG_USB_PD_ALT_MODE
#define CONFIG_USB_PD_ALT_MODE_DFP
#define CONFIG_USB_PD_DUAL_ROLE
#define CONFIG_USB_PD_LOGGING
#define CONFIG_USB_PD_PORT_MAX_COUNT 1
#define CONFIG_USB_PD_TCPM_TCPCI
#define CONFIG_USB_PD_VBUS_DETECT_TCPC
#define CONFIG_USB_PD_5V_EN_CUSTOM
#define CONFIG_USBC_SS_MUX
#define CONFIG_USBC_VCONN
#define CONFIG_USBC_VCONN_SWAP
#define CONFIG_USB_PD_COMM_LOCKED

#define CONFIG_BATTERY_CRITICAL_SHUTDOWN_CUT_OFF
#define CONFIG_BATTERY_CUT_OFF
#define CONFIG_BATTERY_PRESENT_CUSTOM
#define CONFIG_BATTERY_REVIVE_DISCONNECT

#define PD_MAX_POWER_MW       ((PD_MAX_VOLTAGE_MV * PD_MAX_CURRENT_MA) / 1000)
#ifdef BOARD_KODAMA
#define PD_MAX_CURRENT_MA     2000
#else
#define PD_MAX_CURRENT_MA     3000
#endif

#define PD_POWER_SUPPLY_TURN_ON_DELAY  30000  /* us */
#define PD_POWER_SUPPLY_TURN_OFF_DELAY 50000  /* us */
#define PD_VCONN_SWAP_DELAY 5000 /* us */

/* Timer selection */
#define TIM_CLOCK32  2
#define TIM_WATCHDOG 7

/* 48 MHz SYSCLK clock frequency */
#define CPU_CLOCK 48000000

/* Optional for testing */
#undef CONFIG_PECI
#undef CONFIG_PSTORE

/* Modules we want to exclude */
#undef CONFIG_CMD_BATTFAKE
#undef CONFIG_CMD_FLASH
#undef CONFIG_CMD_HASH
#undef CONFIG_CMD_MD
#undef CONFIG_CMD_POWERINDEBUG
#undef CONFIG_CMD_TIMERINFO

#ifdef SECTION_IS_RO
#undef CONFIG_CMD_APTHROTTLE
#undef CONFIG_CMD_MMAPINFO
#undef CONFIG_CMD_PWR_AVG
#undef CONFIG_CMD_REGULATOR
#undef CONFIG_CMD_RW
#undef CONFIG_CMD_SHMEM
#undef CONFIG_CMD_SLEEPMASK
#undef CONFIG_CMD_SLEEPMASK_SET
#undef CONFIG_CMD_SYSLOCK
#undef CONFIG_HOSTCMD_FLASHPD
#undef CONFIG_HOSTCMD_RWHASHPD
#undef CONFIG_CONSOLE_CMDHELP
#endif

#define CONFIG_TASK_PROFILING

#define CONFIG_KEYBOARD_PROTOCOL_MKBP
#define CONFIG_MKBP_EVENT
#define CONFIG_MKBP_USE_GPIO

#ifndef __ASSEMBLER__
#ifdef VARIANT_KUKUI_DP_MUX_GPIO
void board_set_dp_mux_control(int output_enable, int polarity);
#endif /* VARIANT_KUKUI_DP_MUX_GPIO */

/* If POGO pin is providing power. */
int kukui_pogo_extpower_present(void);

#endif /* !__ASSEMBLER__ */

#endif /* __CROS_EC_BASEBOARD_H */
