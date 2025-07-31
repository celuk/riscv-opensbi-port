#ifndef __SERIAL_CUSTOM_UART_H__
#define __SERIAL_CUSTOM_UART_H__

#include <sbi/sbi_types.h>

#define CPU_MHZ 50
#define CPU_CLK (CPU_MHZ * 1000000)
#define BAUD_RATE 921600

#define UART_CPB       (*(volatile unsigned int*)0xFF000000)
#define UART_STP       (*(volatile unsigned int*)0xFF000004)
#define UART_RDR       (*(volatile unsigned int*)0xFF000008)
#define UART_TDR       (*(volatile unsigned int*)0xFF00000c)
#define UART_CFG       (*(volatile unsigned int*)0xFF000010)

typedef union
{
	struct {
		unsigned int cfg_0    : 1;
		unsigned int cfg_1 	  : 1;
		unsigned int cfg_2 	  : 1;
		unsigned int null	  : 29;
	} fields;
	uint32_t bits;
}uart_cfg;

typedef union
{
	struct {
		unsigned int stp    : 2;
		unsigned int null	  : 30;
	} fields;
	uint32_t bits;
}uart_stp;

typedef union
{
	struct {
		unsigned int data    : 8;
		unsigned int null    : 24;
	} fields;
	uint32_t bits;
}uart_tdr;

typedef union
{
	struct {
		unsigned int data    : 8;
		unsigned int null    : 24;
	} fields;
	uint32_t bits;
}uart_rdr;

typedef union
{
	struct {
		unsigned int data    : 32;
	} fields;
	uint32_t bits;
}uart_cpb;

int custom_uart_init(unsigned long base, u32 in_freq, u32 baudrate);

#endif
