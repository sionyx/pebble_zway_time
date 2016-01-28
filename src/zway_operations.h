//
//  zway_operations.h
//  pebble_time_zway
//
//  Created by Vadim Balashov on 18.11.15.
//
//

#pragma once

#include "zway_device.h"

void zway_switchBinary_flip(zway_device_ptr device);
void zway_switchRGBW_flip(zway_device_ptr device);
void zway_toggleButton_activate(zway_device_ptr device);
void zway_switchMultilevel_flip(zway_device_ptr device);
