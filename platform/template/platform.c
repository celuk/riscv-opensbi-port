#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_const.h>
#include <sbi/sbi_platform.h>

#include <sbi_utils/serial/custom_uart.h>
#include <sbi_utils/irqchip/plic.h>
#include <sbi_utils/ipi/fdt_ipi.h>
#include <sbi_utils/timer/fdt_timer.h>

#define PLATFORM_HART_COUNT		1

#define CPU_MHZ 50
#define CPU_CLK (CPU_MHZ * 1000000)
#define BAUD_RATE 921600

static int platform_ipi_init(void)
{
	return fdt_ipi_init();
}

/*
 * Platform early initialization.
 */
static int platform_early_init(bool cold_boot)
{
	if (!cold_boot)
		return 0;

	//return custom_uart_init(0xFF000000, CPU_CLK, BAUD_RATE); //uart8250_init(PLATFORM_UART_ADDR, PLATFORM_UART_INPUT_FREQ, PLATFORM_UART_BAUDRATE, 0, 1, 0, 0);

	custom_uart_init(0xFF000000, CPU_CLK, BAUD_RATE);

	//sbi_hsm_set_device(&custom_hsm);

	return 0;
}

/*
 * Platform final initialization.
 */
static int platform_final_init(bool cold_boot)
{
	//if (!cold_boot)
	//	return 0;
	
	//sbi_hsm_set_device(&custom_hsm);

	return 0;
}

/*
 * Initialize platform timer during cold boot.
 */
static int platform_timer_init(void)
{
	return fdt_timer_init();
}

/*
 * Platform descriptor.
 */
const struct sbi_platform_operations platform_ops = {
	.early_init		= platform_early_init,
	.final_init		= platform_final_init,
	.ipi_init		= platform_ipi_init,
	.timer_init		= platform_timer_init
};
const struct sbi_platform platform = {
	.opensbi_version	= OPENSBI_VERSION,
	.platform_version	= SBI_PLATFORM_VERSION(0x0, 0x00),
	.name			= "custom-platform",
	.features		= SBI_PLATFORM_DEFAULT_FEATURES,
	.hart_count		= 1,
	.hart_stack_size	= SBI_PLATFORM_DEFAULT_HART_STACK_SIZE,
	.heap_size		= SBI_PLATFORM_DEFAULT_HEAP_SIZE(1),
	.platform_ops_addr	= (unsigned long)&platform_ops
};
