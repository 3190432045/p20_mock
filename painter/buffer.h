/*
//=============================================================================
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================
*/


#ifndef __BUFFER__
#define __BUFFER__


/*
// buffer.h
*/


#ifdef __cplusplus
extern "C"
{
#endif


typedef unsigned char ElemType;

typedef struct
{
    int size;
    int get_index;
    int put_index;
    ElemType *address;
} ring_buffer;


void buffer_init(ring_buffer *buffer_ptr, int size);

void buffer_print(ring_buffer *buffer_ptr);
int buffer_full(ring_buffer *buffer_ptr);
int buffer_empty(ring_buffer *buffer_ptr);

void buffer_put(ring_buffer *buffer_ptr, ElemType *elem);
void buffer_get(ring_buffer *buffer_ptr, ElemType *elem);

void buffer_flush(ring_buffer *buffer_ptr);
void buffer_display(ring_buffer *buffer_ptr);


#ifdef __cplusplus
}
#endif


#endif  /*__BUFFER__*/

