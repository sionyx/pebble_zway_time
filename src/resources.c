#include <pebble.h>
#include "resources.h"

#define RESOURCE_COUNT 8

static int resource_ids[RESOURCE_COUNT] = {
    RESOURCE_ID_UNKNOWN,
    RESOURCE_ID_BULB_OFF,
    RESOURCE_ID_BULB_ON,
    RESOURCE_ID_HUMIDITY,
    RESOURCE_ID_TEMPERATURE,
    RESOURCE_ID_BATTERY,
    RESOURCE_ID_MOTION_YES,
    RESOURCE_ID_LUMINOSITY
};
static GBitmap *images[RESOURCE_COUNT];

GBitmap *get_image(enum image_resource resource) {
    if ((int8_t)resource >= RESOURCE_COUNT) {
        return NULL;
    }
    
    if (!images[resource]) {
        int id = resource_ids[resource];
        images[resource] = gbitmap_create_with_resource(id);
    }
    
    return images[resource];
}

void release_image_resources() {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (images[i]) {
            gbitmap_destroy(images[i]);
            images[i] = NULL;
        }
    }
}