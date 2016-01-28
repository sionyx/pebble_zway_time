//
//  protocol.h
//  pebble_time_zway
//
//  Created by Вадим Балашов on 12.11.15.
//
//

#pragma once
#include <pebble.h>

void protocol_init();
void send_device_level(char *device_id, char *level, bool exact);
