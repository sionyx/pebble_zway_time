#pragma once

enum image_resource {
    IMAGE_UNKNOWN = 0,
    IMAGE_SWITCH_OFF = 1,
    IMAGE_SWITCH_ON = 2,
    IMAGE_HUMIDITY = 3,
    IMAGE_TEMPERATURE = 4,
    IMAGE_BATTERY = 5,
    IMAGE_MOTION = 6,
    IMAGE_LUMINOSITY = 7,
    IMAGE_SWITCH_MULTI = 8,
    IMAGE_SCENE = 9
};

GBitmap *get_image(enum image_resource resource);
void release_image_resources();