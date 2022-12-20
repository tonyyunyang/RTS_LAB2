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

  uint8_t oldBP = BusyPrio; // Set BusyPrio as the priority of the currently running task (the one that was executing just before the scheduler is called)

  for(BusyPrio = 0; BusyPrio < oldBP; BusyPrio++) {
    Taskp t = &Tasks[BusyPrio];
    while ((t->Activated != t->Invoked) && (t->Flags & TRIGGERED))
    {
      _EINT(); 
      ExecuteTask(t);
      _DINT();
      // t->Activated = t->Invoked;
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
