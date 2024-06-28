#pragma once

#include <math.h>

#define GRAPH "\xFE"
#define GRAPH_HZ_10 "\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4"
#define GRAPH_HZ_3 "\xC4\xC4\xC4"
#define NUM_DIGITS 9
#define TO_PERCENT 100
#define TO_50_PERCENT 50

//ALPHABETICALLY
/*
Method Name: calc_actl_frequency()
Purpose: calculates the actual frequency for NB Law by dividing one first digit by the total number of first digits
Accepts: frequency array, index of a number, sum of all first digits
Returns: double that represents actual frequency in %
*/
double calc_actl_frequency(unsigned * frequencyArray, unsigned numIndex, double sumOfFirstDigits)
{
	return ((double)frequencyArray[numIndex] / sumOfFirstDigits) * 100;
}
/*
Method Name: calc_exp_frequency()
Purpose: calculates the expected frequency for NB Law using the NB formula
Accepts: size_t digit
Returns: double that represents expected frequency in %
*/
double calc_exp_frequency(size_t digit)
{
	return log10(1 + 1 / (double)digit) * TO_PERCENT;
}
/*
Method Name: calc_mean()
Purpose: calculates an arithmetic mean of elements of the array
Accepts: an array and its size
Returns: an arithmetic mean of the array elements
*/
double calc_mean(double * data, size_t size)
{
	double sum = 0;
	for (unsigned i = 0; i < size; ++i)
		sum += data[i];
	return sum / size;
}
/*
Method Name: calc_median()
Purpose: calculates a statistical median value of the array elements
Accepts: an array and its size
Returns: median of the array elements
*/
double calc_median(double * data, size_t size)
{
	if (size % 2 != 0)
		return (data[size / 2]);
	else
		return ((data[size / 2 - 1] + data[size / 2]) / 2);
}
/*
Method Name: calc_NB_variance()
Purpose: calculates Newcomb-Benford variance
Accepts: 2 arrays for actual and expected frequencies
Returns: nothing
*/
double calc_NB_variance(double *actualFrequencies, double *expectedFrequencies)
{
	double sum = 0;
	for (unsigned i = 0; i < NUM_DIGITS; ++i)
	{
		double difference = actualFrequencies[i] / expectedFrequencies[i] - 1;
		difference *= difference;
		sum += difference;
	}
	return (sum / NUM_DIGITS) * 100;
}
/*
Method Name: calc_std_deviation()
Purpose: calculates the standard deviation of elements (will also be used for NB deviation)
Accepts: double variance
Returns: double that represents standard deviation
*/
double calc_std_deviation(double variance)
{
	return sqrt(variance);
}
/*
Method Name: calc_variance()
Purpose: calculates variance of the array elements
Accepts: an array, arithemtic mean of its elements and size of the array
Returns: calculated variance as a double
*/
double calc_variance(double * data, double mean, size_t size)
{
	double sum = 0;
	for (unsigned i = 0; i < size; ++i)
	{
		double difference = data[i] - mean;
		difference *= difference;
		sum += difference;
	}
	return sum / size;
}
/*
Method Name: selection_sort()
Purpose: sorts the array using selection sort
Accepts: an array of doubles and its size
Returns: nothing
*/
void selection_sort(double * data, size_t size)
{
	unsigned j, min;
	//going through each row
	for (unsigned i = 0; i < size - 1; ++i)
	{
		//getting the minimum element
		min = i;
		for (j = i + 1; j < size; ++j)
			if (data[j] < data[min])
				min = j;

		//swapping elements
		double temp = data[min];
		data[min] = data[i];
		data[i] = temp;
	}
}
/*
Method Name: draw_graph_50()
Purpose: calculates how many squares are needed to draw a graph for 50% fq and prints them
Accepts: actual frequency of a number (double)
Returns: nothing
*/
void draw_graph_50(double actlFrequency)
{
	if (actlFrequency == 0)
		printf("\n");
	else
	{
		double neededSquares = actlFrequency * 40.0 / TO_50_PERCENT; //20 squares per 50%
		for(unsigned i = 0; i < neededSquares; ++i)
			printf("\xFE");
		printf("\n");
	}
}
/*
Method Name: draw_graph_100()
Purpose: calculates how many squares are needed to draw a graph for 100% fq and prints them
Accepts: actual frequency of a number (double)
Returns: nothing
*/
void draw_graph_100(double actlFrequency)
{
	if (actlFrequency == 0)
		printf("\n");
	else
	{
		double neededSquares = actlFrequency * 40.0 / TO_PERCENT; //40 squares per 100%
		for (unsigned i = 0; i < neededSquares; ++i)
			printf("\xFE");
	printf("\n");
	}
}
	
/*
Method Name: draw_at_50()
Purpose: draws a graph for frequencies below 50
Accepts: unsigned maxDgts for cell spacing, 2 arrays for actual and expected frequencies
Returns: nothing
*/
void draw_at_50(unsigned maxDgts, double *expectedFrequencies, double *actualFrequencies)
{
	printf("    exp dig     freq  0      10      20      30      40      50\n");
	printf("%s%s\xC4 \xDA%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2\n", GRAPH_HZ_10, GRAPH_HZ_10, GRAPH_HZ_3,
		GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3);
	for (unsigned i = 0; i < 9; ++i)
	{
		unsigned digit = i + 1;
		printf("%*.2lf%% [%u] = %*.2lf%% \xB3", maxDgts, expectedFrequencies[i], digit, maxDgts, actualFrequencies[i]);
		draw_graph_50(actualFrequencies[i]);
	}
	printf("%s%s\xC4 \xC0%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1\n", GRAPH_HZ_10, GRAPH_HZ_10, GRAPH_HZ_3,
		GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3);
}
/*
Method Name: draw_at_100()
Purpose: draws a graph for frequencies that are higher than 50
Accepts: unsigned maxDgts for cell spacing, 2 arrays for actual and expected frequencies
Returns: nothing
*/
void draw_at_100(unsigned maxDgts, double *expectedFrequencies, double *actualFrequencies)
{
	printf("    exp dig     freq  0  10  20  30  40  50  60  70  80  90 100\n");
	printf("%s%s\xC4 \xDA%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2%s\xC2\n", GRAPH_HZ_10, GRAPH_HZ_10, GRAPH_HZ_3,
		GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3);
	for (unsigned i = 0; i < 9; ++i)
	{
		unsigned digit = i + 1;
		printf("%*.2lf%% [%u] = %*.2lf%% \xB3", maxDgts, expectedFrequencies[i], digit, maxDgts, actualFrequencies[i]);
		draw_graph_100(actualFrequencies[i]);
	}
	printf("%s%s\xC4 \xC0%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1%s\xC1\n", GRAPH_HZ_10, GRAPH_HZ_10, GRAPH_HZ_3,
		GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3, GRAPH_HZ_3);
}