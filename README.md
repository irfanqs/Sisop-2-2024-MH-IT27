# Sisop-1-2024-MH-IT27
Anggota Kelompok :
|  NRP|Nama Anggota  |
|--|--|
|5027231079|Harwinda|
|5027221058|Irfan Qobus Salim|
|5027231038|Dani Wahyu Anak Ary|

List Soal:

 - [Soal 1](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/tree/main#soal_1)
 - [Soal 2](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/tree/main#soal_2)
 - [Soal 3](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/tree/main#soal_3)
 - [Soal 4](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/tree/main#soal_4)

## Soal 1
## Soal 2
### Penjelasan
pertama buat **setup.c**
berikut keseluruhan isi dari **setup.c**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define GENERAL_PID_FILE "all_pids.txt"

int launch_applications(const char *app, int numWindows, const char *pidFilename, int includeInGeneral) {
    FILE *pidFile = fopen(pidFilename, "a");
    FILE *generalPidFile = NULL;
    if (includeInGeneral) {
        generalPidFile = fopen(GENERAL_PID_FILE, "a");
    }

    if (!pidFile || (includeInGeneral && !generalPidFile)) {
        fprintf(stderr, "Error: Cannot open PID file for writing\n");
        return -1;
    }

    for (int j = 0; j < numWindows; j++) {
        pid_t pid = fork();
        if (pid == 0) { 
            execlp(app, app, NULL);
            fprintf(stderr, "Failed to execute %s\n", app);
            exit(1);
        } else if (pid > 0) {
            fprintf(pidFile, "%d\n", pid);
            if (includeInGeneral) {
                fprintf(generalPidFile, "%d\n", pid);
            }
        } else {
            fprintf(stderr, "Failed to fork for %s\n", app);
            fclose(pidFile);
            if (includeInGeneral) fclose(generalPidFile);
            return -1;
        }
    }
    fclose(pidFile);
    if (includeInGeneral) fclose(generalPidFile);
    return 0;
}

int kill_applications(const char *pidFilename) {
    FILE *pidFile = fopen(pidFilename, "r");
    if (!pidFile) {
        fprintf(stderr, "Error: Cannot open PID file %s for reading\n", pidFilename);
        return 1;
    }

    int pid;
    while (fscanf(pidFile, "%d", &pid) == 1) {
        if (kill(pid, SIGTERM) == -1) {
            fprintf(stderr, "Failed to send SIGTERM to PID %d: %s\n", pid, strerror(errno));
        } else {
            printf("Yipiy! Successfully sent SIGTERM to process %d.\n", pid);
        }
    }

    fclose(pidFile);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Penggunaan: %s -o <app1> <num1> <app2> <num2> ... | -f <filename> | -k [filename]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-o") == 0 && argc >= 4) {
        for (int i = 2; i < argc; i += 2) {
            if (launch_applications(argv[i], atoi(argv[i + 1]), GENERAL_PID_FILE, 1) < 0) {
                return 1;
            }
        }
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
        char specificPidFile[256];
        sprintf(specificPidFile, "%s_pids.txt", argv[2]);
        FILE *configFile = fopen(argv[2], "r");
        if (!configFile) {
            perror("Failed to open config file");
            return 1;
        }
        char app[100];
        int numWindows;
        while (fscanf(configFile, "%s %d", app, &numWindows) == 2) {
            if (launch_applications(app, numWindows, specificPidFile, 1) < 0) {
                fclose(configFile);
                return 1;
            }
        }
        fclose(configFile);
    } else if (strcmp(argv[1], "-k") == 0) {
        char pidFilename[256] = GENERAL_PID_FILE;
        if (argc == 3) {
            sprintf(pidFilename, "%s_pids.txt", argv[2]);
        }
        if (kill_applications(pidFilename) != 0) {
            return 1;
        }
    } else {
        fprintf(stderr, "Invalid option %s\n", argv[1]);
        return 1;
    }

    return 0;
}
```c

#### Fungsi `launch_applications`
Fungsi ini bertujuan untuk meluncurkan aplikasi yang diberikan beberapa kali (sesuai dengan jumlah jendela yang diinginkan). Setiap proses yang dibuat akan memiliki yang dicatat dalam satu atau lebih file PID.

*Parameter*
* const char *app: Nama aplikasi yang akan dijalankan.
* int numWindows: Jumlah jendela dari aplikasi yang ingin dijalankan.
* const char *pidFilename: Nama file tempat PID setiap proses akan disimpan.
* int includeInGeneral: Sebuah flag untuk menentukan apakah PID juga perlu dicatat dalam file PID umum.
