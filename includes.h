/*
 * includes.h
 *
 * All includes needed
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

/* Include driver */
#include <ir_temperature_sensor.h>
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Include standard library */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Include user defined */
#include "config.h"
#include "physical_screen.h"
#include "screen_controller.h"
#include "ir_temperature_sensor.h"
#include "image_bitmaps/image_bitmaps.h"
#include "glyph_bitmaps/glyph_bitmaps.h"
#include "ui.h"

#endif /* INCLUDES_H_ */
