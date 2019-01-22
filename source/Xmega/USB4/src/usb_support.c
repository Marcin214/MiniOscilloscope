#include "conf_usb.h"

void main_suspend_action(void)
{
}

void main_resume_action(void)
{
}

void main_sof_action(void)
{
}

bool main_cdc_enable(uint8_t port)
{
	return true;
}

void main_cdc_disable(uint8_t port)
{
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable)
	{
	}
	else
	{
	}
}

void uart_rx_notify(uint8_t port)
{
}

void uart_config(uint8_t port, usb_cdc_line_coding_t * cfg)
{
}