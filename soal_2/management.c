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

// dekripsi nama file ROT19
void deskripsiNamafile(char *filename) {
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

// cek kode di string
bool jenisKode(char *filename, char *code) {
    return strstr(filename, code) != NULL;
}

// tindakan berdasarkan kode
void processFiles(char *mode) {
    struct dirent *de;
    DIR *dr = opendir(".");

    if (dr == NULL) {
        printf("Direktori tidak dapat dibuka\n");
        exit(EXIT_FAILURE);
    }

    while ((de = readdir(dr)) != NULL) {
        char *filename = de->d_name;
        //skip 7 karakter
        if (strlen(filename) < 7) continue;

        deskripsiNamafile(filename);

        // Cek kode nama file
        if (jenisKode(filename, "d3Let3")) {
            remove(filename);
            printf("File %s telah dihapus\n", filename);
        } else if (jenisKode(filename, "r3N4mE")) {
            char *extension = strrchr(filename, '.');
            if (extension != NULL) {
                if (strcmp(extension, ".ts") == 0) {
                    rename(filename, "helper.ts");
                    printf("File %s telah direname menjadi helper.ts\n", filename);
                } else if (strcmp(extension, ".py") == 0) {
                    rename(filename, "calculator.py");
                    printf("File %s telah direname menjadi calculator.py\n", filename);
                } else if (strcmp(extension, ".go") == 0) {
                    rename(filename, "server.go");
                    printf("File %s telah direname menjadi server.go\n", filename);
                } else {
                    rename(filename, "renamed.file");
                    printf("File %s telah direname menjadi renamed.file\n", filename);
                }
            }
        } else {
            rename(filename, "renamed.file");
            printf("File %s telah direname menjadi renamed.file\n", filename);
        }
    }

    closedir(dr);

    // backup
    if (strcmp(mode, "backup") == 0) {
        char *backup_folder = "library/backup";
        mkdir(backup_folder, 0777); 
        dr = opendir(".");
        if (dr == NULL) {
            printf("Direktori tidak dapat dibuka\n");
            exit(EXIT_FAILURE);
        }
        while ((de = readdir(dr)) != NULL) {
            char *filename = de->d_name;
            if (jenisKode(filename, "m0V3")) {
                char new_path[strlen(backup_folder) + strlen(filename) + 2];
                sprintf(new_path, "%s/%s", backup_folder, filename);
                rename(filename, new_path);
                printf("File %s telah dipindahkan ke folder backup\n", filename);
            }
        }
        closedir(dr);
    } else if (strcmp(mode, "restore") == 0) {
        char *backup_folder = "library/backup";
        dr = opendir(backup_folder);
        if (dr == NULL) {
            printf("Backup folder tidak ditemukan\n");
            exit(EXIT_FAILURE);
        }
        while ((de = readdir(dr)) != NULL) {
            char *filename = de->d_name;
            char new_path[strlen(filename) + 2];
            sprintf(new_path, "%s/%s", backup_folder, filename);
            rename(new_path, filename);
            printf("File %s telah dikembalikan ke folder sebelumnya\n", filename);
        }
        closedir(dr);
        remove(backup_folder);
    }
}

void downloadFile() {
    char *url = "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup";
    char *fileName = "library.zip";
    
    // Download menggunakan wget
    char *wget_argv[] = { "wget", "--no-check-certificate", url, "-O", fileName, "-q", NULL };
    run_exec("/usr/bin/wget", wget_argv);
    
    printf("library.zip download complete\n");
}

void unzipFile() {
    char *zipFileName = "library.zip";
    
    // Unzip menggunakan unzip
    char *unzip_argv[] = { "unzip", "-q", zipFileName, "-d", "library", NULL };
    run_exec("/usr/bin/unzip", unzip_argv);
    
    printf("library.zip unzip complete\n");
}

int main(int argc, char *argv[]) {
    char *mode = "default"; 

    if (argc > 2 && strcmp(argv[1], "-m") == 0) {
        mode = argv[2];
    }

    downloadFile();
    unzipFile();
    processFiles(mode);
    return 0;
}
