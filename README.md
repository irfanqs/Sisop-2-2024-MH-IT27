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
<details><summary>Klik untuk melihat soal</summary>
Gavriel adalah seorang cyber security enthusiast. Suatu hari, ia dikontrak oleh sebuah perusahaan ternama untuk membuat sebuah program yang cukup rumit dengan bayaran jutaan rupiah. Karena tergoda dengan nominal bayaran tersebut, Gavriel langsung menerima tawaran tersebut. Setelah mencoba membuat dan mengembangkan program tersebut selama seminggu, laptop yang digunakan Gavriel mengalami overheat dan mati total sehingga harus dilarikan ke tukang servis terdekat. Karena keterbatasan waktu dalam pembuatan program dan tidak ingin mengecewakan perusahaan, Gavriel meminta bantuan kalian untuk membuat program tersebut dengan ketentuan sebagai berikut:
Program dapat menerima input path berupa ‘argv’ untuk mengatur folder dimana file akan dieksekusi
Program tersebut berfungsi untuk mereplace string dengan ketentuan sebagai berikut:
String m4LwAr3 direplace dengan string [MALWARE]
String 5pYw4R3 direplace dengan string [SPYWARE]
String R4nS0mWaR3 direplace dengan string [RANSOMWARE]
Program harus berjalan secara daemon, dan tidak diperbolehkan menggunakan command system() dalam pembuatan program
Program akan secara terus menerus berjalan di background dengan jeda 15 detik
Catat setiap penghapusan string yang dilakukan oleh program pada sebuah file bernama virus.log dengan format: [dd-mm-YYYY][HH:MM:SS] Suspicious string at <file_name> successfully replaced!
</details>
 
### Penjelasan
Berikut merupakan isi dari file **virus.c**.
```c
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
```
- Hal pertama yang perlu dilakukan adalah import semua library yang dibutuhkan
 ```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
```
- Kemudian kita membuat fungsi `gantiString`. Fungsi ini bertujuan untuk mengganti string lama yang disimpan ke dalam variable `strAneh` menjadi string baru yang disimpan ke dalam variable `strNew`. Cara kerja fungsi ini adalah dengan menganalisis tiap line pada sebuah file dan jika ada kata yang namanya sama dengan variable `strAneh`, maka kata tersebut diganti dengan variable `strNew` dengan cara menghapus kata tersebut dan menggantinya dengan kata baru sesuai posisi yang telah ditentukan oleh char `pos`.
```c
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
```
- Selanjutnya adalah membuat fungsi `editLogFile`. Fungsi ini bertujuan untuk mengubah isi file virus.log dengan format yang telah ditentukan.
```c
// Fungsi untuk mencatat waktu perubahan file ke virus.log
void editLogFile(const char *filename, const char *timestamp) {
    FILE *log = fopen("/home/irfanqs/modul2/soal_1/virus.log", "a"); // a = append
    if (log != NULL) {
        fprintf(log, "[%s] Suspicious string at %s successfully replaced!\n", timestamp, filename);
        fclose(log);
    }
}
```
- Sekarang kita masuk ke dalam fungsi `main`, pertama-tama kita perlu menginisisasi variable yang diperlukan.
```c
// Inisiasi variable
    const char *strMalware = "m4LwAr3";
    const char *strSpyware = "5pYw4R3";
    const char *strRansomware = "R4nS0mWaR3";
    const char *strnewMalware = "[MALWARE]";
    const char *strnewSpyware = "[SPYWARE]";
    const char *strnewRansomware = "[RANSOMWARE]";

    const char *dir = argv[1];
```
- Di dalam fungsi ini, kita perlu membuat fungsi *daemon* karena kita ingin program berjalan secara daemon.
```c
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
```
-  Lalu kita akan membuat while loop yang memiliki jeda 15 detik. Dalam loop tersebut, program akan membuka folder dan menganalisis tiap file di dalamnya. Jika dalam sebuah file terdapat string aneh seperti `m4LwAr3`, `5pYw4R3`, dan `R4nS0mWaR3`, maka akan langsung diganti dengan string baru. Proses ini dijalankan oleh fungsi **gantiString**. Kemudian program akan mencatat waktu pergantian string tersebut dan menuliskannya ke file log dengan memanfaatkan fungsi **editLogFile**. Jika sudah selesai, maka program akan melanjutkan ke file berikutnya. Karena program ini akan berjalan terus menerus, maka kita harus kill pid nya lewat terminal.
```c
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
```
Untuk cara menjalankannya, kita dapat menjalankannya dengan cara `./virus` beserta dengan direktori yang ingin kita ubah nilainya.
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/862b96ab-7fbb-4d64-a166-a71efa1afd16)

