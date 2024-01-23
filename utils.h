#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

typedef struct {
    char *data;
    size_t length;
} String;

void initString(String *str, const char *initialValue);
void freeString(String *str);
void printString(const String *str);
void appendString(String *str, const char *appendValue);
int compareStrings(const String *str1, const String *str2);
size_t getStringLength(const String *str);
String substring(const String *str, size_t start, size_t length);

#endif  // STRING_UTILS_H
