#include "filter.h"
#include <stdio.h>

void init_buffer(ring_buffer_t *buffer, int *arr, int buffer_size) {
	int i;
	for (i = 0; i < buffer_size; i++) 
		arr[i] = 0; 
	
	buffer->array = arr;
	buffer->size = buffer_size;
	buffer->current = 0;	
	buffer->denominator = ((float) buffer_size * ((float) buffer_size + 1)) / 2;	
}

float filter_point(int16_t data_point, ring_buffer_t* buffer) {
	buffer->array[buffer->current] = data_point;
	int size = buffer->size;		
	int i = (buffer->current + 1) % size;
	if (buffer->current == size)
		buffer->current = 0;
	int j;
	int numerator = 0;
	
	for (j = 0; j < size; j++) {
		numerator += (j + 1) * buffer->array[i];		
		i += 1;
		if (i == size)
			i = 0;
	}
	
	buffer->current += 1;
	if (buffer->current == size)
		buffer->current = 0;

	return ((float) numerator) / buffer->denominator;
}