/* Copyright 2020 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CONFIG_CHIP_H
#define __CROS_EC_CONFIG_CHIP_H

#include <devicetree.h>

/*
 * The battery enum is used in various drivers and these assume that it is
 * always available (defined in board.h). With Zephyr we don't include board.h
 * so we have a battery_enum.h header in the shim which defines
 * enum battery_type based on settings in the device tree. Include that here.
 */
#ifdef CONFIG_PLATFORM_EC_BATTERY
#include "battery_enum.h"
#endif

/*
 * This file translates Kconfig options to platform/ec options.
 *
 * Options which are from Zephyr platform/ec module (Kconfig) start
 * with CONFIG_PLATFORM_EC_, and can be found in the Kconfig file.
 *
 * Options which are for the platform/ec configuration can be found in
 * common/config.h.
 */

/*
 * Obsolete configs - these are options that are not needed, either because
 * Zephyr features directly replace the option, or because the config option
 * will not be used with Zephyr OS.
 */

/*
 * ROM resident support. The ROM resident capabilities in the Chromium OS
 * code are used with EC chipsets that provide more flash space than
 * executable RAM. These options allow storing the initialized data into
 * an unused area of flash where it is copied directly from flash into data
 * RAM by the early boot code.
 *
 * When ROM resident is disabled, the initialized data is stored in the main
 * image, copied from flash to executable RAM by the chip boot loader, and
 * then copied from executable RAM to data RAM by the early boot code.
 *
 * Supporting this under Zephyr would require linker changes to the common
 * Zephyr linking.
 */
#undef CONFIG_CHIP_DATA_IN_INIT_ROM
#undef CONFIG_CHIP_INIT_ROM_REGION
#undef CONFIG_RO_ROM_RESIDENT_MEM_OFF
#undef CONFIG_RO_ROM_RESIDENT_SIZE
#undef CONFIG_RW_ROM_RESIDENT_MEM_OFF
#undef CONFIG_RW_ROM_RESIDENT_SIZE

/*
 * ECOS specific options, not used in Zephyr.
 */

 /*
  * The Zephyr npcx watchdog driver provider a delay set by
  * CONFIG_WDT_NPCX_DELAY_CYCLES for displaying the warning message.
  * CONFIG_AUX_TIMER_PERIOD_MS isn't used by the Zephyr code.
  */
#undef CONFIG_AUX_TIMER_PERIOD_MS
#undef CONFIG_CONSOLE_UART /* Only used by the Chromium EC chip drivers */
#undef CONFIG_I2C_MULTI_PORT_CONTROLLER /* Not required by I2C shim */
#undef CONFIG_IRQ_COUNT /* Only used by Chromium EC core drivers */
#undef CONFIG_LTO /* Link time optimization enabled by Zephyr build system */
#undef CONFIG_STACK_SIZE /* Only used in Chromium EC core init code */
#ifndef CONFIG_FPU
#undef CONFIG_FPU /* Used in Zephyr as well, enabled in Kconfig directly */
#endif
#ifndef CONFIG_WATCHDOG
#undef CONFIG_WATCHDOG /* Used in Zephyr as well, enabled in Kconfig directly */
#endif

/*
 * The Zephyr I2C shell command provides the same functionality as the Chromium
 * EC i2cscan and i2cxfer commands, so they are always disabled.
 */
#undef CONFIG_CMD_I2C_SCAN
#undef CONFIG_CMD_I2C_XFER

/*
 * This not used by the Zephyr code since we always make cros_crc8() available.
 * Define it here to reduce the delta from the ECOS CONFIG.
 */
#undef CONFIG_CRC8
#define CONFIG_CRC8

/*
 * This is not used by the Zephyr code.
 * Define it here to reduce the delta from the ECOS CONFIG.
 */
#undef CONFIG_CHIP_PRE_INIT
#define CONFIG_CHIP_PRE_INIT

/* EC chipset configuration */
#define HOOK_TICK_INTERVAL	CONFIG_CROS_EC_HOOK_TICK_INTERVAL
#define HOOK_TICK_INTERVAL_MS	(HOOK_TICK_INTERVAL / 1000)

/* Chipset and power configuration */
#ifdef CONFIG_AP_ARM_QUALCOMM_SC7180
#define CONFIG_CHIPSET_SC7180
#endif

#ifdef CONFIG_AP_X86_INTEL_CML
#define CONFIG_CHIPSET_COMETLAKE
#define CONFIG_CHIPSET_HAS_PRE_INIT_CALLBACK
#endif

#ifdef CONFIG_AP_X86_INTEL_TGL
#define CONFIG_CHIPSET_TIGERLAKE
#endif

#undef CONFIG_THROTTLE_AP
#ifdef CONFIG_PLATFORM_EC_THROTTLE_AP
#define CONFIG_THROTTLE_AP
#endif

#undef CONFIG_CHIPSET_CAN_THROTTLE
#ifdef CONFIG_PLATFORM_EC_CHIPSET_CAN_THROTTLE
#define CONFIG_CHIPSET_CAN_THROTTLE
#endif

#undef CONFIG_CMD_APTHROTTLE
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_APTHROTTLE
#define CONFIG_CMD_APTHROTTLE
#endif

#undef CONFIG_BACKLIGHT_LID
#ifdef CONFIG_PLATFORM_EC_BACKLIGHT_LID
#define CONFIG_BACKLIGHT_LID
#endif

/* Battery configuration */
#undef CONFIG_BATTERY
#undef CONFIG_BATTERY_FUEL_GAUGE
#ifdef CONFIG_PLATFORM_EC_BATTERY
#define CONFIG_BATTERY
#define CONFIG_BATTERY_FUEL_GAUGE

#endif /* CONFIG_PLATFORM_EC_BATTERY_FUEL_GAUGE */

#undef CONFIG_BATTERY_SMART
#ifdef CONFIG_PLATFORM_EC_BATTERY_SMART
#define CONFIG_BATTERY_SMART
#endif

#undef CONFIG_BATTERY_PRESENT_CUSTOM
#ifdef CONFIG_PLATFORM_EC_BATTERY_PRESENT_CUSTOM
#define CONFIG_BATTERY_PRESENT_CUSTOM
#endif

#undef CONFIG_BATTERY_PRESENT_GPIO
#ifdef CONFIG_PLATFORM_EC_BATTERY_PRESENT_GPIO
/* This is always GPIO_BATT_PRES_ODL with Zephyr */
#define CONFIG_BATTERY_PRESENT_GPIO	GPIO_BATT_PRES_ODL
#endif

#undef CONFIG_BATTERY_CUT_OFF
#ifdef CONFIG_PLATFORM_EC_BATTERY_CUT_OFF
#define CONFIG_BATTERY_CUT_OFF
#endif

#undef CONFIG_BATTERY_HW_PRESENT_CUSTOM
#ifdef CONFIG_PLATFORM_EC_BATTERY_HW_PRESENT_CUSTOM
#define CONFIG_BATTERY_HW_PRESENT_CUSTOM
#endif

#undef CONFIG_BATTERY_REVIVE_DISCONNECT
#ifdef CONFIG_PLATFORM_EC_BATTERY_REVIVE_DISCONNECT
#define CONFIG_BATTERY_REVIVE_DISCONNECT
#endif

#undef CONFIG_BATT_FULL_CHIPSET_OFF_INPUT_LIMIT_MV
#if defined(CONFIG_PLATFORM_EC_BATT_FULL_CHIPSET_OFF_INPUT_LIMIT_MV) && \
    (CONFIG_PLATFORM_EC_BATT_FULL_CHIPSET_OFF_INPUT_LIMIT_MV > 0)
