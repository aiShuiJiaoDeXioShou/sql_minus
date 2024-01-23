//
// Created by 28322 on 2024/1/23.
//

#include <stdio.h>
#include "../lexer.h"
#include "../parser.h"
#include "../engine.h"

int main() {
    // 编写测试代码
    // 测试词法分析器
    // 测试语法分析器
    char sql_create_table[256] = "CREATE TABLE students (\n"
                                 "\tid INT PRIMARY KEY, \n"
                                 "\tname CHAR(30)\n"
                                 ");";
//    char sql_insert_into[256] = "INSERT INTO students VALUES (1, 'Bob');";
//    char sql_delete_from[256] = "DELETE FROM students WHERE id = 1;";
//    char sql_update[256] = "UPDATE students SET name = 'Bob' WHERE id = 1;";
//    char sql_select[256] = "SELECT * FROM students ORDER BY id ASC;";
//    char sql_select2[256] = "SELECT * FROM students WHERE id = 1;";
//
//    printf("CREATE TABLE\n");
//    parseCreateTable(tokenize(sql_create_table));
//    printf("insert\n");
//    parseInsertInto(tokenize(sql_insert_into));
//    printf("delete\n");
//    parseDeleteFrom(tokenize(sql_delete_from));
//    printf("update\n");
//    parseUpdate(tokenize(sql_update));
//    printf("select ORDER BY\n");
//    parseSelect(tokenize(sql_select));
//    printf("select WHERE\n");
//    parseSelect(tokenize(sql_select2));

    // 测试代码解析器
    // 测试引擎
    // 初始化表格数组和表格数量
    TableList tableList;
    initTableList(&tableList);
    TokenList tokenList = tokenize(sql_create_table);
    parse(tokenList);
    // 执行引擎
    executeEngine(&tokenList, &tableList);

    // insert 在表格当中插入数据
    char sql_insert_into[256] = "INSERT INTO students VALUES (1, 'Bob');";
    TokenList tokenList2 = tokenize(sql_insert_into);
    parse(tokenList2);
    executeEngine(&tokenList2, &tableList);

    // update 在表格当中更新数据
    char sql_update[256] = "UPDATE students SET name = 'Bob2' WHERE id = 1;";
    TokenList tokenList3 = tokenize(sql_update);
    parse(tokenList3);
    executeEngine(&tokenList3, &tableList);

    char sql_insert_into2[256] = "INSERT INTO students VALUES (2, 'Tom');";
    TokenList tokenList4 = tokenize(sql_insert_into2);
    parse(tokenList4);
    executeEngine(&tokenList4, &tableList);

    // remove 在表格当中删除数据
    char sql_delete_from[256] = "DELETE FROM students WHERE id = 2;";
    TokenList tokenList5 = tokenize(sql_delete_from);
    parse(tokenList5);
    executeEngine(&tokenList5, &tableList);

    // select 在表格当中查询数据
    char sql_select[256] = "SELECT * FROM students;";
    TokenList tokenList6 = tokenize(sql_select);
    parse(tokenList6);
    executeEngine(&tokenList6, &tableList);

    // 打印所有创建的表格
//    for (int i = 0; i < tableList.tableCount; i++) {
//        Table *table = getTableByIndex(&tableList, i);
//        printf("Table: %s\n", table->name);
//        for (int j = 0; j < table->columnCount; j++) {
//            TableColumn column = table->columns[j];
//            printf("\tColumn: %s, %s, %s, %s, %s\n", column.name, column.type,
//                   column.isNotNull ? "NOT NULL" : "", column.isPrimaryKey ? "PRIMARY KEY" : "",
//                   column.isUnique ? "UNIQUE" : "");
//        }
//        // 打印table所有的数据 table->data
//        for (int j = 0; j < table->rowCount; j++) {
//            for (int k = 0; k < table->columnCount; k++) {
//                printf("%s\t", table->data[j][k]);
//            }
//            printf("\n");
//        }
//    }
    return 0;
}