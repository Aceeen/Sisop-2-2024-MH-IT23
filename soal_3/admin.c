#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LOG_PATH_MAX 100
#define COMMAND_SIZE 50
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

    fprintf(log_file, "[%s]-%s-%s-%s\n", timeString, pid, process_name, (status == 0) ? "JALAN" : "GAGAL");

    fclose(log_file);
}

void run_monitor(const char *user) {
    char command[COMMAND_SIZE];
    snprintf(command, sizeof(command), "ps -u %s", user);

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

            sscanf(line, "%s %s", pid, process_name);
            printf("Process: %s, PID: %s\n", process_name, pid);

            log_process(user, pid, process_name, 0);
        }

        pclose(process_stream);
        sleep(SLEEP_TIME);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <user>\n", argv[0]);
        printf("Options:\n");
        printf(" -m : Monitor user's processes\n");
        printf(" -s : Stop monitoring user's processes\n");
        printf(" -c : Block user's processes\n");
        printf(" -a : Allow user's processes\n");
        return EXIT_FAILURE;
    }

    char *option = argv[1];
    char *user = argv[2];

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