#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* insert code */
  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
  /* insert code */
}

void Scheduler_P_EDF (Task Tasks[])
{ 
  /* insert code */
  /* Use ExecuteTask function to execute a task */
  uint8_t oldBP = BusyPrio; // Set BusyPrio as the priority of the currently running task (the one that was executing just before the scheduler is called)
  uint8_t i = 0;
  uint8_t new_prio[10];
  uint8_t a = 0;
  uint8_t b = 0;
  for(i = 0; i < NUMTASKS; i++){
    new_prio[i] = i;
  }
  for(i = NUMTASKS - 1; i > 0; i--){
    Taskp t = &Tasks[i];
    Taskp t1 = &Tasks[i - 1];
    a = new_prio[i];
    b = new_prio[i - 1];
    if(t->NextPendingDeadline < t1->NextPendingDeadline){
      new_prio[i] = b;
      new_prio[i - 1] = a;
    }else if(t->NextPendingDeadline == t1->NextPendingDeadline){
      if(t->Prio < t1->Prio){
        new_prio[i] = b;
        new_prio[i - 1] = a;
      }
    }
  }

  for(BusyPrio = 0; BusyPrio < oldBP; BusyPrio++) {
    Taskp t = &Tasks[new_prio[BusyPrio]];
    while ((t->Activated != t->Invoked) && (t->Flags & TRIGGERED))
    {
      _EINT(); 
      ExecuteTask(t);
      _DINT();
      t->Activated = t->Invoked;
    }
    
  }
  /* insert code */
}
