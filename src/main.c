#include <stddef.h>
#include <stdint.h>
#include "uart.h"
#include "io.h"
#include "utils.h"
#include "interrupt.h"
#include "irq.h"
#include "timer.h"
#include "scheduler.h"
#include "fork.h"

void test_process(char c)
{
	while(1) {
		printf("%c", c);
		sleep(100000000);
	}
}

void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{
	// initialize UART for Raspi2
	uart_init(3);
	printf("Hel%co, %s World!\n", 'l', "kernel");
	printf("Current EL:%d\n", get_el());

	irq_vector_init();
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	int r = 0;
	r = copy_process((unsigned long)&test_process, 'a');
	if(r != 0) {
		printf("error process 1");
		return;
	}
	r = copy_process((unsigned long)&test_process, 'c');
	if(r != 0) {
		printf("error process 1");
		return;
	}

	while (1)
		schedule();
}
