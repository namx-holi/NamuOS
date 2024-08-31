/// @file stdio_formatted_io.c

#include <stdio.h> // Implements

int fprintf(FILE* __restrict stream, const char* __restrict format, ...); // Write formatted data to stream
int fscanf(FILE* __restrict stream, const char* __restrict format, ...); // Read formatted data from stream
int printf(const char* __restrict format, ...); // Print formatted data to stdout
int scanf(const char* __restrict format, ...); // Read formatted data from stdin
int snprintf(char* __restrict s, size_t n, const char* __restrict format, ...); // Write formatted output to sized buffer
int sprintf(char* __restrict str, const char* __restrict format, ...); // Write formatted data to string
int sscanf(const char* __restrict s, const char* __restrict format, ...); // Read formatted data from string
int vfprintf(FILE* __restrict stream, const char* __restrict format, va_list arg); // Write formatted data from variable argument list to stream
int vfscanf(FILE* __restrict stream, const char* __restrict format, va_list arg); // Read formatted data from stream into variable argument list
int vprintf(const char* __restrict format, va_list arg); // Print formatted data from variable argument list to stdout
int vscanf(const char* __restrict format, va_list arg); // Read formatted data into variable argument list
int vsnprintf(char* __restrict s, size_t n, const char* __restrict format, va_list arg); // Write formatted data from variable argument list to sized buffer
int vsprintf(char* __restrict s, const char* __restrict format, va_list arg); // Write formatted data from variable argument list to string
int vsscanf(const char* __restrict s, const char* __restrict format, va_list arg); // Read formatted data from string into variable argument list