#define CONFIG_BATT_FULL_CHIPSET_OFF_INPUT_LIMIT_MV \
    CONFIG_PLATFORM_EC_BATT_FULL_CHIPSET_OFF_INPUT_LIMIT_MV
#endif

#undef CONFIG_BOARD_RESET_AFTER_POWER_ON
#ifdef CONFIG_PLATFORM_EC_BOARD_RESET_AFTER_POWER_ON
#define CONFIG_BOARD_RESET_AFTER_POWER_ON
#endif

#undef CONFIG_CHARGER_ISL9241
#ifdef CONFIG_PLATFORM_EC_CHARGER_ISL9241
#define CONFIG_CHARGER_ISL9241

/* TODO(b/175881762): Move to devicetree? */
#define CONFIG_CHARGER_SENSE_RESISTOR		10
#define CONFIG_CHARGER_SENSE_RESISTOR_AC	10

#define CONFIG_CHARGER_INPUT_CURRENT		512
#endif

/*
 * Note - ISL9241 chargers for all channels are configured with the same
 * switching frequency. Use the first ISL9241 instance found in the device tree.
 */
#undef CONFIG_ISL9241_SWITCHING_FREQ
#define ISL9241_NODE	DT_INST(0, intersil_isl9241)
#if DT_NODE_EXISTS(ISL9241_NODE) && \
	DT_NODE_HAS_PROP(ISL9241_NODE, switching_frequency)
#define CONFIG_ISL9241_SWITCHING_FREQ \
	DT_PROP(ISL9241_NODE, switching_frequency)
#endif

#undef CONFIG_CHARGER_ISL9237
#ifdef CONFIG_PLATFORM_EC_CHARGER_ISL9237
#define CONFIG_CHARGER_ISL9237
#endif

#undef CONFIG_CHARGER_ISL9238
#ifdef CONFIG_PLATFORM_EC_CHARGER_ISL9238
#define CONFIG_CHARGER_ISL9238
#endif

#undef CONFIG_CHARGER_NARROW_VDC
#ifdef CONFIG_PLATFORM_EC_CHARGER_NARROW_VDC
#define CONFIG_CHARGER_NARROW_VDC
#endif

#undef CONFIG_CHIPSET_RESET_HOOK
#ifdef CONFIG_PLATFORM_EC_CHIPSET_RESET_HOOK
#define CONFIG_CHIPSET_RESET_HOOK
#endif

#ifdef CONFIG_PLATFORM_EC_EXTPOWER_GPIO
#define CONFIG_EXTPOWER_GPIO

/* This always needs to be defined for this option to work */
#define CONFIG_EXTPOWER
#endif

/* Bringup configuration */
#ifdef CONFIG_PLATFORM_EC_BRINGUP
#define CONFIG_BRINGUP
#endif

/* eSPI configuration */
#ifdef CONFIG_PLATFORM_EC_ESPI

#ifdef CONFIG_PLATFORM_EC_HOSTCMD
#define CONFIG_HOSTCMD_ESPI
#endif

/* eSPI signals */
#ifdef CONFIG_PLATFORM_EC_ESPI_VW_SLP_S3
#define CONFIG_HOSTCMD_ESPI_VW_SLP_S3
#endif

#ifdef CONFIG_PLATFORM_EC_ESPI_VW_SLP_S4
#define CONFIG_HOSTCMD_ESPI_VW_SLP_S4
#endif

#ifdef CONFIG_PLATFORM_EC_ESPI_RESET_SLP_SX_VW_ON_ESPI_RST
#define CONFIG_HOSTCMD_ESPI_RESET_SLP_SX_VW_ON_ESPI_RST
#endif

#endif /* CONFIG_PLATFORM_EC_ESPI */

#if DT_NODE_EXISTS(DT_NODELABEL(flash0))
#define CONFIG_PROGRAM_MEMORY_BASE DT_REG_ADDR(DT_NODELABEL(flash0))
#else
#define CONFIG_PROGRAM_MEMORY_BASE 0X0
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(sram0))
#define CONFIG_RAM_BASE DT_REG_ADDR(DT_NODELABEL(sram0))
#define CONFIG_DATA_RAM_SIZE DT_REG_SIZE(DT_NODELABEL(sram0))
#else
#define CONFIG_RAM_BASE 0x0
#define CONFIG_DATA_RAM_SIZE 0x0
#endif

#define CONFIG_RO_MEM_OFF CONFIG_CROS_EC_RO_MEM_OFF
#define CONFIG_RO_MEM_SIZE CONFIG_CROS_EC_RO_MEM_SIZE
#define CONFIG_RW_MEM_OFF CONFIG_CROS_EC_RW_MEM_OFF
#define CONFIG_RW_MEM_SIZE CONFIG_CROS_EC_RW_MEM_SIZE

#define CONFIG_WP_STORAGE_OFF	CONFIG_EC_PROTECTED_STORAGE_OFF
#define CONFIG_WP_STORAGE_SIZE	CONFIG_EC_PROTECTED_STORAGE_SIZE
#define CONFIG_RO_SIZE		CONFIG_CROS_EC_RO_SIZE
#define CONFIG_RW_SIZE		CONFIG_CROS_EC_RW_SIZE

/* Flash settings */
#undef CONFIG_EXTERNAL_STORAGE
#undef CONFIG_MAPPED_STORAGE
#undef CONFIG_FLASH_PSTATE
#undef CONFIG_FLASH_SIZE_BYTES
#ifdef CONFIG_PLATFORM_EC_FLASH
#define CONFIG_FLASH
#define CONFIG_SPI_FLASH_W25Q80 /* Internal SPI flash type. */
#ifdef CONFIG_FLASH_SIZE
#define CONFIG_FLASH_SIZE_BYTES	(CONFIG_FLASH_SIZE * 1024)
#else
#define CONFIG_FLASH_SIZE_BYTES 0x0
#endif
/* TODO(b:176490413): use DT_PROP(DT_INST(inst, DT_DRV_COMPAT), size) ? */
#define CONFIG_MAPPED_STORAGE_BASE 0x64000000
#define CONFIG_FLASH_WRITE_SIZE		0x1  /* minimum write size */
#define CONFIG_FLASH_WRITE_IDEAL_SIZE	256   /* one page size for write */
#define CONFIG_FLASH_ERASE_SIZE	0x1000
#define CONFIG_FLASH_BANK_SIZE		CONFIG_FLASH_ERASE_SIZE
#define CONFIG_EC_PROTECTED_STORAGE_OFF CONFIG_PLATFORM_EC_PROTECTED_STORAGE_OFF
#define CONFIG_EC_PROTECTED_STORAGE_SIZE \
	CONFIG_PLATFORM_EC_PROTECTED_STORAGE_SIZE
#define CONFIG_EC_WRITABLE_STORAGE_OFF CONFIG_PLATFORM_EC_WRITABLE_STORAGE_OFF
#define CONFIG_EC_WRITABLE_STORAGE_SIZE CONFIG_PLATFORM_EC_WRITABLE_STORAGE_SIZE

/* RO image resides at start of protected region, right after header */
#define CONFIG_RO_STORAGE_OFF	CONFIG_RO_HDR_SIZE

#define CONFIG_RW_STORAGE_OFF 0
#define CONFIG_RAM_SIZE CONFIG_DATA_RAM_SIZE

#ifdef CONFIG_PLATFORM_EC_EXTERNAL_STORAGE
#define CONFIG_EXTERNAL_STORAGE
#endif

#ifdef CONFIG_PLATFORM_EC_MAPPED_STORAGE
#define CONFIG_MAPPED_STORAGE
#endif

