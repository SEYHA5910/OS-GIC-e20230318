/* dir_list_sys.c */
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>    // only for snprintf to format numbers into strings

int main() {
    char buffer[512];
    struct dirent *entry;
    struct stat fileStat;
//1. Open current directory
    DIR* dir = opendir(".");
    if(dir == NULL){
        write(2, "Error opening directory\n", 24);
        return 1;
    }
//2. Print header lines using write()
    const char *header = "FilenameSize (bytes)\n------------------------------ ----------\n";
    write(1, header, strlen(header));

    //3. Loop through entities
    while ((entry = readdir(dir)) != NULL) {
        // 4. Get file stats
        if (stat(entry->d_name, &fileStat) == 0) {
            // 5. Format the text into a string buffer, then write() the string
            int len = snprintf(buffer, sizeof(buffer), "%-30s %10ld\n", entry->d_name, fileStat.st_size);
            write(1, buffer, len);
        }
    }
    // 6. Close directory
    closedir(dir);
    return 0;
}