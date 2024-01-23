// 语法处理器
#ifndef SQL_MINUS_PARSER_H
#define SQL_MINUS_PARSER_H
#include "lexer.h"
#include <stdbool.h>

// 辅助函数，用于打印错误信息
void printError(const char *message);

// 解析 CREATE TABLE 语句
bool parseCreateTable(TokenList tokens);

// 解析 INSERT INTO 语句
bool parseInsertInto(TokenList tokens);

// 解析 DELETE FROM 语句
bool parseDeleteFrom(TokenList tokens);

// 解析 UPDATE 语句
bool parseUpdate(TokenList tokens);

// 解析 SELECT 语句
bool parseSelect(TokenList tokens);

// SQL-Minus 语法分析器
bool parse(TokenList tokens);

#endif