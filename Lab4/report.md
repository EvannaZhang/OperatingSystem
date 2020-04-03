A **program** is a group of instructions to carry out a specified task, they are executable files on the disk in a directory. It is read into the primary memory and executed by the kernel. 

A **process** is an executing instance of a program. It is an execution of a certain program with its own address space It is stored in the main memory and disappears if the machine is power down. 

Several processes may be associated with the same program. The difference is that: program won’t be lost if power down, but the process would; A program can be subdivided into multiple processes executing in parallel.



------

A job is a series of programs submitted to the operating system for some goals. They would be executed by the kernel and might consist of several processes. The difference is that one job can be made of several processes, and to finish a job may require multiple processes.

------

Job scheduling is long-term scheduling, it typically schedules programs that are not currently running and are to be run at a set time, and often on a repetitive basis. It controls the degree of multiprogramming and it selects processes from memory and loads them into memory for execution. Process scheduling normally refers to the part of the operating system that makes decisions to move different parallel processes competing with one another for execution, to different states like Ready to running or running to exit state based on a certain decision strategy. It reduces the degree of multiprogramming and can re-introduce the process into memory and execution can be continued.

------

They are all composite data type and takes all members of it are in the same block of memory. A structure can contain an ordered group of data objects with varies data types. Structures and classes are very similar, they are both containers and can have members, constructors, members can have different access levels, implement interfaces and so on. However, structures are value types, while classes are reference types, class members can be declared as protected, but structures can’t, the structure is not inheritable, but classes are. A union is an object similar to a structure except that all of its members start at the same location in memory. A union variable can represent the value of only one of its members at a time. The size of structure technically equals to the sum of all its members’ size, while union equals to the largest member. If you assign a value to one of the union’s members, the values of other members change too.

------

There are three statues in a job: Ready, Running, Done. Running status means this job is currently running. Ready status means the job is not running right now, but it is waiting for a machine to become available. The Done status means the job has already been executed and finished.

------

Programs: enq, deq, stats, scheduler

Enq: enq is used to send a enqueue request to the scheduler and submit the job for running. 

Deq: deq is used to send a dequeue request to the scheduler. 

Stats: Stats is used to print out the information of currently running jobs and jobs in waiting queue on standard output, including PID, user name, execution name, waiting time, create time and job status. 

Scheduler: The scheduler can create a process for a new job and set its state to READY state and put in the waiting queue; receives deque and enqueue request and do the accordingly operation.

------

FIFO.

The FIFO is an extension of the traditional pipe and can last beyond the process. Processes attach to this FIFO file for inter-process communication. It allows processes to communicate with each other by reading/writing to/from this file. It enters into the file system by calling mkfifo() and it has to be open at both ends simultaneously before proceed to do any input or output operations on it.

------

The job name is placed in cmdarg[0] and we need to use %s in the string to print it.

------

\1. We need to change the FIFO in the job.h since we make a new directory.

\2. In scheduler.c, Line 84, it used the Chinese "；" instead of  ";", and caused compilation problem. 

\3. In updateall(), we need to increase the wait_time by 100 instead of 1 and update the current priority so the ones with a longer wait time can be chosen sooner.

\4. In jobselect(), if the current one has the same current priority as the temporary highest, we also need to compare which one has the highest wait_time. And when choosing, we need to clarify if it's the head that is chosen or not. If the head is chosen, we need to change head to head->next.(Also in do_deq())

 \5. In do_stat(), According to Question 8 before, I additionally print the name of the job and the priority and current priority values so that we can check how the priority based round robin is working.

------

After running the scheduler, it prints: "OK! Scheduler is starting now!!". It would construct a READY queue used to store the submitted jobs whose status is READY. Since the strategy of the scheduler is Priority-based Round Robin, so the scheduler would iterate all the jobs in the queue and select the one with the highest priority(current priority) from the queue with the current job every time slice and let it run. When the wait time of some job exceeds the threshold, its priority would increase, but not larger than 3.

------

After submitting a new job, the do_enq() function in the scheduler will create a new process. Then the scheduler would stop this process for now and change the status of this job to READY, and put it in the READY queue.



The corresponding code: In scheduler.c, 



void do_enq(struct jobinfo *newjob, struct jobcmd enqcmd);

//

sigset_t zeromask;  

sigemptyset(&zeromask);  /* fill jobinfo struct */  

newjob = (struct jobinfo *)malloc(sizeof(struct jobinfo));

...

newnode = (struct waitqueue*)malloc(sizeof(struct waitqueue)); 

newnode->next = NULL; newnode->job = newjob;

//



It first fills the job info and then adds the job to the queue. 



It prints:

"new job: jid=x, pid=y"

------

After receiving the signal or job run finished, the do_deq() function will delete the job from the queue. Here I submitted a job TEMP which basically does nothing. So after finished running, we can see that it is terminated and deleted from the queue.

The corresponding code: In scheduler.c, 

void do_deq(struct jobcmd deqcmd);

//

/* current jodid == deqid, terminate current job */ 

if (current && current->job->jid == deqid) {  

printf("terminate job: %d\n", current->job->jid); 

kill(SIGTERM, current->job->pid);  

for (i = 0; (current->job->cmdarg)[i] != NULL; i++) {  

...

} 

else {  /* maybe in waitqueue, search it */

...

}

//



It would print out the termination way, status code and also jid and pid.

------

The strategy used here is priority-based Round Robin, so the scheduler would iterate all the jobs in the queue every time slice and selects the one with the highest priority (current priority) and let it run. Also if some job waits for a too long time, it’s current priority would increase by 1, but not larger than 3.

Corresponding code:

In scheduler.c:

struct waitqueue* jobselect();

//

if (head) { 

for (prev = head, p = head; p != NULL; prev = p, p = p->next) {
 if (p->job->curpri > highest) { 

select = p; 

selectprev = prev; 

highest = p->job->curpri; }      // add      

else if (p->job->curpri == highest) {        

if (p->job->wait_time > wait_longest) {          

select = p;          

selectprev = prev;          

wait_longest = p->job->wait_time;        

}      } }

//



Execution results:

------

The time slice is set to be 100 milliseconds, so after every 100 milliseconds, the scheduler would reschedule and determine which job to run next.



In scheduler.c:

Every time slice the scheduler would update information and rescheduler.

struct waitqueue* jobselect();



void updateall(){ 

struct waitqueue *p;  /* update running job's run_time */ 

if (current) 

current->job->run_time += 1;    /* update ready job's wait_time */ 

for (p = head; p != NULL; p = p->next) {    

p->job->wait_time += 100;    // Modified, increase the current priority if some job wait too long    

if(p->job->wait_time >= 1000 && p->job->curpri < 3)      

p->job->curpri++; 

}}





Execution results:

------

Through this lab, I learned about different ways of scheduling processes and jobs, and get to use the priority-based robin strategy to schedule processes, and use commands to submit and remove processes. And we used FIFO (named pipe) to let processes communicate with each other and it’s realized by a specific file defined in header files.
