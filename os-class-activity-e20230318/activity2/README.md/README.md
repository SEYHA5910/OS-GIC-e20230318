# Class Activity 2 — Processes & Inter-Process Communication

- **Student Name:** Khim Vyseyha
- **Student ID:** e20230318
- **Date:** 11 November 2026

---

## Task 1: Process Creation on Linux (fork + exec)

### Compilation & Execution

Screenshot of compiling and running `forkchild.c`:

![Task 1 - Compile & Run](/activity2/screenshots/task1_compile_run.png)

### Process Tree

Screenshot of the parent-child process tree (using `ps --forest`, `pstree`, or `htop` tree view):

![Task 1 - Process Tree](/activity2/screenshots/task1_process_tree.png)

### Output

total 32
drwxr-xr-x 2 khim_vyseyha khim_vyseyha  4096 Nov 11 12:31 .
drwxr-xr-x 8 khim_vyseyha khim_vyseyha  4096 Nov 11 12:23 ..
-rwxr-xr-x 1 khim_vyseyha khim_vyseyha 16392 Nov 11 12:31 forkchild
-rw-r--r-- 1 khim_vyseyha khim_vyseyha  1483 Nov 11 12:29 forkchild.c
-rw-r--r-- 1 khim_vyseyha khim_vyseyha     0 Nov 11 12:35 result_forkchild.txt
Parent process (PID: 1338) — creating child...
Parent: waiting for child (PID: 1339) to finish...
Parent: child exited with status 0
Parent: done.

### Questions

1. **What does `fork()` return to the parent? What does it return to the child?**

   To the parent, fork() returns the Process ID (PID) of the newly created child process. To the child, it returns 0. (If it fails, it returns -1).

2. **What happens if you remove the `waitpid()` call? Why might the output look different?**

   If waitpid() is removed, the parent process will not wait for the child to finish. The parent will print "Parent: done." and exit immediately. The child process will continue running in the background (becoming an orphan), and when its 30-second sleep finishes, its ls -la output will print directly over the active command prompt, messing up the terminal display.

3. **What does `execlp()` do? Why don't we see "execlp failed" when it succeeds?**

   execlp() replaces the entire memory image of the current process with a completely new program (in this case, ls). If it succeeds, our original C code is completely wiped from memory and replaced by the ls program, meaning the lines of code below execlp() are physically no longer there to be executed.

4. **Draw the process tree for your program (parent → child). Include PIDs from your output.**

forkchild (PID: 1338)
└── forkchild (PID: 1339)

5. **Which command did you use to view the process tree (`ps --forest`, `pstree`, or `htop`)? What information does each column show?**

   I used ps -ef --forest | grep forkchild. The columns show UID (the user who owns the process), PID (Process ID), PPID (Parent Process ID), C (CPU utilization), STIME (start time), TTY (terminal type), TIME (total CPU time used), and CMD (the command that started the process).

---

## Task 2: Process Creation on Windows

### Compilation & Execution

Screenshot of compiling and running `winprocess.c`:

![Task 2 - Compile & Run](/activity2/screenshots/task2_compile_run.png)

### Task Manager Screenshots

Screenshot showing process tree in the **Processes** tab (mspaint nested under your program):

![Task 2 - Task Manager Tree](/activity2/screenshots/task2_taskmanager_tree.png)

Screenshot showing PID and Parent PID in the **Details** tab:

![Task 2 - Task Manager Details](/activity2/screenshots/task2_taskmanager_details.png)

### Questions

1. **What is the key difference between how Linux creates a process (`fork` + `exec`) and how Windows does it (`CreateProcess`)?**

   Linux uses a two-step process: fork() duplicates the parent process exactly, and exec() replaces the child's memory image with a new program. Windows uses a single-step process: CreateProcess() creates a brand new process and loads the new program into it simultaneously without duplicating the parent.

2. **What does `WaitForSingleObject()` do? What is its Linux equivalent?**

   WaitForSingleObject() blocks the parent process, pausing its execution until the specified object (the child process) finishes running and terminates. Its Linux equivalent is wait() or waitpid().

3. **Why do we need to call `CloseHandle()` at the end? What happens if we don't?**

   We call CloseHandle() to release the system resources allocated for the process and thread handles. If we do not close them, it causes a resource leak (specifically a handle leak), meaning the operating system cannot fully clean up the process data from memory until the parent program itself completely exits.

4. **In Task Manager, what was the PID of your parent program and the PID of mspaint? Do they match your program's output?**

   The PID of my parent program was [33600] and the PID of the child process was [33588]. Yes, they match the exact output printed in my terminal.

5. **Compare the Processes tab (tree view) and the Details tab (PID/PPID columns). Which view makes it easier to understand the parent-child relationship? Why?**

   The Processes tab (tree view) makes it much easier to quickly understand the relationship because it represents it visually through indentation (showing the child nested under the parent). However, the Details tab is better for strict technical verification because it explicitly lists the exact Parent Process ID numbers.

---

## Task 3: Shared Memory IPC

### Compilation & Execution

Screenshot of compiling and running `shm-producer` and `shm-consumer`:

