/* Call clone and spin until child thread has changed global variable.  Verifies that address space is shared. */
#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

int ppid;
#define PGSIZE (4096)

volatile int global = 1;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();
   
   int arg = 50;
   int thread_pid = thread_create(worker, &arg);
   assert(thread_pid > 0);

   int join_pid = thread_join(thread_pid);
   assert(join_pid == thread_pid);
   assert(global == 2);

   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   int arg = *(int*)arg_ptr;
   assert(arg == 50);
   assert(global == 1);
   global++;
   exit();
}
