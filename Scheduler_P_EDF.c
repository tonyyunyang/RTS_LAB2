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
  uint8_t c = 0;

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
      // else if(t->NextPendingDeadline == t1->NextPendingDeadline){
      //   if(t->Prio > t1->Prio && t->Activated != t->Inoked){
      //     new_prio[i] = b;
      //     new_prio[i - 1] = a;
      //   }
      // }
    }
  }
    for(c = 0; c < NUMTASKS; c++){
    for(i = NUMTASKS - 1; i > 0; i--){
      Taskp t = &Tasks[new_prio[i]];
      Taskp t1 = &Tasks[new_prio[i - 1]];
      a = new_prio[i];
      b = new_prio[i - 1];
      if(t1->Activated == t1->Invoked){
        new_prio[i] = b;
        new_prio[i - 1] = a;
      }
      // else if(t->NextPendingDeadline == t1->NextPendingDeadline){
      //   if(t->Prio > t1->Prio && t->Activated != t->Inoked){
      //     new_prio[i] = b;
      //     new_prio[i - 1] = a;
      //   }
      // }
    }
  }
  SetLeds(WHITE, 1);
  SetLeds(WHITE, 0);
  // for(c = 0; c < NUMTASKS; c++){
  //   Taskp t1 = &Tasks[new_prio[c]];
  //   uint8_t QIAN = (t1->NextPendingDeadline / 4) / 1000;
  //   uint8_t BAI = ((t1->NextPendingDeadline / 4) - QIAN * 1000) / 100;
  //   uint8_t SHI = ((t1->NextPendingDeadline / 4) - QIAN * 1000 - BAI * 100) / 10;
  //   uint8_t GE = (t1->NextPendingDeadline / 4) - QIAN * 1000 - BAI * 100 - SHI * 10;
  //   for(i = 0; i < QIAN; i++){
  //     SetLeds(PINK, 1);
  //     SetLeds(PINK, 0);
  //   }
  //   for(i = 0; i < BAI; i++){
  //     SetLeds(WHITE, 1);
  //     SetLeds(WHITE, 0);
  //   }
  //   for(i = 0; i < SHI; i++){
  //     SetLeds(BROWN, 1);
  //     SetLeds(BROWN, 0);
  //   }
  //   for(i = 0; i < GE; i++){
  //     SetLeds(PURPLE, 1);
  //     SetLeds(PURPLE, 0);
  //   }
  // }



      // if(oldBP == 0){
      //   SetLeds(PINK, 1);
      //   SetLeds(PINK, 0);
      // }
      // if(oldBP == 1){
      //   SetLeds(WHITE, 1);
      //   SetLeds(WHITE, 0);
      // }
      // if(oldBP == 2){
      //   SetLeds(BROWN, 1);
      //   SetLeds(BROWN, 0);
      // }
      // if(oldBP == 3){
      //   SetLeds(PURPLE, 1);
      //   SetLeds(PURPLE, 0);
      // }
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
