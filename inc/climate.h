#ifndef _CLIMATE_H_
#define _CLIMATE_H_

#include "bme280.h"

typedef struct {
    struct bme280_dev dev;
    struct bme280_data comp_data;
    uint8_t dev_addr;
    uint32_t req_delay;
} str_climate_t;

int climate_init(void);
int climate_getData(void);

extern str_climate_t climate_info;

#endif // _CLIMATE_H_