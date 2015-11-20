//
//  protocol.h
//  pebble_time_zway
//
//  Created by Вадим Балашов on 12.11.15.
//
//

#pragma once

void protocol_init();
void send_device_command(char *device_id, char *command);