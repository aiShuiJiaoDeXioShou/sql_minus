#include <stdio.h>
#include "parser.h"
#include "lexer.h"
#include "engine.h"

int main() {
    char sql[1000][252] = {
            "CREATE TABLE students (id INT PRIMARY KEY,name CHAR(30));",
            "INSERT INTO students VALUES (1, 'Alice');",
            "INSERT INTO students VALUES (2, 'Bob');",
            "INSERT INTO students VALUES (3, 'Carol');",
            "UPDATE students SET name = 'Bob' WHERE id = 1;",
            "DELETE FROM students WHERE id = 2;",
            "SELECT name FROM students WHERE id = 3;"
    };
    TableList tableList;
    initTableList(&tableList);
    for (int i = 0; i < 7; i++) {
        // 词法分析
        TokenList tokens = tokenize(sql[i]);
        // 语法分析
        if (parse(tokens)) {
            // 执行引擎
            executeEngine(&tokens, &tableList);
        }
    }
    return 0;
}