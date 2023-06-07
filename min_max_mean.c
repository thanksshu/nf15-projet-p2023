#include "includes.h"
double min(int length, double *temperatures)
{
    size_t i; // compter the number
    double temperature_min;

    temperature_min = *temperatures;

    for (i = 1; i < length; ++i)    // get the min temperature
    {
        if (*(temperatures + i) < temperature_min)
        {
            temperature_min = *(temperatures + i);
        }
    }
    return temperature_min; // return the value of the min temperature
}

double max(int length, double *temperatures)
{
    size_t i; // compter the number
    double temperature_max;

    temperature_max = *temperatures;

    for (i = 1; i < length; ++i)     // get the max temperature
    {
        if (*(temperatures + i) > temperature_max)
        {
            temperature_max = *(temperatures + i);
        }
    }
    return temperature_max; //return the value of the max temperature
}

double mean(int length, double *temperatures)
{
    size_t i; // compter the number
    double temperature_mean;
    double temperature_sum = 0;

    for (i = 0; i < length; ++i)    // get the mean temperature
    {
        temperature_sum = temperature_sum + *(temperatures + i);
        temperature_mean = temperature_sum / length; // calculate the value of the mean temperature
    }
    return temperature_mean; // get the value of the mean temperature
}

