#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Function: unlock_directories
 * -----------------------------
 * Unlocks the directories by changing their permissions to read, write, and execute for all users.
 * Logs operations and errors to a file.
 */
void unlock_directories() {
    pid_t pid;
    int status;

    // Open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening logs file.\n");
        return;
    }
    fprintf(fptr, "__________________unlock_directories____________________\n");

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        // Error handling for fork failure
        fprintf(fptr, "ERROR. Fork failed.\n");
        exit(1);
    }
    else if (pid == 0) { // Child process
        // Unlock GenerateDirectory
        char* directory_path = "GenerateDirectory";
        int result = chmod(directory_path, S_IRWXU | S_IRWXG | S_IRWXO);
        if(result != 0) {
            // Error handling for unlocking failure
            fprintf(fptr, "ERROR. Failed to unlock GenerateDirectory.\n");
        }
        else {
            // Log success message for unlocking
            fprintf(fptr, "SUCCESS. GenerateDirectory unlocked.\n");
        }

        // Unlock CollectedDirectory
        char* directory_path2 = "CollectedDirectory";
        int result2 = chmod(directory_path2, S_IRWXU | S_IRWXG | S_IRWXO);
        if(result2 != 0) {
            // Error handling for unlocking failure
            fprintf(fptr, "ERROR. Failed to unlock CollectedDirectory.\n");
        }
        else {
            // Log success message for unlocking
            fprintf(fptr, "SUCCESS. CollectedDirectory unlocked.\n");
        }

        // Unlock backupDirectory
        char* directory_path3 = "backupDirectory";
        int result3 = chmod(directory_path3, S_IRWXU | S_IRWXG | S_IRWXO);
        if(result3 != 0) {
            // Error handling for unlocking failure
            fprintf(fptr, "ERROR. Failed to unlock backupDirectory.\n");
        }
        else {
            // Log success message for unlocking
            fprintf(fptr, "SUCCESS. backupDirectory unlocked.\n");
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


