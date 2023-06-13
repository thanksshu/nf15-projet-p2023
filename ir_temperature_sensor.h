/*
 * Functions for communicating with MLX90614
 */

#include "includes.h"

/**
 * Initialise MLX90614 I2C communication
 */
void init_ir_temperature_sensor_communication();

/**
 * Measure temperature once
 * Return values:
 *   -1 for I2C error
 *   -2 for temperature error
 *   Other value are temperature measured
 */
double get_ir_sensor_temperature();

/**
 * Read sensor emissivity setting
 * Return values:
 *   -1 for I2C error
 *   Other value are temperature measured
 */
double get_ir_sensor_emissivity();

/**
 * Read sensor emissivity setting
 * Return values:
 *   -1 for clearing EPPROM stage I2C error
 *   -2 for setting EPPROM stage I2C error
 *   0 for success
 */
int set_ir_sensor_emissivity(double value);
