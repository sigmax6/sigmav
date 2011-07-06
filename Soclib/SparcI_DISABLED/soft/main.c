/*
 *
 * SOCLIB_GPL_HEADER_BEGIN
 *
 * This file is part of SoCLib, GNU GPLv2.
 *
 * SoCLib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * SoCLib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SoCLib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * SOCLIB_GPL_HEADER_END
 *
 * Copyright (c) UPMC, Lip6, SoC
 *         Nicolas Pouillon <nipo@ssji.net>, 2006-2007
 *         Francois Pecheux <francois.pecheux@lip6.fr>, 2006-2007
 */

#include "soclib/timer.h"
#include "system.h"
#include "stdio.h"

#include "../segmentation.h"

static const int period[4] = {10000, 11000, 12000, 13000};

static int interrupt_counter = 0;

void irq_handler(int irq)
{
    printf("IRQ handle\n\n");
	uint32_t ti;
	int count = atomic_add(&interrupt_counter, 1);

	ti = soclib_io_get(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_VALUE);
	printf("IRQ %d received at cycle %d on cpu %d, %d interrupts received\n\n", irq, ti, procnum(), count);
	soclib_io_set(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_RESETIRQ,
		0);
}

int main(void)
{
	const int cpu = procnum();
    unsigned int time;
	printf("Hello from processor %d\n", procnum());

	set_irq_handler(irq_handler);
//    irq_set_pil(0);
//	enable_hw_irq(0);

//	irq_enable();
//	 included in enable_hw_irq();

    printf("timer base:%x\n",base(TIMER));
    printf("TIMER_SPAN:%d\n",TIMER_SPAN);
    printf("TIMER_RERIOD:%d\n",TIMER_PERIOD);
   	time = soclib_io_get(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_VALUE);
    printf("timer value:%d\n",time );


	soclib_io_set(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_PERIOD,
        10000);
	//	period[cpu]);

	soclib_io_set(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_MODE,
		TIMER_RUNNING|TIMER_IRQ_ENABLED);

    int i=0,j=0;
    for(;i<100000;i++)
    {
        for(;j<10000;j++)
            ;
    }

	time = soclib_io_get(
		base(TIMER),
		procnum()*TIMER_SPAN+TIMER_VALUE);
    printf("timer value:%d\n",time );

    printf("timer reg:%d\n",(unsigned int)*(unsigned int *)base(TIMER));

	while (1)
        ;
	//	pause();
	return 0;
}
