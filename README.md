# EDF_Scheduler
### The EDF Scheduler Algorithm Is Suited To Work In An Environment Where These Assumptions Applies:
> 1) The requests for all tasks for which hard deadlines exist are periodic, with constant interval between requests.
> 2) Deadlines consist of run-ability constraints only, i.e. each task must be completed before the next requests for it occurs.
> 3) The tasks are independent in that requests for a certain task do not depend on the initialization or the completion of requests for other tasks.
> 4) Run-time for each task is constant for that task and does not vary with time. Run-time refers to the time which is taken by a processor to execute the task without interruption.
> 5) Any non-periodic tasks in the system are special; they are initialization or failurerecovery routines; they displace periodic tasks while they themselves are being run, and do not themselves have hard, critical deadlines.
### EDF Scheduler Creation Steps:
> 1) Create a new Ready list able to manage a dynamic task priority behaviour, It will contain tasks ordered by increasing deadline time, where the positions in the list represent the running task.
> 2) Add  an extra parameter to the task control block TCB structure to store the task's period.
> 3) Create a new API to initialize the tasks with an EDF scheduler based on their deadlines. I named it  "xTaskPeriodicCreate".
> 4) Make sure to assign the Idle task with the highest deadline.
> 5) In the context switch API make sure that the current task chosen to execute is in the head of the new Ready List.
### Main Application Program Description:
> The main application contains 6 tasks:
> 1) **Load_1_Simulation:** { _Deadline: 10ms, Execution_Time: 5ms, Periodicity: 10ms_ }
> 2) **Load_2_Simulation:** { _Deadline: 100ms, Execution_Time: 12ms, Periodicity: 100ms_ }
> 3) **Button_1_Monitor:** { _Deadline: 50ms, Execution_Time: 12.8us, Periodicity: 50ms_ }
> 4) **Button_2_Monitor:** { _Deadline: 50ms, Execution_Time: 12.5us, Periodicity: 50ms_ }
> 5) **Periodic_Transimtter:** { _Deadline: 100ms, Execution_Time: 18us, Periodicity:100ms_ }
> 6) **Uart_Receiver:** { _Deadline: 20ms, Execution_Time: 25us, Periodicity:20ms_ }

>**_NOTES:_**
> - Load_1_Simulation and Load_2_Simulation tasks are represents the load on the system.
> - Button_1_Monitor ,Button_2_Monitor, Periodic_Transimtter and Uart_Receiver are communicate with each other using queue.
#### Links to the projecct video;
> https://drive.google.com/file/d/16O4sVeSRSXb8KlvDQQBBYTETF3l8p_Tj/view?usp=share_linkn 
