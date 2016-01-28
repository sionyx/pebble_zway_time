//
//  zway_location.h
//  pebble_time_zway
//
//  Created by Vadim Balashov
//
//

#pragma once

#include <pebble.h>
#include "zway_device.h"
#include "utarray.h"

typedef struct {
    uint8_t id;
    char *title;
    UT_array *devices;
} zway_location;
typedef zway_location* zway_location_ptr;

extern UT_icd zway_location_ut_icd;

zway_location_ptr zway_location_new(uint8_t id, char *title);
void zway_location_delete(void *location);

void zway_location_add_device(zway_location_ptr location, zway_device_ptr device);