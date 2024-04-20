#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// variabel menyimpan mode
char *mode = "default";
bool running = true; // tanda jalan / tidak

// menyimpan log
FILE *log_file;

void run_exec(char path[], char *argv[]) {
    pid_t child_id;
    int status;
    child_id = fork();

    if (child_id < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (child_id == 0) {
        execv(path, argv);
        perror("execv failed");
        exit(EXIT_FAILURE);
    } else {
        while (wait(&status) > 0);
    }
}

char *getCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;
    static char buffer[9]; 

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%T", timeinfo);
    return buffer;
}

// Fungsi dekripsi nama file
void deskripsiNamafile(char *filename) {
     if (strcmp(mode, "default") == 0) {
        int i = 0;
        while (filename[i] != '\0') {
            if ((filename[i] >= 'a' && filename[i] <= 'z')) {
                filename[i] = (((filename[i] - 'a') + 7) % 26) + 'a';
            } else if ((filename[i] >= 'A' && filename[i] <= 'Z')) {
                filename[i] = (((filename[i] - 'A') + 7) % 26) + 'A';
            }
            i++;
        }
    }
}

// Fungsi proses file
void processFiles() {
     if (strcmp(mode, "backup") == 0) {
        printf("Backup mode activated\n");
        fprintf(log_file, "[%s][%s] - Successfully moved to backup\n", getenv("USER"), getCurrentTime());
    } else if (strcmp(mode, "restore") == 0) {
        printf("Restore mode activated\n");
        fprintf(log_file, "[%s][%s] - Successfully restored from backup\n", getenv("USER"), getCurrentTime());
    } else {
        printf("Default mode activated\n");
    }
}

int main(int argc, char *argv[]) {
   log_file = fopen("history.log", "a");
    if (log_file == NULL) {
        perror("Failed to open history.log");
        return EXIT_FAILURE;
    }

      fprintf(log_file, "[%s][%s] - Starting in default mode\n", getenv("USER"), getCurrentTime());

      if (argc > 2 && strcmp(argv[1], "-m") == 0) {
        mode = argv[2];
        fprintf(log_file, "[%s][%s] - Mode set to %s\n", getenv("USER"), getCurrentTime(), mode);
    }

    while (running) {
        processFiles();
        sleep(1); // Tunda 1 detik 
    }

    // menutup log
    fclose(log_file);

    // Keluar dari program
    return 0;
}
