#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  // lock
  lock_init(&call_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

// calling handlers with arguments required
static void
syscall_handler (struct intr_frame *f UNUSED)
{
  uintptr_t *esp = f->esp;
  // use switch case to invoke corresponding system call
  switch(*esp)
  {
    case SYS_EXIT:
      exit((int) *((esp)+1));
      break;
    case SYS_CREATE:
      f->eax = create((const char *) *((esp)+1), 
                            (unsigned) *((esp) + 2));
      break;
    case SYS_REMOVE:
      f->eax = remove((const char *) *((esp)+1));
      break;
    case SYS_OPEN:
      f->eax = open((const char *) *((esp)+1));
      break;
    case SYS_FILESIZE:
      f->eax = filesize((int) *((esp)+1));
      break;
    case SYS_READ:
      f->eax = read((int) *((esp)+1), 
                      (void *) *((esp)+2),
                      (unsigned) *((esp) + 3));
      break;
    case SYS_WRITE:
      f->eax = write((int) *((esp)+1), 
                      (void *) *((esp)+2),
                      (unsigned) *((esp) + 3));
      break;
    case SYS_CLOSE:
      close((int) *((esp)+1));
      break;
  }
  thread_exit ();
}

// exit system call
static void
exit (int status){

}

// system call to create a file
static bool
create (const char *file, unsigned initial_size){

}

// system call to remove file
static bool
remove (const char *file){

}

// system call to open a file and return file descriptor
static int
open (const char *file){

}

// system call to get size of the file
static int
filesize (int fd){

}

// system call to read data from file descriptor
static int 
read (int fd, void *buffer, unsigned size){

}

// system call to write data to file descriptor
static int 
write (int fd, const void *buffer, unsigned size){

}

// system call to close file descriptor
static void
close (int fd){
  
}


