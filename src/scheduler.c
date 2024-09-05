#include "scheduler.h"
#include "irq.h"

static task_struct init_task = INIT_TASK;
task_struct *current = &init_task;
task_struct *task[NR_TASKS] = {&init_task, };
int nr_tasks = 1;

void _schedule()
{
	preempt_disable();
	int next = 0;
	while(1) {
		int c = -1;
		task_struct *p;
		for(int i = 0; i < NR_TASKS; i++) {
			p = task[i];
			if(p && p->state == TASK_RUNNING && p->counter > c) {
				c = p->counter;
				next = i;
			}
		}
		if(c) break;
		for(int i = 0; i < NR_TASKS; i++) {
			p = task[i];
			if(p)
				p->counter = (p->counter >> 1) + p->priority;
		}
	}
	switch_to(task[next]);
	preempt_enable();
}

void preempt_enable()
{
	current->preempt_count--;
}

void preempt_disable()
{
	current->preempt_count++;
}

void switch_to(task_struct *next)
{
	if(current == next) return;
	task_struct *prev = current;
	current = next;
	cpu_switch_to(prev, next);
}

void schedule()
{
	current->counter = 0;
	_schedule();
}

void timer_tick()
{
	current->counter--;
	if(current->counter > 0 || current->preempt_count > 0)
		return;

	current->counter = 0;
	enable_irq();
	_schedule();
	disable_irq();
}

void schedule_tail()
{
	preempt_enable();
}
