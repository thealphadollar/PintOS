#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H
#include <list.h>
#include "threads/thread.h"

// Process definitions

typedef int pid_t;

// Process status ENUMS
#define PROC_LOADING 0
#define PROC_RUNNING 1
#define PROC_FAIL 2
#define PROC_EXIT 4

// Define error
#define PID_ERROR ((pid_t) -1)

// struct of process
typedef struct process_
{
    pid_t pid;          /* process identifier */
    struct process *parent;      /* parent of the current process */
    struct list child_list;     /* list of children */
    struct list file_list;      /* list of files */
    struct file *exec_file;     /* executable file */
    struct list_elem proc_elem;      /* list element */

    // define state variables
    int status, exit_status;
    bool is_waiting;            /* whether parent is waiting or not, if not then orphan*/
    int fd_tracker;             /* track file descriptors */
} process;

// a file locked or attached to a process
typedef struct process_file
{
    int fd;         /* file descriptor */
    struct file *file;
    struct list_elem elem;
} proc_file;

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);
process *process_current(void);
process *find_proc_child(process *proc, pid_t pid);
struct file *get_file(int fd);
int get_descriptor(struct file *file);
#endif /* userprog/process.h */
