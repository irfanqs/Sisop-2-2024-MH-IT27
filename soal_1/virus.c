#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>

// Fungsi untuk mengganti string
void gantiString(char *line, const char *strAneh, const char *strNew) {
    char *pos;
    int strAnehLen = strlen(strAneh);
    int strNewLen = strlen(strNew);

    // Loop hingga tidak ada string lagi yang ditemukan
    while ((pos = strstr(line, strAneh)) != NULL) {
        // Mengganti string yang ditemukan dengan string pengganti
        memmove(pos + strNewLen, pos + strAnehLen, strlen(pos + strAnehLen) + 1);
        memcpy(pos, strNew, strNewLen);
        line += strNewLen;
    }
}

// Fungsi untuk mencatat waktu perubahan file ke virus.log
void editLogFile(const char *filename, const char *timestamp) {
    FILE *log = fopen("/home/irfanqs/modul2/soal_1/virus.log", "a"); // a = append
    if (log != NULL) {
        fprintf(log, "[%s] Suspicious string at %s successfully replaced!\n", timestamp, filename);
        fclose(log);
    }
}

int main(int argc, char *argv[]) {
    // Inisiasi variable
    const char *strMalware = "m4LwAr3";
    const char *strSpyware = "5pYw4R3";
    const char *strRansomware = "R4nS0mWaR3";
    const char *strnewMalware = "[MALWARE]";
    const char *strnewSpyware = "[SPYWARE]";
    const char *strnewRansomware = "[RANSOMWARE]";

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

    // looping untuk masuk ke dalam directory
    while(1) {
        DIR *dp = opendir(dir);
        struct dirent *ep;

        while((ep = readdir(dp)) != NULL) {
            if (ep->d_type == 8) { // Seharusnya ep->d_type == DT_REG, tapi karena error maka diganti menjadi ep->d_type == 8
                char filename[1024];
                snprintf(filename, sizeof(filename), "%s/%s", dir, ep->d_name);

                // Untuk melewati file dengan ekstensi .c dan .log agar tidak merubah code dan virus.log
                char *ext = strrchr(filename, '.');
                if (ext && (strcmp(ext, ".c") == 0 || strcmp(ext, ".log") == 0)) {
                    continue;
                }

                // Membuka tiap file
                FILE *file = fopen(filename, "r+");
                if (file != NULL) {

                    char line[256];
                    while (fgets(line, sizeof(line), file)) {
                        // Mengganti string yang sesuai dalam baris
                        gantiString(line, strMalware, strnewMalware);
                        gantiString(line, strSpyware, strnewSpyware);
                        gantiString(line, strRansomware, strnewRansomware);

                        // Mengembalikan penunjuk posisi dalam file ke awal baris
                        fseek(file, -strlen(line), SEEK_CUR);

                        // Menulis kembali baris yang telah dimodifikasi ke dalam file
                        fputs(line, file);
                    }

                        time_t waktu;
                        struct tm *info;
                        char timestamp[80];

                        time(&waktu);
                        info = localtime(&waktu);
                        strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", info);

                        editLogFile(filename, timestamp);
                }
                    sleep(15);
                    fclose(file);
            }
        }
    }
  return 0;
}

