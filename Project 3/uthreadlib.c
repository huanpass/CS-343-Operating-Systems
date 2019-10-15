#include "types.h"
#include "user.h"
#include "x86.h"

#define PGSIZE (4096)

// Creates a new thread by first allocating a page-aligned user stack, 
// then calling the clone syscall.  
// Returns the pid of the new thread.
int
thread_create(void (*start_routine)(void*), void* arg) {
   void *stack = malloc(2*PGSIZE);

   // if malloc failed
   if (!stack) {
      return -1;
   }

   // make page aligned
   // if stack not multiple of page size
   if ((uint)stack % PGSIZE) {
      // make multiple of page size
      stack += PGSIZE - (uint)stack % PGSIZE;
   }

   return clone(start_routine, arg, stack);
}

// Calls join to wait for the thread specified by pid to complete.  
// Cleans up the completed thread's user stack.  
// It should return 0 on success and any other number to indicate an error.
int
thread_join(int pid) {
   int userstack = ustack(pid);

   // check if user stack exists
   // if not, return -1, failure
   if (userstack < 0) {
      return -1;
   }

   // calls join
   int join_pid = join(pid);
   // cleans up completed thread's user stack
   free((void*)userstack);

   // join returns 0 on success
   return join_pid;
}

// Acquires the lock pointed to by lock.  
// If the lock is already held, spin until it becomes available.
void
lock_acquire(lock_t* lock) {
   while(xchg(&lock->locked, 1) != 0);
}

// Release the lock pointed to by lock.
void
lock_release(lock_t* lock) {
   xchg(&lock->locked, 0);
}

// Initialize the lock pointed to by lock.
void
lock_init(lock_t* lock) {
   lock->locked = 0;
}

void
cv_wait(cond_t* conditionVariable, lock_t* lock) {
   csleep(conditionVariable, lock);
}

void
cv_signal(cond_t* conditionVariable) {
   cwake(conditionVariable);
}