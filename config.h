/*
 * config.h
 *
 * Configurations and predefined elements here
 */

#include "includes.h"

#ifndef CONFIG_H_
#define CONFIG_H_

/* Configuration */

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define PAGE_HEIGHT 8 // MUST be exactly divisible by the SCREEN_HEIGHT

/* End configuration */

#define PAGE_COUNT (SCREEN_HEIGHT / PAGE_HEIGHT)

#endif /* CONFIG_H_ */
