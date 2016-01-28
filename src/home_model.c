//
//  home_model.c
//  pebble_time_zway
//
//  Created by Vadim Balashov on 26.01.16.
//
//

#include "home_model.h"
#include "zway_operations.h"

#include "protocol.h"
#include "devices_window.h"

static UT_array *locations = NULL;

void home_locations_reset(uint8_t new_locations_count) {
    if (locations) {
        utarray_clear(locations);
    }
    else {
        utarray_new(locations, &zway_location_ut_icd);
    }
    utarray_reserve(locations, new_locations_count);
}

void home_location_add(uint8_t id, char *title) {
    utarray_push_back(locations, zway_location_new(id, title));
}

void home_locations_remove_empty() {
    bool has_empty = true;
    
    while (has_empty) {
        zway_location_ptr location = NULL;
        while((location = (zway_location_ptr)utarray_next(locations, location))) {
            if (!location->devices) {
                break;
            }
        }
        
        if (location) {
            signed pos = utarray_eltidx(locations, location);
            if (pos >= 0) {
                utarray_erase(locations, (unsigned)pos, 1);
            }
        }
        else {
            has_empty = false;
        }
    }
}

uint8_t home_locations_num() {
    return (locations) ? utarray_len(locations) : 0;
}

zway_location_ptr home_location_with_id(uint8_t id) {
    zway_location_ptr location = NULL;
    while((location = (zway_location_ptr)utarray_next(locations, location))) {
        if (location->id == id) {
            return location;
        }
    }
    return NULL;
}

void home_device_add(char *type, char *id, char *level, char *title, device_icon icon, uint8_t location_id) {
    zway_location_ptr location = home_location_with_id(location_id);
    if (!location) {
        return;
    }
    
    zway_location_add_device(location, zway_device_new(type, id, level, title, icon));
}

uint8_t home_devices_num_at_location_index(uint8_t index) {
    zway_location_ptr location = home_location_at_index(index);
    return (location && location->devices) ? utarray_len(location->devices) : 0;
}


zway_location_ptr home_location_at_index(uint8_t index) {
    return (locations) ? (zway_location_ptr)utarray_eltptr(locations, index) : NULL;
}

zway_device_ptr home_device_at_location(uint8_t location_index, uint8_t device_index) {
    zway_location_ptr location = home_location_at_index(location_index);
    
    if (!location) {
        return NULL;
    }
    
    return (zway_device_ptr)utarray_eltptr(location->devices, device_index);
}

bool home_activate_device_at_location(uint8_t location_index, uint8_t device_index) {
    zway_device_ptr device = home_device_at_location(location_index, device_index);
    if (!device) {
        return false;
    }
    
    bool exact_level = false;
    if (strcmp(device->type, "switchBinary") == 0) {
        zway_switchBinary_flip(device);
    }
    else if (strcmp(device->type, "switchRGBW") == 0) {
        zway_switchRGBW_flip(device);
    }
    else if (strcmp(device->type, "toggleButton") == 0) {
        zway_toggleButton_activate(device);
    }
    else if (strcmp(device->type, "switchMultilevel") == 0) {
        zway_switchMultilevel_flip(device);
        exact_level = true;
    }
    else {
        return false;
    }

    send_device_level(device->id, device->level, exact_level);
    update_devices_list();
    return true;
}