#undef CONFIG_CMD_FLASH
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_FLASH
#define CONFIG_CMD_FLASH
#endif

#undef CONFIG_CMD_FLASHINFO
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_FLASHINFO
#define CONFIG_CMD_FLASHINFO
#endif

#undef CONFIG_CMD_FLASH_WP
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_FLASH_WP
#define CONFIG_CMD_FLASH_WP
#endif

#endif /* CONFIG_PLATFORM_EC_FLASH */

#undef CONFIG_ADC
#ifdef CONFIG_PLATFORM_EC_ADC
#define CONFIG_ADC
#endif

#undef CONFIG_CMD_ADC
#ifdef CONFIG_PLATFORM_EC_ADC_CMD
#define CONFIG_CMD_ADC
#endif

#undef CONFIG_TEMP_SENSOR
#ifdef CONFIG_PLATFORM_EC_TEMP_SENSOR
#define CONFIG_TEMP_SENSOR
#endif

#undef CONFIG_THERMISTOR
#ifdef CONFIG_PLATFORM_EC_THERMISTOR
#define CONFIG_THERMISTOR
#endif

#undef CONFIG_STEINHART_HART_3V0_22K6_47K_4050B
#ifdef CONFIG_PLATFORM_EC_STEINHART_HART_3V0_22K6_47K_4050B
#define CONFIG_STEINHART_HART_3V0_22K6_47K_4050B
#endif

#undef CONFIG_STEINHART_HART_3V3_13K7_47K_4050B
#ifdef CONFIG_PLATFORM_EC_STEINHART_HART_3V3_13K7_47K_4050B
#define CONFIG_STEINHART_HART_3V3_13K7_47K_4050B
#endif

#undef CONFIG_STEINHART_HART_3V3_30K9_47K_4050B
#ifdef CONFIG_PLATFORM_EC_STEINHART_HART_3V3_30K9_47K_4050B
#define CONFIG_STEINHART_HART_3V3_30K9_47K_4050B
#endif

#undef CONFIG_STEINHART_HART_3V3_51K1_47K_4050B
#ifdef CONFIG_PLATFORM_EC_STEINHART_HART_3V3_51K1_47K_4050B
#define CONFIG_STEINHART_HART_3V3_51K1_47K_4050B
#endif

#ifdef CONFIG_PLATFORM_EC_I2C
/* Also see shim/include/i2c/i2c.h which defines the ports enum */
#define CONFIG_I2C
#define CONFIG_I2C_CONTROLLER
#endif

#undef CONFIG_KEYBOARD_PROTOCOL_8042
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_PROTOCOL_8042
#define CONFIG_KEYBOARD_PROTOCOL_8042
#endif /* CONFIG_PLATFORM_EC_KEYBOARD_PROTOCOL_8042 */

#undef CONFIG_KEYBOARD_PROTOCOL_MKBP
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_PROTOCOL_MKBP
#define CONFIG_KEYBOARD_PROTOCOL_MKBP
#endif

#undef CONFIG_CMD_KEYBOARD
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_KEYBOARD
#define CONFIG_CMD_KEYBOARD
#endif

#undef CONFIG_KEYBOARD_COL2_INVERTED
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_COL2_INVERTED
#define CONFIG_KEYBOARD_COL2_INVERTED
#endif  /* CONFIG_PLATFORM_EC_KEYBOARD_COL2_INVERTED */

#undef CONFIG_KEYBOARD_BOARD_CONFIG
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_BOARD_CONFIG
#define CONFIG_KEYBOARD_BOARD_CONFIG
#endif

#undef CONFIG_KEYBOARD_KEYPAD
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_KEYPAD
#define CONFIG_KEYBOARD_KEYPAD
#endif

#undef CONFIG_KEYBOARD_VIVALDI
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_VIVALDI
#define CONFIG_KEYBOARD_VIVALDI
#endif

#undef CONFIG_KEYBOARD_PWRBTN_ASSERTS_KSI2
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_PWRBTN_ASSERTS_KSI2
#define CONFIG_KEYBOARD_PWRBTN_ASSERTS_KSI2
#endif

#undef CONFIG_KEYBOARD_PWRBTN_ASSERTS_KSI3
#ifdef CONFIG_PLATFORM_EC_KEYBOARD_PWRBTN_ASSERTS_KSI3
#define CONFIG_KEYBOARD_PWRBTN_ASSERTS_KSI3
#endif

#undef CONFIG_VOLUME_BUTTONS
#ifdef CONFIG_PLATFORM_EC_VOLUME_BUTTONS
#define CONFIG_VOLUME_BUTTONS
#endif

#undef CONFIG_CMD_BUTTON
#ifdef CONFIG_PLATFORM_EC_CMD_BUTTON
#define CONFIG_CMD_BUTTON
#endif

#undef CONFIG_PWM_KBLIGHT
#undef CONFIG_KEYBOARD_BACKLIGHT
#ifdef CONFIG_PLATFORM_EC_PWM_KBLIGHT
#define CONFIG_PWM_KBLIGHT
#define CONFIG_KEYBOARD_BACKLIGHT
#endif

#undef CONFIG_LED_COMMON
#ifdef CONFIG_PLATFORM_EC_LED_COMMON
#define CONFIG_LED_COMMON
#endif

#undef CONFIG_LED_PWM
#ifdef CONFIG_PLATFORM_EC_LED_PWM
#define CONFIG_LED_PWM
#endif

#undef CONFIG_LED_PWM_COUNT
#ifdef CONFIG_PLATFORM_EC_LED_PWM_COUNT
#define CONFIG_LED_PWM_COUNT CONFIG_PLATFORM_EC_LED_PWM_COUNT
#endif

#undef CONFIG_CMD_LEDTEST
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_LEDTEST
#define CONFIG_CMD_LEDTEST
#endif

#undef CONFIG_PWM_DISPLIGHT
#ifdef CONFIG_PLATFORM_EC_PWM_DISPLIGHT
#define CONFIG_PWM_DISPLIGHT
#endif

#undef CONFIG_CPU_PROCHOT_ACTIVE_LOW
#ifdef CONFIG_PLATFORM_EC_POWERSEQ_CPU_PROCHOT_ACTIVE_LOW
#define CONFIG_CPU_PROCHOT_ACTIVE_LOW
#endif

#undef CONFIG_POWER_TRACK_HOST_SLEEP_STATE
#ifdef CONFIG_PLATFORM_EC_POWERSEQ_HOST_SLEEP
#define CONFIG_POWER_TRACK_HOST_SLEEP_STATE
#endif

#undef CONFIG_POWER_SLEEP_FAILURE_DETECTION
#ifdef CONFIG_PLATFORM_EC_POWER_SLEEP_FAILURE_DETECTION
#define CONFIG_POWER_SLEEP_FAILURE_DETECTION
#endif

#undef CONFIG_HOSTCMD_AP_RESET
#ifdef CONFIG_PLATFORM_EC_HOSTCMD_AP_RESET
#define CONFIG_HOSTCMD_AP_RESET
#endif

#ifdef CONFIG_PLATFORM_EC_POWERSEQ_RSMRST_DELAY
#define CONFIG_CHIPSET_X86_RSMRST_DELAY
#endif

#ifdef CONFIG_PLATFORM_EC_POWERSEQ_SLP_S3_L_OVERRIDE
#define CONFIG_CHIPSET_SLP_S3_L_OVERRIDE
#endif

#ifdef CONFIG_PLATFORM_EC_POWERSEQ_PP3300_RAIL_FIRST
#define CONFIG_CHIPSET_PP3300_RAIL_FIRST
#endif

