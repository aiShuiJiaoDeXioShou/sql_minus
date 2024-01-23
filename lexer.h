#ifndef SQL_MINUS_LEXER_H
#define SQL_MINUS_LEXER_H
#define KEY_WORD_SIZE 30
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token类型
typedef enum {
    KEYWORD, IDENTIFIER, OPERATOR, LITERAL, SEMICOLON, COMMA, LEFT_BRACKET, RIGHT_BRACKET, STRING
} TokenType;

// 结构体表示一个标记
typedef struct {
    TokenType type;
    char value[256];
} Token;

// Token列表结构体
typedef struct {
    Token *tokens;
    size_t size;
} TokenList;

TokenList initTokenList();

// 词法分析器函数声明
TokenList tokenize(char *input);

// 销毁TokenList
void destroyTokenList(TokenList *list);

TokenList* parseFile(const char* filename);
#endif
