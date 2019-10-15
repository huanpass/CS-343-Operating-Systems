#include "types.h"
#include "stat.h" // referenced by main()
#include "user.h" // where getprocs fxn is defined that can be called thru the shell
#include "ProcessInfo.h" // where our struct is defined
#include "param.h" // referenced by exit(), and defines NPROC (max # of processes)
// found what each files refs from http://www.cse.iitd.ernet.in/~sbansal/os/previous_years/2011/xv6_html/files.html

// argc is argument count (# args passed in); 
// argv is ptr array which pts to each arg passed into the program
int 
main(int argc, char *argv[])
{
  // if user tried to input any args besides 'ps', let them know they can't and exit
  if (argc > 1) {
    // 1 is stdout; print error
    printf(1, "ERROR: ps takes no inputs!");
    // and exit
    exit();
  }

  // grab the ProcessInfo table we populated in getprocs
  struct ProcessInfo processInfoTable[NPROC];

  // get number of current processes from getprocs (which returns counts)
  int numprocs = getprocs(processInfoTable);

  // else (user just typed 'ps')
  // in proc.h: enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
  char* procstate[6] = {"UNUSED", "EMBRYO", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"};

  int i;
  // cycle through process table we populated and print out relevant info for each proc on new line
  for (i=0; i<numprocs; i++) {

    int pid = processInfoTable[i].pid; // process id
    int ppid = processInfoTable[i].ppid; // parent process id of the process
    char* state = procstate[processInfoTable[i].state]; // state of the process
    unsigned int sz = processInfoTable[i].sz; // size of the process
    
    // reqs: each proc on new line, 2 spaces b/w info, print -1 for ppid of 1st proc, state must be human readable text
    printf(1, "%d  %d  %s  %d  %s\n", pid, ppid, state, sz, processInfoTable[i].name);
  }

  exit();

}