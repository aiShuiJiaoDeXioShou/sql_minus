//
// Created by 28322 on 2024/1/22.
// 语法执行引擎

#include "engine.h"

// 判断字段位于表单的哪一行
int getTableColumnIndex(const Table *table, const char *literal) {
    int columnIndex;
    for (int i = 0; i < table->columnCount; i++) {
        if (strcmp(table->columns[i].name, literal) == 0) {
            columnIndex = i;
            break;
        }
    }
    return columnIndex;
}

// 根据解析结果提供不同的操作
void charOrNumber(TokenList *tokens, char *value) {
    if (tokens->tokens[0].type == LITERAL) {
        strcpy(value, tokens->tokens[0].value);
    } else if (tokens->tokens[0].type == STRING) {
        // 跳过一个单引号
        tokens->tokens++;
        // 赋值且跳过字符串
        strcpy(value, tokens->tokens[0].value);
        // 跳过一个单引号
        tokens->tokens++;
    }
}

void initTableList(TableList *tableList) {
    tableList->tableCount = 0;
    tableList->tables = (Table *) malloc(sizeof(Table));
}

void executeEngine(TokenList *tokens, TableList *tableList) {
    // 根据语法树执行相应的操作
    switch (tokens->tokens[0].type) {
        case KEYWORD:
            if (strcmp(tokens->tokens[0].value, "CREATE") == 0) {
                executeCreateTable(tokens, tableList);
            } else if (strcmp(tokens->tokens[0].value, "INSERT") == 0) {
                executeInsertInto(tokens, tableList);
            } else if (strcmp(tokens->tokens[0].value, "DELETE") == 0) {
                executeDeleteFrom(tokens, tableList);
            } else if (strcmp(tokens->tokens[0].value, "UPDATE") == 0) {
                executeUpdate(tokens, tableList);
            } else if (strcmp(tokens->tokens[0].value, "SELECT") == 0) {
                executeSelect(tokens, tableList);
            } else {
                // printError("Unsupported operation");
                printError("ERROR");
            }
            break;
        default:
            printError("ERROR");
    }
}

// 销毁TableList
void destroyTableList(TableList *tableList) {
    free(tableList->tables);
    tableList->tables = NULL;
    tableList->tableCount = 0;
}

// 添加Table到TableList
void addTable(TableList *tableList, Table table) {
    tableList->tables = realloc(tableList->tables, (tableList->tableCount + 1) * sizeof(Table));
    if (tableList->tables == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    tableList->tables[tableList->tableCount++] = table;
}

// 通过下标获取Table
Table *getTableByIndex(TableList *tableList, int index) {
    return &tableList->tables[index];
}

// 删除TableList中的Table
void removeTable(TableList *tableList, const char *tableName) {
    for (int i = 0; i < tableList->tableCount; i++) {
        if (strcmp(tableList->tables[i].name, tableName) == 0) {
            removeTableByIndex(tableList, i);
            break;
        }
    }
}

// 通过下标删除
void removeTableByIndex(TableList *tableList, int index) {
    for (int i = index; i < tableList->tableCount - 1; i++) {
        tableList->tables[i] = tableList->tables[i + 1];
    }
    tableList->tableCount--;
}

// 从TableList中获取Table
Table *getTable(TableList *tableList, const char *tableName) {
    for (int i = 0; i < tableList->tableCount; i++) {
        if (strcmp(tableList->tables[i].name, tableName) == 0) {
            return &tableList->tables[i];
        }
    }
    return NULL;
}

// 打印一个表格 使用\t 分隔
void printTable(Table *table) {
    for(int i=0;i<table->columnCount;i++){
        printf("%s\t",table->columns[i].name);
    }
    printf("\n");
    // 打印table所有的数据 table->data
    for (int i = 0; i < table->rowCount; i++) {
        for (int j = 0; j < table->columnCount; j++) {
            printf("%s\t", table->data[i][j]);
        }
        printf("\n");
    }
}

// 判断一个元素是不是在数组里面
int isInArray(int *array, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1;
        }
    }
    return 0;
}

