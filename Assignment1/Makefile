CC=gcc
CFLAGS=-Wall -g

all: daemon backup reports lock unlock check collect

daemon: daemon.c backup_dashboard.o check_file_uploads.o lock_directories.o update_timer.o collect_reports.o unlock_directories.o sig_handler.o
	$(CC) $(CFLAGS) -o daemon daemon.c backup_dashboard.o check_file_uploads.o lock_directories.o update_timer.o collect_reports.o unlock_directories.o sig_handler.o

backup: Backup_Dashboard_file.c
	$(CC) $(CFLAGS) -o Backup_Dashboard_file Backup_Dashboard_file.c

reports: generate_report_file.c
	$(CC) $(CFLAGS) -o generate_report_file generate_report_file.c

lock: lock_directories_2.c
	$(CC) $(CFLAGS) -o lock_directories_2 lock_directories_2.c

unlock: unlock_directories_2.c
	$(CC) $(CFLAGS) -o unlock_directories_2 unlock_directories_2.c

check: check__file_uploads_2.c
	$(CC) $(CFLAGS) -o check__file_uploads_2 check__file_uploads_2.c

collect: collect_report_file.c
	$(CC) $(CFLAGS) -o collect_report_file collect_report_file.c

sig_handler: sig_handler.c
	$(CC) $(CFLAGS) -c sig_handler.c

clean:
	rm -f daemon Backup_Dashboard_file generate_report_file lock_directories_2 unlock_directories_2 check__file_uploads_2 collect_report_file sig_handler.o