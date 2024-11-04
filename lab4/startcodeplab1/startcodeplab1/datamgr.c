//
// Created by fhr on 11/4/24.
//
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "datamgr.h"

#include "lib/dplist.h"

void *element_copy(void *element);
void element_free(void **element);
int element_compare(void *x, void *y);

typedef struct
{
    room_id_t room_id;
    sensor_id_t sensor_id;
    sensor_value_t sensor_value;
    sensor_ts_t sensor_ts;
} my_element_t;

void *element_copy(void *element)
{
    my_element_t *copy = malloc(sizeof(my_element_t));
    assert(copy != NULL);
    copy->room_id = ((my_element_t *)element)->room_id;
    copy->sensor_id = ((my_element_t *)element)->sensor_id;
    copy->sensor_value = ((my_element_t *)element)->sensor_value;
    copy->sensor_ts = ((my_element_t *)element)->sensor_ts;

    return (void *)copy;
}

void element_free(void **element)
{
    free(*element);
    *element = NULL;
}

int element_compare(void *x, void *y)
{
    my_element_t *elem1 = (my_element_t *)x;
    my_element_t *elem2 = (my_element_t *)y;
    if (elem1->room_id == elem2->room_id && elem1->sensor_id == elem2->sensor_id && elem1->sensor_value == elem2->sensor_value && elem1->sensor_ts == elem2->sensor_ts) {
        return 0;
    }
    return -1;
}

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){
    if (fp_sensor_map == NULL) {
        perror("Error opening fp_sensor_map file");
        return;
    }

    if (fp_sensor_data == NULL) {
        perror("Error opening fp_sensor_data file");
        return;
    }

    //create data_list to store all data
    dplist_t *data_list = dpl_create(element_copy, element_free,element_compare);

    int current_room_id = 0;
    int current_sensor_id = 0;
    while (fscanf(fp_sensor_map, "%d %d", &current_room_id, &current_sensor_id) == 2) {
        my_element_t * current_element = (my_element_t *)malloc(sizeof(my_element_t));
        current_element->room_id = current_room_id;
        current_element->sensor_id = current_sensor_id;
        dpl_insert_at_index(data_list,current_element, -1,false);
        printf("%d, %d\n", current_element->room_id,current_element->sensor_id);
    }

}

void datamgr_free(){

}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){

}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){

}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){

}

int datamgr_get_total_sensors(){

}
