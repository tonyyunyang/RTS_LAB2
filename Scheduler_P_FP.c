#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t, uint16_t time)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */

  t->Invoked++;
  t->Taskf(time); // execute task

  /* ---------------------------------------------------------------- */

}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */

  /* Super simple, single task example */
  uint8_t i = 0;
  uint16_t Tcu = 0;
  uint16_t Tnx = 0;
  uint16_t EP = 0;
  uint16_t ready_queue[10];
  for(i = 0; i < 10; i++){
    ready_queue[i] = 0;
  }

  for(i = 0; i < NUMTASKS; i++){
    Taskp t = &Tasks[i];
    if(t->Flags == 9 || t->Flags == 13){
      Tcu = t->NextRelease - t->Period;
    }
    if(t->Flags == TT){ 
      Tnx = t->NextRelease;
    }
  }
  EP = Tcu - Tnx;
  // if(EP == 256) { 
    
    
  // }
  for(i = 0; i < NUMTASKS; i++){
    Taskp t = &Tasks[i];
    if(EP < t->Remain){
      ready_queue[i] = EP;
      t->Remain -= EP;
      break;
    }
    else{
      ready_queue[i] = t->Remain;
      EP -= t->Remain;
      t->Remain = 0;
    }
  }

  for(i = 0; i < NUMTASKS; i++){
    if(ready_queue[i] == 0){ break; }
    Taskp t = &Tasks[i];
    ExecuteTask(t, ready_queue[i]);
  }
  /* End of example*/

  /* ---------------------------------------------------------------- */
}