#ifdef CONFIG_PLATFORM_EC_POWERSEQ_RTC_RESET
#define CONFIG_BOARD_HAS_RTC_RESET
#endif

#ifdef CONFIG_PLATFORM_EC_POWERSEQ_PP5000_CONTROL
#define CONFIG_POWER_PP5000_CONTROL
#endif

#undef CONFIG_POWER_S0IX
#ifdef CONFIG_PLATFORM_EC_POWERSEQ_S0IX
#define CONFIG_POWER_S0IX
#endif

#undef CONFIG_POWER_BUTTON_X86
#ifdef CONFIG_PLATFORM_EC_POWERSEQ_INTEL
#define CONFIG_POWER_BUTTON_X86
#endif

#undef CONFIG_FAKE_SHMEM
#ifdef CONFIG_ARCH_POSIX
#define CONFIG_FAKE_SHMEM
#endif

#undef CONFIG_PWM
#ifdef CONFIG_PLATFORM_EC_PWM
#define CONFIG_PWM
#endif

#undef CONFIG_CMD_SHMEM
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_SHMEM
#define CONFIG_CMD_SHMEM
#endif

#ifdef CONFIG_PLATFORM_EC_TIMER
#define CONFIG_HWTIMER_64BIT
#define CONFIG_HW_SPECIFIC_UDELAY

#undef CONFIG_CMD_GETTIME
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_GETTIME
#define CONFIG_CMD_GETTIME
#endif  /* CONFIG_PLATFORM_EC_CONSOLE_CMD_GETTIME */

#undef CONFIG_CMD_TIMERINFO
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_TIMERINFO
#define CONFIG_CMD_TIMERINFO
#endif  /* CONFIG_PLATFORM_EC_CONSOLE_CMD_TIMERINFO */

#undef CONFIG_CMD_WAITMS
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_WAITMS
#define CONFIG_CMD_WAITMS
#endif  /* CONFIG_PLATFORM_EC_CONSOLE_CMD_TIMERINFO */

#endif  /* CONFIG_PLATFORM_EC_TIMER */

/* USB-C things */
#ifdef CONFIG_PLATFORM_EC_USBC

/* Zephyr only supports v2 so we always define this */
#define CONFIG_USB_PD_TCPMV2

/*
 * Define these here for now. They are not actually CONFIG options in the EC
 * code base. Ideally they would be defined in the devicetree (perhaps for a
 * 'board' driver if not in the USB chip driver itself).
 *
 * SN5S30 PPC supports up to 24V VBUS source and sink, however passive USB-C
 * cables only support up to 60W.
 */
#define PD_OPERATING_POWER_MW	15000
#define PD_MAX_POWER_MW		60000
#define PD_MAX_CURRENT_MA	3000
#define PD_MAX_VOLTAGE_MV	20000

/* TODO: b/144165680 - measure and check these values on Volteer */
#define PD_POWER_SUPPLY_TURN_ON_DELAY	30000 /* us */
#define PD_POWER_SUPPLY_TURN_OFF_DELAY	30000 /* us */
#endif

#undef CONFIG_CMD_PPC_DUMP
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_PPC_DUMP
#define CONFIG_CMD_PPC_DUMP
#endif

#undef CONFIG_CMD_TCPC_DUMP
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_TCPC_DUMP
#define CONFIG_CMD_TCPC_DUMP
#endif

#undef CONFIG_USB_POWER_DELIVERY
#ifdef CONFIG_PLATFORM_EC_USB_POWER_DELIVERY
#define CONFIG_USB_POWER_DELIVERY
#endif

#undef CONFIG_CHARGER
#undef CONFIG_CHARGE_MANAGER
#ifdef CONFIG_PLATFORM_EC_CHARGE_MANAGER
#define CONFIG_CHARGE_MANAGER
#define CONFIG_CHARGER

/* TODO: Put these charger defines in the devicetree? */
#define CONFIG_CHARGER_SENSE_RESISTOR		10
#define CONFIG_CHARGER_SENSE_RESISTOR_AC	10

#endif

#undef CONFIG_CHARGER_INPUT_CURRENT
#ifdef CONFIG_PLATFORM_EC_CHARGER_INPUT_CURRENT
#define CONFIG_CHARGER_INPUT_CURRENT	CONFIG_PLATFORM_EC_CHARGER_INPUT_CURRENT
#endif

#undef CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON
#ifdef CONFIG_PLATFORM_EC_CHARGER_MIN_BAT_PCT_FOR_POWER_ON
#define CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON \
		CONFIG_PLATFORM_EC_CHARGER_MIN_BAT_PCT_FOR_POWER_ON
#endif

#undef CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON_WITH_AC
#ifdef CONFIG_PLATFORM_EC_CHARGER_MIN_BAT_PCT_FOR_POWER_ON_WITH_AC
#define CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON_WITH_AC \
		CONFIG_PLATFORM_EC_CHARGER_MIN_BAT_PCT_FOR_POWER_ON_WITH_AC
#endif

#undef CONFIG_CHARGER_MIN_POWER_MW_FOR_POWER_ON_WITH_BATT
#ifdef CONFIG_PLATFORM_EC_CHARGER_MIN_POWER_MW_FOR_POWER_ON_WITH_BATT
#define CONFIG_CHARGER_MIN_POWER_MW_FOR_POWER_ON_WITH_BATT \
		CONFIG_PLATFORM_EC_CHARGER_MIN_POWER_MW_FOR_POWER_ON_WITH_BATT
#endif

#undef CONFIG_CHARGER_MIN_POWER_MW_FOR_POWER_ON
#ifdef CONFIG_PLATFORM_EC_CHARGER_MIN_POWER_MW_FOR_POWER_ON
#define CONFIG_CHARGER_MIN_POWER_MW_FOR_POWER_ON \
		CONFIG_PLATFORM_EC_CHARGER_MIN_POWER_MW_FOR_POWER_ON
#endif

#undef CONFIG_CHARGE_RAMP_SW
#ifdef CONFIG_PLATFORM_EC_CHARGE_RAMP_SW
#define CONFIG_CHARGE_RAMP_SW
#endif

#undef CONFIG_CHARGE_RAMP_HW
#ifdef CONFIG_PLATFORM_EC_CHARGE_RAMP_HW
#define CONFIG_CHARGE_RAMP_HW
#endif

#undef CONFIG_CMD_CHGRAMP
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_CHGRAMP
#define CONFIG_CMD_CHGRAMP
#endif

#undef CONFIG_USB_PID
#ifdef CONFIG_PLATFORM_EC_USB_PID
#define CONFIG_USB_PID		CONFIG_PLATFORM_EC_USB_PID
#endif

#undef CONFIG_USB_BCD_DEV
#ifdef CONFIG_PLATFORM_EC_USB_BCD_DEV
#define CONFIG_USB_BCD_DEV	CONFIG_PLATFORM_EC_USB_BCD_DEV
#endif

/* VBUS-voltage measurement */
#undef CONFIG_USB_PD_VBUS_MEASURE_NOT_PRESENT
#undef CONFIG_USB_PD_VBUS_MEASURE_CHARGER
#undef CONFIG_USB_PD_VBUS_MEASURE_TCPC
#undef CONFIG_USB_PD_VBUS_MEASURE_ADC_EACH_PORT
#ifdef CONFIG_PLATFORM_EC_USB_PD_VBUS_MEASURE_NOT_PRESENT
#define CONFIG_USB_PD_VBUS_MEASURE_NOT_PRESENT
#elif defined(CONFIG_PLATFORM_EC_USB_PD_VBUS_MEASURE_CHARGER)
#define CONFIG_USB_PD_VBUS_MEASURE_CHARGER
#elif defined(CONFIG_PLATFORM_EC_USB_PD_VBUS_MEASURE_TCPC)
#define CONFIG_USB_PD_VBUS_MEASURE_TCPC
#elif defined(CONFIG_PLATFORM_EC_USB_PD_VBUS_MEASURE_ADC_EACH_PORT)
#define CONFIG_USB_PD_VBUS_MEASURE_ADC_EACH_PORT
#endif /* VBUS-voltage measurement */

