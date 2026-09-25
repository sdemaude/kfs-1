#ifndef STRING_H
# define STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
int memcpy(void *dest, const void *src, size_t n);


#endif // STRING_H