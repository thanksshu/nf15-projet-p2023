/*
 * Function for get the valeur min max mean of the  temperature
 */

#ifndef MIN_MAX_MEAN_H_
#define MIN_MAX_MEAN_H_

#include "includes.h"

double min(int length, double *temperatures);
double max(int length, double *temperatures);
double mean(int length, double *temperatures);

#endif /* MIN_MAX_MEAN_H_ */
