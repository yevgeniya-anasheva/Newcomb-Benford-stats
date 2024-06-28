#pragma once
/*!	\file		array_t.h
	\author		Yevgeniya Anasheva
	\date		2019-02-13

	Array abstract data type from which we can create an object.
*/

#include <stdint.h>
#include <stdbool.h>

//data type for the pointer attribute (members or fields)
typedef double array_value_t;

//a struct is quick way to encapsulate multiple variables in one type
typedef struct array_t_tag
{
	double	       *data;			//represents a pointer to a dynamic array
	size_t			size;			//represents a tracker of number of elements
	size_t			capacity;		//represents the capacity of the array, the amount of memory allocation
} array_t;							//represents the data type for the struct; an easier name to use!

//declare functions which can use a struct object
array_t array();
bool array_push_back(array_t * pArray, array_value_t value);
bool array_resize(array_t * pArray, size_t newSize);
void array_free(array_t * pArray);
