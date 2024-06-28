/*
Program Name: nbstats_v1.2.c
Purpose: calculates a set of statistics on a list of numbers, then performs a Newcomb-Benford analysis of the data set
Coder: Yevgeniya Anasheva, 0881163
Date: Feb 15, 2019
*/
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS

#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "array_t.h"
#include <Windows.h>
#include <math.h>
#include <float.h>
#include "Nbstats_functions.h"

HANDLE hStdin;
DWORD fdwSaveOldMode;

#define BLOCK "\xB8"
#define HORIZONTAL "\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD"

int main(int argc, char* argv[])
{
#if defined(_DEBUG)
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
	dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF;
	dbgFlags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(dbgFlags);
#endif

	////create a FILE object to associate with a file, using a command line argument
	//FILE * stream = fopen(argv[1], "r");
	FILE * stream = stdin;

	//variables 
	bool hasNotified = false;
	bool isDecimal = false;
	bool isNotANumber = false;
	double value; //accumulator used for getchar(), it is also a temp variable that saves a number in the dynamic array
	double division_counter;//accumulator as well, used for calculating the decimal part
	size_t elementCounter = 0;

	//create dynamic array
	array_t statsArray = array(); //use pushback for it
	size_t capacity = 1000;		//sets initial capacity
	/*if (!array_resize(&statsArray, capacity))
	{
		puts("Not enough memory");
		return EXIT_FAILURE;
	}*/
	//changing code page to 850 to display special characters
	UINT currentCP = GetConsoleOutputCP();
	if (!SetConsoleOutputCP(850))
	{
		puts("SetConsoleOutputCP Failed");
		EXIT_FAILURE;
	}

	char *s = "Newcomb-Benford Stats (v1.0.0), " BLOCK " 2019 Yevgeniya Anasheva\n";
	printf("%s", s);
	
	if (argc > 2)
	{
		printf("Error: too many args\n");
		return EXIT_FAILURE;
	}
	else if (argc == 2)
	{
		if ((stream = fopen(argv[1], "r")) == NULL)
		{
			printf("Error: file opening failed\n");
			return EXIT_FAILURE;
		}
	}
	
	//INPUT
	puts("Enter white-space separated real numbers. Terminate input with ^Z");
	int ch = 0;
	//this outer loop controls the sequence of numbers
	while(ch != EOF)
	{
		//initializing the accumulator, also resets the value after we got our number
		value = 0;
		//reseting these 2 values after getting the decimal part of an individual number
		isDecimal = false;
		double fraction = 0;
		division_counter = 10.0;

		//this loop gets an individual number
		while((ch = fgetc(stream)) != ' ')
		{
			++elementCounter;
			if (value == 0 && ch == '-') //checks for negative numbers
			{
				if (!hasNotified) //will only run once in case if there are a lot of negative numbers
				{
					hasNotified = true;
					printf("You entered one or several negative numbers, so I made them positive \n");
				}
				break;
			}
			else if (isDecimal && ch <= '9' && ch >= '0')
			{
				ch = ch - '0';
				fraction = ch / division_counter;
				value += fraction;
				division_counter *= 10;
			}
			else if (ch == '.')
			{
				if (isDecimal)//this condition makes sure that we don't count decimal a second time if user inputs a number like "1.2.4"
					break;
				isDecimal = true;
			}
			else if (ch <= '9' && ch >= '0')//getting a number successfully 
			{
				value *= 10;
				value += ch - '0';
			}
			else if (isalpha(ch) && value == 0)//if the symbol is a letter and it comes first
			{
				isNotANumber = true;
				break;
			}
			else if((ch > '9' || ch < '0'))//if the symbol is not a number (like a punctuation), ignore
				break;
		}//end inner while

		if (isNotANumber)
		{
			printf("Element #%u is not a number\n", elementCounter);
			printf("Element was: %c\n", ch);
			printf("Quitting");
			array_free(&statsArray);
			return EXIT_FAILURE;
		}
		if (statsArray.capacity < statsArray.size)
		{
			if (!array_resize(&statsArray, capacity))
			{
				puts("Not enough memory");
				return EXIT_FAILURE;
			}
		}
		if (isinf(value) || value >= DBL_MAX)
			printf("Error: Value #%lu is equal %lf\n", elementCounter, value);
		else
		{
			if (value != 0 && !array_push_back(&statsArray, value))
			{
				puts("Memory allocation error");
				array_free(&statsArray);
				return EXIT_FAILURE;
			}
		}
	}//end outer while

	if (statsArray.data == NULL)
	{
		puts("Array cannot be filled. Quitting");
		return EXIT_FAILURE;
	}
	puts("Standard analysis");
	printf("%s%s%s\n", HORIZONTAL, HORIZONTAL, HORIZONTAL);

	//sorting the array
	selection_sort(statsArray.data, statsArray.size);

	printf("# of elements: %u\n", statsArray.size);

	//range
	printf("Range = [%g...%g]\n", statsArray.data[0], statsArray.data[statsArray.size - 1]);

	//average
	double mean = calc_mean(statsArray.data, statsArray.size);
	printf("Arithmetic mean = %g\n", mean);

	//median
	double median = calc_median(statsArray.data, statsArray.size);
	printf("Arithmetic median = %g\n", median);

	//variance
	double variance = calc_variance(statsArray.data, mean, statsArray.size);
	printf("Variance = %g\n", variance);

	//standard deviation
	double std_deviation = calc_std_deviation(variance);
	printf("Standard deviation = %g\n", std_deviation);

	//mode
	array_t numbersInMode = array();
	int count = 1;
	int maxCount = 0;
	double mode = statsArray.data[0];
	//this loop gets the most repeated count
	for (size_t j = 0; j < statsArray.size - 1; ++j)
	{
		if (statsArray.data[j] == statsArray.data[j + 1])
		{
			++count;
			if (count > maxCount)
				maxCount = count;
		}
		else
			count = 1; //reset the counter
	}
	count = 1;//resetting the counter again

	//this loop checks for any counts that are equal to most repeated number
	for (size_t i = 0; i < statsArray.size - 1; ++i)
	{
		if (statsArray.data[i] == statsArray.data[i + 1])
		{
			++count;
			if (count == maxCount)
			{
				mode = statsArray.data[i];
				array_push_back(&numbersInMode, mode);
			}
		}
		else
			count = 1; //reset the counter
	}
	printf("Mode: ");
	if (maxCount == 0)
		printf(" no mode\n");
	else if (numbersInMode.size == statsArray.size / maxCount)
		printf(" no mode\n");
	else
	{
		printf(" { ");
		for (size_t i = 0; i < numbersInMode.size; ++i)
			printf("%g ", numbersInMode.data[i]);
		printf(" } x%d\n", maxCount);
	}

	//Newcomb-Benford's Law
	unsigned frequencyArray[9] = { 0 };
	double divided;
	unsigned firstDigit;
	//getting the first digit by dividing by 10
	for (size_t i = 0; i < statsArray.size; ++i)
	{
		divided = statsArray.data[i];
		if (statsArray.data[i] >= 10)//keep dividing by 10 if the number has more than 1 digit
			for (size_t j = 10; divided >= 10; j *= 10)
			{
				divided = statsArray.data[i] / j;
			}
		else
			divided = (unsigned)statsArray.data[i];

		firstDigit = (unsigned)divided;
		if (firstDigit == 1)
			++frequencyArray[0];
		else if (firstDigit == 2)
			++frequencyArray[1];
		else if (firstDigit == 3)
			++frequencyArray[2];
		else if (firstDigit == 4)
			++frequencyArray[3];
		else if (firstDigit == 5)
			++frequencyArray[4];
		else if (firstDigit == 6)
			++frequencyArray[5];
		else if (firstDigit == 7)
			++frequencyArray[6];
		else if (firstDigit == 8)
			++frequencyArray[7];
		else if (firstDigit == 9)
			++frequencyArray[8];
	}
	double sumOfFirstDigits = 0;
	for (size_t i = 0; i < 9; ++i)
		sumOfFirstDigits += frequencyArray[i];
	//Raw frequency table
	printf(" [1] = %u\n", frequencyArray[0]);
	printf(" [2] = %u\n", frequencyArray[1]);
	printf(" [3] = %u\n", frequencyArray[2]);
	printf(" [4] = %u\n", frequencyArray[3]);
	printf(" [5] = %u\n", frequencyArray[4]);
	printf(" [6] = %u\n", frequencyArray[5]);
	printf(" [7] = %u\n", frequencyArray[6]);
	printf(" [8] = %u\n", frequencyArray[7]);
	printf(" [9] = %u\n", frequencyArray[8]);

	//Expected frequencies
	double expectedFrequencies[9];//size is 9 because there are 9 digits
	for (size_t j = 0; j < 9; ++j)
		expectedFrequencies[j] = calc_exp_frequency(j + 1);

	unsigned maxDgts = 6;//sets the cell size for the graph
	printf("Newcomb-Benford's Law Analysis\n");
	printf("%s%s%s\n", HORIZONTAL, HORIZONTAL, HORIZONTAL);

	//Actual frequencies
	double actualFrequencies[9];//size is 9 because there are 9 digits
	for (size_t j = 0; j < 9; ++j)//fill up the array
		actualFrequencies[j] = calc_actl_frequency(frequencyArray, j, sumOfFirstDigits);

	double maxFrequency = actualFrequencies[0];//finding the max frequency for a graph
	for (size_t j = 0; j < 8; ++j)
	{
		if (actualFrequencies[j + 1] > maxFrequency)
			maxFrequency = actualFrequencies[j + 1];
	}

	//drawing graph	
	if (maxFrequency < 50)
		draw_at_50(maxDgts, expectedFrequencies, actualFrequencies);
	else
		draw_at_100(maxDgts, expectedFrequencies, actualFrequencies);

	//NB Variance
	double nbVariance = calc_NB_variance(actualFrequencies, expectedFrequencies);
	printf("Variance = %.5lf%%\n", nbVariance);

	//NB Deviation
	double nbDeviation = calc_std_deviation(nbVariance)*10;//multiplying by 10 to match the result
	printf("Standard deviation = %.5lf%%\n", nbDeviation);
	
	//NB Relationship analysis
	nbDeviation = nbDeviation / 100;
	if (nbDeviation >= 0 && nbDeviation < 0.1)
		printf("The Newcomb-Benford relationship is very strong\n");
	else if (nbDeviation >= 0.1 && nbDeviation < 0.2)
		printf("The Newcomb-Benford relationship is strong\n");
	else if (nbDeviation >= 0.2 && nbDeviation < 0.35)
		printf("The Newcomb-Benford relationship is moderate\n");
	else if (nbDeviation >= 0.35 && nbDeviation < 0.5)
		printf("The Newcomb-Benford relationship is weak\n");
	else
		printf("There is no Newcomb-Benford relationship\n");
	printf("%s%s%s\n", HORIZONTAL, HORIZONTAL, HORIZONTAL);

	if (!SetConsoleOutputCP(currentCP)) 
	{
		puts("SetConsoleOutputCP");
		EXIT_FAILURE;
	}
	array_free(&statsArray);
	array_free(&numbersInMode);

	fclose(stream);

	_CrtDumpMemoryLeaks();

	return 0;
}