![Task 3 - Compile & Run](/activity2/screenshots/task3_compile_run.png)

### Output

Consumer: reading from shared memory 'OS-Seyha'
Consumer: message = "Hello, this is shared memory IPC!"
Consumer: shared memory unlinked.

### Questions

1. **What does `shm_open()` do? How is it different from `open()`?**

    shm_open() creates a new POSIX shared memory object or opens an existing one, returning a file descriptor. It is different from open() because open() is used for traditional disk-based files, whereas shm_open() creates a memory-resident object (typically stored in RAM under /dev/shm in Linux) specifically optimized for inter-process communication.

2. **What does `mmap()` do? Why is shared memory faster than other IPC methods?**

    mmap() (memory map) takes the shared memory object and maps it directly into the virtual address space of the calling process. Shared memory is the fastest IPC method because, once the memory is mapped, processes communicate by reading and writing directly to RAM. It avoids the overhead of making kernel system calls (like read() or write()) and eliminates copying data back and forth between user space and kernel space.

3. **Why must the shared memory name match between producer and consumer?**

   The name acts as the global, unique identifier for the shared memory object within the operating system. If the producer and consumer use different names, they will map two completely separate and unrelated blocks of memory, making communication impossible.

4. **What does `shm_unlink()` do? What would happen if the consumer didn't call it?**

   shm_unlink() removes the shared memory object's name from the system directory (/dev/shm), which tells the OS to destroy the object and free the memory once all processes have closed it. If the consumer doesn't call it, the shared memory block will permanently remain in RAM (causing a memory leak) even after the programs finish executing, and will only be cleared if you manually delete the file or reboot the system.

5. **If the consumer runs before the producer, what happens? Try it and describe the error.**

   The consumer process fails and crashes immediately. Because the consumer uses shm_open() with the O_RDONLY flag (and no O_CREAT flag), it expects the shared memory object to already exist. Since the producer hasn't created it yet, the program throws an error: shm_open failed: No such file or directory.

---

## Task 4: Message Queue IPC

### Compilation & Execution

Screenshot of compiling and running `sender` and `receiver`:

![Task 4 - Compile & Run](/activity2/screenshots/task4_compile_run.png)

### Output

Receiver: message received from queue '/queue-Seyha'
Receiver: message = "Hello from sender! This is message queue IPC."
Receiver: queue unlinked.


### Questions

1. **How is a message queue different from shared memory? When would you use one over the other?**

   A message queue passes discrete, structured packets of data (messages) and is automatically synchronized by the operating system, meaning you don't have to worry about race conditions. Shared memory provides a raw block of bytes that processes access directly; it is faster but requires the programmer to manually write synchronization code (like mutexes or semaphores) to prevent data corruption. Use shared memory when transferring large amounts of data at maximum speed, and use message queues when sending smaller, distinct commands or data packets where OS-managed synchronization is safer and easier.

2. **Why does the queue name in `common.h` need to start with `/`?**

   This is a strict requirement of the POSIX standard. For the operating system to properly identify and map the IPC object into the virtual file system (specifically inside the /dev/mqueue/ directory on Linux), the name must begin with a forward slash (/) and contain no other slashes.

3. **What does `mq_unlink()` do? What happens if neither the sender nor receiver calls it?**

   mq_unlink() removes the message queue's name from the system, marking it for deletion once all processes have closed their connections to it. If neither program calls it, the message queue remains active in the operating system's memory even after the programs finish running, causing a resource leak that persists until the computer is rebooted or the queue is manually deleted.

4. **What happens if you run the receiver before the sender?**

   By default, the mq_receive() function is "blocking." If the receiver successfully opens the queue before the sender has sent anything, the receiver program will simply pause (block) and wait patiently. Once the sender is executed and puts a message into the queue, the receiver will instantly wake up, read the message, and finish executing. (Note: If the receiver code does not use the O_CREAT flag to create the queue, it will instead crash with a "No such file or directory" error because the sender hasn't built the queue yet).

5. **Can multiple senders send to the same queue? Can multiple receivers read from the same queue?**

   Yes to both. POSIX message queues support multiple concurrent senders and receivers. Multiple senders can push messages into the same queue, and multiple receivers can listen to it. However, it is important to note that a single message can only be consumed by one receiver; once a receiver pulls a message out, it is removed from the queue and cannot be read by the others.
---

## Reflection

What did you learn from this activity? What was the most interesting difference between Linux and Windows process creation? Which IPC method do you prefer and why?

I learned how to directly interface with operating system APIs to create and manage processes in both Windows (CreateProcess) and Linux (fork/exec).The most interesting difference is the architectural approach: Linux uses an elegant two-step method where fork() clones the parent and exec() replaces the memory space, whereas Windows uses a single massive function (CreateProcess()) that builds a completely new process from scratch but requires complex struct initializations. Additionally, debugging the strict execution permissions on modern Windows Store apps (like the MS Paint WindowsApps folder) highlighted how Windows actively restricts process execution compared to standard Linux binaries.I prefer Message Queues for general communication because they are much safer and easier to implement.