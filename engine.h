//
// Created by 28322 on 2024/1/22.
// 语法执行引擎

#ifndef SQL_MINUS_ENGINE_H
#define SQL_MINUS_ENGINE_H
#include <stdio.h>
#include <string.h>
#include "lexer.h"

// 这个参数不能改 100 * 100 会令它内存爆炸
#define MAX_COLUMNS 10
#define MAX_ROWS 100

// 表格的列
typedef struct {
    char name[256];
    char type[256];
    int isNotNull;
    int isPrimaryKey;
    int isUnique;
} TableColumn;

// 表格
typedef struct {
    char name[256];
    TableColumn columns[MAX_COLUMNS];
    int columnCount;
    char data[MAX_ROWS][MAX_COLUMNS][256];  // 内存中简单表示，实际中可能需要更复杂的数据结构
    int rowCount;
} Table;

typedef struct {
    Table* tables;
    int tableCount;
} TableList;

// 初始化TableList
void initTableList(TableList *tableList);

// 销毁TableList
void destroyTableList(TableList *tableList);

// 添加Table到TableList
void addTable(TableList *tableList, Table table);

// 通过下标获取Table
Table* getTableByIndex(TableList *tableList, int index);

// 删除TableList中的Table
void removeTable(TableList *tableList, const char *tableName);

// 通过下标删除
void removeTableByIndex(TableList *tableList, int index);

// 从TableList中获取Table
Table* getTable(TableList *tableList, const char *tableName);

// 执行引擎
void executeEngine(TokenList *tokens, TableList *tableList);

void executeCreateTable(TokenList *tokens, TableList *tableList);

void executeInsertInto(TokenList *tokens, TableList *tableList);

void executeDeleteFrom(TokenList *tokens, TableList *tableList);

void executeUpdate(TokenList *tokens, TableList *tableList);

void executeSelect(TokenList *tokens, TableList *tableList);

void printError(const char *message);
#endif //SQL_MINUS_ENGINE_H