// 打印指定行列的数据
void printData(Table *table, int* row, int* column) {
    for(int i=0;i<table->columnCount;i++){
        printf("%s\t",table->columns[i].name);
    }
    printf("\n");
    for (int i = 0; i < table->rowCount; i++) {
        // 判断当前行是否在row数组里面
        if (isInArray(row, sizeof(row) / sizeof(int), i)) {
            for (int j = 0; j < table->columnCount; j++) {
                // 判断当前列是否在column数组里面
                if (isInArray(column, sizeof(column) / sizeof(int), j)) {
                    printf("%s\t", table->data[i][j]);
                }
            }
            printf("\n");
        }
    }
}

void executeCreateTable(TokenList *tokens, TableList *tableList) {
    // 跳过 CREATE TABLE
    tokens->tokens++;
    tokens->tokens++;

    // 获取表格名
    char tableName[256];
    strcpy(tableName, tokens->tokens[0].value);

    // 查找表格是否已经存在
    if (getTable(tableList, tableName) != NULL) {
        printError("Table already exists");
        return;
    }

    // 创建新表格
    Table newTable;
    strcpy(newTable.name, tableName);
    newTable.columnCount = 0;
    newTable.rowCount = 0;

    // 跳过表格名
    tokens->tokens++;

    // 跳过左括号
    tokens->tokens++;

    // 逐个解析列定义
    while (tokens->tokens[0].type != SEMICOLON) {
        // 获取列名
        TableColumn newColumn;
        strcpy(newColumn.name, tokens->tokens[0].value);
        // 跳过列名
        tokens->tokens++;

        // 获取列类型
        // 判断列类型是否为INT或CHAR
        if (strcmp(tokens->tokens[0].value, "INT") == 0) {
            strcpy(newColumn.type, tokens->tokens[0].value);
        } else {
            strcpy(newColumn.type, tokens->tokens[0].value);
            tokens->tokens++;
            strcat(newColumn.type, "(");
            tokens->tokens++;
            strcat(newColumn.type, tokens->tokens[0].value);
            tokens->tokens++;
            strcat(newColumn.type, ")");
        }
        // 跳过列类型
        tokens->tokens++;

        // 处理列属性（例如 NOT NULL、PRIMARY KEY、UNIQUE）
        newColumn.isNotNull = 0;
        newColumn.isPrimaryKey = 0;
        newColumn.isUnique = 0;

        while (tokens->tokens[0].type != COMMA && tokens->tokens[0].type != SEMICOLON) {
            if (strcmp(tokens->tokens[0].value, "NOT") == 0) {
                newColumn.isNotNull = 1;
            } else if (strcmp(tokens->tokens[0].value, "PRIMARY") == 0) {
                newColumn.isPrimaryKey = 1;
            } else if (strcmp(tokens->tokens[0].value, "UNIQUE") == 0) {
                newColumn.isUnique = 1;
            }

            // 跳过属性
            tokens->tokens++;
        }

        // 将新列添加到表格
        newTable.columns[newTable.columnCount++] = newColumn;

        if (tokens->tokens[0].type == COMMA) {
            // 跳过逗号
            tokens->tokens++;
        }
    }

    // 将新表格添加到表格列表
    addTable(tableList, newTable);
    printf("CREATE TABLE SUCCESSFULLY\n");
}