#undef CONFIG_USB_CHARGER
#ifdef CONFIG_PLATFORM_EC_USB_CHARGER
#define CONFIG_USB_CHARGER
#endif

#define USB_PORT_COUNT	CONFIG_PLATFORM_EC_USB_A_PORT_COUNT

#undef CONFIG_USB_PORT_POWER_DUMB
#ifdef CONFIG_PLATFORM_EC_USB_PORT_POWER_DUMB
#define CONFIG_USB_PORT_POWER_DUMB
#endif

#undef CONFIG_BC12_DETECT_PI3USB9201
#ifdef CONFIG_PLATFORM_EC_BC12_DETECT_PI3USB9201
#define CONFIG_BC12_DETECT_PI3USB9201
#endif

#undef CONFIG_USB_PD_DUAL_ROLE
#ifdef CONFIG_PLATFORM_EC_USB_PD_DUAL_ROLE
#define CONFIG_USB_PD_DUAL_ROLE
#endif

#undef CONFIG_USB_PD_DUAL_ROLE_AUTO_TOGGLE
#ifdef CONFIG_PLATFORM_EC_USB_PD_DUAL_ROLE_AUTO_TOGGLE
#define CONFIG_USB_PD_DUAL_ROLE_AUTO_TOGGLE
#endif

#undef CONFIG_USB_PD_DISCHARGE_PPC
#ifdef CONFIG_PLATFORM_EC_USB_PD_DISCHARGE_PPC
#define CONFIG_USB_PD_DISCHARGE_PPC
#endif

#undef CONFIG_USB_PD_LOGGING
#ifdef CONFIG_PLATFORM_EC_USB_PD_LOGGING
#define CONFIG_USB_PD_LOGGING
#endif

#undef CONFIG_USBC_OCP
#ifdef CONFIG_PLATFORM_EC_USBC_OCP
#define CONFIG_USBC_OCP
#endif

#undef CONFIG_USB_PD_CONSOLE_CMD
#ifdef CONFIG_PLATFORM_EC_USB_PD_CONSOLE_CMD
#define CONFIG_USB_PD_CONSOLE_CMD
#endif

#undef CONFIG_USB_PD_HOST_CMD
#ifdef CONFIG_PLATFORM_EC_USB_PD_HOST_CMD
#define CONFIG_USB_PD_HOST_CMD
#endif

#undef CONFIG_USB_PD_REV30
#ifdef CONFIG_PLATFORM_EC_USB_PD_REV30
#define CONFIG_USB_PD_REV30

/*
 * Support USB PD 3.0 Extended Messages. Note that Chromebooks disabling this
 * config item are non-compliant with PD 3.0, because they have batteries but do
 * not support Get_Battery_Cap or Get_Battery_Status.
 */
#define CONFIG_USB_PD_EXTENDED_MESSAGES
#endif

#undef CONFIG_USB_PD_VBUS_DETECT_TCPC
#undef CONFIG_USB_PD_VBUS_DETECT_CHARGER
#undef CONFIG_USB_PD_VBUS_DETECT_GPIO
#undef CONFIG_USB_PD_VBUS_DETECT_PPC
#undef CONFIG_USB_PD_VBUS_DETECT_NONE
#ifdef CONFIG_PLATFORM_EC_USB_PD_VBUS_DETECT_TCPC
#define CONFIG_USB_PD_VBUS_DETECT_TCPC
#endif
#ifdef CONFIG_PLATFORM_EC_USB_PD_VBUS_DETECT_CHARGER
#define CONFIG_USB_PD_VBUS_DETECT_CHARGER
#endif
#ifdef CONFIG_PLATFORM_EC_USB_PD_VBUS_DETECT_PPC
#define CONFIG_USB_PD_VBUS_DETECT_PPC
#endif
#ifdef CONFIG_PLATFORM_EC_USB_PD_VBUS_DETECT_NONE
#define CONFIG_USB_PD_VBUS_DETECT_NONE
#endif

#undef CONFIG_USB_PD_5V_EN_CUSTOM
#ifdef CONFIG_PLATFORM_EC_USB_PD_5V_EN_CUSTOM
#define CONFIG_USB_PD_5V_EN_CUSTOM
#endif

#undef CONFIG_USB_TYPEC_SM
#ifdef CONFIG_PLATFORM_EC_USB_TYPEC_SM
#define CONFIG_USB_TYPEC_SM
#endif

#undef CONFIG_USB_PRL_SM
#ifdef CONFIG_PLATFORM_EC_USB_PRL_SM
#define CONFIG_USB_PRL_SM
#endif

#undef CONFIG_USB_PE_SM
#ifdef CONFIG_PLATFORM_EC_USB_PE_SM
#define CONFIG_USB_PE_SM
#endif

#undef CONFIG_USB_PD_DECODE_SOP
#ifdef CONFIG_PLATFORM_EC_USB_PD_DECODE_SOP
#define CONFIG_USB_PD_DECODE_SOP
#endif

#undef CONFIG_USB_VPD
#ifdef CONFIG_PLATFORM_EC_USB_VPD
#define CONFIG_USB_VPD
#endif

#undef CONFIG_USB_CTVPD
#ifdef CONFIG_PLATFORM_EC_USB_CTVPD
#define CONFIG_USB_CTVPD
#endif

#undef CONFIG_USB_DRP_ACC_TRYSRC
#ifdef CONFIG_PLATFORM_EC_USB_DRP_ACC_TRYSRC
#define CONFIG_USB_DRP_ACC_TRYSRC
#endif

#undef CONFIG_USB_PD_TCPM_PS8751
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_PS8751
#define CONFIG_USB_PD_TCPM_PS8751
#endif

#undef CONFIG_USB_PD_TCPM_PS8805
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_PS8805
#define CONFIG_USB_PD_TCPM_PS8805
#endif

#undef CONFIG_USB_PD_TCPM_PS8815
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_PS8815
#define CONFIG_USB_PD_TCPM_PS8815
#define CONFIG_USB_PD_TCPM_PS8815_FORCE_DID
#endif

#undef CONFIG_USB_PD_TCPM_MULTI_PS8XXX
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_MULTI_PS8XXX
#define CONFIG_USB_PD_TCPM_MULTI_PS8XXX
#endif

#undef CONFIG_USB_PD_TCPM_RT1715
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_RT1715
#define CONFIG_USB_PD_TCPM_RT1715
#endif

#undef CONFIG_USB_PD_TCPM_TUSB422
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_TUSB422
#define CONFIG_USB_PD_TCPM_TUSB422
#endif

#undef CONFIG_USB_PD_TCPM_TCPCI
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_TCPCI
#define CONFIG_USB_PD_TCPM_TCPCI
#endif

#undef CONFIG_USB_PD_PORT_MAX_COUNT
#ifdef CONFIG_PLATFORM_EC_USB_PD_PORT_MAX_COUNT
#define CONFIG_USB_PD_PORT_MAX_COUNT CONFIG_PLATFORM_EC_USB_PD_PORT_MAX_COUNT
#endif

