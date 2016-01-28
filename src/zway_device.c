//
//  zway_device.c
//  pebble_time_zway
//
//  Created by Vadim Balashov on 26.01.16.
//
//

#include <pebble.h>
#include "zway_device.h"
#include "str.h"

zway_device_ptr zway_device_new(char *type, char *id, char *level, char *title, device_icon icon) {
    zway_device_ptr self = malloc(sizeof(zway_device));
    
    self->type = strdup(type);
    self->id = strdup(id);
    self->level = strdup(level);
    self->title = strdup(title);
    self->icon = icon;
    
    return self;
}

void zway_device_delete(void *device) {
    zway_device_ptr self = (zway_device_ptr)device;
    
    if (self->type)  free(self->type);
    if (self->id)    free(self->id);
    if (self->level) free(self->level);
    if (self->title) free(self->title);
}

UT_icd zway_device_ut_icd = {sizeof(zway_device), NULL, NULL, zway_device_delete};