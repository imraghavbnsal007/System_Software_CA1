#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <errno.h>

/*
 * Function: lock_directories
 * ---------------------------
 * Locks the directories by changing their permissions to read-only for all users.
 * Logs operations and errors to a file.
 */
void lock_directories() {
    pid_t pid;
    int status;

    // Open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening logs file.\n");
        return;
    }
    fprintf(fptr, "__________________lock_directories____________________\n");

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        // Error handling for fork failure
        fprintf(fptr, "ERROR. Fork failed.\n");
        exit(1);
    }
    else if (pid == 0) { // Child process
        // Lock GenerateDirectory
        char* directory_path = "GenerateDirectory";
        int result = chmod(directory_path, S_IRUSR | S_IRGRP | S_IROTH);
        if(result != 0) {
            // Error handling for locking failure
            fprintf(fptr, "ERROR. Failed to lock GenerateDirectory.\n");
        } 
        else {
            // Log success message for locking
            fprintf(fptr, "SUCCESS. GenerateDirectory locked.\n");
        }

        // Lock CollectedDirectory
        char* directory_path2 = "CollectedDirectory";
        int result2 = chmod(directory_path2, S_IRUSR | S_IRGRP | S_IROTH);
        if(result2 != 0) {
            // Error handling for locking failure
            fprintf(fptr, "ERROR. Failed to lock CollectedDirectory.\n");
        } 
        else {
            // Log success message for locking
            fprintf(fptr, "SUCCESS. CollectedDirectory locked.\n");
        }

        // Lock backupDirectory
        char* directory_path3 = "backupDirectory";
        int result3 = chmod(directory_path3, S_IRUSR | S_IRGRP | S_IROTH);
        if(result3 != 0) {
            // Error handling for locking failure
            fprintf(fptr, "ERROR. Failed to lock backupDirectory.\n");
        } 
        else {
            // Log success message for locking
            fprintf(fptr, "SUCCESS. backupDirectory locked.\n");
        }
        exit(1);
    }
    else { // Parent process
        // Wait for child process to finish
        waitpid(pid, &status, 0);
        // Close log file
        fprintf(fptr, "___________________________________________\n\n");
        fclose(fptr);
    }
}
