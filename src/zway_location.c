//
//  zway_location.h
//  pebble_time_zway
//
//  Created by Vadim Balashov
//
//

#include <pebble.h>
#include "zway_location.h"
#include "str.h"


zway_location_ptr zway_location_new(uint8_t id, char *title) {
    zway_location_ptr self = malloc(sizeof(zway_location));
    self->id = id;
    self->title = strdup(title);
    self->devices = NULL;

    return self;
}

void zway_location_delete(void *location) {
    zway_location_ptr self = (zway_location_ptr)location;
    
    if (self->title)   free(self->title);
    if (self->devices) utarray_free(self->devices);
}

UT_icd zway_location_ut_icd = {sizeof(zway_location), NULL, NULL, zway_location_delete};

void zway_location_add_device(zway_location_ptr location, zway_device_ptr device) {
    if (!device) {
        return;
    }
    
    if (!location->devices) {
        utarray_new(location->devices, &zway_device_ut_icd);
    }
    
    utarray_push_back(location->devices, device);
}