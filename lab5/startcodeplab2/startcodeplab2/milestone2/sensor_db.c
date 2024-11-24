//
// Created by fhr on 11/23/24.
//
#include "sensor_db.h"
//
// Created by fhr on 11/21/24.
//
#include "sensor_db.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>


FILE * open_db(char * filename, bool append){
    FILE * file;
    if (append) {
        file = fopen(filename, "a");
    } else {
        file = fopen(filename, "r+");
    }
    if (file == NULL) {
        return NULL;
    }
    create_log_process();
    write_to_log_process("Data file opened.");
    return file;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if (f == NULL){
        return -1;
    }
    write_to_log_process("Data inserted.");

    int result = fprintf(f, "%d, %.6f, %ld\n", id, value, ts);

    if (result < 0) {
        return result;
    }

    return 0;
}

int close_db(FILE * f){
    if (f == NULL){
        return -1;
    }
    write_to_log_process("Data file closed.");
    end_log_process();
    return fclose(f);
}