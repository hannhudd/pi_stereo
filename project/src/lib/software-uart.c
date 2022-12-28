#include "gpio.h"
#include "timer.h"
#include "malloc.h"

#define SOFT_RX GPIO_PIN5
#define SOFT_TX GPIO_PIN6
#define SEC_TO_USEC 1000000
#define LOW 0
#define HIGH 1

struct soft_uart_t {
    unsigned int tx;
    unsigned int rx; // didnt end up needing rx
    unsigned int baud;
};

static struct soft_uart_t *soft_uart = NULL;

// initialize software uart module
void soft_uart_init(unsigned int baud)
{
    if (soft_uart != NULL) free(soft_uart);
    soft_uart = malloc(sizeof(struct soft_uart_t));
    
    soft_uart->tx = SOFT_TX;
    soft_uart->rx = SOFT_RX;
    soft_uart->baud = SEC_TO_USEC / baud;
    
    gpio_set_output(soft_uart->tx);
    gpio_write(soft_uart->tx, HIGH);
    gpio_set_input(soft_uart->rx);
}

// send character
void soft_uart_putchar(unsigned char ch)
{
    gpio_write(soft_uart->tx, LOW); // start byte
    timer_delay_us(soft_uart->baud);
    for (int i = 0; i < 8; i++) {
        gpio_write(soft_uart->tx, ch & (HIGH << i)); // lsb first
        timer_delay_us(soft_uart->baud);
    }
    gpio_write(soft_uart->tx, HIGH); // stop byte
    timer_delay_us(soft_uart->baud);
}

