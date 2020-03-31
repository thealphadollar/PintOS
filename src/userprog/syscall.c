#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

// helper functions
static int get_byte(const uint8_t *unsigned_addr);
static bool put_byte (uint8_t *unsigned_dst, uint8_t byte);
static uint32_t get_word (const uint32_t *unsigned_addr);

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
  uint32_t *esp = f->esp;
  // use switch case to invoke corresponding system call
  switch(get_word(esp))
  {
    case SYS_EXIT:
      exit((int) get_word((esp)+1));
      break;
    case SYS_CREATE:
      f->eax = create((const char *) get_word((esp)+1), 
                            (unsigned) get_word((esp) + 2));
      break;
    case SYS_REMOVE:
      f->eax = remove((const char *) get_word((esp)+1));
      break;
    case SYS_OPEN:
      f->eax = open((const char *) get_word((esp)+1));
      break;
    case SYS_FILESIZE:
      f->eax = filesize((int) get_word((esp)+1));
      break;
    case SYS_READ:
      f->eax = read((int) get_word((esp)+1), 
                      (void *) get_word((esp)+2),
                      (unsigned) get_word((esp) + 3));
      break;
    case SYS_WRITE:
      f->eax = write((int) get_word((esp)+1), 
                      (void *) get_word((esp)+2),
                      (unsigned) get_word((esp) + 3));
      break;
    case SYS_CLOSE:
      close((int) get_word((esp)+1));
      break;
  }
  thread_exit ();
  // make sure this part is never reached
  NOT_REACHED();
}

// exit system call
static void
exit (int status){
  thread_exit();
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
  if (fd == 1) putbuf(buffer, size);
  return size;
}

// system call to close file descriptor
static void
close (int fd){

}


/*
HELPER FUNCTIONS BELOW
*/

// Write a byte at user's virtual address provided as argument
static bool
put_byte(uint8_t *unsigned_dst, uint8_t byte){
  if (is_user_vaddr(unsigned_dst)){
    // catch error if any
    int error;
    asm ("movl $1f, %0; movb %b2, %1; 1:"
       : "=&a" (error), "=m" (*unsigned_dst) : "q" (byte));
    if (error == -1) return false;
    return true;
  }
  return false;
}

// Read a word at at user's virtual address provided as argument
static uint32_t
get_word(const uint32_t *unsigned_addr){
  if (is_user_vaddr(unsigned_addr)){
    uint32_t result;
    int byte, i;
    // 4 byte words
    for (i=0;i<4;i++){
      byte = get_byte((uint8_t *)unsigned_addr+i);
      if (byte != -1){
        *((uint8_t *) &result+i) = (uint8_t) byte;
      }
      // failed to get byte
      exit(-1);
    }
  }
  exit(-1);
}

// Read a byte at user's virtual address provided as argument
static int
get_byte(const uint8_t *unsigned_addr){
  if (is_user_vaddr(unsigned_addr)){
    int result;
    asm("movl $1f, %0; movzbl %1, %0; 1:"
      : "=&a" (result) : "m" (unsigned_addr));
    return result;
  }
  // when not user memory
  return -1;
}