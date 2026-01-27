#include <sbi/riscv_asm.h>
#include <sbi/riscv_io.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_domain.h>
#include <sbi_utils/serial/custom_uart.h>

static void uart_txfull()
{
    while ((UART_CFG & 0x4) == 0) { }
}

static void zputchar(char c)
{
    uart_txfull();
    UART_CFG &= ~0x4;
    UART_TDR = c;
    UART_CFG = UART_CFG | 0x1;
    uart_txfull();
}

static void custom_uart_putc(char ch)
{
	zputchar(ch);
}

//static int uart_rxempty()
//{
//    return (UART_CFG & 0x2) == 0;
//}
//
//static char zgetchar()
//{
//    while (uart_rxempty()) {}
//    char c = (char)UART_RDR;
//    UART_CFG &= ~0x2;
//    return c;
//}

static int custom_uart_getc(void)
{
    if ((UART_CFG & 0x2) == 0) {
    	return -1;
    }

    return (int)UART_RDR;

	//return zgetchar();
    //return uart_rxempty() ? -1 : zgetchar();
	//return -1;
}

static struct sbi_console_device custom_uart_console = {
	.name = "custom_uart",
	.console_putc = custom_uart_putc,
	.console_getc = custom_uart_getc
};

int custom_uart_init(unsigned long base, u32 in_freq, u32 baudrate)
{
	uart_cpb uart_cpb;
    uart_cpb.fields.data = CPU_CLK / BAUD_RATE;
    UART_CFG = UART_CFG | 0x7;
    UART_CPB = uart_cpb.bits;
    UART_STP = UART_STP | 0x1;	

	sbi_console_set_device(&custom_uart_console);

	return sbi_domain_root_add_memrange(base, PAGE_SIZE, PAGE_SIZE,
					    (SBI_DOMAIN_MEMREGION_MMIO |
					    SBI_DOMAIN_MEMREGION_SHARED_SURW_MRW));
}
