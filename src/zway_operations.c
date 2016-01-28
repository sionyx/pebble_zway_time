//
//  zway_operations.c
//  pebble_time_zway
//
//  Created by Vadim Balashov on 18.11.15.
//
//

#include <pebble.h>
#include "str.h"
#include "zway_operations.h"

static char *zway_switch_level_on  = "on";
static char *zway_switch_level_off = "off";
static char *zway_toggle_active    = "on";
static char *zway_dimmer_on        = "99";
static char *zway_dimmer_off       = "0";

void zway_switchBinary_flip(zway_device_ptr device) {
    char *new_level = (strcmp(device->level, zway_switch_level_off) == 0)
        ? zway_switch_level_on
        : zway_switch_level_off;

    if (device->level) {
        free(device->level);
    }
    device->level = strdup(new_level);
}

void zway_switchRGBW_flip(zway_device_ptr device) {
    zway_switchBinary_flip(device);
}

void zway_toggleButton_activate(zway_device_ptr device) {
    if (device->level) {
        free(device->level);
    }
    device->level = strdup(zway_toggle_active);
}

void zway_switchMultilevel_flip(zway_device_ptr device) {
    char *new_level = (strcmp(device->level, zway_dimmer_off) != 0)
        ? zway_dimmer_off
        : zway_dimmer_on;

    if (device->level) {
        free(device->level);
    }
    device->level = strdup(new_level);
}