Setelah itu kita tunggu selama beberapa detik agar file terubah namanya.<br>
*File sebelum* <br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/95a9b45d-d2e1-4763-85e1-a03baefc0261)
<br>
*File Sesudah*<br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/0fc88d17-9b40-4f3d-b197-733015a0ae74)

Jika kita telah melihat semua file terubah stringnya, kita perlu kill pid dari virus tersebut atau pkill dengan nama programnya.
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/0d24862b-3bf6-4333-8f66-831843d7cc7b)

Setelah itu, kita dapat cek isi file virus.log untuk melihat perubahan apa saja yang dibuat. Program terlihat menjalankan fungsi setelah jeda 15 detik.<br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/158997c0-2a50-498e-941a-a7d50e3e0300)

### Kendala
Tidak ada kendala pada soal ini
### Revisi
Tidak ada revisi pada soal ini

## Soal 2
<details><summary>Klik untuk melihat soal</summary>
Paul adalah seorang mahasiswa semester 4 yang diterima magang di perusahaan XYZ. Pada hari pertama magang, ia diberi tugas oleh atasannya untuk membuat program manajemen file sederhana. Karena kurang terbiasa dengan bahasa C dan environment Linux, ia meminta bantuan kalian untuk mengembangkan program tersebut.
a. Atasannya meminta agar program tersebut dapat berjalan secara daemon dan dapat mengunduh serta melakukan unzip terhadap file berikut. Atasannya juga meminta program ini dibuat **tanpa menggunakan command system()**
b. Setelah ditelusuri, ternyata hanya 6 file teratas yang nama filenya tidak dienkripsi. Oleh karena itu, bantulah Paul untuk melakukan dekripsi terhadap nama file ke-7 hingga terakhir menggunakan algoritma ROT19
c. Setelah dekripsi selesai, akan terlihat bahwa setiap file memuat salah satu dari kode berikut: r3N4mE, d3Let3, dan m0V3. Untuk setiap file dengan nama yang memuat kode d3Let3, hapus file tersebut. Sementara itu, untuk setiap file dengan nama yang memuat kode r3N4mE, lakukan hal berikut:
   - Jika ekstensi file adalah “.ts”, rename filenya menjadi “helper.ts”
   - Jika ekstensi file adalah “.py”, rename filenya menjadi “calculator.py”
   - Jika ekstensi file adalah “.go”, rename filenya menjadi “server.go”
   - Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “renamed.file”

d. Atasan Paul juga meminta agar program ini dapat membackup dan merestore file. Oleh karena itu, bantulah Paul untuk membuat program ini menjadi 3 mode, yaitu:
   - default: program berjalan seperti biasa untuk me-rename dan menghapus file. Mode ini dieksekusi ketika program dijalankan tanpa argumen tambahan, yaitu dengan command ./management saja
   - backup: program memindahkan file dengan kode m0V3 ke sebuah folder bernama “backup”
   - restore: program mengembalikan file dengan kode m0V3 ke folder sebelum file tersebut dipindahkan
Contoh penggunaan: **./management -m backup**

e. Terkadang, Paul perlu mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
   - SIGRTMIN untuk mode default
   - SIGUSR1 untuk mode backup
   - SIGUSR2 untuk mode restore
Contoh penggunaan: **kill -SIGUSR2 <pid_program>**

f. Program yang telah dibuat ini tidak mungkin akan dijalankan secara terus-menerus karena akan membebani sistem. Maka dari itu, bantulah Paul untuk membuat program ini dapat dimatikan dengan aman dan efisien

