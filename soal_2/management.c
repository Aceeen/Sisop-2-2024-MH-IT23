#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <zlib.h>
#include <errno.h>


#define MAX_PATH_LEN 1024
#define MAX_FILENAME_LEN 256
#define LOG_FILE "history.log"
#define LIBRARY_FOLDER "library"
#define BACKUP_FOLDER "backup"
#define SIGRTMIN_VALUE 34

char mode = 'd';

void log_event(const char *source, const char *filename, const char *message);
void decrypt_filename(char *filename);

// Fungsi callback
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    fflush(stream);
    return written;
}

// Fungsi download dari URL
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

// Fungsi unzip
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

void handle_signal(int signum) {
    switch (signum) {
        case SIGRTMIN_VALUE:
            mode = 'd'; // Default
            break;
        case SIGUSR1:
            mode = 'b'; // Backup
            break;
        case SIGUSR2:
            mode = 'r'; // Restore
            break;
    }
}

void download_and_extract() {
    const char *url = "https://drive.google.com/uc?id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup&export=download";
    const char *filename = "library.zip";
    if (download_file(url, filename) == 0) {
        unzip_file(filename);
    }
}

// Dekripsi ROT19
void decrypt_filename(char *filename) {
    for (int i = 0; i < strlen(filename); i++) {
        if (filename[i] >= 'a' && filename[i] <= 'z') {
            filename[i] = 'a' + (filename[i] - 'a' + 19) % 26;
        } else if (filename[i] >= 'A' && filename[i] <= 'Z') {
            filename[i] = 'A' + (filename[i] - 'A' + 19) % 26;
        }
    }
}

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

// Backup
void move_to_backup(char *filename) {
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