#undef CONFIG_USBC_PPC_SN5S330
#ifdef CONFIG_PLATFORM_EC_USBC_PPC_SN5S330
#define CONFIG_USBC_PPC_SN5S330
#endif

#undef CONFIG_USBC_PPC_SYV682X
#ifdef CONFIG_PLATFORM_EC_USBC_PPC_SYV682X
#define CONFIG_USBC_PPC_SYV682X
#endif

#undef CONFIG_USB_PD_TCPC_RUNTIME_CONFIG
#undef CONFIG_USB_MUX_RUNTIME_CONFIG
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPC_RUNTIME_CONFIG
#define CONFIG_USB_PD_TCPC_RUNTIME_CONFIG

#ifdef CONFIG_PLATFORM_EC_USB_MUX_RUNTIME_CONFIG
#define CONFIG_USB_MUX_RUNTIME_CONFIG
#endif /* CONFIG_PLATFORM_EC_USB_MUX_RUNTIME_CONFIG */

#endif /* CONFIG_PLATFORM_EC_USB_PD_TCPC_RUNTIME_CONFIG */

#undef CONFIG_USB_PD_ALT_MODE
#ifdef CONFIG_PLATFORM_EC_USB_PD_ALT_MODE
#define CONFIG_USB_PD_ALT_MODE
#endif

#undef CONFIG_USB_PD_ALT_MODE_DFP
#ifdef CONFIG_PLATFORM_EC_USB_PD_ALT_MODE_DFP
#define CONFIG_USB_PD_ALT_MODE_DFP
#endif

#undef CONFIG_USB_PD_ALT_MODE_UFP
#ifdef CONFIG_PLATFORM_EC_USB_PD_ALT_MODE_UFP
#define CONFIG_USB_PD_ALT_MODE_UFP
#endif

#undef CONFIG_USBC_RETIMER_FW_UPDATE
#ifdef CONFIG_PLATFORM_EC_USBC_RETIMER_FW_UPDATE
#define CONFIG_USBC_RETIMER_FW_UPDATE
#endif

#undef CONFIG_USBC_RETIMER_INTEL_BB
#ifdef CONFIG_PLATFORM_EC_USBC_RETIMER_INTEL_BB

#define USBC_PORT_C1_BB_RETIMER_I2C_ADDR \
	DT_REG_ADDR(DT_NODELABEL(usb_c1_bb_retimer))
#define CONFIG_USBC_RETIMER_INTEL_BB
#endif

#undef CONFIG_USBC_RETIMER_INTEL_BB_RUNTIME_CONFIG
#ifdef CONFIG_PLATFORM_EC_USBC_RETIMER_INTEL_BB_RUNTIME_CONFIG
#define CONFIG_USBC_RETIMER_INTEL_BB_RUNTIME_CONFIG
#endif

#undef CONFIG_USBC_SS_MUX
#ifdef CONFIG_PLATFORM_EC_USBC_SS_MUX
#define CONFIG_USBC_SS_MUX
#endif

#undef CONFIG_USB_MUX_VIRTUAL
#ifdef CONFIG_PLATFORM_EC_USB_MUX_VIRTUAL
#define CONFIG_USB_MUX_VIRTUAL
#endif

#undef CONFIG_USB_PD_TCPM_MUX
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPM_MUX
#define CONFIG_USB_PD_TCPM_MUX
#endif

#undef CONFIG_USBC_PPC_DEDICATED_INT
#ifdef CONFIG_PLATFORM_EC_USBC_PPC_DEDICATED_INT
#define CONFIG_USBC_PPC_DEDICATED_INT
#endif

#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_PD
#define CONFIG_CONSOLE_CMD_PD
#endif

#ifdef CONFIG_HAS_TASK_PD_INT_C0
/* This must be defined if any task is active */
#define CONFIG_HAS_TASK_PD_INT
#endif

#undef CONFIG_MKBP_EVENT
#undef CONFIG_MKBP_USE_GPIO
#undef CONFIG_MKBP_USE_HOST_EVENT
#undef CONFIG_MKBP_USE_GPIO_AND_HOST_EVENT
#undef CONFIG_MKBP_USE_CUSTOM
#ifdef CONFIG_PLATFORM_EC_MKBP_EVENT
#define CONFIG_MKBP_EVENT
#ifdef CONFIG_PLATFORM_EC_MKBP_USE_GPIO
#define CONFIG_MKBP_USE_GPIO
#elif PLATFORM_EC_MKBP_USE_HOST_EVENT
#define CONFIG_MKBP_USE_HOST_EVENT
#elif PLATFORM_EC_MKBP_USE_GPIO_AND_HOST_EVENT
#define CONFIG_MKBP_USE_GPIO_AND_HOST_EVENT
#elif PLATFORM_EC_MKBP_USE_CUSTOM
#define CONFIG_MKBP_USE_CUSTOM
#endif
#endif /* CONFIG_PLATFORM_EC_MKBP_EVENT */

#undef CONFIG_USB_PD_TCPC_LOW_POWER
#undef CONFIG_USB_PD_TCPC_LPM_EXIT_DEBOUNCE
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPC_LOW_POWER
#define CONFIG_USB_PD_TCPC_LOW_POWER
#define CONFIG_USB_PD_TCPC_LPM_EXIT_DEBOUNCE \
		CONFIG_PLATFORM_EC_USB_PD_TCPC_LPM_EXIT_DEBOUNCE_US
#endif /* CONFIG_PLATFORM_EC_USB_PD_TCPC_LOW_POWER */

#undef CONFIG_USB_PD_DEBUG_LEVEL
#ifdef CONFIG_PLATFORM_EC_USB_PD_DEBUG_FIXED_LEVEL
#define CONFIG_USB_PD_DEBUG_LEVEL	CONFIG_PLATFORM_EC_USB_PD_DEBUG_LEVEL
#endif

#undef CONFIG_USBC_VCONN
#ifdef CONFIG_PLATFORM_EC_USBC_VCONN
#define CONFIG_USBC_VCONN

/* This must be defined as well */
#define CONFIG_USBC_VCONN_SWAP
#endif /* CONFIG_PLATFORM_EC_USBC_VCONN */

#undef CONFIG_USB_PD_TRY_SRC
#ifdef CONFIG_PLATFORM_EC_USB_PD_TRY_SRC
#define CONFIG_USB_PD_TRY_SRC
#endif

#undef CONFIG_USBC_PPC_POLARITY
#ifdef CONFIG_PLATFORM_EC_USBC_PPC_POLARITY
#define CONFIG_USBC_PPC_POLARITY
#endif

#undef CONFIG_USBC_PPC_SBU
#ifdef CONFIG_PLATFORM_EC_USBC_PPC_SBU
#define CONFIG_USBC_PPC_SBU
#endif

#undef CONFIG_USBC_PPC_VCONN
#ifdef CONFIG_PLATFORM_EC_USBC_PPC_VCONN
#define CONFIG_USBC_PPC_VCONN
#endif

#undef CONFIG_USB_PD_USB32_DRD
#ifdef CONFIG_PLATFORM_EC_USB_PD_USB32_DRD
#define CONFIG_USB_PD_USB32_DRD
#endif

#undef CONFIG_HOSTCMD_PD_CONTROL
#ifdef CONFIG_PLATFORM_EC_HOSTCMD_PD_CONTROL
#define CONFIG_HOSTCMD_PD_CONTROL
#endif

#undef CONFIG_CMD_HCDEBUG
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_HCDEBUG
#define CONFIG_CMD_HCDEBUG
#endif

#undef CONFIG_CMD_USB_PD_PE
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_USB_PD_PE
#define CONFIG_CMD_USB_PD_PE
#endif

