#include <pebble.h>
#include "zway.h"

#define MAX_LOCATIONS 8
#define MAX_DEVICES 32

static ZWayLocation s_locations[MAX_LOCATIONS];
static uint8_t s_locations_num = 0;

static ZWayDevice s_devices[8];
static uint8_t s_devices_num = 0;

void zway_locations_reset(uint8_t new_locations_count) {
    s_locations_num = 0;
}

void zway_devices_reset(uint8_t new_devices_count) {
    s_devices_num = 0;
}


void zway_location_add(uint8_t id, char *title) {
    if (s_locations_num == MAX_LOCATIONS - 1) {
        return;
    }
    
    s_locations[s_locations_num].id = id;
    strncpy(s_locations[s_locations_num].title, title, 31);
    s_locations_num++;
}

void zway_device_add(char *type, char *id, char *level, char *title, device_icon icon, uint8_t location) {
    if(s_devices_num == MAX_DEVICES) {
        return;
    }
    
    if (s_locations_num == 0) {
        zway_location_add(0, "Devices");
    }
    
    strncpy(s_devices[s_devices_num].type, type, 31);
    strncpy(s_devices[s_devices_num].id, id, 31);
    strncpy(s_devices[s_devices_num].level, level, 31);
    strncpy(s_devices[s_devices_num].title, title, 31);
    
    s_devices[s_devices_num].location = location;
    s_devices[s_devices_num].icon = icon;
    s_devices_num++;
}


uint8_t zway_locations_num() {
    return s_locations_num;
}

uint8_t zway_devices_num(uint8_t location) {
    if (s_devices_num == 0 || s_locations_num == 0) return 0;
    
    uint8_t num = 0;
    uint8_t location_id = s_locations[location].id;
    for (uint8_t i = 0; i < s_devices_num; i++) {
        if (s_devices[i].location == location_id)
            num++;
    }
    
    return num;
}

ZWayLocation *zway_location(uint8_t location) {
    return &s_locations[location];
}

ZWayDevice *zway_device(uint8_t location, uint8_t device) {
    if (s_devices_num == 0 || s_locations_num == 0) return NULL;

    uint16_t index = 0;
    uint8_t location_id = s_locations[location].id;
    for (uint16_t i = 0, num = 0; i < s_devices_num; i++) {
        if (s_devices[i].location == location_id) {
            if (num == device) {
                index = i;
                break;
            }
            num++;
        }
    }
    
    return &s_devices[index];
}