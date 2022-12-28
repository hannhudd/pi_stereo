#include "software-uart.h"
#include "timer.h"
#include "gpio.h"
#include "pi.h"

void main(void)
{
    gpio_init();
    soft_uart_init(9600);
    soft_uart_putchar(0x7e);
    soft_uart_putchar(0xff);
    soft_uart_putchar(0x06);
    soft_uart_putchar(0x01);
    soft_uart_putchar(0x00);
    soft_uart_putchar(0x00);
    soft_uart_putchar(0x00);
    soft_uart_putchar(0xfe);
    soft_uart_putchar(0xfa);
    soft_uart_putchar(0xef);
    pi_reboot();
}
