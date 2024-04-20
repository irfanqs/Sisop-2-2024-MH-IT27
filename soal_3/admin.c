#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

void psUserToTerminal(const char *namaUser) {
    // Membuat command untuk mendapatkan informasi proses
    char command[100];
    snprintf(command, 100, "ps -u %s -o user,pid,comm", namaUser);

    // Menjalankan command dan menulis output langsung ke terminal
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to execute command.\n");
        return;
    }

    char output[100];
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output); // Output langsung ke terminal
    }

    // Menutup pipe
    pclose(fp);
}

void psUserToTXT(const char *namaUser) {
    // Membuat command untuk mendapatkan informasi proses
    char command[100];
    snprintf(command, 100, "ps -u %s -o user,pid,comm --no-headers", namaUser);

    // Membuka file ps_user.txt untuk menulis
    FILE *outputFile = fopen("ps_user.txt", "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error: Unable to open or create output file.\n");
        return;
    }

    // Menjalankan command dan menulis output ke file
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to execute command.\n");
        fclose(outputFile);
        return;
    }

    char output[100];
    while (fgets(output, sizeof(output), fp) != NULL) {
        fprintf(outputFile, "%s", output);
    }

    // Menutup file dan pipe
    pclose(fp);
    fclose(outputFile);
}

void psUserToLog(const char *namaUser) {
    pid_t pid, sid; 
    pid = fork(); 
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/home/irfanqs/modul2/soal_3")) < 0) {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  
    // Membuat nama file log
    char logFileName[100];
    snprintf(logFileName, 100, "/home/irfanqs/modul2/soal_3/%s.log", namaUser);

    // Menghapus file ps_user.txt (jika ada)
    remove("ps_user.txt");

    // Loop utama
    while(1) {
        // Memanggil fungsi untuk menulis ps_user.txt
        psUserToTXT(namaUser);

        // Membuka file log untuk ditulis
        FILE *logFile = fopen(logFileName, "a");
        if (logFile == NULL) {
            fprintf(stderr, "Error: Unable to open or create log file.\n");
            return;
        }

        // Menyimpan timestamp saat ini
        time_t now;
        struct tm *tm_info;
        time(&now);
        tm_info = localtime(&now);
        char timestamp[100];
        strftime(timestamp, 100, "%d:%m:%Y-%H:%M:%S", tm_info);

        // Membuka file ps_user.txt untuk dibaca
        FILE *psFile = fopen("/home/irfanqs/modul2/soal_3/ps_user.txt", "r");
        if (psFile == NULL) {
            fprintf(stderr, "Error: Unable to open ps_user.txt.\n");
            fclose(logFile);
            return;
        }

        // Membaca setiap baris dari file ps_user.txt dan menulis ke file log
        char line[100];
        while ((fgets(line, sizeof(line), psFile) != NULL)) {
            // Memecah baris menjadi token
            char *nameToken = strtok(line, " ");
            char *pidToken = strtok(NULL, " ");
            char *processNameToken = strtok(NULL, " ");
            char *newlinePos = strchr(processNameToken, '\n');
            if (newlinePos != NULL) {
                *newlinePos = '\0';
            }

            // Menulis ke file log
            fprintf(logFile, "[%s]-%s-%s_JALAN\n", timestamp, pidToken, processNameToken);
        }

        // Menutup file
        fclose(psFile);
        fclose(logFile);

        // Menghapus file ps_user.txt
        remove("/home/irfanqs/modul2/soal_3/ps_user.txt");

        // Delay sebelum iterasi berikutnya
        sleep(1);
    }
}

void killUserProcessToLog(char *namaUser) {
    pid_t pid, sid; 
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/home/irfanqs/modul2/soal_3/")) < 0) {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  
    // Membuat nama file log
    char logFileName[100];
    snprintf(logFileName, 100, "/home/irfanqs/modul2/soal_3/%s.log", namaUser);

    // Loop utama
    while(1) {
        // Memanggil fungsi untuk menulis ps_user.txt
        psUserToTXT(namaUser);

        // Membuka file log untuk ditulis
        FILE *logFile = fopen(logFileName, "a");
        if (logFile == NULL) {
            fprintf(stderr, "Error: Unable to open or create log file.\n");
        }

        // Membuka file ps_user.txt untuk dibaca
        FILE *psFile = fopen("/home/irfanqs/modul2/soal_3/ps_user.txt", "r");
        if (psFile == NULL) {
            fprintf(stderr, "Error: Unable to open ps_user.txt.\n");
            fclose(logFile);
        }

        // Membaca setiap baris dari file ps_user.txt dan menulis ke file log
        char line[100];
        while ((fgets(line, sizeof(line), psFile) != NULL)) {
            // Menyimpan timestamp saat ini
            time_t now;
            struct tm *tm_info;
            time(&now);
            tm_info = localtime(&now);
            char timestamp[100];
            strftime(timestamp, 100, "%d:%m:%Y-%H:%M:%S", tm_info);
            // Memecah baris menjadi token
            char *nameToken = strtok(line, " ");
            char *pidToken = strtok(NULL, " ");
            char *processNameToken = strtok(NULL, " ");
            char *newlinePos = strchr(processNameToken, '\n');
            if (newlinePos != NULL) {
                *newlinePos = '\0';
            }

                if (strcmp(processNameToken, "bash") == 0) {
                    continue;
                } else if (strcmp(processNameToken, "sh") == 0) {
                    continue;
                } else if (strcmp(processNameToken, "ps") == 0) {
                    continue;
                } else if (strcmp(processNameToken, "admin") == 0) {
                    continue;
                } else {
                    pid_t targetPid = atoi(pidToken);
                    kill(targetPid, SIGTERM);                       
                    fprintf(logFile, "[%s]-%s-%s_GAGAL\n", timestamp, pidToken, processNameToken);
                }        
            sleep(1);    
        }

        // Menutup file
        fclose(psFile);
        fclose(logFile);

        // Menghapus file ps_user.txt
        remove("/home/irfanqs/modul2/soal_3/ps_user.txt");
    }
}

void killAdmin() {
  char *args[] = {"pkill", "admin", NULL}; // Ganti "admin" dengan nama proses yang ingin Anda hentikan
    if (execvp("pkill", args) == -1) {
        perror("Error executing pkill");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    char *option = argv[1];
    char *namaUser = argv[2];

    if (strcmp(option, "-m") == 0) {
      psUserToLog(namaUser);
    } else if (strcmp(option, "-s") == 0) {
       killAdmin();
    } else if (strcmp(option, "-c") == 0) {
       killUserProcessToLog(namaUser);
    } else if (strcmp(option, "-a") == 0) {
       killAdmin();
    } else {
       psUserToTerminal(option);
    }
    
    return 0;
}