//
//  zway_operations.c
//  pebble_time_zway
//
//  Created by Вадим Балашов on 18.11.15.
//
//

#include <pebble.h>
#include "zway_operations.h"
#include "protocol.h"
#include "devices_window.h"

void zway_switchBinary_toggle(ZWayDevice *device) {
    if (strcmp(device->type, "switchBinary") == 0) {
        if (strcmp(device->level, "off") == 0) {
            strncpy(device->level, "on", 31);
        }
        else {
            strncpy(device->level, "off", 31);
        }
        
        send_device_command(device->id, device->level);
        update_devices_list();
    }
}