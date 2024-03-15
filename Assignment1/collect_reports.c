#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <stdlib.h>

/*
 * Function: collect_reports
 * --------------------------
 * Collects reports by moving the contents of a shared directory to a dashboard directory.
 * Uses fork and execvp to execute the move command.
 * Logs operations and errors to a file.
 */
void collect_reports(void) {
    // Open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(fptr, "__________________collect_reports____________________\n");

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(fptr, "ERROR. Fork failed.\n");
        exit(1);
    }
    else if (pid == 0) { // Child process
        // Execute move command to move contents from GenerateDirectory to CollectedDirectory
        // Arguments for execvp command
        char *args[] = {"find", "GenerateDirectory", "-mindepth", "1", "-exec", "mv", "-t", "CollectedDirectory", "{}", "+", NULL};
        // Execute find command with specified arguments
        execvp("find", args);
        // If execvp fails, log error
        fprintf(fptr, "ERROR. execvp failed.\n");
        exit(1);
    }
    else { // Parent process
        // Wait for child process to finish
        int status;
        waitpid(pid, &status, 0);
        fprintf(fptr, "All contents of GenerateDirectory moved to CollectedDirectory.\n");
    }
    // Close log file
    fprintf(fptr, "___________________________________________\n\n");
    fclose(fptr);
}
