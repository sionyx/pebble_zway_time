//
//  devices_window.c
//  pebble_time_zway
//
//  Created by Вадим Балашов on 12.11.15.
//
//

#include <pebble.h>
#include "devices_window.h"
#include "resources.h"
#include "home_model.h"
#include "protocol.h"

Window *devices_window;
MenuLayer *menu_layer;
TextLayer *status_layer;

#ifdef PBL_PLATFORM_BASALT
StatusBarLayer *bar_layer;
#define MENU_HEIHGT 168
#endif

#ifdef PBL_PLATFORM_APLITE
#define STATUS_BAR_LAYER_HEIGHT 0
#define MENU_HEIHGT 152
#endif



void show_message(char *message) {
    if (!status_layer) {
        status_layer = text_layer_create(GRect(0, 68, 144, 30));
        text_layer_set_font(status_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
        text_layer_set_text_alignment(status_layer, GTextAlignmentCenter);
    }
    
    text_layer_set_text(status_layer, message);
    layer_add_child(window_get_root_layer(devices_window), text_layer_get_layer(status_layer));
}

void hide_message() {
    if (status_layer) {
        layer_remove_from_parent(text_layer_get_layer(status_layer));
        text_layer_destroy(status_layer);
        status_layer = NULL;
    }
}


uint16_t num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
    return home_locations_num();
}

int16_t header_height(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
    return 16;
}

void draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context) {
    zway_location_ptr location = home_location_at_index(section_index);
    char *title = (location) ? location->title : "";
    
    menu_cell_basic_header_draw(ctx, cell_layer, title);
}

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
    zway_device_ptr device = home_device_at_location(cell_index->section, cell_index->row);
    if (!device) {
        return;
    }
    
    GBitmap *icon;
    switch (device->icon) {
        case ICON_SWITCH:
            icon = (strcmp(device->level, "on") == 0)
                ? get_image(IMAGE_SWITCH_ON)
                : get_image(IMAGE_SWITCH_OFF);
            break;
        case ICON_HUMIDITY:
            icon = get_image(IMAGE_HUMIDITY);
            break;
        case ICON_TEMPERATURE:
            icon = get_image(IMAGE_TEMPERATURE);
            break;
        case ICON_BATTERY:
            icon = get_image(IMAGE_BATTERY);
            break;
        case ICON_MOTION:
            icon = get_image(IMAGE_MOTION);
            break;
        case ICON_LUMINOSITY:
            icon = get_image(IMAGE_LUMINOSITY);
            break;
        case ICON_SWITCH_MULTI:
            icon = get_image(IMAGE_SWITCH_MULTI);
            break;
        case ICON_SCENE:
            icon = get_image(IMAGE_SCENE);
            break;
        default:
            icon = get_image(IMAGE_UNKNOWN);
            break;
    }
    menu_cell_basic_draw(ctx, cell_layer, device->title, device->level, icon);
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
    return home_devices_num_at_location_index(section_index);
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
    bool activated = home_activate_device_at_location(cell_index->section, cell_index->row);
    if (activated) {
        vibes_short_pulse();
    }
}

void select_long_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
}


void update_devices_list() {
    menu_layer_reload_data(menu_layer);
}

void create_status_bar(Window *window) {
#ifdef PBL_PLATFORM_BASALT
    bar_layer = status_bar_layer_create();
    status_bar_layer_set_colors(bar_layer, GColorCobaltBlue, GColorWhite);
    layer_add_child(window_get_root_layer(window), status_bar_layer_get_layer(bar_layer));
#endif
}

void create_menu(Window *window) {
    menu_layer = menu_layer_create(GRect(0, STATUS_BAR_LAYER_HEIGHT, 144, MENU_HEIHGT));
    menu_layer_set_click_config_onto_window(menu_layer, window);
    
    MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)num_rows_callback,
        .get_num_sections = (MenuLayerGetNumberOfSectionsCallback)num_sections_callback,
        .get_header_height = (MenuLayerGetHeaderHeightCallback)header_height,
        .draw_header = (MenuLayerDrawHeaderCallback)draw_header_callback,
        .select_click = (MenuLayerSelectCallback)select_click_callback,
        .select_long_click = (MenuLayerSelectCallback)select_long_click_callback
    };
    menu_layer_set_callbacks(menu_layer, NULL, callbacks);
    
#ifdef PBL_COLOR
    menu_layer_set_normal_colors(menu_layer, GColorWhite, GColorDukeBlue);
    menu_layer_set_highlight_colors(menu_layer, GColorVividCerulean, GColorWhite);
#endif
    
    layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

void window_load(Window *window) {
    create_status_bar(window);
    create_menu(window);
}

void window_unload(Window *window) {
    hide_message();
    
    menu_layer_destroy(menu_layer);
#ifdef PBL_PLATFORM_BASALT
    status_bar_layer_destroy(bar_layer);
#endif
    
    window_destroy(window);
}

void window_appear(Window *window) {
    show_message("Waiting for phone...");
}

//#pragma mark - public

void devices_window_push() {
    devices_window = window_create();
    WindowHandlers handlers = {
        .load = window_load,
        .unload = window_unload,
        .appear = window_appear
    };
    window_set_window_handlers(devices_window, (WindowHandlers) handlers);
    window_stack_push(devices_window, true);
}