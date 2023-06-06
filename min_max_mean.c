#include "includes.h"
double min(int length, double *temperature)
{
    int i; // compter the number
    double temperature_min;

    temperature_min = *temperature;

    // get the min temperature

    for (i = 1; i < length; ++i)
    {
        if (*(temperature + i) < temperature_min)
        {
            temperature_min = *(temperature + i);
        }
    }
    return temperature_min;
}

double max(int length, double *temperature)
{
    int i; // compter the number
    double temperature_max;

    temperature_max = *temperature;

    // get the max temperature

    for (i = 1; i < length; ++i)
    {
        if (*(temperature + i) > temperature_max)
        {
            temperature_max = *(temperature + i);
        }
    }
    return temperature_max;
}

double mean(int length, double *temperature)
{
    int i; // compter the number
    double temperature_mean;
    double temperature_sum = 0;

    // get the mean temperature

    for (i = 0; i < length; ++i)
    {
        temperature_sum = temperature_sum + *(temperature + i);
        temperature_mean = temperature_sum / length;
    }
    return temperature_mean;
}


