//
//  zway_device.h
//  pebble_time_zway
//
//  Created by Vadim Balashov on 26.01.16.
//
//

#pragma once
#include "utarray.h"

typedef enum {
    ICON_UNKNOWN = 0,
    ICON_SWITCH = 1,
    ICON_HUMIDITY = 2,
    ICON_TEMPERATURE = 3,
    ICON_BATTERY = 4,
    ICON_MOTION = 5,
    ICON_LUMINOSITY = 6,
    ICON_SWITCH_MULTI = 7,
    ICON_SCENE = 8,
} device_icon;

typedef struct {
    char *type;
    char *id;
    char *level;
    char *title;
    device_icon icon;
} zway_device;
typedef zway_device* zway_device_ptr;

extern UT_icd zway_device_ut_icd;

zway_device_ptr zway_device_new(char *type, char *id, char *level, char *title, device_icon icon);
void zway_device_delete(void *device);