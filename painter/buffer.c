/*

===== buffer.c ========================================================

*/

/* ring buffer */

#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"



void buffer_init(ring_buffer *buffer_ptr, int size)
{
    buffer_ptr->size = size;
    buffer_ptr->get_index = 0;
    buffer_ptr->put_index = 0;
    buffer_ptr->address = (ElemType *)calloc(buffer_ptr->size, sizeof(ElemType));
}

void buffer_print(ring_buffer *buffer_ptr)
{
    printf("size = 0x%x, get_index = %d, put_index = %d\n", buffer_ptr->size, buffer_ptr->get_index, buffer_ptr->put_index);
}

int buffer_full(ring_buffer *buffer_ptr)
{
    return buffer_ptr->put_index == (buffer_ptr->get_index ^ buffer_ptr->size);
}

int buffer_empty(ring_buffer *buffer_ptr)
{
    return buffer_ptr->put_index == buffer_ptr->get_index;
}

int _buffer_incr(ring_buffer *buffer_ptr, int pos)
{
    return (pos + 1) & (2 * buffer_ptr->size - 1);
}

void buffer_put(ring_buffer *buffer_ptr, ElemType *elem)
{
    buffer_ptr->address[buffer_ptr->put_index & (buffer_ptr->size - 1)] = *elem;
    if (buffer_full(buffer_ptr))
        buffer_ptr->get_index = _buffer_incr(buffer_ptr, buffer_ptr->get_index);
    buffer_ptr->put_index = _buffer_incr(buffer_ptr, buffer_ptr->put_index);
}

void buffer_get(ring_buffer *buffer_ptr, ElemType *elem)
{
    *elem = buffer_ptr->address[buffer_ptr->get_index & (buffer_ptr->size - 1)];
    buffer_ptr->get_index = _buffer_incr(buffer_ptr, buffer_ptr->get_index);
}

void buffer_flush(ring_buffer *buffer_ptr)
{
    int i=0;
    buffer_ptr->get_index = 0;
    buffer_ptr->put_index = 0;
    for(i=0; i<buffer_ptr->size; i++)
        buffer_ptr->address[i] = 0;
}

void buffer_display(ring_buffer *buffer_ptr)
{
    int i=0;
    printf("\n");
    for(i=0; i<buffer_ptr->size; i++)
        printf("%d ",buffer_ptr->address[i]);
    printf("\n");
}
