#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define TARGET_FOLDER_PATH argv[1] // Jalur folder target
#define MALWARE_STRING "m4LwAr3" // String malware
#define MALWARE_REPLACE "[MALWARE]" // Pengganti string malware
#define SPYWARE_STRING "5pYw4R3" // String spyware
#define SPYWARE_REPLACE "[SPYWARE]" // Pengganti string spyware
#define RANSOMWARE_STRING "R4nS0mWaR3" // String ransomware
#define RANSOMWARE_REPLACE "[RANSOMWARE]" // Pengganti string ransomware
#define LOG_FILE_PATH "virus.log" // Jalur file log

// Deklarasi fungsi str_replace
char *str_replace(const char *orig, const char *rep, const char *with) {
    char *result; // string hasil penggantian
    const char *ins; // pointer ke string sementara
    char *tmp; // pointer untuk alokasi memori
    int len_rep; // panjang string yang diganti
    int len_with; // panjang string pengganti
    int len_front; // panjang string hingga pertemuan pertama
    int count; // jumlah penggantian

    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // string pengganti tidak boleh kosong

    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    // alokasi memori cukup untuk string baru
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        tmp = strstr(orig, rep);
        len_front = tmp - orig;
        tmp = strncpy(tmp, with, len_with) + len_with;
        orig += len_front + len_rep; // maju ins melewati string yang diganti
        strcpy(tmp, orig); // copy sisa string
    }

    return result;
}

// Fungsi untuk membuat file log jika belum ada
void create_log_file() {
    FILE *log_file = fopen(LOG_FILE_PATH, "a");
    if (!log_file) {
        printf("Error creating log file: %s\n", LOG_FILE_PATH);
        exit(1);
    }
    fclose(log_file);
}

// Fungsi untuk mengganti string dalam file
void replace_strings_in_file(const char *file_path) {
    FILE *file, *log_file;
    char *file_content, *new_file_content;
    size_t file_size, new_file_size;

    // Membuka file
    file = fopen(file_path, "r");
    if (!file) {
        printf("Error opening file: %s\n", file_path);
        return;
    }

    // Mendapatkan ukuran file
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Membaca isi file
    file_content = malloc(file_size + 1);
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';
    fclose(file);

    // Mengganti string malware
    new_file_content = str_replace(file_content, MALWARE_STRING, MALWARE_REPLACE);
    if (!new_file_content) {
        printf("Error replacing string in file: %s\n", file_path);
        free(file_content);
        return;
    }

    // Mengganti string spyware
    free(file_content); // Dealokasi memori file_content sebelum alokasi baru
    file_content = new_file_content;
    new_file_content = str_replace(file_content, SPYWARE_STRING, SPYWARE_REPLACE);
    if (!new_file_content) {
        printf("Error replacing string in file: %s\n", file_path);
        free(file_content);
        return;
    }

    // Mengganti string ransomware
    free(file_content); // Dealokasi memori file_content sebelum alokasi baru
    file_content = new_file_content;
    new_file_content = str_replace(file_content, RANSOMWARE_STRING, RANSOMWARE_REPLACE);
    if (!new_file_content) {
        printf("Error replacing string in file: %s\n", file_path);
        free(file_content);
        return;
    }

    // Menghitung ukuran isi file baru
    new_file_size = strlen(new_file_content);

    // Menulis isi file baru
    file = fopen(file_path, "w");
    if (!file) {
        printf("Error opening file: %s\n", file_path);
        free(file_content);
        free(new_file_content);
        return;
    }

    fwrite(new_file_content, 1, new_file_size, file);
    fclose(file);

    // Mencatat log penggantian string
    log_file = fopen(LOG_FILE_PATH, "a");
    if (log_file) {
        time_t current_time = time(NULL);
        struct tm *time_info = localtime(&current_time);
        char buffer[26];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", time_info);
        fprintf(log_file, "[%s] Suspicious string at <%s> successfully replaced!\n", buffer, file_path);
        fclose(log_file);
    }

    free(file_content);
    free(new_file_content);
}

int main(int argc, char *argv[]) {
    // Memastikan jumlah argumen command line yang benar
    if (argc < 2) {
        printf("Usage: %s <target_folder>\n", argv[0]);
        exit(1);
    }

    // Membuat file log jika belum ada
    create_log_file();

    // Mendapatkan daftar file di folder target
    DIR *dir = opendir(TARGET_FOLDER_PATH);
    if (!dir) {
        printf("Error opening directory: %s\n", TARGET_FOLDER_PATH);
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char file_path[1024]; // Meningkatkan uk
         char file_path[1024]; // Meningkatkan ukuran file_path untuk menghindari warning snprintf
        snprintf(file_path, sizeof(file_path), "%s/%s", TARGET_FOLDER_PATH, entry->d_name);

        // Mengganti string dalam file
        replace_strings_in_file(file_path);
    }

    closedir(dir);

    return 0;
}
