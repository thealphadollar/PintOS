#include <stdbool.h>

#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
#define pid_t int

// implement sys call definition
static void exit (int status);
static bool create (const char *file, unsigned initial_size);
static bool remove (const char *file);
static int open (const char *file);
static int filesize (int fd);
static int read (int fd, void *buffer, unsigned size);
static int write (int fd, const void *buffer, unsigned size);
static void close (int fd);

#endif /* userprog/syscall.h */
