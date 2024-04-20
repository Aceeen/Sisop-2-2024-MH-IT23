#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_APPS 10
#define MAX_LINE 100

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
