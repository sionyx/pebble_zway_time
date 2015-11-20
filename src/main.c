#include <pebble.h>
#include "resources.h"
#include "devices_window.h"
#include "protocol.h"

void init() {
    protocol_init();
    devices_window_push();
}

void deinit() {
    release_image_resources();
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}