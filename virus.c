#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

// Fungsi untuk mengganti string dalam buffer
void replaceString(char *buffer, const char *toReplace, const char *replaceWith) {
    char *pos;
    int replaceLen = strlen(toReplace);
    int withLen = strlen(replaceWith);

    // Loop hingga tidak ada string lagi yang ditemukan
    while ((pos = strstr(buffer, toReplace)) != NULL) {
        // Mengganti string yang ditemukan dengan string pengganti
        memmove(pos + withLen, pos + replaceLen, strlen(pos + replaceLen) + 1);
        memcpy(pos, replaceWith, withLen);
        buffer += withLen;
    }
}

// Fungsi untuk mencatat penggantian string ke file log
void logReplacement(const char *filename, const char *timestamp) {
    FILE *logfile = fopen("/home/irfanqs/modul2_soal1/virus.log", "a");
    if (logfile != NULL) {
        fprintf(logfile, "[%s] Suspicious string at %s successfully replaced!\n", timestamp, filename);
        fclose(logfile);
    }
}

int main(int argc, char *argv[]) {
    const char *strMalware = "m4LwAr3";
    const char *strSpyware = "5pYw4R3";
    const char *strRansomware = "R4nS0mWaR3";

    const char *dir = argv[1];

    // Daemon process
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
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(1) {
        DIR *dp = opendir(dir);
        struct dirent *ep;

        while((ep = readdir(dp)) != NULL) {
            if (ep->d_type == 8) {
                char filename[1024];
                snprintf(filename, sizeof(filename), "%s/%s", dir, ep->d_name);

                FILE *file = fopen(filename, "r+");
                if (file != NULL) {
                    fseek(file, 0, SEEK_END);
                    long fileSize = ftell(file);
                    fseek(file, 0, SEEK_SET);

                    char *buffer = malloc(fileSize + 1);
                    if (buffer != NULL) {
                        fread(buffer, 1, fileSize, file);

                        replaceString(buffer, strMalware, "[MALWARE]");
                        replaceString(buffer, strSpyware, "[SPYWARE]");
                        replaceString(buffer, strRansomware, "[RANSOMWARE]");

                        fseek(file, 0, SEEK_SET);
                        fwrite(buffer, 1, fileSize, file);

                        free(buffer);

                        time_t rawtime;
                        struct tm *info;
                        char timestamp[80];

                        time(&rawtime);
                        info = localtime(&rawtime);
                        strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", info);

                        logReplacement(filename, timestamp);
                    }

                    fclose(file);
                }
            }
        }

        sleep(15);
    }

  return 0;
}