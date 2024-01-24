//
// Created by 28322 on 2024/1/24.
//
#include <stdio.h>
#include "../parser.h"
#include "../lexer.h"
#include "../engine.h"

int main() {
    // World 测试数据集 1
    char sql[1000][252] = {
            "CREATE TABLE table_a (\n"
            "    id_a INT PRIMARY KEY,\n"
            "    id_b INT PRIMARY KEY,\n"
            "    id_c INT PRIMARY KEY\n"
            ");",
            "CREATE TABLE table_b (\n"
            "    id_a INT PRIMARY KEY INT\n"
            ");",
            "CREATE TABLE table_a (\n"
            "    id_a INT PRIMARY KEY,\n"
            "    id_b INT UNIQUE);",
            "CREATE TABLE table_b (\n"
            "    id_a PRIMARY KEY,\n"
            "    id_b INT UNIQUE);",
            "CREATE TABLE table_c (\n"
            "    id_a;",
            "CREATE TABLE table_c (\n"
            "    id_a PRIMARY KEY,\n"
            "    uname CHAR(20 NOT NULL),\n"
            ");",
            "CREATE TABLE table_a(\n"
            "    id_a INT PRIMARY KEY,\n"
            "    id_b INT UNIQUE,\n"
            "    id_c INT UNIQUE\n"
            ");",
            "CREATE TABLE table_c (\n"
            "    id_a INT PRIMARY KEY,\n"
            "    uname CHAR(20) NOT NULL NULL, \n"
            ");",
            "INSERT INTO table_a VALU;",
            "INSERT INTO VALUES table_a (\n"
            "    1, 2\n"
            ");",
            "INSERT INTO 2table VALUES (\n"
            "    3, 4\n"
            ");",
            "INSERT IN'TO table_a VALUES (\n"
            "    3, 4\n"
            ");",
            "INSERT, INTO table_a VALUES (\n"
            "    3, 4\n"
            ");",
            "INSERT ( INTO table_a VALUES\n"
            "    3, 4\n"
            ");",
            "INSERT INTO table_a VALUES (\n"
            "    3 4);",
            "INSERT INTO table_a VALUES 3, 4);",
            "InSERT INTO table_a VALUES (3, 4);",
            "Select * FROM * table_a;",
            "SELECT * From table_a;",
            "SELECT * FROM table_a WHERE;",
            "SELECT * FROM table_a Where id_a = 1;",
            "SELECT * FROM table_a id_a = 1;",
            "SELECT * FROM table_a;",
            "DELETE FROM table_a;",
            "DELETE FROM table_a WHERE;",
            "DELETE FROM table_a WHERE id_a = 1;",
            "DELETE FROM table_c WHERE id_a = 1;",
            "DELETE (FROM) table_a WHERE id_a = 1;",
            "UPDATE table_a WHERE id_a = 1;",
            "UPDATE table_a sET id_b = 1 WHERE id_a = 1;",
            "UPDATE table_a SET id_b = 1 WHERE id _ a = 1;",
            "UPDATE table_a SET (id_b = 1) WHERE id_a = 1;"
    };

    printf("World Test the dataset 1\n");
    int arrSize = 30;
    TableList tableList;
    initTableList(&tableList);
    for (int i = 0; i <= arrSize; i++) {
        // 词法分析
        TokenList tokens = tokenize(sql[i]);
        // 语法分析
        if (parse(tokens)) {
            // 执行引擎
            executeEngine(&tokens, &tableList);
        }
    }

    // World 测试数据集 2
    char sql2[1000][252] = {
            "CREATE TABLE messages (\n"
            "    uname CHAR(10),\n"
            "    id INT PRIMARY KEY,\n"
            "    msg CHAR(100) NOT NULL,\n"
            "    favs INT NOT NULL,\n"
            "    create_date CHAR(10) NOT NULL\n"
            ");",
            "CREATE TABLE users (\n"
            "    uname CHAR(10),\n"
            "    userid INT PRIMARY KEY,\n"
            "    avatar CHAR(150)\n"
            ");",
            "INSERT INTO users VALUES (\n"
            "    'San Zhang',\n"
            "    1,\n"
            "    'Qk1eAAAAAAAAAD4AAAAoAAAACAAAAAgAAAABAAEAAAAAACAAAAAlFgAAJRYAAAAAAAAAAAAAAAAAAP///wD/AAAA5wAAAMMAAACZAAAAmQAAAMMAAADnAAAA/wAAAA=='\n"
            ");",
            "INSERT INTO messages VALUES (\n"
            "    'San Zhang',\n"
            "    1,\n"
            "    'Hello World',\n"
            "    0,\n"
            "    '2023 12 28'\n"
            ");",
            "INSERT INTO messages VALUES (\n"
            "    'San Zhang',\n"
            "    2,\n"
            "    'I am San Zhang.',\n"
            "    0,\n"
            "    '2023 2 29'\n"
            ");",
            "INSERT INTO users VALUES (\n"
            "    'Si Li',\n"
            "    2,\n"
            "    'Qk1eAAAAAAAAAD4AAAAoAAAACAAAAAgAAAABAAEAAAAAACAAAAAlFgAAJRYAAAAAAAAAAAAAAAAAAP///wDAAAAAgAAAAIAAAAABAAAAAQAAAIMAAADHAAAA/wAAAA=='\n"
            ");",
            "INSERT INTO messages VALUES (\n"
            "    'Si Li',\n"
            "    3,\n"
            "    'Hello World!',\n"
            "    0,\n"
            "    '2023 12 29'\n"
            ");",
            "UPDATE messages SET favs = 1 WHERE uname = 'San Zhang';",
            "UPDATE messages SET favs = 2 WHERE id = 3;",
            "UPDATE users SET avatar = 'Qk1eAAAAAAAAAD4AAAAoAAAACAAAAAgAAAABAAEAAAAAACAAAAAlFgAAJRYAAAAAAAAAAAAAAAAAAP///wD/AAAA5wAAAAkAAAB+AAAACQAAAOcAAAD/AAAA/wAAAA==' WHERE userid = 1;",
            "INSERT INTO users VALUES (\n"
            "    'Wu Wang',\n"
            "    3,\n"
            "    'Qk1eAAAAAAAAAD4AAAAoAAAACAAAAAgAAAABAAEAAAAAACAAAAAlFgAAJRYAAAAAAAAAAAAAAAAAAP///wDnAAAA5wAAABgAAAC9AAAAvQAAABgAAADnAAAA7wAAAA=='\n"
            ");",
            "INSERT INTO users VALUES (\n"
            "    'Wu Wang',\n"
            "    4,\n"
            "    'Hello, I am Wu Wang.',\n"
            "    0,\n"
            "    '2023 12 30'\n"
            ");",
            "INSERT INTO messages VALUES (\n"
            "    'Wu Wang',\n"
            "    4,\n"
            "    'Hello, I\\'m Wu Wang.',\n"
            "    0,\n"
            "    '2023 12 30'\n"
            ");",
            "SELECT uname, msg FROM messages;",
            "SELECT * FROM users;",
            "INSERT INTO messages VALUES (\n"
            "    'San Zhang',\n"
            "    5,\n"
            "    'Goodbye',\n"
            "    0,\n"
            "    '2024 1 1'\n"
            ");",
            "DELETE FROM users WHERE uname = 'San Zhang';",
            "DELETE FROM messages WHERE uname = 'San Zhang';",
            "SELECT * FROM messages;",
            "SELECT * FROM users;"
    };
    int arrSize2 = 19;
    TableList tableList2;
    initTableList(&tableList2);
    for (int i = 0; i <= arrSize2; i++) {
        // 词法分析
        TokenList tokens2 = tokenize(sql2[i]);
        // 语法分析
        if (parse(tokens2)) {
            // 执行引擎
            executeEngine(&tokens2, &tableList2);
        }
    }
    return 0;
}