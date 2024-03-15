#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

/*
 * Function: check_file_uploads
 * -----------------------------
 * Checks for uploaded files in the share directory based on today's date.
 * Logs operations and errors to a file.
 */
void check_file_uploads(void) {
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    char *sales_file = NULL;
    char *warehouse_file = NULL;
    char *manufacturing_file = NULL;
    char *distribution_file = NULL;
    char *files[4];
    char *missing_files[4] = { "Sales", "Warehouse", "Manufacturing", "Distribution" };

    // Get the current date
    time_t time_now = time(NULL);
    struct tm *t = localtime(&time_now);
    char datetime_str[20];
    strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d", t);

    // Open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(fptr, "__________________check_file_uploads____________________\n");

    // Open the share directory
    dir = opendir("GenerateDirectory");
    if (dir == NULL) {
        fprintf(fptr, "ERROR. Unable to open directory.\n");
        return;
    }

    // Read all the filenames in the directory
    while ((ent = readdir(dir)) != NULL) {
        // Check if the filename starts with today's date
        if (strncmp(ent->d_name, datetime_str, 10) == 0) {
            // Add the filename to the files array
            files[count] = ent->d_name;
            count++;

            // Check if the filename contains sales, warehouse, manufacturing, or distribution
            if (strstr(ent->d_name, "Sales") != NULL) {
                sales_file = ent->d_name;
                missing_files[0] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Sales report is present.\n");
            }
            else if (strstr(ent->d_name, "Warehouse") != NULL) {
                warehouse_file = ent->d_name;
                missing_files[1] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Warehouse report is present.\n");
            }
            else if (strstr(ent->d_name, "Manufacturing") != NULL) {
                manufacturing_file = ent->d_name;
                missing_files[2] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Manufacturing report is present.\n");
            }
            else if (strstr(ent->d_name, "Distribution") != NULL) {
                distribution_file = ent->d_name;
                missing_files[3] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Distribution report is present.\n");
            }
        }
    }

    // Close the directory
    closedir(dir);

    // Check if there are exactly four files that match the criteria
    if (count == 4 && sales_file != NULL && warehouse_file != NULL &&
        manufacturing_file != NULL && distribution_file != NULL) {
        fprintf(fptr, "SUCCESS. All four reports are present.\n");
    }
    else {
        // Log missing reports
        fprintf(fptr, "ERROR. The following reports are missing:\n");
        for (int i = 0; i < 4; i++) {
            if (missing_files[i] != NULL) {
                fprintf(fptr, "- %s\n", missing_files[i]);
            }
        }
    }
    // Close log file
    fprintf(fptr, "___________________________________________\n\n");
    fclose(fptr);
}
