#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t length;
} String;

// 初始化字符串
void initString(String *str, const char *initialValue) {
    str->length = strlen(initialValue);
    str->data = malloc(str->length + 1);
    if (str->data != NULL) {
        strcpy(str->data, initialValue);
    } else {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
}

// 释放字符串内存
void freeString(String *str) {
    free(str->data);
}

// 打印字符串
void printString(const String *str) {
    printf("%s\n", str->data);
}

// 追加字符串
void appendString(String *str, const char *appendValue) {
    size_t appendLength = strlen(appendValue);
    str->data = realloc(str->data, str->length + appendLength + 1);
    if (str->data != NULL) {
        strcat(str->data, appendValue);
        str->length += appendLength;
    } else {
        perror("Memory reallocation failed");
        exit(EXIT_FAILURE);
    }
}

// 比较两个字符串
int compareStrings(const String *str1, const String *str2) {
    return strcmp(str1->data, str2->data);
}

// 获取字符串长度
size_t getStringLength(const String *str) {
    return str->length;
}

// 提取子串
String substring(const String *str, size_t start, size_t length) {
    if (start >= str->length) {
        fprintf(stderr, "Start index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    size_t actualLength = length;
    if (start + length > str->length) {
        actualLength = str->length - start;
    }

    String sub;
    sub.data = malloc(actualLength + 1);
    if (sub.data != NULL) {
        strncpy(sub.data, str->data + start, actualLength);
        sub.data[actualLength] = '\0';
        sub.length = actualLength;
    } else {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    return sub;
}