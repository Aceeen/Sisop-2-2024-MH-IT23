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


### SOAL 1

Pada soal ini diminta untuk membuat program :
1. Input Path: Program harus dapat menerima input berupa lokasi direktori (argv) yang menentukan folder di mana file-file akan dieksekusi atau dipindai.
2. Replace String: Program harus mencari dan mengganti string tertentu dalam file dengan string lainnya sesuai aturan berikut:
- String "m4LwAr3" diganti dengan "[MALWARE]"
- String "5pYw4R3" diganti dengan "[SPYWARE]"
- String "R4nS0mWaR3" diganti dengan "[RANSOMWARE]"
3. Daemon Process: Program harus berjalan sebagai daemon, yang berarti ia beroperasi di background secara terus-menerus tanpa intervensi pengguna.
4. Tanpa Command system(): Dalam pembuatan program, tidak diizinkan menggunakan command system() yang sering digunakan untuk menjalankan perintah shell.
5. Interval Waktu: Program harus memiliki jeda operasi selama 15 detik antara setiap eksekusi tugasnya.
6. Logging: Program harus mencatat setiap penggantian string yang berhasil dilakukan dalam file dengan nama "virus.log" dan dengan format catatan log yang spesifik: [dd-mm-YYYY][HH:MM:SS] Suspicious string at <file_name> successfully replaced!

## SOAL

1. Fungsi replaceStringInFile mencari tiga string spesifik dalam file ("m4LwAr3", "5pYw4R3", "R4nS0mWaR3") dan menggantinya dengan string yang lebih jelas menunjukkan jenis malware ("[MALWARE]", "[SPYWARE]", "[RANSOMWARE]"). Fungsi replaceStringInFile bertanggung jawab untuk membuka file, mencari dan mengganti string, serta menulis kembali isi yang sudah diubah ke dalam file yang sama.Fungsi ini juga mengelola sumber daya seperti alokasi memori dan penutupan file dengan tepat.
   
```sh
// Function to perform string replacement in a file
void replaceStringInFile(const char *filePath, const char *searchString, const char *replaceString) {
    FILE *file = fopen(filePath, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    size_t searchLen = strlen(searchString);
    size_t replaceLen = strlen(replaceString);

    // Allocate memory for a temporary buffer to store the file contents
    char fileContent[MAX_FILE_LENGTH];
    char *tempBuffer = malloc(MAX_FILE_LENGTH);
    if (tempBuffer == NULL) {
        fclose(file);
        perror("Memory allocation failed");
        return;
    }

    // Read the file content into memory
    size_t bytesRead = fread(fileContent, 1, MAX_FILE_LENGTH, file);
    if (bytesRead == 0) {
        fclose(file);
        free(tempBuffer);
        perror("Error reading file");
        return;
    }

    // Perform string replacement
    char *ptr = fileContent;
    while ((ptr = strstr(ptr, searchString)) != NULL) {
        // Calculate the position of the found string
        size_t pos = ptr - fileContent;
        // Copy the content before the found string to the temporary buffer
        memcpy(tempBuffer, fileContent, pos);
        // Copy the replacement string to the temporary buffer
        memcpy(tempBuffer + pos, replaceString, replaceLen);
        // Copy the remaining content after the found string to the temporary buffer
        memcpy(tempBuffer + pos + replaceLen, ptr + searchLen, bytesRead - (pos + searchLen));
        // Copy the modified content back to the fileContent buffer
        memcpy(fileContent, tempBuffer, bytesRead);
        // Move the pointer past the replaced string
        ptr += replaceLen;
    }

    // Truncate the file to remove the old content
    ftruncate(fileno(file), 0);
    // Move the file pointer to the beginning
    fseek(file, 0, SEEK_SET);
    // Write the modified content back to the file
    fwrite(fileContent, 1, bytesRead, file);

    // Cleanup
    fclose(file);
    free(tempBuffer);
}
```
2. Lalu, setiap kali string berhasil diganti dalam file, fungsi logReplacement mencatat aksi tersebut dalam file "virus.log" dengan timestamp dan nama file yang dimodifikasi.
   
