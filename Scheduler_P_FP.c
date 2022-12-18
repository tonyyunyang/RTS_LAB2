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
    if(t->Flags == 9 || t->Flags == 13 || t->Flags == 14){
      Tcu = t->NextRelease - t->Period;
    }
    if(t->Flags == TT || t->Flags == 13){ 
      Tnx = t->NextRelease;
    }
  }
  
  // uint16_t a = Tnx / 4;
  // Taskp t = &Tasks[1];
  // uint8_t QIAN = t->Remain / 1000;
  // uint8_t BAI = (t->Remain - QIAN * 1000) / 100;
  // uint8_t SHI = (t->Remain - QIAN * 1000 - BAI * 100) / 10;
  // uint8_t GE = t->Remain - QIAN * 1000 - BAI * 100 - SHI * 10;
  // for(i = 0; i < QIAN; i++){
  //   SetLeds(RED, 1);
  //   SetLeds(RED, 0);
  // }
  // for(i = 0; i < BAI; i++){
  //   SetLeds(WHITE, 1);
  //   SetLeds(WHITE, 0);
  // }
  // for(i = 0; i < SHI; i++){
  //   SetLeds(BROWN, 1);
  //   SetLeds(BROWN, 0);
  // }
  // for(i = 0; i < GE; i++){
  //   SetLeds(PURPLE, 1);
  //   SetLeds(PURPLE, 0);
  // }
  // if(Tcu == 0){
  //   SetLeds(PURPLE, 1);
  //   SetLeds(PURPLE, 0);
  // }

  EP = (Tnx - Tcu) / 4;

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
// Taskp t = &Tasks[0];
//   uint8_t QIAN1 = t->Remain / 1000;
//   uint8_t BAI1 = (t->Remain - QIAN1 * 1000) / 100;
//   uint8_t SHI1 = (t->Remain - QIAN1 * 1000 - BAI1 * 100) / 10;
//   uint8_t GE1 = t->Remain - QIAN1 * 1000 - BAI1 * 100 - SHI1 * 10;
//   for(i = 0; i < QIAN1; i++){
//     SetLeds(PINK, 1);
//     SetLeds(PINK, 0);
//   }
//   for(i = 0; i < BAI1; i++){
//     SetLeds(WHITE, 1);
//     SetLeds(WHITE, 0);
//   }
//   for(i = 0; i < SHI1; i++){
//     SetLeds(BROWN, 1);
//     SetLeds(BROWN, 0);
//   }
//   for(i = 0; i < GE1; i++){
//     SetLeds(PURPLE, 1);
//     SetLeds(PURPLE, 0);
//   }

  for(i = 0; i < NUMTASKS; i++){
    Taskp t = &Tasks[i];
    if(ready_queue[i] != 0){
      ExecuteTask(t, ready_queue[i]);
    }
    
  }
  /* End of example*/

  /* ---------------------------------------------------------------- */
}
