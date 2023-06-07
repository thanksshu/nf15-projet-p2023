#include "includes.h"
double min(int length, double *temperatures)
{
    size_t i; // compter the number
    double temperature_min;

    temperature_min = *temperatures;

    // get the min temperature

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
    size_t i; // compter the number
    double temperature_max;

    temperature_max = *temperatures;

    // get the max temperature

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
    size_t i; // compter the number
    double temperature_mean;
    double temperature_sum = 0;

    // get the mean temperature

    for (i = 0; i < length; ++i)
    {
        temperature_sum = temperature_sum + *(temperatures + i);
        temperature_mean = temperature_sum / length;
    }
    return temperature_mean;
}


