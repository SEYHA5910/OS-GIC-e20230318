# Class Activity 1 — System Calls in Practice

- **Student Name:** KHim Vyseyha
- **Student ID:** e20230318
- **Date:** [Date of Submission]

---

## Warm-Up: Hello System Call

Screenshot of running `hello_syscall.c` on Linux:

![Hello syscall](/activity1/screenshorts/Hello_Syscall.png)

Screenshot of running `hello_winapi.c` on Windows (CMD/PowerShell/VS Code):

![Hello WinAPI](/activity1/screenshorts/hello_winapi.png)

Screenshot of running `copyfilesyscall.c` on Linux:

![Copy file syscall](/activity1/screenshorts/copyfilesyscall.png)

---

## Task 1: File Creator & Reader

### Part A — File Creator

**Describe your implementation:** [What differences did you notice between the library version and the system call version?]

**Version A — Library Functions (`file_creator_lib.c`):**

<!-- Screenshot: gcc -o file_creator_lib file_creator_lib.c && ./file_creator_lib && cat output.txt -->
![Task 1A - Library](/activity1/screenshorts/file_creator_lib.png)

**Version B — POSIX System Calls (`file_creator_sys.c`):**

<!-- Screenshot: gcc -o file_creator_sys file_creator_sys.c && ./file_creator_sys && cat output.txt -->
![Task 1A - Syscall](/activity1/screenshorts/file_ccreator_sys.png)

**Questions:**

1. **What flags did you pass to `open()`? What does each flag mean?**

   * Flags passed to open():
    - O_WRONLY: Opends the file for write only acces.
    - O_CREAT: Create the file if it does notalready exist on the hard drive.
    - O_TRUNC: This ensures you start with a completely empty file instead of accidentally mixing your new text with old text.

2. **What is `0644`? What does each digit represent?**

   * The 0644 permission argument:
    This is an octal (base-8) number that sets Linux file permissions based on a point system where Read = 4 Write = 2 and Execute = 1.

    0 Tells the compiler this is an octal number.

    6 (Owner): 4 (Read) + 2 (Write) = 6. The user who created the file can read and edit it.

    4 (Group): 4 (Read). Other users in the same user group can only read it.

    4 (Others): 4 (Read). Everyone else on the system can only read it.
3. **What does `fopen("output.txt", "w")` do internally that you had to do manually?**

   * fopen("output.txt", "w") does internally:
   When you use the convenient "w" (write mode) string in the high-level fopen() library function, the C library automatically translates that behind the scenes into the exact O_WRONLY | O_CREAT | O_TRUNC flags. It also automatically applies standard default file permissions if it has to create a new file, saving you from having to manually calculate and type 0644.

### Part B — File Reader & Display

**Describe your implementation:** [Your notes]

**Version A — Library Functions (`file_reader_lib.c`):**

![Task 1B - Library](/activity1/screenshorts/task1_reader_lib.png)

**Version B — POSIX System Calls (`file_reader_sys.c`):**

![Task 1B - Syscall](/activity1/screenshorts/task1_reader_sys.png)

**Questions:**

1. **What does `read()` return? How is this different from `fgets()`?**

   - read() returns an interger representing the exact number of bytes it successfully read (or 0 if it hits the end of the file). It reads raw bytes regardless of where the text lines end.

2. **Why do you need a loop when using `read()`? When does it stop?**

   - You need a loop because read() can only grab a specific chunk of data at a time (in our code, a maximum of 256 bytes per chunk). If the file was huge, one single read() would only get the first tiny piece.
   - The loop automatically stops when read() returns 0, which is the operating system's signal that it has reached the End of File (EOF).

---

## Task 2: Directory Listing & File Info

**Describe your implementation:** [Your notes]

### Version A — Library Functions (`dir_list_lib.c`)

![Task 2 - Version A](/activity1/screenshorts/task2_lib.png)

### Version B — System Calls (`dir_list_sys.c`)

![Task 2 - Version B](/activity1/screenshorts/task2_sys.png)

