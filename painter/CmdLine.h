/*
//
//
//
// file: CmdLine.h
// last modified:
// copyright:
// version:
//
// email:
// web:
//
*/

#ifndef INCLUDED_CMDLINE
#define INCLUDED_CMDLINE

#include <cstdio>
#include <vector>
#include <iterator>


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    unsigned char x0;
    unsigned char y0;
    unsigned char x1;
    unsigned char y1;
} line_t;

#ifdef __cplusplus
}
#endif

typedef std::vector<line_t> drawlines_t;


#endif // INCLUDED_CMDLINE
