#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#define OFFSET_CUP_CONTEXT  0

#ifndef __ASSEMBLER__

#define NR_TASKS      64
#define TASK_RUNNING  0

#define THREAD_SIZE   4096

#define INIT_TASK { \
  {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
  0, 0, 1, 0 }

struct cpu_context {
  unsigned long x19;
  unsigned long x20;
  unsigned long x21;
  unsigned long x22;
  unsigned long x23;
  unsigned long x24;
  unsigned long x25;
  unsigned long x26;
  unsigned long x27;
  unsigned long x28;
  unsigned long fp;
  unsigned long sp;
  unsigned long pc;
};

typedef struct {
  struct cpu_context cpu_context;
  long state;
  long counter;
  long priority;
  long preempt_count;
}task_struct;

extern task_struct *current;
extern task_struct *task[NR_TASKS];
extern int nr_tasks;

void preempt_enable();
void preempt_disable();
void switch_to(task_struct *next);
void cpu_switch_to(task_struct *prev, task_struct *next);
void schedule();
void timer_tick();
void schedule_tail();

#endif //__ASSEMBLER__
#endif //__SCHEDULER_H__