### Questions

1. **What struct does `readdir()` return? What fields does it contain?**

- readdir() returns a pointer to a struct dirent. It contains several fields, but the most important ones are d_name (the actual name of the file/directory), d_ino (the inode serial number), d_type (the type of file, like a regular file or a directory), and d_reclen (the length of the record).

2. **What information does `stat()` provide beyond file size?**

- Beyond st_size (file size in bytes), the stat struct provides file permissions (st_mode), the owner and group IDs (st_uid, st_gid), the number of hard links (st_nlink), and timestamps for exactly when the file was last accessed, modified, or had its status changed (st_atime, st_mtime, st_ctime).

3. **Why can't you `write()` a number directly — why do you need `snprintf()` first?**

- The low-level write() function only understands raw text characters (bytes), not mathematical integer values. If you try to pass an integer like 1024 directly into write(), it will try to read it as a memory address or raw ASCII characters, printing out gibberish or crashing. snprintf() is required to translate the integer value into a human-readable text string (like "1024") so write() can display it properly.

---

## Optional Bonus: Windows API (`file_creator_win.c`)

Screenshot of running on Windows:

![Task 1 - Windows]()

### Bonus Questions

1. **Why does Windows use `HANDLE` instead of integer file descriptors?**

   While Linux uses simple integers (file descriptors) as indexes for an array of open files, Windows uses a HANDLE. A HANDLE is an opaque pointer/reference to an internal kernel object. Windows treats almost everything (files, threads, memory maps, processes) as an "object." Using HANDLES provides stronger security and abstraction, as the user program cannot guess or easily manipulate the underlying memory address of the object.

2. **What is the Windows equivalent of POSIX `fork()`? Why is it different?**

   The Windows equivalent is CreateProcess(). The POSIX fork() model creates an exact, duplicate clone of the parent process (sharing memory until modified). Windows does not support this cloning model. Instead, CreateProcess() spins up a completely new process from scratch and loads a new executable into it all at once, requiring you to specify the program you want to run immediately.

3. **Can you use POSIX calls on Windows?**

   Yes, you can! You can use compatibility layers like Cygwin or MinGW, which take POSIX code and automatically translate the system calls into Windows API calls under the hood during compilation. Alternatively, you can use WSL (Windows Subsystem for Linux)—which is exactly what we used for this assignment—to run a genuine Linux kernel directly inside Windows, allowing native POSIX system calls to execute perfectly.

---

## Task 3: strace Analysis

**Describe what you observed:** [What surprised you about the strace output? How many more system calls did the library version make?]

### strace Output — Library Version (File Creator)

<!-- Screenshot: strace -e trace=openat,read,write,close ./file_creator_lib -->
<!-- IMPORTANT: Highlight/annotate the key system calls in your screenshot -->
![strace - Library version File Creator](/activity1/screenshorts/strace_lib_creator.png)

### strace Output — System Call Version (File Creator)

<!-- Screenshot: strace -e trace=openat,read,write,close ./file_creator_sys -->
<!-- IMPORTANT: Highlight/annotate the key system calls in your screenshot -->
![strace - System call version File Creator](/activity1/screenshorts/strace_sys_creator.png)

### strace Output — Library Version (File Reader or Dir Listing)

![strace - Library version](/activity1/screenshorts/strace_lib_reader.png)

### strace Output — System Call Version (File Reader or Dir Listing)

![strace - System call version](/activity1/screenshorts/strace_sys_creator.png)

### strace -c Summary Comparison

<!-- Screenshot of `strace -c` output for both versions -->
![strace summary - Library](/activity1/screenshorts/strace_summary_lib.png)
![strace summary - Syscall](/activity1/screenshorts/strace_summary_sys.png)

### Questions

1. **How many system calls does the library version make compared to the system call version?**

  Based on the strace -c output we captured, both the library version and the system call version made exactly 38 system calls. While library programs often have a higher count due to background overhead, in this specific environment, both versions undergo the exact same dynamic linking startup routine (loading standard OS libraries into memory) before running, which caused the totals to tie.

