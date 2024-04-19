# Sisop-2-2024-MH-IT23

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




