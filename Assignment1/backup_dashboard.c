#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/*
 * Function: backup_dashboard
 * ---------------------------
 * Performs backup of dashboard directory to backup directory.
 * Uses fork and execvp to execute the copy command.
 * Logs operations and errors to a file.
 */
void backup_dashboard(void) {
    pid_t pid;
    int status;

    // Open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(fptr, "__________________backup_dashboard____________________\n");

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        // Error handling for fork failure
        fprintf(fptr, "ERROR. Fork failed.\n");
        exit(1);
    }
    else if (pid == 0) { // Child process
        // Execute copy command to copy contents from CollectedDirectory to backupDirectory
        // Arguments for execvp command
        char *args[] = {"find", "CollectedDirectory", "-mindepth", "1", "-exec", "cp", "-t", "backupDirectory", "{}", "+", NULL};
        // Execute find command with specified arguments
        execvp("find", args);
        // If execvp fails, log error
        fprintf(fptr, "ERROR. execvp failed.\n");
        exit(1);
    }
    else { // Parent process
        // Wait for the child process to finish 
        waitpid(pid, &status, 0);
        // Check the status of child process
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // If child process exits normally, log success message
            fprintf(fptr, "SUCCESS. All contents of CollectedDirectory copied to backupDirectory.\n");
        }
        else {
            // If child process exits with error, log error message
            fprintf(fptr, "ERROR. Failed to copy contents of CollectedDirectory to backupDirectory.\n");
        }
    }
    // Close log file
    fprintf(fptr, "___________________________________________\n\n");
    fclose(fptr);
}

