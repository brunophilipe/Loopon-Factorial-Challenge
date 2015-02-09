//
//  main.c
//  Loopon-Factorial-Challenge
//
//  Created by Bruno Philipe on 2/8/15.
//  Copyright (c) 2015 Bruno Philipe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define OUT_LEN 36000
#define IN_LEN 4

char * longfat(char *number, int *status);

int is_valid_number(char *number);
int decrement(char *input, int size, int value);
int is_one_or_less(char *input, int size);

int main()
{
	char *number = malloc(sizeof(char) * 10);
	char *output = NULL;
	int isValid = 0;
	
	printf("Insert value to calculate factorial: ");
	scanf("%s", number);
	
	output = longfat(number, &isValid);
	
	if (!isValid)
	{
		printf("%s doesn't seem to be a valid number!\n", number);
	}
	else if (isValid == 2)
	{
		printf("Factorial for negative numbers are not defined!\n");
	}
	else
	{
		printf("Result: %s\n", output);
	}
	
	free(number);
	free(output);
	
	return 0;
}

/**
 * Function that validates the input and calculates the factorial of a number.
 * The input number should be passed as a string, for example 15 should be "15".
 *
 * The maximum supported input string legth is IN_LEN characters, and the 
 * maximum guaranteed correct output string support is OUT_LEN characters (above
 * this value it will either truncate the value or, if much higher, mess up the
 * calculation entirely).
 *
 * The result will be a string such as "1307674368000" that should be freed
 * by the caller.
 *
 * The algorithm operates upon the number strings in reverse, so that the index
 * of a digit is proportional to its significance, easing the implementation.
 *
 * The status value will be as follows:
 *		0 if the input string is not a valid number (returns NULL).
 *		1 if the input was correct and the factorial was calculated.
 *		2 if the input was a negative number (returns NULL).
 */
char * longfat(char *number, int *status)
{
	int isValid = is_valid_number(number);
	
	if (isValid == 1)
	{
		long long int f, i, j;
		char * number_multiplicand = malloc(sizeof(char) * OUT_LEN + IN_LEN);
		char * number_multiplier   = malloc(sizeof(char) * IN_LEN);
		char * number_result	   = malloc(sizeof(char) * OUT_LEN + IN_LEN);
		
		// Fill each string with '0' characters.
		// Note strings are not null terminated.
		memset(number_multiplicand, '0', OUT_LEN + IN_LEN);
		memset(number_multiplier,   '0', IN_LEN);
		memset(number_result,		'0', OUT_LEN + IN_LEN);
		
		// Write the number to the number_multiplicand string in reverse.
		for (i=0; i<strlen(number); i++)
		{
			number_multiplicand[strlen(number) - i - 1] = number[i];
		}
		
		// Copy the number_multiplicand value to number_multiplicand so it can
		// be decremented later by the algorithm.
		strncpy(number_multiplier, number_multiplicand, IN_LEN);
		
		// Perform the calculation itself. (Here the magic happens)
		// Each loop represents a factorial multiplication.
		// It stops when `number_multiplier` <= 1 (because 1! = 1).
		while (!is_one_or_less(number_multiplier, IN_LEN))
		{
			decrement(number_multiplier, IN_LEN, 1);
			
			// The carry value for each individual multiplication.
			int carry = 0;
			
			// The result of each individual multiplication.
			int result = 0;
			
			// This algorithm runs a long multiplication, and performs exactly
			// like the notation used to solve multiplications by hand, except
			// it performs the multiplication and the sum in a single step.
			for (i=0; i<IN_LEN; i++)
			{
				for (j=0; j<OUT_LEN; j++)
				{
					result = (number_multiplicand[j] - '0') \
						* (number_multiplier[i] - '0') \
						+ carry + number_result[i+j] - '0';
					
					carry = result/10;
					result = (result - carry * 10);
					number_result[i+j] = result + '0';
				}
			}
			
			strncpy(number_multiplicand, number_result, OUT_LEN + IN_LEN);
			memset(number_result, '0', OUT_LEN + IN_LEN);
		}
		
		char *output = malloc(sizeof(char) * OUT_LEN + IN_LEN);
		char curChar = 0;
		int skipZeros = 1;
		int skipped = 0;
		
		memset(output, '0', OUT_LEN + IN_LEN);
		
		// Write the number_multiplicand to the output string in reverse.
		for (f=0; f<OUT_LEN + IN_LEN; f++)
		{
			curChar = number_multiplicand[OUT_LEN + (IN_LEN - 1) - f];
			
			if (skipZeros == 1 && curChar == '0')
			{
				skipped++;
				continue;
			}
			
			skipZeros = 0;
			output[f-skipped] = curChar;
		}
		
		output[f-skipped] = '\0';
		
		free(number_multiplicand);
		free(number_multiplier);
		free(number_result);
		
		if (status != NULL)
			*status = 1;
		
		return output;
	}
	else
	{
		if (status != NULL)
			*status = isValid;
	}
	
	return NULL;
}

int is_one_or_less(char *input, int size)
{
	int i;
	int sum = 0;
	
	for (i=size; i>=0; i--)
	{
		if (sum > 1)
		{
			return 0;
		}
		else if (input[i] == 0)
		{
			continue;
		}
		else
		{
			sum += (input[i] - '0') * pow(10, i);
		}
	}
	
	return sum <= 1;
}

int decrement(char *input, int size, int value)
{
	int i, j, result, success = 0;
	
	for (i=0; i<size; i++)
	{
		result = input[i] - '0' - value;
		if (result >= 0)
		{
			input[i] = '0' + result;
			success = 1;
			
			for (j=i-1; j>=0; j--) {
				input[j] = '9';
			}
			
			break;
		}
	}
	
	return success;
}

int is_valid_number(char *number)
{
	int i=0;
	int diff = -1;
	int negative = 0;
	
	for (i=0; i<strlen(number); i++)
	{
		if (i==0 && number[0] == '-')
		{
			negative = 1;
			continue;
		}
		
		diff = number[i] - '0';
		
		if (diff < 0 || diff > 9)
		{
			return 0;
		}
	}
	return (negative ? 2 : 1);
}
