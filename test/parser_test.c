//
// Created by 28322 on 2024/1/22.
//
#include <stdio.h>
#include "../lexer.h"
#include "../parser.h"

int main() {
    // 编写测试代码
    // 测试词法分析器
    // 测试语法分析器
    printf("CREATE TABLE\n");
    parseCreateTable(tokenize("CREATE TABLE students (\n"
                              "\tid INT PRIMARY KEY, \n"
                              "\tname CHAR(30)\n"
                              ");"));
    printf("insert\n");
    parseInsertInto(tokenize("INSERT INTO students VALUES (1, 'Bob');"));
    printf("delete\n");
    parseDeleteFrom(tokenize("DELETE FROM students WHERE id = 1;"));
    printf("update\n");
    parseUpdate(tokenize("UPDATE students SET name = 'Bob' WHERE id = 1;"));
    printf("select ORDER BY\n");
    parseSelect(tokenize("SELECT * FROM students ORDER BY id ASC;"));
    printf("select WHERE\n");
    parseSelect(tokenize("SELECT * FROM students WHERE id = 1;"));
    return 0;
}