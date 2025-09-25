#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_const.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_console.h>

#include <sbi_utils/ipi/aclint_mswi.h>
#include <sbi_utils/timer/aclint_mtimer.h>
#include <sbi_utils/serial/custom_uart.h>

//#include <sbi/sbi_hart.h>
//#include <sbi/sbi_ipi.h>
//#include <sbi/sbi_hsm.h>

#define PLATFORM_CLINT_ADDR		0xFF080000
#define PLATFORM_ACLINT_MTIMER_FREQ	1000000
#define PLATFORM_ACLINT_MSWI_ADDR	(PLATFORM_CLINT_ADDR + CLINT_MSWI_OFFSET)
#define PLATFORM_ACLINT_MTIMER_ADDR	(PLATFORM_CLINT_ADDR + CLINT_MTIMER_OFFSET)

#define PLATFORM_HART_COUNT		1

#define CPU_MHZ 50
#define CPU_CLK (CPU_MHZ * 1000000)
#define BAUD_RATE 921600

//static int custom_hsm_hart_start(u32 hartid, ulong saddr)
//{
//	sbi_ipi_send_smode(hartid, 0);
//	return 0;
//}
//
//// While power downing a hart we are just hanging.
//static int custom_hsm_hart_stop()
//{
//	sbi_hart_hang();
//	return 0;
//}
//
//const struct sbi_hsm_device custom_hsm = {
//	.name		= "custom-hsm",
//	.hart_start	= custom_hsm_hart_start,
//	.hart_stop	= custom_hsm_hart_stop,
//};

static struct aclint_mswi_data mswi = {
	.addr = PLATFORM_ACLINT_MSWI_ADDR,
	.size = ACLINT_MSWI_SIZE,
	.first_hartid = 0,
	.hart_count = PLATFORM_HART_COUNT,
};

static struct aclint_mtimer_data mtimer = {
	.mtime_freq = PLATFORM_ACLINT_MTIMER_FREQ,
	.mtime_addr = PLATFORM_ACLINT_MTIMER_ADDR +
		      ACLINT_DEFAULT_MTIME_OFFSET,
	.mtime_size = ACLINT_DEFAULT_MTIME_SIZE,
	.mtimecmp_addr = PLATFORM_ACLINT_MTIMER_ADDR +
			 ACLINT_DEFAULT_MTIMECMP_OFFSET,
	.mtimecmp_size = ACLINT_DEFAULT_MTIMECMP_SIZE,
	.first_hartid = 0,
	.hart_count = PLATFORM_HART_COUNT,
	.has_64bit_mmio = false,
};

static int platform_ipi_init(void)
{
	return aclint_mswi_cold_init(&mswi);
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
	return aclint_mtimer_cold_init(&mtimer, NULL);
	//return 0;
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
