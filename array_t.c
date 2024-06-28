/*!	\file		array_t.c
	\author		Yevgeniya Anasheva
	\date		2019-02-13

	Array abstract data type and the implementation of functions.
*/

#include "array_t.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//Creates an empty array object of array_t type and initializes the attributes of the struct
array_t array()
{
	//initially creates an empty object 
	array_t array = { NULL, 0, 0 }; //better to initialize
	return array;
}

//adds an element to the end of the array
//memory is allocated as needed
//parameters are a pointer to "this" object and a value to be added
//note use bool to be able to test if the push_back is successful
//however, push_back will be used in a void way
bool array_push_back(array_t * pArray, array_value_t value)
{
	//Expand if necessary
	if (pArray->size == (*pArray).capacity)//dereference operator, can't use the dot operator here
	{
		//create a temp capacity, in case of an empty state
		//otherwise double it
		size_t newCapacity = pArray->capacity * 2;
		if (newCapacity == 0)
			++newCapacity;

		//allocate memory
		array_value_t * newPtr = (array_value_t *)realloc(pArray->data, newCapacity * sizeof(array_value_t));
		//check for memory allocation failure
		if (newPtr == NULL)
			return false;

		//assigns new values to this object's attributes
		pArray->capacity = newCapacity;
		pArray->data = newPtr;
	}

	//now add the new value to the object's array
	pArray->data[pArray->size] = value;
	++pArray->size;

	return true;
}

//memory is allocated using newSize and maintains the existing array
//parameters are a pointer to "this" object and number of elements
//note use of bool to be able to test if the resize is successful
bool array_resize(array_t * pArray, size_t newSize)
{
	//reset this object's size to the parameter, if newSize is less, shrinking the array's dataset size
	if (newSize < pArray->size)
	{
		pArray->size = newSize;
		return true;
	}

	//expand if necessary
	if (newSize > pArray->capacity)
	{
		size_t newCapacity = newSize;
		if (newCapacity == 0)
			++newCapacity;

		array_value_t * newBlock = (array_value_t *)realloc(pArray->data, newCapacity * sizeof(array_value_t));
		if (newBlock == NULL)
			return false;

		//assign new attributes
		pArray->data = newBlock;
		pArray->capacity = newCapacity;
	}

	//Initialize any additional elements to zero
	while (pArray->size < newSize)
	{
		pArray->data[pArray->size] = 0;
		++pArray->size;
	}

	return true;
}


void array_free(array_t * pArray)
{
	//think of pArray as "this" object
	free(pArray->data);		//deallocates the memory block of object's array
	*pArray = array();		//resets the object to an empty state
}