#undef CONFIG_CMD_USB_PD_CABLE
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_USB_PD_CABLE
#define CONFIG_CMD_USB_PD_CABLE
#endif

#undef CONFIG_USB_PD_TBT_COMPAT_MODE
#ifdef CONFIG_PLATFORM_EC_USB_PD_TBT_COMPAT_MODE
#define CONFIG_USB_PD_TBT_COMPAT_MODE
#endif

#undef CONFIG_USB_PD_USB4
#ifdef CONFIG_PLATFORM_EC_USB_PD_USB4
#define CONFIG_USB_PD_USB4
#endif

#undef CONFIG_USB_PD_FRS
#ifdef CONFIG_PLATFORM_EC_USB_PD_FRS
#define CONFIG_USB_PD_FRS
#endif

#undef CONFIG_USB_PD_FRS_TCPC
#ifdef CONFIG_PLATFORM_EC_USB_PD_FRS_TCPC
#define CONFIG_USB_PD_FRS_TCPC
#endif

#undef CONFIG_USB_PD_FRS_PPC
#ifdef CONFIG_PLATFORM_EC_USB_PD_FRS_PPC
#define CONFIG_USB_PD_FRS_PPC
#endif

#undef CONFIG_VSTORE
#undef VSTORE_SLOT_COUNT
#ifdef CONFIG_PLATFORM_EC_VSTORE
#define CONFIG_VSTORE
#define CONFIG_VSTORE_SLOT_COUNT	CONFIG_PLATFORM_EC_VSTORE_SLOT_COUNT
#endif

/* motion sense */
#undef CONFIG_MOTIONSENSE
#ifdef CONFIG_PLATFORM_EC_MOTIONSENSE
#define CONFIG_MOTIONSENSE

#undef CONFIG_ACCEL_FIFO
#undef CONFIG_ACCEL_FIFO_SIZE
#undef CONFIG_ACCEL_FIFO_THRES
#ifdef CONFIG_PLATFORM_EC_ACCEL_FIFO
#define CONFIG_ACCEL_FIFO
#define CONFIG_ACCEL_FIFO_SIZE CONFIG_PLATFORM_EC_ACCEL_FIFO_SIZE
#define CONFIG_ACCEL_FIFO_THRES CONFIG_PLATFORM_EC_ACCEL_FIFO_THRES
#endif /* CONFIG_PLATFORM_EC_ACCEL_FIFO */

#undef CONFIG_CMD_ACCELS
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_ACCELS
#define CONFIG_CMD_ACCELS
#endif

#undef CONFIG_CMD_ACCEL_INFO
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_ACCEL_INFO
#define CONFIG_CMD_ACCEL_INFO
#endif

#undef CONFIG_ACCEL_SPOOF_MODE
#ifdef CONFIG_PLATFORM_EC_ACCEL_SPOOF_MODE
#define CONFIG_ACCEL_SPOOF_MODE
#endif

#undef CONFIG_CMD_ACCEL_SPOOF
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_ACCEL_SPOOF
#define CONFIG_CMD_ACCEL_SPOOF
#endif

#undef CONFIG_SENSOR_TIGHT_TIMESTAMPS
#ifdef CONFIG_PLATFORM_EC_SENSOR_TIGHT_TIMESTAMPS
#define CONFIG_SENSOR_TIGHT_TIMESTAMPS
#endif

#undef CONFIG_ACCEL_INTERRUPTS
#ifdef CONFIG_PLATFORM_EC_ACCEL_INTERRUPTS
#define CONFIG_ACCEL_INTERRUPTS
#endif

#undef CONFIG_ALS
#undef CONFIG_ALS_COUNT
#ifdef CONFIG_PLATFORM_EC_ALS
#define CONFIG_ALS
#define ALS_COUNT CONFIG_PLATFORM_EC_ALS_COUNT
#else
#define ALS_COUNT 0
#endif

#undef CONFIG_DYNAMIC_MOTION_SENSOR_COUNT
#ifdef CONFIG_PLATFORM_EC_DYNAMIC_MOTION_SENSOR_COUNT
#define CONFIG_DYNAMIC_MOTION_SENSOR_COUNT
#endif

#undef CONFIG_LID_ANGLE
#ifdef CONFIG_PLATFORM_EC_LID_ANGLE
#define CONFIG_LID_ANGLE
#endif

#undef CONFIG_LID_ANGLE_UPDATE
#ifdef CONFIG_PLATFORM_EC_LID_ANGLE_UPDATE
#define CONFIG_LID_ANGLE_UPDATE
#endif

#undef CONFIG_TABLET_MODE
#ifdef CONFIG_PLATFORM_EC_TABLET_MODE
#define CONFIG_TABLET_MODE
#endif

#undef CONFIG_GMR_TABLET_MODE
#ifdef CONFIG_PLATFORM_EC_GMR_TABLET_MODE
#define CONFIG_GMR_TABLET_MODE
#endif

/* sensors */
#undef CONFIG_ACCELGYRO_BMI260
#ifdef CONFIG_PLATFORM_EC_ACCELGYRO_BMI260
#define CONFIG_ACCELGYRO_BMI260
#endif

#undef CONFIG_ACCEL_BMA255
#ifdef CONFIG_PLATFORM_EC_ACCEL_BMA255
#define CONFIG_ACCEL_BMA255
#endif

#undef CONFIG_ALS_TCS3400
#ifdef CONFIG_PLATFORM_EC_ALS_TCS3400
#define CONFIG_ALS_TCS3400
#endif

#endif /* CONFIG_PLATFORM_EC_MOTIONSENSE */

#undef CONFIG_HOSTCMD_GET_UPTIME_INFO
#ifdef CONFIG_PLATFORM_EC_HOSTCMD_GET_UPTIME_INFO
#define CONFIG_HOSTCMD_GET_UPTIME_INFO
#endif

#undef CONFIG_CMD_AP_RESET_LOG
#ifdef CONFIG_PLATFORM_EC_AP_RESET_LOG
#define CONFIG_CMD_AP_RESET_LOG
#endif

#undef CONFIG_POWER_BUTTON
#ifdef CONFIG_PLATFORM_EC_POWER_BUTTON
#define CONFIG_POWER_BUTTON
#endif

#undef CONFIG_COMMON_PANIC_OUTPUT
#ifdef CONFIG_PLATFORM_EC_PANIC
#define CONFIG_COMMON_PANIC_OUTPUT
#endif

#undef CONFIG_SOFTWARE_PANIC
#ifdef CONFIG_PLATFORM_EC_SOFTWARE_PANIC
#define CONFIG_SOFTWARE_PANIC
#endif

#undef CONFIG_CMD_CRASH
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_CRASH
#define CONFIG_CMD_CRASH
#endif

#undef CONFIG_CMD_STACKOVERFLOW
#ifdef CONFIG_PLATFORM_EC_STACKOVERFLOW
#define CONFIG_CMD_STACKOVERFLOW
#endif

#undef CONFIG_CMD_MEM
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_MEM
#define CONFIG_CMD_MEM
#endif

#undef CONFIG_CMD_MD
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_MD
#define CONFIG_CMD_MD
#endif

#undef CONFIG_CMD_RW
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_RW
#define CONFIG_CMD_RW
#endif

#undef CONFIG_RTC
#ifdef CONFIG_PLATFORM_EC_RTC
#define CONFIG_RTC
#endif

#undef CONFIG_CMD_RTC
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_RTC
#define CONFIG_CMD_RTC
#endif

#undef CONFIG_CMD_RTC_ALARM
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_RTC_ALARM
#define CONFIG_CMD_RTC_ALARM
#endif

