//
//  str.c
//  pebble_time_zway
//
//  Created by Vadim Balashov on 26.01.16.
//
//

#include "str.h"
#include <pebble.h>

char *strdup(char *src) {
    size_t lenght = strlen(src) + 1;
    char *dup = malloc(sizeof(char) * lenght);
    strncpy(dup, src, lenght);
    return dup;
}