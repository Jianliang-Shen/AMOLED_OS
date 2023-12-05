#include "bike.h"
#include "sky.h"
#include "sun.h"
#include "river.h"
#include "flower.h"
#include "fire.h"

const uint16_t *photos[] = {fire, flower, sky, river, bike, sun};

const uint8_t barcode_black[] = {3,5,3,1,2,3,1,2,4,2,2,2,2,1,3,
                                 2,1,4,2,2,2,2,1,4,5,1,4,1,1,2};

const uint8_t barcode_white[] = {2,1,1,1,3,1,1,1,1,1,1,2,2,2,1,
                                 2,2,2,1,1,2,2,1,1,1,1,1,3,1,1};
