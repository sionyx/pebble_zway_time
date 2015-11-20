//
//  protocol.c
//  pebble_time_zway
//
//  Created by Вадим Балашов on 12.11.15.
//
//

#include "protocol.h"
#include <pebble.h>
#include "zway.h"
#include "devices_window.h"
#include "dialog_message_window.h"

enum {
    KEY_STATUS = 0,
    KEY_LOCATIONS_START = 1,
    KEY_LOCATIONS_END = 2,
    KEY_DEVICES_START = 3,
    KEY_DEVICES_END = 4,
    
    KEY_LOCATION_ID = 100,
    KEY_LOCATION_TITLE = 101,
    
    KEY_DEVICE_TYPE = 200,
    KEY_DEVICE_ID = 201,
    KEY_DEVICE_LOCATION = 202,
    KEY_DEVICE_LEVEL = 203,
    KEY_DEVICE_TITLE = 204,
    KEY_DEVICE_ICON = 205
};

enum {
    KEY_STATUS_AUTHENTICATE = 0,
    KEY_STATUS_LOADLOCATIONS = 1,
    KEY_STATUS_LOADDEVICES = 2,
    
    KEY_STATUS_UNKNOWNERROR = 100,
    KEY_STATUS_NOCONNECTION = 101,
    KEY_STATUS_AUTHFAILED = 102,
    KEY_STATUS_NEEDSETUP = 103
};


// APP MESSAGES
void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    // Location
    Tuple *id_tuple = dict_find(iterator, KEY_LOCATION_ID);
    Tuple *title_tuple = dict_find(iterator, KEY_LOCATION_TITLE);
    if (id_tuple && title_tuple) {
        zway_location_add(id_tuple->value->uint8, title_tuple->value->cstring);
        return;
    }

    // Device
    Tuple *dev_type_tuple = dict_find(iterator, KEY_DEVICE_TYPE);
    Tuple *dev_id_tuple = dict_find(iterator, KEY_DEVICE_ID);
    Tuple *dev_location_tuple = dict_find(iterator, KEY_DEVICE_LOCATION);
    Tuple *dev_level_tuple = dict_find(iterator, KEY_DEVICE_LEVEL);
    Tuple *dev_title_tuple = dict_find(iterator, KEY_DEVICE_TITLE);
    Tuple *dev_icon_tuple = dict_find(iterator, KEY_DEVICE_ICON);
    if (dev_type_tuple && dev_id_tuple && dev_location_tuple && dev_level_tuple && dev_title_tuple && dev_icon_tuple) {
        zway_device_add(dev_type_tuple->value->cstring,
                        dev_id_tuple->value->cstring,
                        dev_level_tuple->value->cstring,
                        dev_title_tuple->value->cstring,
                        dev_icon_tuple->value->uint8,
                        dev_location_tuple->value->uint8);
        return;
    }
    
    
    Tuple *t = dict_read_first(iterator);
    while(t != NULL) {
        switch(t->key) {
            case KEY_STATUS:
                switch (t->value->uint8) {
                    case KEY_STATUS_AUTHENTICATE:
                        show_message("Connecting...");
                        break;
                    case KEY_STATUS_LOADLOCATIONS:
                        show_message("Loading...");
                        break;
                    case KEY_STATUS_LOADDEVICES:
                        break;
                    case KEY_STATUS_UNKNOWNERROR:
                        dialog_message_window_push("Unknown error");
                        break;
                    case KEY_STATUS_NOCONNECTION:
                        dialog_message_window_push("No connection");
                        break;
                    case KEY_STATUS_AUTHFAILED:
                        dialog_message_window_push("Authorization failed");
                        break;
                    case KEY_STATUS_NEEDSETUP:
                        show_message("Please, configure ZWay app");
                        break;
                }
                break;
            case KEY_LOCATIONS_START:
                zway_locations_reset(t->value->uint8);
                break;
            case KEY_LOCATIONS_END:
                break;
            case KEY_DEVICES_START:
                zway_devices_reset(t->value->uint8);
                break;
            case KEY_DEVICES_END:
                update_devices_list();
                hide_message();
                break;
            default:
                APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
                break;
        }
        
        t = dict_read_next(iterator);
    }
}


static void inbox_dropped_callback(AppMessageResult reason, void *context) {
    dialog_message_window_push("Error during receive data from the phone");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
    dialog_message_window_push("Can't send data to the phone");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
}

void send_device_command(char *device_id, char *command) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    
    dict_write_cstring(iter, KEY_DEVICE_ID, device_id);
    dict_write_cstring(iter, KEY_DEVICE_LEVEL, command);
    
    app_message_outbox_send();
}

void protocol_init() {
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);
}
