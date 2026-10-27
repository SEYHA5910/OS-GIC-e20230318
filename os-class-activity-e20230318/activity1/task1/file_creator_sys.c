/* file_creator_sys.c */
#include <fcntl.h>    // open(), O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>   // write(), close()
#include <string.h>   // strlen()

int main() {
  char buffer[1024];
        ssize_t bytesRead;
    
     int fd = open("output.txt", O_WRONLY| O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        write(2, "Error opening file\n", 19);  
        return 1;
    }           
    // 2. Write the exact same message into the file (fd)
    const char *text = "Hello from Operating Systems class!\n";
    write(fd, text, 36); 

    // 3. Close the file
    close(fd);

    // 4. Print the success message to the terminal screen (fd 1 = standard output)
    const char *success = "File created successfully!\n";
    write(1, success, 27);

    return 0;
}                    
    
