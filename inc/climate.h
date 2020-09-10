#ifndef _CLIMATE_H_
#define _CLIMATE_H_

#include "bme280.h"

int8_t climate_init(void);
int8_t climate_getData(void);

extern struct bme280_data climate_data;

#endif // _CLIMATE_H_