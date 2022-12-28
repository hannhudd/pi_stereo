/* Implement bit-banging based software UART */

// initialize software uart module
void soft_uart_init(unsigned int baud);

// send character
void soft_uart_putchar(unsigned char ch);
