# Sisop-2-2024-MH-IT27
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
### 1. buat **setup.c**

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
```

#### Fungsi `launch_applications`
Fungsi ini bertujuan untuk meluncurkan aplikasi yang diberikan beberapa kali (sesuai dengan jumlah jendela yang diinginkan). Setiap proses yang dibuat akan memiliki yang dicatat dalam satu atau lebih file PID.

*Parameter*
* `const char *app`: Nama aplikasi yang akan dijalankan.
* `int numWindows`: Jumlah jendela dari aplikasi yang ingin dijalankan.
* `const char *pidFilename`: Nama file tempat PID setiap proses akan disimpan.
* `int includeInGeneral`: Sebuah flag untuk menentukan apakah PID juga perlu dicatat dalam file PID umum.
```c
int launch_applications(const char *app, int numWindows, const char *pidFilename, int includeInGeneral) {
    FILE *pidFile = fopen(pidFilename, "a");  // Membuka atau membuat file untuk menulis PID dengan mode append
    FILE *generalPidFile = NULL;              
    if (includeInGeneral) {                   // Jika includeInGeneral true, buka GENERAL_PID_FILE untuk append
        generalPidFile = fopen(GENERAL_PID_FILE, "a");
    }

    if (!pidFile || (includeInGeneral && !generalPidFile)) {  // Untuk periksa jika file tidak dapat dibuka
        fprintf(stderr, "Error: Cannot open PID file for writing\n"); 
        return -1;  // Mengembalikan -1 jika ada kesalahan dalam membuka file
    }

    for (int j = 0; j < numWindows; j++) {  // Loop sebanyak numWindows untuk meluncurkan aplikasi
        pid_t pid = fork();                // Membuat proses anak
        if (pid == 0) {                    // Jika ini adalah proses anak
            execlp(app, app, NULL);        // Menjalankan aplikasi
            fprintf(stderr, "Failed to execute %s\n", app);  // Jika gagal, cetak error
            exit(1);  // Keluar dengan status 1.
        } else if (pid > 0) {              // Jika ini adalah proses induk
            fprintf(pidFile, "%d\n", pid);  // Menulis PID ke pidFile
            if (includeInGeneral) {         // Jika includeInGeneral true, tulis juga ke generalPidFile
                fprintf(generalPidFile, "%d\n", pid);
            }
        } else {                            // Jika fork gagal
            fprintf(stderr, "Failed to fork for %s\n", app);  // Cetak error fork
            fclose(pidFile);                // Tutup pidFile
            if (includeInGeneral) fclose(generalPidFile);  // Jika includeInGeneral, tutup generalPidFile juga
            return -1;  // Kembali dengan -1
        }
    }
    fclose(pidFile);  // Tutup pidFile
    if (includeInGeneral) fclose(generalPidFile);  // Tutup generalPidFile jika perlu
    return 0;  // Mengembalikan 0 jika sukses
}
```

#### Fungsi `kill_applications`
Fungsi ini untuk menghentikan semua proses yang PID-nya tercatat dalam file yang diberikan. Ini mengirim sinyal SIGTERM ke setiap proses tersebut.
 
*Parameter*
* `const char *pidFilename`: Nama file yang berisi daftar PID yang akan dihentikan.
```c
int kill_applications(const char *pidFilename) {
    FILE *pidFile = fopen(pidFilename, "r");  // Buka dan membaca file PID
    if (!pidFile) {  // Jika gagal membuka file
        fprintf(stderr, "Error: Cannot open PID file %s for reading\n", pidFilename); //mencetak teks dalam tanda petik
        return 1; 
    }

    int pid;
    while (fscanf(pidFile, "%d", &pid) == 1) {  // Membaca PID dari File
        if (kill(pid, SIGTERM) == -1) {  // Mengirim sinyal SIGTERM ke PID
            fprintf(stderr, "Failed to send SIGTERM to PID %d: %s\n", pid, strerror(errno));  // Jika gagal, cetak error
        } else {
            printf("Yipiy! Successfully sent SIGTERM to process %d.\n", pid);  // Jika berhasil, cetak sukses
        }
    }

    fclose(pidFile);  // Tutup file.
    return 0;  // Kembali dengan 0 jika sukses.
}
```

#### Fungsi `main`
fungsi utama yang menguraikan argumen baris perintah yang diberikan dan memanggil fungsi `launch_applications` atau `kill_applications` berdasarkan opsi yang diberikan.

*Parameter*
* `int argc`: Jumlah argumen baris perintah
* `char *argv[]`: Array yang berisi argumen baris perintah
```c
int main(int argc, char *argv[]) {
    if (argc < 2) {  // Memeriksa jumlah argumen yang diberikan.
        fprintf(stderr, "Penggunaan: %s -o <app1> <num1> <app2> <num2> ... | -f <filename> | -k [filename]\n", argv[0]);
        return 1;  // Kembali dengan 1 jika argumen tidak cukup
    }

    if (strcmp(argv[1], "-o") == 0 && argc >= 4) {  // Jika opsi '-o' dipilih dan argumen cukup
        for (int i = 2; i < argc; i += 2) { 
            if (launch_applications(argv[i], atoi(argv[i + 1]), GENERAL_PID_FILE, 1) < 0) {  // Membuka aplikasi
                return 1;  // Kembali dengan 1 jika ada kegagalan
            }
        }
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {  // Jika opsi '-f' dipilih
        char specificPidFile[256];
        sprintf(specificPidFile, "%s_pids.txt", argv[2]);  // Membuat nama file PID
        FILE *configFile = fopen(argv[2], "r");  // Membuka file konfigurasi
        if (!configFile) {  // Jika gagal membuka
            perror("Failed to open config file");
            return 1;  
        }
        char app[100];
        int numWindows;
        while (fscanf(configFile, "%s %d", app, &numWindows) == 2) {  // Membaca aplikasi dan jumlah jendela dari file
            if (launch_applications(app, numWindows, specificPidFile, 1) < 0) {  // Membuka aplikasi
                fclose(configFile);
                return 1; 
            }
        }
        fclose(configFile);  // Tutup file konfigurasi
    } else if (strcmp(argv[1], "-k") == 0) {  // Jika opsi '-k' dipilih
        char pidFilename[256] = GENERAL_PID_FILE;  // Gunakan file PID umum jika tidak ada argumen tambahan
        if (argc == 3) {
            sprintf(pidFilename, "%s_pids.txt", argv[2]);  // Format nama file PID spesifik
        }
        if (kill_applications(pidFilename) != 0) {  // Kill aplikasi berdasarkan file PID
            return 1;  
        }
    } else {  // Jika opsi yang diberikan tidak valid.
        fprintf(stderr, "Invalid option %s\n", argv[1]);  // Cetak pesan kesalahan
        return 1; 
    }

    return 0;  // Kembali dengan 0 jika semua proses sukses.
}
```


### 2. Kompilasi Program
pada terminal lakukan komplikasi file `setupc.` dengan `gcc setup.c -o setup`. untuk menjalankan program atau Membuka aplikasi

### 3. Menjalankan Program
**a. Membuka Aplikasi**

membuka aplikasi dengan menggunakan sintaks `-o` diikuti dengan nama aplikasi dan jumlah instansi yang ingin dibuka. contoh
```c
./setup -o firefox 2 wireshark 3
```
ini akan membuka dua instansi dari firefox dan tiga instansi dari wireshark

**b. Membuka Aplikasi Melalui File Konfigurasi**

menjalankan aplikasi berdasarkan konfigurasi dari file. Misalnya, telah membuat filw `file.conf` dengan isi sebagai berikut:
```c
wireshark 2
firefox 1
```
dan menjalankan program tersebut dengan perintah 
```c
./setup -f file.conf
```
ini akan membuka dua instansi wireshark dan satu instansi firefox

**c. Menghentikan Semua Aplikasi**

Untuk menghentikan semua aplikasi yang telah dibuka oleh program ini:
```c
./setup -k
```
**d. Menghentikan Aplikasi Tertentu Melalui FIle Konfigurasi**

Menghentikan aplikasi sesuai yang dibuka oleh file konfigurasi, gunakan perintah:
```c
./setup -k file.conf
```


**Dokumentasi**

*1. Tampilan Terminal*
![Screenshot from 2024-04-24 15-37-16](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/79549192/71df56a4-2989-4864-abc7-bfdbd0856750)

*2. Tampilan ketika melakukan perintah `./setup -o firefox 2 wieshark 2`*
![Screenshot from 2024-04-24 15-41-20](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/79549192/d9a101f3-c746-4f6b-8d7c-520a0f4f63d9)

*3. Tampilan ketika melakukan perintah `./setup -f file.conf` isi `file.conf` adalah `wireshark 1` sehingga apk wireshark ketambah 1 dan menjadi 3 jendela*
![Screenshot from 2024-04-24 15-41-48](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/79549192/db86db04-1f33-47dc-bb39-a7fdebe1b660)

*4. Tampilan ketika melakukan perintah `./setup -k file.conf` perintah ini hanya mematikan perintah dari file.conf sehingga wireshark dimatikan 1 jendela*
![Screenshot from 2024-04-24 15-42-15](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/79549192/a3c6e414-22af-4e4a-829f-fdf1f2905ce2)

*5. Tampilan ketika kill keseluruhan dengan perintah `./setup -k` maka semua tab yang dibuka akan mati/tertutup*
![Screenshot from 2024-04-24 15-42-37](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/79549192/f79f7cd1-1ae7-4db4-b7e1-5f9c7f5d63c4)




### Kendala
Tidak ada kendala pada nomor ini

### Revisi
Tidak ada Revisi pada nomor ini