g. Terakhir, program ini harus berjalan setiap detik dan mampu mencatat setiap peristiwa yang terjadi ke dalam file .log yang bernama “history.log” dengan ketentuan:
    - Format: [nama_user][HH:MM:SS] - <nama_file> - <action>
    - nama_user adalah username yang melakukan action terhadap file
    - Format action untuk setiap kode:
       - kode r3N4mE: Successfully renamed.
       - kode d3Let3: Successfully deleted.
       - mode backup: Successfully moved to backup.
       - mode restore: Successfully restored from backup.

Contoh pesan log:
[paul][00:00:00] - r3N4mE.ts - Successfully renamed.
[paul][00:00:00] - m0V3.xk1 - Successfully restored from backup.

Berikut adalah struktur folder untuk pengerjaan nomor 2:
    soal_2/
    ├── history.log
    ├── management.c
    └── library/
        └── backup/
</details>

## Soal 3
<details><summary>Klik untuk melihat soal</summary>
Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!
Nama program tersebut dengan nama admin.c
Program tersebut memiliki fitur menampilkan seluruh proses yang dilakukan oleh seorang user dengan menggunakan command:
./admin <user>
Program dapat memantau proses apa saja yang dilakukan oleh user. Fitur ini membuat program berjalan secara daemon dan berjalan terus menerus. Untuk menjalankan fitur ini menggunakan command: 
./admin -m <user>
Dan untuk mematikan fitur tersebut menggunakan: 
./admin -s <user>
Program akan mencatat seluruh proses yang dijalankan oleh user di file <user>.log dengan format:
[dd:mm:yyyy]-[hh:mm:ss]_pid-process_nama-process_GAGAL/JALAN
Program dapat menggagalkan proses yang dijalankan user setiap detik secara terus menerus dengan menjalankan: 
./admin -c user
sehingga user tidak bisa menjalankan proses yang dia inginkan dengan baik. Fitur ini dapat dimatikan dengan command:
./admin -a user
Ketika proses yang dijalankan user digagalkan, program juga akan melog dan menset log tersebut sebagai GAGAL. Dan jika di log menggunakan fitur poin c, log akan ditulis dengan JALAN
</details>

### Penjelasan
- Berikut merupakan isi file dari **admin.c**
```c
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
```
- Pertama-tama kita membuat fungsi **psUserToTerminal**. Fungsi ini berfungsi untuk menjalankan `ps -u` dengan nama user dan memunculkannya di terminal.
```c
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
```
- Lalu kita membuat fungsi **psUserToTXT**. Fungsi ini berfungsi untuk menjalankan `ps -u` dengan nama user lalu menyimpannya ke dalam suatu file.txt.
```c
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
```
- Kemudian kita membuat fungsi **psUserToLog**. Fungsi ini berfungsi untuk menjalankan `ps -u` dengan nama user lalu menyimpan setiap baris proses ke dalam file <nama user>.log berdasarkan format yang dibutuhkan. Proses ini berjalan secara *daemon*.
```c
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
```
- Kemudian kita akan membuat fungsi **killUserProcessToLog**. Fungsi ini berfungsi untuk membunuh semua proses yang dilakukan user dan mencatatnya ke dalam file <nama user>.log. Pencatatan dilakukan setiap 1 detik. Fungsi ini bergantung kepada file txt yang menyimpan seluruh kegiatan yang dilakukan user, dan file ini dibuat dengan fungsi **psUserToTXT**.
```c
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
```
- Kemudian kita membuat fungsi **killAdmin**, di mana fungsi ini digunakan untuk membunuh program `./admin` yang sedang berjalan. 
```c
void killAdmin() {
  char *args[] = {"pkill", "admin", NULL}; // Ganti "admin" dengan nama proses yang ingin Anda hentikan
    if (execvp("pkill", args) == -1) {
        perror("Error executing pkill");
        exit(EXIT_FAILURE);
    }
}
```
- Setelah itu kita lanjut ke fungsi **main()**. Di dalam fungsi ini, kita mengatur input apa saja yang dibutuhkan untuk menjalankan fungsi-fungsi tertentu.
```c
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
```

