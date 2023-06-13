#include "includes.h"

double min(int length, double *temperatures)
{
    size_t i;
    double temperature_min;

    temperature_min = *temperatures;

    for (i = 1; i < length; ++i)
    {
        if (*(temperatures + i) < temperature_min)
        {
            temperature_min = *(temperatures + i);
        }
    }
    return temperature_min;
}

double max(int length, double *temperatures)
{
    size_t i;
    double temperature_max;

    temperature_max = *temperatures;

    for (i = 1; i < length; ++i)
    {
        if (*(temperatures + i) > temperature_max)
        {
            temperature_max = *(temperatures + i);
        }
    }
    return temperature_max;
}

double mean(int length, double *temperatures)
{
    size_t i;
    double temperature_mean;
    double temperature_sum = 0;

    for (i = 0; i < length; ++i)
    {
        temperature_sum = temperature_sum + *(temperatures + i);
        temperature_mean = temperature_sum / length; // Calculate the value of the mean temperature
    }
    return temperature_mean;
}

