//
// Created by 28322 on 2024/1/23.
//
#include <stdio.h>
#include "../parser.h"
#include "../lexer.h"
#include "../engine.h"

int main() {
    char sql[1000];
    TableList tableList;
    initTableList(&tableList);
    while (scanf("%[^;]", sql) != EOF) {
        // 清除了尾部;
        getchar();
        // 为sql重新添加分号
        strcat(sql, ";");
        // printf("sql: %s\n", sql);
        // 词法分析
        TokenList tokens = tokenize(sql);
        // 语法分析
        if (parse(tokens)) {
            // 执行引擎
            executeEngine(&tokens, &tableList);
        }
    }
    return 0;
}