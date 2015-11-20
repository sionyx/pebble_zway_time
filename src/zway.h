#pragma once

typedef enum {
    ICON_UNKNOWN = 0,
    ICON_SWITCH = 1,
    ICON_HUMIDITY = 2,
    ICON_TEMPERATURE = 3,
    ICON_BATTERY = 4,
    ICON_MOTION = 5,
    ICON_LUMINOSITY = 6,
} device_icon;

typedef struct {
    uint8_t id;
    char title[32];
} ZWayLocation;

typedef struct {
    char type[32];
    char id[32];
    char level[32];
    char title[32];
    device_icon icon;
    uint8_t location;
} ZWayDevice;

void zway_locations_reset(uint8_t new_locations_count);
void zway_devices_reset(uint8_t new_devices_count);

void zway_location_add(uint8_t id, char *title);
void zway_device_add(char *type, char *id, char *level, char *title, device_icon icon, uint8_t location);

uint8_t zway_locations_num();
uint8_t zway_devices_num(uint8_t location);

ZWayLocation *zway_location(uint8_t location);
ZWayDevice *zway_device(uint8_t location, uint8_t device);