```sh
// Function to log string replacements in virus.log file
void logReplacement(const char *fileName) {
    time_t now;
    struct tm *timeinfo;
    char timestamp[20];
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "[%d-%m-%Y][%H:%M:%S]", timeinfo);

    // Open virus.log in append mode
    FILE *logFile = fopen(LOG_FILE, "a");
    if (logFile == NULL) {
        perror("Error opening log file");
        return;
    }

    fprintf(logFile, "%s Suspicious string at %s successfully replaced!\n", timestamp, fileName);
    fclose(logFile);
}
```
3. Fungsi pada main ini berfungsi untuk
- Program membuka direktori yang ditunjukkan oleh argumen yang diberikan.
- Melakukan penggantian string pada setiap file reguler (selain file dengan ekstensi .c untuk menghindari modifikasi kode sumber).
- Mencatat penggantian string ke log.
- Daemon mengambil alih, program akan berpindah ke direktori root, menutup file descriptor standar (stdin, stdout, stderr), dan berjalan di latar belakang.
  
```sh
int main(int argc, char *argv[]) {
    // Ensure correct command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <folder_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Perform string replacement in files within the specified folder
    DIR *dir;
    struct dirent *entry;
    dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    char filePath[MAX_PATH_LENGTH];
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && !strstr(entry->d_name, ".c")) { // Skip .c files
            snprintf(filePath, sizeof(filePath), "%s/%s", argv[1], entry->d_name);
            replaceStringInFile(filePath, "m4LwAr3", "[MALWARE]");
            replaceStringInFile(filePath, "5pYw4R3", "[SPYWARE]");
            replaceStringInFile(filePath, "R4nS0mWaR3", "[RANSOMWARE]");
            logReplacement(entry->d_name);
        }
    }
    closedir(dir);
```
4. Setelah menjadi daemon, program tidak melakukan aktivitas tambahan dalam loopnya, hanya tidur selama 15 detik antar iterasi.
   
```sh
    // Implement daemon
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

    // Main loop to run the program as a daemon with a 15-second interval
    while (1) {
        // Tasks to be performed in the main loop go here
        sleep(15); // Wait for 15 seconds
    }

    return EXIT_SUCCESS;
}
```
### Dokumentasi ketika program di jalankan

![Screenshot 2024-04-24 100903](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/151058945/cbbe940e-9d09-4e43-b381-9876c5858e73)

![Screenshot 2024-04-24 101004](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/151058945/b982ea8e-12a1-4c2b-8cd0-4ff2a99b14e1)

### REVISI SOAL NO 1

Saat program dijalankan, ia akan terus-menerus memonitoring jalur file yang telah ditentukan oleh pengguna. Proses ini berlangsung secara berkelanjutan sehingga meskipun pada awalnya tidak terdapat string yang mencurigakan di dalam file yang ada di jalur tersebut, program tetap akan melakukan pemeriksaan secara berkala. Dengan demikian, ketika terdapat file baru yang dimasukkan ke dalam jalur tersebut dan file tersebut mengandung string yang dicurigai, program akan segera mendeteksi dan melakukan modifikasi yang diperlukan pada file tersebut secara otomatis, tanpa perlu ada intervensi untuk menjalankan ulang program. Ini memastikan bahwa setiap perubahan yang mencurigakan dapat segera ditanggapi dan ditangani dengan cepat.

Di gambar ini terdapat dua file txt, yang sebelumnya sudah saya cek berubah stringnya yaitu isifile.txt kemudian saya memasukkan file baru lagi yang bernama filefile.txt. File tersebut mengandung string yang dicurigai, program akan segera mendeteksi dan melakukan modifikasi yang diperlukan pada file tersebut secara otomatis, tanpa perlu ada intervensi untuk menjalankan ulang program. 

![SISOPREVISIII](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/151058945/070503d6-70e7-4262-adce-e6b45e373252)

![SISOPREVISIDUA](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/151058945/b9e38b2e-1327-4e11-ab41-da3fd5b9bfab)


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

