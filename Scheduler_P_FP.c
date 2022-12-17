#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */

  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task

  /* ---------------------------------------------------------------- */

}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */

  /* Super simple, single task example */
  Taskp t = &Tasks[0];
  if (t->Activated != t->Invoked)
  {
    ExecuteTask(t);
  }
  /* End of example*/

  /* ---------------------------------------------------------------- */
}
