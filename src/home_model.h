//
//  home_model.h
//  pebble_time_zway
//
//  Created by Vadim Balashov on 26.01.16.
//
//

#pragma once
#include "zway_location.h"
#include "zway_device.h"

void home_locations_reset(uint8_t new_locations_count);
void home_location_add(uint8_t id, char *title);
void home_locations_remove_empty();
uint8_t home_locations_num();

void home_device_add(char *type, char *id, char *level, char *title, device_icon icon, uint8_t location_id);
uint8_t home_devices_num_at_location_index(uint8_t index);

zway_location_ptr home_location_at_index(uint8_t index);
zway_device_ptr   home_device_at_location(uint8_t location_index, uint8_t device_index);

bool home_activate_device_at_location(uint8_t location_index, uint8_t device_index);