#undef CONFIG_HOSTCMD_RTC
#ifdef CONFIG_PLATFORM_EC_HOSTCMD_RTC
#define CONFIG_HOSTCMD_RTC
#endif

#undef CONFIG_SWITCH
#ifdef CONFIG_PLATFORM_EC_SWITCH
#define CONFIG_SWITCH
#endif

#undef CONFIG_LN9310
#ifdef CONFIG_PLATFORM_EC_SWITCHCAP_LN9310
#define CONFIG_LN9310
#endif

#undef CONFIG_BOARD_VERSION_CBI
#ifdef CONFIG_PLATFORM_EC_BOARD_VERSION_CBI
#define CONFIG_BOARD_VERSION_CBI
#endif

#undef CONFIG_BOARD_VERSION_GPIO
#ifdef CONFIG_PLATFORM_EC_BOARD_VERSION_GPIO
#define CONFIG_BOARD_VERSION_GPIO
#endif

#undef CONFIG_BOARD_VERSION_CUSTOM
#ifdef CONFIG_PLATFORM_EC_BOARD_VERSION_CUSTOM
#define CONFIG_BOARD_VERSION_CUSTOM
#endif

#undef CONFIG_CROS_BOARD_INFO
#ifdef CONFIG_PLATFORM_EC_CBI
#define CONFIG_CROS_BOARD_INFO
#endif

#undef CONFIG_VBOOT_HASH
#ifdef CONFIG_PLATFORM_EC_VBOOT_HASH
#define CONFIG_VBOOT_HASH
#endif

#undef CONFIG_SHA256_HW_ACCELERATE
#ifdef CONFIG_PLATFORM_EC_SHA256_HW_ACCELERATE
#define CONFIG_SHA256_HW_ACCELERATE
#endif

#undef CONFIG_RO_HDR_MEM_OFF
#ifdef CONFIG_PLATFORM_EC_RO_HEADER_OFFSET
#define CONFIG_RO_HDR_MEM_OFF	CONFIG_PLATFORM_EC_RO_HEADER_OFFSET
#else
#define CONFIG_RO_HDR_MEM_OFF 0
#endif

#undef CONFIG_RO_HDR_SIZE
#ifdef CONFIG_PLATFORM_EC_RO_HEADER_SIZE
#define CONFIG_RO_HDR_SIZE	CONFIG_PLATFORM_EC_RO_HEADER_SIZE
#else
#define CONFIG_RO_HDR_SIZE 0
#endif

#undef CONFIG_SYSTEM_UNLOCKED
#ifdef CONFIG_PLATFORM_EC_SYSTEM_UNLOCKED
#define CONFIG_SYSTEM_UNLOCKED
#endif

#undef CONFIG_BYPASS_CBI_EEPROM_WP_CHECK
#ifdef CONFIG_PLATFORM_EC_BYPASS_CBI_EEPROM_WP_CHECK
#define CONFIG_BYPASS_CBI_EEPROM_WP_CHECK
#endif

#undef CONFIG_SPI_FLASH_REGS
#ifdef CONFIG_PLATFORM_EC_SPI_FLASH_REGS
#define CONFIG_SPI_FLASH_REGS
#endif

#undef CONFIG_CMD_CHARGEN
#ifdef PLATFORM_EC_CONSOLE_CMD_CHARGEN
#define CONFIG_CMD_CHARGEN
#endif

#undef CONFIG_DEBUG_ASSERT
#ifdef CONFIG_PLATFORM_EC_DEBUG_ASSERT
#define CONFIG_DEBUG_ASSERT
#endif

#undef CONFIG_DEBUG_ASSERT_BRIEF
#ifdef CONFIG_PLATFORM_EC_DEBUG_ASSERT_BRIEF
#define CONFIG_DEBUG_ASSERT_BRIEF
#endif

#undef CONFIG_DEBUG_ASSERT_REBOOTS
#ifdef CONFIG_PLATFORM_EC_DEBUG_ASSERT_REBOOTS
#define CONFIG_DEBUG_ASSERT_REBOOTS
#endif

#undef CONFIG_MPU
#ifdef CONFIG_PLATFORM_EC_MPU
#define CONFIG_MPU
#endif

#undef CONFIG_CMD_SYSJUMP
#ifdef CONFIG_PLATFORM_EC_CONSOLE_CMD_SYSJUMP
#define CONFIG_CMD_SYSJUMP
#endif

#undef CONFIG_WATCHDOG_PERIOD_MS
#ifdef CONFIG_PLATFORM_EC_WATCHDOG_PERIOD_MS
#define CONFIG_WATCHDOG_PERIOD_MS CONFIG_PLATFORM_EC_WATCHDOG_PERIOD_MS
/*
 * Chromium ec uses hook tick to reload the watchdog. Interval between reloads
 * of the watchdog timer should be less than half of the watchdog period.
 */
#if (CONFIG_WATCHDOG_PERIOD_MS) < ((HOOK_TICK_INTERVAL_MS) * 2)
#error "CONFIG_WATCHDOG_PERIOD_MS must be at least 2x HOOK_TICK_INTERVAL_MS"
#endif
#endif /* CONFIG_PLATFORM_EC_WATCHDOG_PERIOD_MS */

#undef CONFIG_VBOOT_EFS
#undef CONFIG_VBOOT_EFS2
#ifdef CONFIG_PLATFORM_EC_VBOOT
#define CONFIG_VBOOT_EFS2
#endif

#undef CONFIG_USB_PD_TCPC_VCONN
#ifdef CONFIG_PLATFORM_EC_USB_PD_TCPC_VCONN
#define CONFIG_USB_PD_TCPC_VCONN
#endif

#undef CONFIG_DPTF
#ifdef CONFIG_PLATFORM_EC_DPTF
#define CONFIG_DPTF
#endif

#undef CONFIG_CHARGER_BQ25710
#ifdef CONFIG_PLATFORM_EC_CHARGER_BQ25710
#define CONFIG_CHARGER_BQ25710
#endif

#undef CONFIG_CHARGER_BQ25720
#ifdef CONFIG_PLATFORM_EC_CHARGER_BQ25720
#define CONFIG_CHARGER_BQ25720
#endif

#undef CONFIG_HIBERNATE_PSL
#ifdef CONFIG_PLATFORM_EC_HIBERNATE_PSL
#define CONFIG_HIBERNATE_PSL
#endif

#undef CONFIG_BATTERY_DEVICE_CHEMISTRY
#ifdef CONFIG_PLATFORM_EC_USE_BATTERY_DEVICE_CHEMISTRY
#define CONFIG_BATTERY_DEVICE_CHEMISTRY	\
	CONFIG_PLATFORM_EC_BATTERY_DEVICE_CHEMISTRY
#endif

#undef CONFIG_BATTERY_REVIVE_DISCONNECT
#ifdef CONFIG_PLATFORM_EC_BATTERY_REVIVE_DISCONNECT
#define CONFIG_BATTERY_REVIVE_DISCONNECT
#endif

#undef CONFIG_CHARGER_DISCHARGE_ON_AC
#ifdef CONFIG_PLATFORM_EC_CHARGER_DISCHARGE_ON_AC
#define CONFIG_CHARGER_DISCHARGE_ON_AC
#endif

#undef CONFIG_CHARGER_DISCHARGE_ON_AC_CUSTOM
#ifdef CONFIG_PLATFORM_EC_CHARGER_DISCHARGE_ON_AC_CUSTOM
#define CONFIG_CHARGER_DISCHARGE_ON_AC_CUSTOM
#endif

#endif  /* __CROS_EC_CONFIG_CHIP_H */
