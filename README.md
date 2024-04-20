# Sisop-2-2024-MH-IT23

## Anggota Kelompok

| NRP        | Nama                            |
|:----------:|:-------------------------------:|
| 5027231020 | Acintya Edria Sudarsono         |
| 5027231044 | Dionisius Marcell Putra Indranto|
| 5027231072 | Aisyah Rahmasari                |

- [Peraturan](#peraturan) 

## Peraturan
1. Waktu pengerjaan dimulai dari hari Senin (15 April 2024) hingga hari Sabtu (20 April 2024) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme(github).
3. Format nama repository github “Sisop-[Nomor Modul]-2024-[Kode Dosen Kelas]-[Nama Kelompok]” (contoh:Sisop-1-2024-MH-IT01).
4. Struktur repository seperti berikut:
	- soal_1:
		- virus.c
	- soal_2:
		- management.c
	- soal_3:
		- admin.c
	- soal_4:
		- setup.c
<br />
Jika melanggar struktur repo akan dianggap sama dengan curang dan menerima konsekuensi sama dengan melakukan kecurangan. <br />
        1. Setelah pengerjaan selesai, semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github diletakkan pada form yang disediakan. Pastikan     
           github di setting ke publik. <br />
        2. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir. <br />
        3. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan. <br />
        4. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan. <br />
        5. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya. <br />
        6. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal shift, maka nilai dianggap 0. <br />
        7. Pengerjaan soal shift sesuai dengan modul yang telah diajarkan. <br />
        8. Zip dari repository dikirim ke email asisten penguji dengan subjek yang sama dengan nama judul repository, dikirim sebelum deadline dari soal shift <br />
        9. Jika terdapat revisi soal akan dituliskan pada halaman terakhir



### SOAL 2
Program manajemen file <br />
Fungsi ini akan mendownload file dari URL lalu melakukan unzipping file
```
// Menulis data ke file
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    fflush(stream);
    return written;
}

// Download Zip file
int download_file(const char *url, const char *filename) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("Failed to initialize curl\n");
        return -1;
    }

    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to open file for writing\n");
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    fclose(file);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK) ? 0 : -1;
}

// Unzip
int unzip_file(const char *filename) {
    char command[1024];
    sprintf(command, "unzip %s", filename);
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        syslog(LOG_ERR, "Failed to unzip file");
        return -1;
    }

    
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
    
    }

    int status = pclose(pipe);
    if (status == -1) {
        syslog(LOG_ERR, "Failed to close pipe");
        return -1;
    } else if (WEXITSTATUS(status) != 0) {
        syslog(LOG_ERR, "Unzip command failed");
        return -1;
    }

    syslog(LOG_INFO, "File unzipped successfully");
    return 0;
}
```
 <br /> <br />
 Fungsi ini membuat proses menjadi daemon dengan menggunakan fork dan umask agar dapat berjalan dengan hak akses maksimum
```
void daemonize() {
    pid_t pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    pid_t sid = setsid();

    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
```
<br /> <br />
Fungsi ini untuk menambahkan mode yang dapat digunakan
```
void handle_signal(int signum) {
    switch (signum) {
        case SIGRTMIN_VALUE:
            mode = 'd';
            break;
        case SIGUSR1:
            mode = 'b';
            break;
        case SIGUSR2:
            mode = 'r';
            break;
    }
}
```
<br /> <br />
Fungsi yang akan dipanggil ketika proses download dan ekstrak file
```
void download_and_extract() {
    const char *url = "https://drive.google.com/uc?id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup&export=download";
    const char *filename = "library.zip";
    if (download_file(url, filename) == 0) {
        unzip_file(filename);
    }
}
```
<br /> <br />
Fungsi untuk dekripsi nama file berdasarkan ROT19 menggunakan caesar ciper
```
//Dekripsi ROT19
void decrypt_filename(char *filename) {
    for (int i = 0; i < strlen(filename); i++) {
        if (filename[i] >= 'a' && filename[i] <= 'z') {
            filename[i] = 'a' + (filename[i] - 'a' + 19) % 26;
        } else if (filename[i] >= 'A' && filename[i] <= 'Z') {
            filename[i] = 'A' + (filename[i] - 'A' + 19) % 26;
        }
    }
}
```
<br /> <br />
Fungsi untuk memproses nama dari file yang sudah di ekstrak dari zip dan di dekripsi. Jika ditemukan d3Let3 maka file akan dihapus. Jika ditemukan r3N4mE maka file akan di rename sesuai dengan ekstensinya.
```
void process_files() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(LIBRARY_FOLDER);
    if (dir == NULL) {
        syslog(LOG_ERR, "Failed to open library folder");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { 
            char filename[MAX_FILENAME_LEN];
            strcpy(filename, entry->d_name);
            decrypt_filename(filename);
            
            // Cek "d3Let3"
            if (strstr(filename, "d3Let3") != NULL) {
                char filepath[MAX_PATH_LEN];
                sprintf(filepath, "%s/%s", LIBRARY_FOLDER, entry->d_name);
                if (remove(filepath) == 0) {
                    syslog(LOG_INFO, "Deleted file: %s", filename);
                    log_event("management", filename, "Successfully deleted");
                } else {
                    syslog(LOG_ERR, "Failed to delete file: %s", filename);
                }
            }
            
            // Cek "r3N4mE"
            else if (strstr(filename, "r3N4mE") != NULL) {
                char *extension = strrchr(filename, '.');
                if (extension != NULL) {
                    char new_filename[MAX_FILENAME_LEN];
                    if (strcmp(extension, ".ts") == 0) {
                        sprintf(new_filename, "helper.ts");
                    } else if (strcmp(extension, ".py") == 0) {
                        sprintf(new_filename, "calculator.py");
                    } else if (strcmp(extension, ".go") == 0) {
                        sprintf(new_filename, "server.go");
                    } else {
                        sprintf(new_filename, "renamed%s", extension);
                    }
                    char old_filepath[MAX_PATH_LEN];
                    char new_filepath[MAX_PATH_LEN];
                    sprintf(old_filepath, "%s/%s", LIBRARY_FOLDER, entry->d_name);
                    sprintf(new_filepath, "%s/%s", LIBRARY_FOLDER, new_filename);
                    if (rename(old_filepath, new_filepath) == 0) {
                        syslog(LOG_INFO, "Renamed file: %s -> %s", filename, new_filename);
                        log_event("management", filename, "Successfully renamed");
                    } else {
                        syslog(LOG_ERR, "Failed to rename file: %s -> %s", filename, new_filename);
                    }
                }
            }
        }
    }
    closedir(dir);
}

```
<br /> <br />
Fungsi ini untuk memindahkan file ke folder backup. Jika folder belum ada maka folder akan dibuat.
```
void move_to_backup(char *filename) {
    //Cek backup
    if (mkdir(BACKUP_FOLDER, 0777) == -1 && errno != EEXIST) {
        syslog(LOG_ERR, "Failed to create backup directory: %s", strerror(errno));
        return;
    }

    char old_path[MAX_PATH_LEN];
    char new_path[MAX_PATH_LEN];
    sprintf(old_path, "%s/%s", LIBRARY_FOLDER, filename);
    sprintf(new_path, "%s/%s", BACKUP_FOLDER, filename);
    if (rename(old_path, new_path) == 0) {
        syslog(LOG_INFO, "Moved %s to backup", filename);
    } else {
        syslog(LOG_ERR, "Failed to move %s to backup: %s", filename, strerror(errno));
    }
}
```
<br /> <br />
Fungsi untuk restore file dari folder backup
```
//restore dari backup
void restore_from_backup(char *filename) {
    char old_path[MAX_PATH_LEN];
    char new_path[MAX_PATH_LEN];
    sprintf(old_path, "%s/%s", BACKUP_FOLDER, filename);
    sprintf(new_path, "%s/%s", LIBRARY_FOLDER, filename);
    if (rename(old_path, new_path) == 0) {
        syslog(LOG_INFO, "Restored %s from backup", filename);
    } else {
        syslog(LOG_ERR, "Failed to restore %s from backup", filename);
    }
}
```
<br /> <br />
Fungsi log_event untuk menambahkan informasi waktu ke dalam file log dan exit_program untuk menghentikan program secara aman dan efisien
```
void log_event(const char *username, const char *filename, const char *action) {
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[9];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file != NULL) {
        fprintf(log_file, "[%s][%s] - %s - %s\n", username, timestamp, filename, action);
        fclose(log_file);
    }
}

void exit_program(int signum) {
    closelog();
    exit(EXIT_SUCCESS);
}
```
<br /> <br />
Fungsi main yang akan recall fungsi-fungsi sebelumnya untuk menjalankan program manajemen file dalam 3 mode, yaitu mode defaut, backup, dan restore
```
int main(int argc, char *argv[]) {
    openlog("management", LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_USER);
    if (argc > 1) {
        if (strcmp(argv[1], "-m") == 0) {
            if (strcmp(argv[2], "backup") == 0) {
                mode = 'b';
            } else if (strcmp(argv[2], "restore") == 0) {
                mode = 'r';
            }
        }
    }

    signal(SIGRTMIN, handle_signal);
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    // SIGINT (CTRL+C) dan SIGTERM
    signal(SIGINT, exit_program);
    signal(SIGTERM, exit_program);


    if (mode == 'b' || mode == 'r') {
        download_and_extract();
        process_files();

        if (mode == 'b') {
            // Backup
            DIR *dir;
            struct dirent *entry;
            dir = opendir(LIBRARY_FOLDER);
            if (dir != NULL) {
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_type == DT_REG) {
                        char filename[MAX_FILENAME_LEN];
                        strcpy(filename, entry->d_name);
                        decrypt_filename(filename);
                        if (strstr(filename, "m0V3") != NULL) {
                            move_to_backup(entry->d_name);
                        }
                    }
                }
                closedir(dir);
            }
        } else if (mode == 'r') {
            // Restore
            DIR *dir;
            struct dirent *entry;
            dir = opendir(BACKUP_FOLDER);
            if (dir != NULL) {
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_type == DT_REG) {
                        char filename[MAX_FILENAME_LEN];
                        strcpy(filename, entry->d_name);
                        restore_from_backup(entry->d_name);
                    }
                }
                closedir(dir);
            }
        }
    }

    closelog();
    return 0;
}
```
<br /> <br />
Jika program dijalankan, program akan mendownload file zip dari URL, mengekstrak, lalu file akan di dekripsi.
![Screenshot 2024-04-20 213713](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/a1d35308-6bbb-4732-9699-89326ee2ea36)

<br />
**Error yang terjadi** <br />
Ketika command ./management -m backup dijalankan, folder backup tidak dapat dibuat.
![Screenshot 2024-04-20 212930](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/79446083-a41f-4b93-933c-e04c5daf9db8)

Untuk command ./management -m restore dijalankan, fungsi me-restore file dalam folder library tetapi karena mode backup tidak berjalan dengan benar, maka yang di restore hanyalah berdasarkan isi folder library.
![Screenshot 2024-04-20 214150](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/1741e443-c3cb-4098-85ea-863d0720c7d6)

![Screenshot 2024-04-20 214209](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/f327b4b1-2c17-4db4-8a32-a3eacd805844)




### SOAL 3
Program admin.c yang akan memantau penggunanya <br /> <br />
**Menyimpan informasi proses ke dalam file log**
```
void log_process(const char *user, const char *pid, const char *process_name, int status) {
    FILE *log_file;
    char log_path[LOG_PATH_MAX];

    snprintf(log_path, sizeof(log_path), "%s.log", user);

    log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    time_t timevar;
    struct tm *timeInfo;
    char timeString[20];

    time(&timevar);
    timeInfo = localtime(&timevar);
    strftime(timeString, sizeof(timeString), "%d:%m:%Y-%H:%M:%S", timeInfo);

    fprintf(log_file, "[%s]_%s_%s_process_%s\n", timeString, pid, process_name, (status == 0) ? "JALAN" : "GAGAL");

    fclose(log_file);
}
```
Pertama, set file path untuk file log <br />
``` snprintf(log_path, sizeof(log_path), "%s.log", user); ```
<br />Lalu buka filenya agar informasi proses bisa disimpan di dalam file log
```
  log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        perror("Error opening log file"); //jika file tidak dapat dibuka
        exit(EXIT_FAILURE);
    }
```
<br />Untuk mendapatkan informasi waktu dari proses :
```
    time_t timevar;
    struct tm *timeInfo;
    char timeString[20];

    time(&timevar);
    timeInfo = localtime(&timevar);
    strftime(timeString, sizeof(timeString), "%d:%m:%Y-%H:%M:%S", timeInfo);
```
<br />Simpan informasi waktu ke dalam file
```
 fprintf(log_file, "[%s]_%s_%s_process_%s\n", timeString, pid, process_name, (status == 0) ? "JALAN" : "GAGAL");

    fclose(log_file);
```
<br /> <br />
**Memantau proses yang dilakukan pengguna**
```
void run_monitor(const char *user) {
    char command[COMMAND_SIZE];
    // Create the command to get the user's processes
    snprintf(command, sizeof(command), "ps -u %s o pid,comm", user);

    while (1) {
        // Open a stream to the command's output
        FILE *process_stream = popen(command, "r");
        if (process_stream == NULL) {
            perror("Error opening process stream");
            exit(EXIT_FAILURE);
        }

        char line[LINE_SIZE];
        fgets(line, sizeof(line), process_stream);
        while (fgets(line, sizeof(line), process_stream) != NULL) {
            char pid[PID_SIZE];
            char process_name[PROCESS_NAME_SIZE];

            // Parse the process ID and name from the line
            char *last_space = strrchr(line, ' ');
            if (last_space != NULL) {
                strncpy(process_name, last_space + 1, PROCESS_NAME_SIZE - 1);
                process_name[PROCESS_NAME_SIZE - 1] = '\0';
                *last_space = '\0';
            }

            strncpy(pid, line, PID_SIZE - 1);
            pid[PID_SIZE - 1] = '\0';

            printf("Process: %s, PID: %s\n", process_name, pid);

            // Log the process information
            log_process(user, pid, process_name, 0);
        }

        // Close the stream
        pclose(process_stream);
        sleep(SLEEP_TIME);
    }
}

```
Dapatkan proses yang sedang dilakukan pengguna
``` snprintf(command, sizeof(command), "ps -u %s o pid,comm", user); ``` <br />
Lalu buka stream untuk mendapat informasi proses yang sedang dilakukan
```
 while (1) {
        FILE *process_stream = popen(command, "r");
        if (process_stream == NULL) {
            perror("Error opening process stream");
            exit(EXIT_FAILURE);
        }

        char line[LINE_SIZE];
        fgets(line, sizeof(line), process_stream);
        while (fgets(line, sizeof(line), process_stream) != NULL) {
            char pid[PID_SIZE];
            char process_name[PROCESS_NAME_SIZE];

            // Ambil ID dan nama proses
            char *last_space = strrchr(line, ' ');
            if (last_space != NULL) {
                strncpy(process_name, last_space + 1, PROCESS_NAME_SIZE - 1);
                process_name[PROCESS_NAME_SIZE - 1] = '\0';
                *last_space = '\0';
            }

            strncpy(pid, line, PID_SIZE - 1);
            pid[PID_SIZE - 1] = '\0';

            printf("Process: %s, PID: %s\n", process_name, pid);

            // Log proses
            log_process(user, pid, process_name, 0);
        }

        pclose(process_stream);
        sleep(SLEEP_TIME);
    }
}
```
<br /> <br />
**Fungsi main untuk mengambil input command dan menjalankan fitur yang sesuai**
```
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Command: %s <option> <user>\n", argv[0]);
        printf("Options:\n");
        printf(" -m : Monitor proses user\n");
        printf(" -s : Stop monitoring proses user\n");
        printf(" -c : Menghentikan proses user\n");
        printf(" -a : Menjalankan proses user\n");
        return EXIT_FAILURE;
    }

    char *option = argv[1];
    char *user = argv[2];

    // Switch case untuk setiap fitur yang bisa dijalankan
    switch(option[1]) {
        case 'm':
            log_process(user, "-", "-", 0);
            run_monitor(user);
            break;
        case 's':
            printf("Stop monitoring user's processes\n");
            log_process(user, "-", "-", 0);
            break;
        case 'c':
            printf("Block user's processes\n");
            log_process(user, "-", "-", 1);
            break;
        case 'a':
            printf("Allow user's processes\n");
            break;
        default:
            printf("Invalid option\n");
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```
<br /> <br /> Ketika dijalankan, kode akan memonitor proses pengguna <br />
![Screenshot 2024-04-20 022140](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/930655f0-669b-4a08-8384-2ca7fd96a1be)
<br /> <br />Kemudian disimpan ke dalam file log <br />
![Screenshot 2024-04-20 022257](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/dce96e2b-2a95-449d-9b2b-3a9624d7d829)
<br /> <br />Lalu jika menghentikan monitoring dan proses pengguna <br />
![Screenshot 2024-04-20 022420](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/f54e69f9-4f8b-4bdb-a6c0-b596fdf8e542)
![Screenshot 2024-04-20 022318](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/e1aee9ce-8311-496a-8f23-1d11cf724a6e)