void executeInsertInto(TokenList *tokens, TableList *tableList) {
    // 跳过 INSERT INTO
    tokens->tokens++;
    tokens->tokens++;

    // 获取表名
    char tableName[256];
    strcpy(tableName, tokens->tokens[0].value);

    // 查找表格
    Table *table = getTable(tableList, tableName);

    // 如果表格不存在，返回错误
    if (table == NULL) {
        printError("Table does not exist");
        return;
    }

    // 跳过表名和VALUES
    tokens->tokens += 2;

    // 解析插入的值
    char values[256][256];
    int valueCount = 0;
    while (tokens->tokens[0].type != SEMICOLON) {
        // 判断当前值是否为(
        while (tokens->tokens[0].type == LEFT_BRACKET || tokens->tokens[0].type == RIGHT_BRACKET ||
               tokens->tokens[0].type == COMMA) {
            tokens->tokens++;
        }
        // 判断当前值是否为字面量或者为字符串
        if (tokens->tokens[0].type == LITERAL) {
            strcpy(values[valueCount++], tokens->tokens[0].value);
            tokens->tokens++;
        } else if (tokens->tokens[0].type == STRING) {
            // 跳过一个单引号
            tokens->tokens++;
            // 赋值且跳过字符串
            strcpy(values[valueCount++], tokens->tokens[0].value);
            tokens->tokens++;
            // 跳过一个单引号
            tokens->tokens++;
        }
    }

    // 检查插入的值的数量是否与表的列数量匹配
    if (valueCount != table->columnCount) {
        printError("Value count does not match column count");
        return;
    }

    // 在表中插入新的记录
    for (int i = 0; i < valueCount; i++) {
        strcpy(table->data[table->rowCount][i], values[i]);
    }
    table->rowCount++;

    // 输出插入的记录数量
    printf("%d RECORDS INSERTED\n", 1);
}

void executeDeleteFrom(TokenList *tokens, TableList *tableList) {
    // 跳过 DELETE FROM
    tokens->tokens++;
    tokens->tokens++;

    // 获取表名
    char tableName[256];
    strcpy(tableName, tokens->tokens[0].value);

    // 查找表格
    Table *table = getTable(tableList, tableName);

    // 如果表格不存在，返回错误
    if (table == NULL) {
        printError("Table does not exist");
        return;
    }

    // 跳过表名和WHERE
    tokens->tokens += 2;

    // 获取约束条件
    char literal[256];
    strcpy(literal, tokens->tokens[0].value);
    tokens->tokens++;

    // 获取操作符
    char operator[256];
    strcpy(operator, tokens->tokens[0].value);
    tokens->tokens++;

    // 获取值
    char value[256];
    charOrNumber(tokens, value);

    // 判断字段位于表单的哪一行
    int columnIndex = getTableColumnIndex(table, literal);

    // 遍历表中的所有行
    int deletedCount = 0;
    for (int i = 0; i < table->rowCount; i++) {
        // 在这里添加应用条件子句的代码
        // 如果条件子句为真，删除这一行
        // 判断操作符 = > <
        if (strcmp(operator, "=") == 0) {
            if (strcmp(table->data[i][columnIndex], value) == 0) {
                // 删除这一行
                for (int j = i; j < table->rowCount - 1; j++) {
                    for (int k = 0; k < table->columnCount; k++) {
                        strcpy(table->data[j][k], table->data[j + 1][k]);
                    }
                }
                table->rowCount--;
                deletedCount++;
            }
        } else if (strcmp(operator, ">") == 0) {
            if (strcmp(table->data[i][columnIndex], value) > 0) {
                // 删除这一行
                for (int j = i; j < table->rowCount - 1; j++) {
                    for (int k = 0; k < table->columnCount; k++) {
                        strcpy(table->data[j][k], table->data[j + 1][k]);
                    }
                }
                table->rowCount--;
                deletedCount++;
            }
        } else if (strcmp(operator, "<") == 0) {
            if (strcmp(table->data[i][columnIndex], value) < 0) {
                // 删除这一行
                for (int j = i; j < table->rowCount - 1; j++) {
                    for (int k = 0; k < table->columnCount; k++) {
                        strcpy(table->data[j][k], table->data[j + 1][k]);
                    }
                }
                table->rowCount--;
                deletedCount++;
            }
        }
    }

    // 输出删除的记录数量
    printf("%d RECORDS DELETED\n", deletedCount);
}