Untuk menjalankan program ini, kita perlu mencoba dengan `./admin <user>`
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/4787fc8a-8d7d-4cec-a611-daa0bf92c931)

Selanjutnya, kita akan mencoba `./admin -m <user>` dan mengecek <user>.log
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/0302da43-be05-4e7b-b470-346f72d52308)
<br>Program berjalan secara terus menerus dan berjalan dalam mode *daemon*<br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/5da8c549-111c-4b36-abfe-5d9efc61cff2)
<br>Isi file irfanqs.log<br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/1024ab50-d031-4bf0-b26f-f35dd376eb5a)

Selanjutnya, kita akan mencoba `./admin -s <user>`untuk membunuh proses sebelumnya
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/ab076f5a-2eb4-439a-b919-8b552c081d3f)
<br>Proses sebelumnya terbunuh<br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/c686d028-06d1-4b5b-9236-49fa289dfd37)

Selanjutnya kita akan mencoba membunuh semua proses dengan `./admin -c <user>` dengan jeda satu detik
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/8f3d8ef7-04b9-4497-ab72-cc7932540f0c)
<br>Mengecek jika program sedang berjalan terus menerus dalam mode daemon<br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/260fbafd-e04f-4d5b-b818-e8d6bc10730a)
<br>Proses pembunuhan dicatat dalam file irfanqs.log setiap 1 detik<br>
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/86672664-a10a-4639-a564-f0f7effaf4aa)

Selanjutnya, kita akan membunuh proses admin agar tidak membunuh semua proses dari user lagi dengan `./admin -a <user>`.
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/6869f6dc-abd8-4a15-9112-71c2e4813944)
<br>Mengecek jika program benar-benar mati atau tidak dengan `ps aux`
![image](https://github.com/irfanqs/Sisop-2-2024-MH-IT27/assets/130438307/7e3059e8-b16d-4e71-8bad-6e4ec2c5d58c)

### Kendala
Tidak ada kendala pada soal ini

### Revisi
Tidak ada revisi pada soal ini

## Soal 4
<details><summary>Klik untuk melihat soal</summary>
Salomo memiliki passion yang sangat dalam di bidang sistem operasi. Saat ini, dia ingin mengotomasi kegiatan-kegiatan yang ia lakukan agar dapat bekerja secara efisien. Bantulah Salomo untuk membuat program yang dapat mengotomasi kegiatan dia!
(NB: Soal di uji coba dan berhasil di sistem yang menggunakan MacOS dan linux yang menggunakan dual boot dan VM. Untuk teman-teman yang menggunakan WSL bisa mengerjakan soal yang lain ya)
Salomo ingin dapat membuka berbagai macam aplikasi dengan banyak jendela aplikasi dalam satu command. Namai file program tersebut setup.c
Program dapat membuka berbagai macam aplikasi dan banyak jendela aplikasi sekaligus (bisa membuka 1 atau lebih aplikasi dengan 1 atau lebih window (kecuali aplikasi yang tidak bisa dibuka dengan banyak window seperti discord)) dengan menjalankan: 
./setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>
Contoh penggunaannya adalah sebagai berikut: 
./setup -o firefox 2 wireshark 2
Program akan membuka 2 jendela aplikasi firefox dan 2 jendela aplikasi wireshark.
Program juga dapat membuka aplikasi dengan menggunakan file konfigurasi dengan menggunakan 
./setup -f file.conf 
Format file konfigurasi dibebaskan, namun pastikan dapat menjalankan fitur dari poin 2.
Contoh isi file.conf:
Firefox 2
Wireshark 3
Ketika menjalankan command contoh, program akan membuka 2 jendela aplikasi firefox dan 3 jendela aplikasi wireshark.
Program dapat mematikan semua aplikasi yg dijalankan oleh program tersebut dengan: 
./setup -k
Program juga dapat mematikan aplikasi yang dijalankan sesuai dengan file konfigurasi. 
Contohnya: 
./setup -k file.conf 
Command ini hanya mematikan aplikasi yang dijalankan dengan 
./setup -f file.conf
</details>

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
