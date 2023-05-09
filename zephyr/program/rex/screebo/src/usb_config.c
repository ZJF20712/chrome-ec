/* Copyright 2023 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Screebo board-specific USB-C configuration */

#include "console.h"
#include "cros_board_info.h"
#include "cros_cbi.h"
#include "driver/ppc/nx20p348x.h"
#include "driver/tcpm/ps8xxx_public.h"
#include "hooks.h"
#include "ppc/syv682x_public.h"
#include "system.h"
#include "usb_config.h"
#include "usbc/ppc.h"
#include "usbc/tcpci.h"
#include "usbc/usb_muxes.h"
#include "usbc_config.h"
#include "usbc_ppc.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include <dt-bindings/gpio_defines.h>

LOG_MODULE_REGISTER(screebo, LOG_LEVEL_INF);

uint32_t usb_db_type;

void screebo_ppc_interrupt(enum gpio_signal signal)
{
	switch (signal) {
	case GPIO_USB_C0_PPC_INT_ODL:
		syv682x_interrupt(USBC_PORT_C0);
		break;
	case GPIO_USB_C1_PPC_INT_ODL:
		if (usb_db_type == FW_USB_DB_USB3) {
			nx20p348x_interrupt(USBC_PORT_C1);
		} else {
			syv682x_interrupt(USBC_PORT_C1);
		}
		break;
	default:
		break;
	}
}

void board_reset_pd_mcu(void)
{
	/* Reset TCPC0 */
	reset_nct38xx_port(USBC_PORT_C0);

	/* Reset TCPC1 */
	if (usb_db_type == FW_USB_DB_USB3) {
		if (tcpc_config[1].rst_gpio.port) {
			gpio_pin_set_dt(&tcpc_config[1].rst_gpio, 1);
			msleep(PS8XXX_RESET_DELAY_MS);
			gpio_pin_set_dt(&tcpc_config[1].rst_gpio, 0);
			msleep(PS8815_FW_INIT_DELAY_MS);
		}
	} else {
		reset_nct38xx_port(USBC_PORT_C1);
	}
}

static int gpio_unused(const struct gpio_dt_spec *spec)
{
	return gpio_pin_configure(spec->port, spec->pin, GPIO_INPUT_PULL_UP);
}

static void setup_runtime_gpios(void)
{
	int ret;

	ret = cros_cbi_get_fw_config(FW_USB_DB, &usb_db_type);
	if (ret != EC_SUCCESS) {
		LOG_INF("Failed to get FW_USB_DB from CBI");
		usb_db_type = FW_USB_DB_NOT_CONNECTED;
	}

	switch (usb_db_type) {
	case FW_USB_DB_USB3:
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rst_odl));
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rt_int_odl));
		gpio_pin_configure_dt(GPIO_DT_FROM_ALIAS(ps_usb_c1_rt_rst_odl),
				      (GPIO_ODR_HIGH | GPIO_ACTIVE_LOW));
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_frs_en));
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rt_3p3_sx_en));
		break;

	case FW_USB_DB_USB4_HB:
		gpio_pin_configure_dt(GPIO_DT_FROM_ALIAS(nct_usb_c1_rst_odl),
				      (GPIO_ODR_HIGH | GPIO_ACTIVE_LOW));
		gpio_pin_configure_dt(GPIO_DT_FROM_ALIAS(hbr_usb_c1_rt_int_odl),
				      GPIO_INPUT);
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rt_rst_r_odl));
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_frs_en));
		gpio_pin_configure_dt(GPIO_DT_FROM_ALIAS(hbr_usb_c1_rt_pwr_en),
				      GPIO_OUTPUT_LOW);
		gpio_pin_configure_dt(GPIO_DT_FROM_ALIAS(hbr_usb_c1_rt_rst),
				      GPIO_OUTPUT);
		gpio_pin_configure_dt(GPIO_DT_FROM_ALIAS(syv_usb_c1_frs_en),
				      GPIO_OUTPUT_LOW);
		break;

	default:
		/* GPIO37 */
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rst_odl));
		/* GPIO72 */
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rt_int_odl));
		/* GPIO74 */
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rt_rst_r_odl));
		/* GPIO83 */
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_frs_en));
		/* GPIOB1 */
		gpio_unused(GPIO_DT_FROM_NODELABEL(gpio_usb_c1_rt_3p3_sx_en));
		break;
	}
}
DECLARE_HOOK(HOOK_INIT, setup_runtime_gpios, HOOK_PRIO_FIRST);

static void setup_alt_db(void)
{
	if (usb_db_type == FW_USB_DB_USB3) {
		LOG_INF("USB DB: USB3 DB connected");
		USB_MUX_ENABLE_ALTERNATIVE(usb_mux_chain_ps8815_port1);
		TCPC_ENABLE_ALTERNATE_BY_NODELABEL(USBC_PORT_C1,
						   tcpc_ps8815_port1);
		PPC_ENABLE_ALTERNATE_BY_NODELABEL(USBC_PORT_C1,
						  ppc_nx20p_port1);
	}
}
DECLARE_HOOK(HOOK_INIT, setup_alt_db, HOOK_PRIO_POST_I2C);