void executeUpdate(TokenList *tokens, TableList *tableList) {
    // 跳过 UPDATE
    tokens->tokens++;
    // 获取表名
    char tableName[256];
    strcpy(tableName, tokens->tokens[0].value);

    // 查找表格
    Table *table = getTable(tableList, tableName);
    // 如果表格不存在，返回错误
    if (table == NULL) {
        printError("Table does not exist");
        return;
    }
    // 跳过表名和SET
    tokens->tokens += 2;

    // 解析字段名和新值
    char field[256];
    char value[256];
    strcpy(field, tokens->tokens[0].value);
    tokens->tokens += 2;  // 跳过字段名和等号
    // 判断是否为字符串或者字面量
    charOrNumber(tokens, value);
    tokens->tokens++;  // 跳过新值

    // 跳过WHERE
    tokens->tokens++;

    // 解析条件子句
    // 在这里添加解析条件子句的代码
    // 获取字面量
    char literal[256];
    strcpy(literal, tokens->tokens[0].value);
    tokens->tokens++;

    // 获取操作符
    char operator[256];
    strcpy(operator, tokens->tokens[0].value);
    tokens->tokens++;

    // 获取值
    char conditionValue[256];
    charOrNumber(tokens, conditionValue);

    // 判断条件字段位于表单的哪一行
    int columnIndex = getTableColumnIndex(table, literal);
    int updateColumnIndex = getTableColumnIndex(table, field);

    // 遍历表中的所有行
    int updatedCount = 0;
    for (int i = 0; i < table->rowCount; i++) {
        // 在这里添加应用条件子句的代码
        // 如果条件子句为真，更新这一行的指定字段
        // 判断操作符 = > <
        if (strcmp(operator, "=") == 0) {
            if (strcmp(table->data[i][columnIndex], conditionValue) == 0) {
                // 更新这一行的指定字段
                strcpy(table->data[i][updateColumnIndex], value);
                updatedCount++;
            }
        } else if (strcmp(operator, ">") == 0) {
            if (strcmp(table->data[i][columnIndex], conditionValue) > 0) {
                // 更新这一行的指定字段
                strcpy(table->data[i][updateColumnIndex], value);
                updatedCount++;
            }
        } else if (strcmp(operator, "<") == 0) {
            if (strcmp(table->data[i][columnIndex], conditionValue) < 0) {
                // 更新这一行的指定字段
                strcpy(table->data[i][updateColumnIndex], value);
                updatedCount++;
            }
        }
    }

    // 输出更新的记录数量
    printf("%d RECORDS UPDATED\n", updatedCount);
}

