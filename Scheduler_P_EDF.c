#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
}

void Scheduler_P_EDF (Task Tasks[])
{ 
  uint8_t oldBP = BusyPrio; // Set BusyPrio as the priority of the currently running task (the one that was executing just before the scheduler is called)
  uint8_t i = 0;
  uint8_t new_prio[10];
  uint8_t a = 0;
  uint8_t b = 0;
  uint8_t c = 0;
  for(i = 0; i < NUMTASKS; i++){
    new_prio[i] = i;
  }

  for(c = 0; c < NUMTASKS; c++){
    for(i = NUMTASKS - 1; i > 0; i--){
      Taskp t = &Tasks[new_prio[i]];
      Taskp t1 = &Tasks[new_prio[i - 1]];
      a = new_prio[i];
      b = new_prio[i - 1];
      if(t->NextPendingDeadline < t1->NextPendingDeadline){
        new_prio[i] = b;
        new_prio[i - 1] = a;
      }
    }
  }
  for(c = 0; c < NUMTASKS; c++){
    for(i = NUMTASKS - 1; i > 0; i--){
      Taskp t = &Tasks[new_prio[i]];
      Taskp t1 = &Tasks[new_prio[i - 1]];
      a = new_prio[i];
      b = new_prio[i - 1];
      // if(t1->Activated == t1->Invoked){
      //   new_prio[i] = b;
      //   new_prio[i - 1] = a;
      // }
      if(t->NextPendingDeadline == t1->NextPendingDeadline){
        if(t1->BusyFlag == 1){
          new_prio[i] = b;
          new_prio[i - 1] = a;
        }
      }
    }
  }

  for(i = 0; i < NUMTASKS; i++){
    if(new_prio[i] == 0){
      SetLeds(PINK, 1);
      SetLeds(PINK, 0);      
    }
    for(c = 0; c < new_prio[i]; c++){
      SetLeds(WHITE, 1);
      SetLeds(WHITE, 0);
    }
      SetLeds(BROWN, 1);
      SetLeds(BROWN, 0);
  }

  for(BusyPrio = 0; BusyPrio < oldBP; BusyPrio++) {
    Taskp t = &Tasks[new_prio[BusyPrio]];
    t->BusyFlag = 1;
    while ((t->Activated != t->Invoked) && (t->Flags & TRIGGERED))
    { 
      _EINT(); 
      ExecuteTask(t);
      _DINT();
    }
    t->BusyFlag = 0;
  }

}
