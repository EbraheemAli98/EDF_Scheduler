# EDF_Scheduler
#### The EDF scheduler algorithm is suited to work in an environment where these assumptions applies:
  1) The requests for all tasks for which hard deadlines exist are periodic, with constant interval between requests.
  2) Deadlines consist of run-ability constraints only, i.e. each task must be completed before the next requests for it occurs.
  3) The tasks are independent in that requests for a certain task do not depend on the initialization or the completion of requests for other tasks.
  4) Run-time for each task is constant for that task and does not vary with time.Run-time refers to the time which is taken by a processor to execute the task without interruption.
  5) Any non-periodic tasks in the system are special; they are initialization or failurerecovery routines; they displace periodic tasks while they themselves are being run, and do not themselves have hard, critical deadlines.
#### EDF Scheduler Creation Steps:
  1) Create new Ready List able to manage a dynamic task priority behaviour, It will contain tasks ordered by increasing deadline time, Where the positions in the list represent the head of the list containing the running task.
  2) Add an extra parameter to the task TCB to store its deadline.
  3) Create a new API to initialize the tasks in an EDF scheduler based on their deadlines. I named it "xTaskPeriodicCreate"
  4) Make sure to set the Idle task with the highest deadline.
  5) In a context switch API make sure that the current task chosen to exeute is in the head of the Ready List.

