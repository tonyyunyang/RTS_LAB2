#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */

  if (t->RemainExecutionTime <= 0) {
    t->Invoked++;
  }
  t->Taskf(t->CurrentExecutionTime);


  // t->Invoked++;
  // t->Taskf(t->ExecutionTime); // execute task

  /* ---------------------------------------------------------------- */

}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */

  /* Super simple, single task example */
  uint8_t i;
  uint16_t eliminateWindowTime = 0;
  Taskp t = &Tasks[NUMTASKS - 1];
  uint16_t window = t->WindowTime;
  
  for (i = (NUMTASKS - 1); i >= 0; i--) {
    Taskp ts = &Tasks[i];
    if (window <= 0) {
      break;
    }
    if (ts->Activated != ts->Invoked) {
      if (ts->RemainExecutionTime > window) {
        eliminateWindowTime = window;
        ts->CurrentExecutionTime = eliminateWindowTime;
        ts->RemainExecutionTime -= eliminateWindowTime;
      }else {
        eliminateWindowTime = ts->RemainExecutionTime;
        ts->CurrentExecutionTime = eliminateWindowTime;
        ts->RemainExecutionTime -= eliminateWindowTime;
      }
      ExecuteTask(ts);
      ts->CurrentExecutionTime = 0;
      window -= eliminateWindowTime;
    }
  }


  // Taskp t = &Tasks[0];
  // if (t->Activated != t->Invoked)
  // {
  //   ExecuteTask(t);
  // }
  /* End of example*/

  /* ---------------------------------------------------------------- */
}