## REVISI SOAL NO 3
Sebelumnya, command -c untuk menghentikan proses yang berjalan belum dapat tereksekusi dengan benar. Program sudah direvisi agar fungsi -c dapat berjalan dengan benar <br />
![Screenshot 2024-04-27 210917](https://github.com/Aceeen/Sisop-2-2024-MH-IT23/assets/150018995/daa4452f-91ae-41f4-988a-6f69820d1d8e)

Kode setelah direvisi
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define LOG_PATH_MAX 100
#define COMMAND_SIZE 200
#define LINE_SIZE 200
#define PID_SIZE 10
#define PROCESS_NAME_SIZE 50
#define SLEEP_TIME 1

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

void run_monitor(const char *user) {
    char command[COMMAND_SIZE];
    snprintf(command, sizeof(command), "ps -u %s o pid,comm", user);

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

            char *last_space = strrchr(line, ' ');
            if (last_space != NULL) {
                strncpy(process_name, last_space + 1, PROCESS_NAME_SIZE - 1);
                process_name[PROCESS_NAME_SIZE - 1] = '\0';
                *last_space = '\0';
            }

            strncpy(pid, line, PID_SIZE - 1);
            pid[PID_SIZE - 1] = '\0';

            printf("Process: %s, PID: %s\n", process_name, pid);

            log_process(user, pid, process_name, 0);
        }

        pclose(process_stream);
        sleep(SLEEP_TIME);
    }
}


void stop_running_processes(const char *user) {
    char command[COMMAND_SIZE];
    snprintf(command, sizeof(command), "ps -u %s -o pid,state,comm | grep -w R | awk '{print $1}'", user);

    FILE *process_stream = popen(command, "r");
    if (process_stream == NULL) {
        perror("Error opening process stream");
        exit(EXIT_FAILURE);
    }

    char pid[PID_SIZE];
    while (fgets(pid, sizeof(pid), process_stream) != NULL) {
        pid[strlen(pid) - 1] = '\0'; 
        printf("Stopping process with PID: %s\n", pid);
        kill(atoi(pid), SIGSTOP); 
    }

    pclose(process_stream);
}


void resume_stopped_processes(const char *user) {
    char command[COMMAND_SIZE];
    snprintf(command, sizeof(command), "ps -u %s -o pid,state,comm | grep -w T | awk '{print $1}'", user);

    FILE *process_stream = popen(command, "r");
    if (process_stream == NULL) {
        perror("Error opening process stream");
        exit(EXIT_FAILURE);
    }

    char pid[PID_SIZE];
    while (fgets(pid, sizeof(pid), process_stream) != NULL) {
        pid[strlen(pid) - 1] = '\0'; 
        printf("Resuming process with PID: %s\n", pid);
        kill(atoi(pid), SIGCONT); 
    }

    pclose(process_stream);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Command: %s <fitur> <user>\n", argv[0]);
        printf("Fitur:\n");
        printf(" -m : Monitor proses user\n");
        printf(" -s : Stop monitoring proses user\n");
        printf(" -c : Menghentikan proses user\n");
        printf(" -a : Menjalankan proses user\n");
        return EXIT_FAILURE;
    }

    char *fitur = argv[1];
    char *user = argv[2];

    switch (fitur[0]) {
        case '-':
            switch (fitur[1]) {
                case 'm':
                    printf("Monitoring proses user\n");
                    log_process(user, "-", "-", 0);
                    run_monitor(user);
                    break;
                case 's':
                    printf("Stop monitoring proses user\n");
                    log_process(user, "-", "-", 0);
                    break;
                case 'c':
                    printf("Menghentikan proses user\n");
                    log_process(user, "-", "-", 1);
                    stop_running_processes(user);
                    break;
                case 'a':
                    printf("Menjalankan proses user\n");
                    log_process(user, "-", "-", 0);
                    resume_stopped_processes(user);
                    break;
                default:
                    printf("Invalid option\n");
                    return EXIT_FAILURE;
            }
            break;
        default:
            printf("Invalid option\n");
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```


## SOAL 4
Pada soal ini diminta untuk membuat program otomatisasi aplikasi
```
void open_apps(char *apps[], int num_apps, int counts[]) {
    for (int i = 0; i < num_apps; i++) {
        for (int j = 0; j < counts[i]; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                char *args[] = {apps[i], NULL};
                execvp(apps[i], args);
                exit(0);
            }
        }
    }
}
```
Fungsi ini akan membantu untuk membuka aplikasi yang diinginkan serta jumlah window yang diinginkan dari aplikasi tersebut dengan menggunakan command
```
./setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>
```
Dengan command tersebut maka aplikasi yang ingin dibuka akan berjalan sesuai command seperti ini
![image](https://github.com/v0rein/Sisop-2-2024-MH-IT23/assets/143814923/be5e31ef-b24c-4f86-9d65-73aacecacd48)

```
void spawn_process(char *app) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {app, NULL};
        execvp(app, args);
        exit(0);
    }
}