2. **What extra system calls appear in the library version? What do they do?**

   The library version includes hidden memory and file-management setup calls. For example, it uses mmap and brk to allocate memory and manage the heap for its internal data buffers, and fstat to securely check the file's status and properties. The raw system call version skips this background management completely.

3. **How many `write()` calls does `fprintf()` actually produce?**

   The trace reveals exactly two write() calls (one for writing to the file, and one for printing the success message to the screen). This perfectly matches expectations because fprintf() is simply a high-level wrapper. It handles the text formatting and buffering in memory, but it ultimately has to trigger the kernel's raw write() system call to physically put that data on the disk.

4. **In your own words, what is the real difference between a library function and a system call?**

   A system call is a raw, direct command to the operating system's kernel. It is fast and unfiltered, but requires the programmer to manually specify low-level details (like the exact 0644 file permissions). A library function acts like an assistant—it provides a simplified interface, automatically manages tedious background tasks like memory buffering, and then eventually makes the necessary system calls on your behalf.

---

## Task 4: Exploring OS Structure

### System Information

> 📸 Screenshot of `uname -a`, `/proc/cpuinfo`, `/proc/meminfo`, `/proc/version`, `/proc/uptime`:

![System Info](/activity1/screenshorts/task4_system_info.png)

### Process Information

> 📸 Screenshot of `/proc/self/status`, `/proc/self/maps`, `ps aux`:

![Process Info](/activity1/screenshorts/task4_process_info.png)

### Kernel Modules

> 📸 Screenshot of `lsmod` and `modinfo`:

![Kernel Modules](/activity1/screenshorts/task4_modules.png)

### OS Layers Diagram

> 📸 Your diagram of the OS layers, labeled with real data from your system:

![OS Layers Diagram](/activity1/screenshorts/task4_os_layers_diagram.png)

### Questions

1. **What is `/proc`? Is it a real filesystem on disk?**

  /proc is a virtual filesystem. It does not exist on a physical hard drive. Its contents are dynamically generated in real-time by the Linux kernel to expose system data, hardware details, and running process information to user space.

2. **Monolithic kernel vs. microkernel — which type does Linux use?**

   A monolithic kernel runs all core OS services (drivers, file systems, memory management) in a single large kernel space for high performance, whereas a microkernel runs only absolute essentials in kernel space and delegates the rest to user space. Linux is a modular monolithic kernel; it operates in a single space but, as shown by the lsmod output, allows drivers (modules) to be loaded and unloaded dynamically at runtime.

3. **What memory regions do you see in `/proc/self/maps`?**

   The output shows the virtual memory layout for the running process. The distinct regions include the executable code itself, mapped shared libraries (like libc.so), the heap (for dynamically allocated memory), and the stack (for local variables and function execution).

4. **Break down the kernel version string from `uname -a`.**

   The string breaks down system details. It typically includes the OS kernel name (Linux), the network hostname, the exact kernel release version (e.g., 5.15.0-X), the kernel compilation timestamp/details, the machine hardware architecture (e.g., x86_64), and the operating system environment (GNU/Linux).

5. **How does `/proc` show that the OS is an intermediary between programs and hardware?**

   OS as an Intermediary:/proc perfectly demonstrates this role because user programs are not allowed to directly probe the physical CPU or RAM. Instead, the OS securely reads the hardware state and translates it into readable text files inside /proc so user programs (like cat or htop) can access the data safely.
---

## Reflection

What did you learn from this activity? What was the most surprising difference between library functions and system calls?

I learned how to visualize the layered architecture of an operating system in real-time. By exploring the /proc virtual filesystem, I was able to see exactly how the OS acts as a secure intermediary, translating raw hardware data (like CPU model and RAM size) and kernel states into readable text files for user space programs. I also learned how to use strace to intercept and monitor the exact communication happening between a running program and the Linux kernel, proving that even simple tasks require complex interactions behind the scenes.