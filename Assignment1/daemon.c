/*
 * Daemon Process
 *
 * This daemon process performs periodic tasks related to file management and backup.
 * Tasks include checking for uploaded XML files, backing up XML reports, and managing directories.
 * Directories are locked during transfer/backup operations.
 * The process is designed to run continuously and is daemonized to detach from the controlling terminal.
 * It employs signal handling for graceful shutdown.
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>
#include "backup_dashboard.h"
#include "check_file_uploads.h"
#include "collect_reports.h"
#include "lock_directories.h"
#include "sig_handler.h"
#include "unlock_directories.h"
#include "update_timer.h"

int main() {
    // Open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening logs file.\n");
        return 1;
    }
    fprintf(fptr, "__________________daemon____________________\n");
    fprintf(fptr, "Daemon is running.\n");

    time_t now;
    struct tm backup_time;
    time(&now);  /* get current time; same as: now = time(NULL)  */
    backup_time = *localtime(&now);
    backup_time.tm_hour = 1; 
    backup_time.tm_min = 0; 
    backup_time.tm_sec = 0; 

    // Create a child process      
    int pid = fork();
 
    if (pid > 0) {
        // Parent process
        exit(EXIT_SUCCESS);
    } 
    else if (pid == 0) {
        // Orphan process
        fprintf(fptr, "Daemon Step 1 complete.\n");
       
        // Elevate the orphan process to session leader to lose controlling TTY
        if (setsid() < 0) { 
            fprintf(fptr, "ERROR. setsid failed.\n");
            exit(EXIT_FAILURE); 
        }
        fprintf(fptr, "Daemon Step 2 complete.\n");
      
        // Change file mode creation mask to 0
        umask(0);
        fprintf(fptr, "Daemon Step 3 complete.\n");
        
        // Change the current working directory to root
        if (chdir("/") < 0 ) { 
            fprintf(fptr, "ERROR. chdir failed.\n");
            exit(EXIT_FAILURE); 
        }
        fprintf(fptr, "Daemon Step 4 complete.\n");
          
        // Close all open file descriptors
        for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
            if (close(x) == -1) {
                fprintf(fptr, "ERROR. All file descriptors not closed.\n");
                break; // Exit the loop if there was an error
            }
        }  
        fprintf(fptr, "Daemon Step 5 complete.\n");

        // Signal handling
        
        // Logging
        
        // Initialize time for checking uploads
        struct tm check_uploads_time;
        time(&now);  
        check_uploads_time = *localtime(&now);
        check_uploads_time.tm_hour = 23; 
        check_uploads_time.tm_min = 30; 
        check_uploads_time.tm_sec = 0;

        // Close log file
        fprintf(fptr, "___________________________________________\n");
        fclose(fptr);
         
        // Main loop
        while(1) {
            sleep(1);

            // Signal handling
            if(signal(SIGINT, sig_handler) == SIG_ERR) {
               syslog(LOG_ERR, "ERROR: daemon.c : SIG_ERR RECEIVED");
            } 

            // Countdown to 23:30 for file upload check
            time(&now);
            double seconds_to_files_check = difftime(now, mktime(&check_uploads_time));
            if(seconds_to_files_check == 0) {
               check_file_uploads();

               // Update time for next day
               update_timer(&check_uploads_time);
            }
                  
            // Countdown to 1:00 for backup
            time(&now);
            double seconds_to_transfer = difftime(now, mktime(&backup_time));
            if(seconds_to_transfer == 0) {
               lock_directories();
               collect_reports();	  
               backup_dashboard();
               sleep(30);
               unlock_directories();
               
               // Update time for next day
               update_timer(&backup_time);
            }	
         }
      }	
      closelog();
      return 0;
}
