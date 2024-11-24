// Created by fhr on 11/23/24.
#include "logger.h"

static int log_pipe[2];
static pid_t logger_pid = -1;
static int sequence_number = 0;
FILE *log_file = NULL;

int create_log_process() {
    sequence_number = 0;
    if (pipe(log_pipe) == -1) {
        perror("Failed to create pipe");
        return -1;
    }

    logger_pid = fork();

    if (logger_pid == -1) {
        perror("Failed to fork");
        return -1;
    }

    if (logger_pid > 0) {
        close(log_pipe[0]);
        return 0;
    }

    close(log_pipe[1]);

    log_file = fopen("gateway.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (1) {
        ssize_t bytes_read = read(log_pipe[0], buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            break;
        }

        buffer[bytes_read] = '\0';

        time_t now = time(NULL);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%a %b %d %H:%M:%S %Y", localtime(&now));

        fprintf(log_file, "%d - %s - %s\n", sequence_number++, timestamp, buffer);
        fflush(log_file);
    }
    fclose(log_file);
    exit(0);
}

int write_to_log_process(char *msg) {
    if (log_pipe[1] == -1) {
        fprintf(stderr, "Logger process not initialized.\n");
        return -1;
    }

    if (write(log_pipe[1], msg, strlen(msg)) == -1) {
        perror("Failed to write to log pipe");
        return -1;
    }

    if (write(log_pipe[1], "\n", 1) == -1) {
        perror("Failed to write newline to log pipe");
        return -1;
    }

    return 0;
}

int end_log_process() {
    if (logger_pid != -1) {
        close(log_pipe[1]);
        waitpid(logger_pid, NULL, 0);
    }
    return 0;
}