void read_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    char app[MAX_APPS];
    int num_apps;

    while (fscanf(file, "%s %d", app, &num_apps)==2) {
        for (int j = 0; j < num_apps; j++) {
            spawn_process(app);
        }
    }
}
```
Lalu, kita membuat fungsi baru untuk membaca '.conf file' yang berisi nama aplikasi serta jumlah window aplikasi yang ingin dibuka. Kita dapat melakukan command
```
./setup -f file.conf
```
Maka akan muncul aplikasi - aplikasi yang telah kita tulis di '.conf file' seperti ini 
![image](https://github.com/v0rein/Sisop-2-2024-MH-IT23/assets/143814923/30ffbdb6-74a1-42a1-b7c0-0405ce71b6ee)

```
void close_apps_by_name(char **argv, char *app_name) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"pkill", "-9", "-f", app_name, NULL};
        execvp("pkill", args);
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void close_all_apps(char **argv) {
    close_apps_by_name(argv, "firefox");
    close_apps_by_name(argv, "wireshark");
}
```
Kemudian kita membuat fungsi agar dapat menutup file yang telah kita buka melalui command `./setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>`
Kita dapat mengakses fungsi ini dengan command 
```
./setup -k
```
```
void close_apps_from_config(char *config_file, char **argv) {
    FILE *file = fopen(config_file, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", config_file);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file)) {
        char app[MAX_LINE];
        if (sscanf(line, "%s", app) == 1) {
            pid_t pid = fork();
            if (pid == 0) {
                char *args[] = {"pkill", "-9", app, NULL};
                execvp("pkill", args);
                exit(0);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    fclose(file);
}
```
Lalu kita juga membuat fungsi untuk menutup aplikasi yang telah kita buka melalui '.conf file' yang dapat diakses dengan command
```
./setup -k file.conf
```
Lalu untuk memanggil fungsi - fungsi yang telah kita buat sebelumnya kita membuat fungsi main yang akan tertulis seperti ini
```
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s -o <app1> <num1> <app2> <num2>... <appN> <numN>\n", argv[0]);
        printf("   or: %s -f <file.conf>\n", argv[0]);
        printf("   or: %s -k [<file.conf>]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-o") == 0) {
        if (argc % 2 != 0) {
            printf("Error: Invalid arguments\n");
            return 1;
        }
        int num_apps = (argc - 2) / 2;
        char *apps[MAX_APPS];
        int counts[MAX_APPS];
        for (int i = 0; i < num_apps; i++) {
            apps[i] = argv[2 + i * 2];
            counts[i] = atoi(argv[3 + i * 2]);
        }
        open_apps(apps, num_apps, counts);
    } else if (strcmp(argv[1], "-f") == 0) {
        if (argc != 3) {
            printf("Error: Invalid arguments\n");
            return 1;
        }
        read_config(argv[2]);
    } else if (strcmp(argv[1], "-k") == 0) {
    if (argc == 2) {
        close_all_apps(argv);
    } else if (argc == 3) {
        close_apps_from_config(argv[2], argv);
    } else {
        printf("Error: Invalid arguments\n");
        return 1;
    }
} else {
    printf("Error: Invalid option\n");
    return 1;
}
    return 0;
}
```
Selain untuk memanggil fungsi - fungsi tersebut kita juga mengatur bagaimana fungsi tersebut dipanggil melalui command yang telah diinginkan serta kita juga membuat beberapa error message apabila ada kesalahan dalam menulis command
