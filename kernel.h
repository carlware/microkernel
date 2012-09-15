#include <stddef.h>
#include <stdint.h>
#include "list.h"

typedef uint8_t Signal;
typedef struct task Task;
typedef struct message Message;
typedef void (Function)(void);

#define PRIORITY_IDLE (INT8_MIN)
#define PRIORITY_LOW (INT8_MIN / 2)
#define PRIORITY_NORMAL (0)
#define PRIORITY_HIGH (INT8_MAX / 2)

/* kernel_init: Allocate and initialize kernel data
structures. The function may only be called once. Zero is
returned if the call succeeded. */
uint8_t kernel_init(void);

/* kernel_start: Start the scheduler. This function returns
the init task (itself) on success, or NULL on error. */
Task *kernel_start(void);


/* task_create: Allocate and initialize a task. The task
entry point is given by entry. At least stacksize bytes will
be allocated for program stack. The task is added to the
scheduler. A pointer to the newly created task is returned,
or NULL if an error occured. Creating tasks is allowed only
between kernel_init() and kernel_start(). */
Task *task_create(char *name, Priority priority, Function *entry,
  size_t stacksize);

/* task_free: Free the resources used by a task. */
void task_free(Task *task);

/* task_self: Find self. A pointer to the running task is
returned. */
Task *task_self(void);

/* task_get_priority: Get task priority. */
Priority task_get_priority(Task *task);

/* task_set_priority: Set scheduling priority of task. */
void task_set_priority(Task *task, Priority priority);

/* task_signal: Send signal to task. */
void task_signal(Task *task, Signal signal);

/* task_wait: Wait for signals. */
Signal task_wait(Signal mask);

/* message_create: Allocate and prepare a message. A pointer
to the newly created task is returned, or NULL if an error
occured. The function temporary disables multitasking. */
Message *message_create(void);

/* message_free: Free the resources used by message. */
void message_free(Message *message);

/* message_set_data: Associate data with message. The data is
refered to, not copied. */
void message_set_data(Message *message, void *data);

/* message_get_data: Get the data associated with a
message. Data is not copied. */
void *message_get_data(Message *message);

/* message_put: Add a message to task's message queue. The
content of message is owned by the destination task at return
of the call. Reuse of message is allowed after it has been
replied to by the destination. The call never blocks but may
cause a task switch. */
void message_put(Task *destination, Message *message);

/* message_put_isr: Send message from an Interrupt Service
Routine. See message_put(). */
void message_put_isr(Task *destination, Message *message);

/* message_get: Deliver the next message. The call blocks if
the task's message queue is empty. Messages are delivered in
FIFO order. The receiving task will get temporary access to
the message and may read and modify it. After the message is
replied to with message_reply(), access is forbidden. */
Message *message_get(void);

/* message_reply: Reply on a message. */
void message_reply(Message *message);