void executeSelect(TokenList *tokens, TableList *tableList) {
    // 跳过 SELECT
    tokens->tokens++;

    // 储存字段名
    char fields[256][256];
    int fieldCount = 0;

    // 判断是否为*
    if (strcmp(tokens->tokens[0].value, "*") == 0) {
        // 跳过*
        tokens->tokens++;
    } else {
        // 解析字段名
        while (tokens->tokens[0].type != KEYWORD) {
            strcpy(fields[fieldCount++], tokens->tokens[0].value);
            // 跳过当前字段
            tokens->tokens++;
            // 如果下一个token是逗号，跳过它
            if (tokens->tokens[0].type == COMMA) {
                tokens->tokens++;
            }
        }
    }
    // 跳过 FROM
    tokens->tokens++;

    // 获取表名
    char tableName[256];
    strcpy(tableName, tokens->tokens[0].value);

    // 查找表格
    Table *table = getTable(tableList, tableName);

    // 如果表格不存在，返回错误
    if (table == NULL) {
        printError("Table does not exist");
        return;
    }

    // 跳过表名
    tokens->tokens++;
    char literal[256];
    char operator[256];
    char value[256];
    int columnIndex;
    // 判断是否有WHERE
    if(strcmp(tokens->tokens[0].value, "WHERE")!=0) {
        tokens->tokens++;
    } else {
        // 跳过WHERE
        tokens->tokens++;
        // 获取字段名
        strcpy(literal, tokens->tokens[0].value);
        tokens->tokens++;
        // 获取操作符
        strcpy(operator, tokens->tokens[0].value);
        tokens->tokens++;
        // 获取值
        charOrNumber(tokens, value);
        // 获取字段位于表单的哪一行
        columnIndex = getTableColumnIndex(table, literal);
    }

    // fieldCount 为 0 时，输出所有字段
    if (!fieldCount) {
        // 输出表格的所有字段
        for (int i = 0; i < table->columnCount; i++) {
            printf("%s\t", table->columns[i].name);
        }
        printf("\n");
        // 遍历表中的所有行
        for (int i = 0; i < table->rowCount; i++) {
            // 在这里添加应用条件子句的代码
            // 如果条件子句为真，输出这一行
            // 判断操作符 = > <
            if (strcmp(operator, "=") == 0) {
                if (strcmp(table->data[i][columnIndex], value) == 0) {
                    // 输出这一行
                    for (int j = 0; j < table->columnCount; j++) {
                        printf("%s\t", table->data[i][j]);
                    }
                    printf("\n");
                }
            } else if (strcmp(operator, ">") == 0) {
                if (strcmp(table->data[i][columnIndex], value) > 0) {
                    // 输出这一行
                    for (int j = 0; j < table->columnCount; j++) {
                        printf("%s\t", table->data[i][j]);
                    }
                    printf("\n");
                }
            } else if (strcmp(operator, "<") == 0) {
                if (strcmp(table->data[i][columnIndex], value) < 0) {
                    // 输出这一行
                    for (int j = 0; j < table->columnCount; j++) {
                        printf("%s\t", table->data[i][j]);
                    }
                    printf("\n");
                }
            } else {
                // 说明没有操作符打印所有
                for (int f = 0; f < table->rowCount; f++) {
                    for (int j = 0; j < table->columnCount; j++) {
                        printf("%s\t", table->data[f][j]);
                    }
                    printf("\n");
                }
            }
        }
    }
    // fieldCount 不为 0 时，输出指定字段
    else {
        // 输出指定的字段
        for (int i = 0; i < fieldCount; i++) {
            printf("%s\t", fields[i]);
        }
        printf("\n");
        // 遍历表中的所有行
        for (int i = 0; i < table->rowCount; i++) {
            // 在这里添加应用条件子句的代码
            // 如果条件子句为真，输出这一行
            // 判断操作符 = > <
            if (strcmp(operator, "=") == 0) {
                if (strcmp(table->data[i][columnIndex], value) == 0) {
                    // 输出这一行
                    for (int j = 0; j < fieldCount; j++) {
                        printf("%s\t", table->data[i][getTableColumnIndex(table, fields[j])]);
                    }
                    printf("\n");
                }
            } else if (strcmp(operator, ">") == 0) {
                if (strcmp(table->data[i][columnIndex], value) > 0) {
                    // 输出这一行
                    for (int j = 0; j < fieldCount; j++) {
                        printf("%s\t", table->data[i][getTableColumnIndex(table, fields[j])]);
                    }
                    printf("\n");
                }
            } else if (strcmp(operator, "<") == 0) {
                if (strcmp(table->data[i][columnIndex], value) < 0) {
                    // 输出这一行
                    for (int j = 0; j < fieldCount; j++) {
                        printf("%s\t", table->data[i][getTableColumnIndex(table, fields[j])]);
                    }
                    printf("\n");
                }
            } else {
                // 说明没有该操作符打印所有
                for (int f = 0; f < table->rowCount; f++) {
                    for (int j = 0; j < fieldCount; j++) {
                        printf("%s\t", table->data[f][getTableColumnIndex(table, fields[j])]);
                    }
                    printf("\n");
                }
            }
        }
    }
}
