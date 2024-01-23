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
    while (scanf("%[^\n]", sql) != EOF) {
        getchar();
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