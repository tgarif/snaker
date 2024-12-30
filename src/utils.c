#include "utils.h"

#include <stdlib.h>

float RandomFloat(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}
