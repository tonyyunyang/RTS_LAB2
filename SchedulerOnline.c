#ifndef __Schedule_c
#define __Schedule_c

#include "Clock.h"
#include "Scheduler.h"
#include "Led.h"
#include "Context.h"

Task Tasks[NUMTASKS];           /* Lower indices: lower priorities           */
uint16_t NextInterruptTime;     /* Timestamp at which next interrupt should occur */
volatile uint8_t BusyPrio;      /* Current priority being served */
volatile uint8_t Pending;       /* Is set when a task is pending */

uint16_t IntDisable (void)
{
  uint16_t sw;
    // sw = r2
  asm volatile ("mov.w r2, %0\n\t" : "=r"(sw));
  _DINT();
  return (sw);
}

void RestoreSW (uint16_t sw)
{
    // r2 = sw
  asm volatile ("mov.w %0, r2\n\t" :: "r"(sw));
}  

/* 
 * Initialize and clear task structures.
 * Should be called with interrupt disabled.
 * The clock must be started elsewhere.
 */

void InitTasks (void)
{			
  uint8_t i=NUMTASKS-1; 
  do { 
    Taskp t = &Tasks[i];
    t->Flags = t->Activated = t->Invoked = 0;
  } while (i--);
}

/*
 * Register a task, TRIGGERED only, with flags.
 * Testing and filling in defaults is done.
 * Each priority level has at most one task.
 */

uint8_t RegisterTask (uint16_t Phasing, uint16_t Period, uint16_t ExecutionTime,
                      void (*TaskFunc) (uint16_t), uint8_t Prio, uint8_t Flags)
{
  uint8_t  rtc = E_SUCCESS;
  uint16_t sw;

  Phasing *= 4; // correct to get ms approximately
  Period *=4; // correct to get ms approximately

  if (Prio>=NUMTASKS) return (E_BOUNDS); // out of bounds
  if (Period == 0) return (E_WRONGPAR);
  sw = IntDisable (); 
  Taskp t = &Tasks[Prio]; 
  if (t->Flags) rtc = E_BUSY; 
  else {
    t->NextRelease = 0 + Phasing;
    t->Period    = Period; 
    t->NextPendingDeadline = t->NextRelease + Period;
    t->Activated = t->Invoked = 0; 
    t->Taskf     = TaskFunc; 
    t->ExecutionTime = ExecutionTime;
    t->Flags     = Flags | TRIGGERED;

  }
  RestoreSW (sw);
  return (rtc);
}

uint8_t UnRegisterTask (uint8_t t)
{
  Tasks[t].Flags = 0;
  return (E_SUCCESS);
}  

static void DetermineNextInterruptTime (CandidateValue)
{
  if (CandidateValue < NextInterruptTime)
  {
    NextInterruptTime = CandidateValue;
  }
}

/*
  The timer interrupt is configured in continious mode with compare mode interrupts enabled.
  The source for Timer_A is ACLK/8 (32768 / 8 = 4096 Hz). Thus every 1/4096 second, the TAR register is incremented.
  When the TAR register reaches TACCR0, an interrupt is generated. Note that TAR will NOT be reset!
  It continues incrementing until it reaches it's max value (16-bit timer -> 65535), it will continue counting from 0.

  Relevant registers:
    TAR     Current value of Timer_A. 
    TACCR0  Compare register, when TAR reached TACCR0, an interrupt is generated

  Your job is to set a new value of TACCR0 every timer interrupt, to get the next interrupt

  Use the Task data structure defined in 'Scheduler.h' to store information which can be used by the scheduler.

*/
// for calculating hyper period
int gcd(int a, int b) {
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
}

// for calculating hyper period
int lcm(int a, int b) {
  return (a * b) / gcd(a, b);
}

interrupt (TIMERA0_VECTOR) TimerIntrpt (void)
{
  ContextSwitch();

  /* ----------------------- INSERT CODE HERE ----------------------- */

  /* Insert timer interrupt logic, what tasks are pending? */ 
  /* When should the next timer interrupt occur? Note: we only want interrupts at job releases */

  /* Super simple, single task example */
  // This part is to only get the next interrupt time
  // uint16_t hyperperiod = 0;
  uint8_t i = 0;
  uint16_t oldNextInterruptTime = NextInterruptTime; // This is the current time when interrupt occurrs
  // for (i = 0; i < NUMTASKS; i++) {
  //   Taskp t = &Tasks[i];
  //   hyperperiod = lcm(hyperperiod, t->Period);
  // }

  if (NextInterruptTime % (1024*4) == 0) {
    for (i = 0; i < NUMTASKS; i++) {
      Taskp t = &Tasks[i];
      t->NextRelease += t->Period;
      t->Activated++;
      t->FlagNextInterrupt = 0;
    }
  }


  for (i = 0; i < NUMTASKS; i++) {
    Taskp t = &Tasks[i];
    if (t->FlagNextInterrupt == TT) {
      t->NextRelease += t->Period;
      t->FlagNextInterrupt = 0;
    }
  }

  Taskp t = &Tasks[0];
  NextInterruptTime = t->NextRelease;


  for (i = 0; i < NUMTASKS; i++) {
    Taskp t = &Tasks[i];
    DetermineNextInterruptTime(t->NextRelease);
  }

  
  for (i = 0; i < NUMTASKS; i++) {
    Taskp t = &Tasks[i];
    if (t->NextRelease == NextInterruptTime) {
      t->FlagNextInterrupt = TT;
    }
    if ((oldNextInterruptTime % t->Period == 0) && (oldNextInterruptTime % (1024*4) != 0)) {
      t->Activated++;
    }
  }
  /* End of example*/

  /* ---------------------------------------------------------------- */
 

  TACCR0 = NextInterruptTime;

  CALL_SCHEDULER;

  ResumeContext();
}

#endif

