#include <stdbool.h>
#include "parser.h"

void printError(const char *message) {
    printf("%s\n", message);
}

// 判断末尾是否有分号
bool hasSemicolon(TokenList tokens) {
    if (tokens.size == 0) {
        return false;
    }
    return tokens.tokens[tokens.size - 1].type == SEMICOLON && strcmp(tokens.tokens[tokens.size - 1].value, ";") == 0;
}

bool checkToken(Token token, int type, const char *value) {
    return token.type == type && strcmp(token.value, value) == 0;
}

bool checkTokenType(Token token, int type) {
    return token.type == type;
}

bool parse(TokenList tokens) {
    if (tokens.size == 0) {
        return false;
    }
    if (!checkToken(tokens.tokens[0], KEYWORD, "CREATE") &&
        !checkToken(tokens.tokens[0], KEYWORD, "INSERT") &&
        !checkToken(tokens.tokens[0], KEYWORD, "DELETE") &&
        !checkToken(tokens.tokens[0], KEYWORD, "UPDATE") &&
        !checkToken(tokens.tokens[0], KEYWORD, "SELECT")) {
        printError("SYNTAX ERROR");
        return false;
    }
    if (checkToken(tokens.tokens[0], KEYWORD, "CREATE") && tokens.size > 1 &&
        checkToken(tokens.tokens[1], KEYWORD, "TABLE")) {
        return parseCreateTable(tokens);
    } else if (checkToken(tokens.tokens[0], KEYWORD, "INSERT") && tokens.size > 1 &&
               checkToken(tokens.tokens[1], KEYWORD, "INTO")) {
        return parseInsertInto(tokens);
    } else if (checkToken(tokens.tokens[0], KEYWORD, "DELETE") && tokens.size > 1 &&
               checkToken(tokens.tokens[1], KEYWORD, "FROM")) {
        return parseDeleteFrom(tokens);
    } else if (checkToken(tokens.tokens[0], KEYWORD, "UPDATE")) {
        return parseUpdate(tokens);
    } else if (checkToken(tokens.tokens[0], KEYWORD, "SELECT")) {
        return parseSelect(tokens);
    } else {
        printError("SYNTAX ERROR");
        return false;
    }
}

// 数据类型判断函数
bool checkDataType(Token token) {
    if (token.type != KEYWORD) {
        return false;
    }
    if (strcmp(token.value, "INT") == 0) {
        return true;
    } else if (strcmp(token.value, "CHAR") == 0) {
        return true;
    } else {
        return false;
    }
}

bool parseCreateTable(TokenList tokens) {
    if (tokens.size < 4) {
        // printError("Incomplete CREATE TABLE statement");
        printError("SYNTAX ERROR");
        return false;
    }

    if (tokens.tokens[2].type != IDENTIFIER) {
        // printError("Expected table name after CREATE TABLE");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkToken(tokens.tokens[3], LEFT_BRACKET, "(")) {
        // printError("Expected '(' after table name");
        printError("SYNTAX ERROR");
        return false;
    }

    int i = 4;
    int primaryKeyNumber = 0;
    while (i < tokens.size - 1 &&
           (tokens.tokens[i].type != RIGHT_BRACKET || strcmp(tokens.tokens[i].value, ")") != 0)) {
        if (tokens.tokens[i].type != IDENTIFIER) {
            // printError("Expected column name in column definition");
            printError("SYNTAX ERROR");
            return false;
        }
        i++;

        // 判断是否是数据类型
        if (!checkDataType(tokens.tokens[i])) {
            // printError("Expected data type in column definition");
            printError("SYNTAX ERROR");
            return false;
        }

        // 判断类型
        while (i < tokens.size && tokens.tokens[i].type == KEYWORD) {
            // 定义int数据类型
            if (strcmp(tokens.tokens[i].value, "INT") == 0) {
                i++;
            }
                // 定义char() 数据类型
            else if (tokens.tokens[i].type == KEYWORD && strcmp(tokens.tokens[i].value, "CHAR") == 0) {
                i++;
                if (tokens.tokens[i].type == LEFT_BRACKET || strcmp(tokens.tokens[i].value, "(") == 0) {
                    i++;
                    // 字面量的值不能大于256
                    if (tokens.tokens[i].type == LITERAL) {
                        if (atoi(tokens.tokens[i].value) > 256) {
                            // printError("Expected a number less than 256 after CHAR(");
                            printError("SYNTAX ERROR");
                            return false;
                        }
                        i++;
                    } else {
                        // printError("Expected a number after CHAR(");
                        printError("SYNTAX ERROR");
                        return false;
                    }
                    if (i >= tokens.size || tokens.tokens[i].type != RIGHT_BRACKET ||
                        strcmp(tokens.tokens[i].value, ")") != 0) {
                        // printError("Expected ')' at the end of column definition");
                        printError("SYNTAX ERROR");
                        return false;
                    }
                    i++;
                } else {
                    // printError("Expected '(' after CHAR");
                    printError("SYNTAX ERROR");
                    return false;
                }
            } else {
                break;
            }
        }
        // 判断其他关键字
        while (i < tokens.size && tokens.tokens[i].type == KEYWORD) {
            if (strcmp(tokens.tokens[i].value, "PRIMARY") == 0) {
                i++;
                if (i >= tokens.size || tokens.tokens[i].type != KEYWORD ||
                    strcmp(tokens.tokens[i].value, "KEY") != 0) {
                    // printError("Expected KEY after PRIMARY");
                    printError("SYNTAX ERROR");
                    return false;
                }
                i++;
                // 判断是否有多个主键,如果有多个主键, 那么就是错误的
                if (primaryKeyNumber > 0) {
                    // printError("Only one PRIMARY KEY is allowed");
                    printError("ERROR");
                    return false;
                }
                primaryKeyNumber++;
            } else if (strcmp(tokens.tokens[i].value, "NOT") == 0) {
                i++;
                if (i >= tokens.size || tokens.tokens[i].type != KEYWORD ||
                    strcmp(tokens.tokens[i].value, "NULL") != 0) {
                    // printError("Expected NULL after NOT");
                    printError("SYNTAX ERROR");
                    return false;
                }
                i++;
            } else if (strcmp(tokens.tokens[i].value, "UNIQUE") == 0) {
                i++;
            } else {
                // printError("Unknown keyword in column definition");
                printError("SYNTAX ERROR");
                return false;
            }
        }
        i++;
    }

    if (!hasSemicolon(tokens)) {
        // printError("Expected ';' at the end of the statement");
        printError("SYNTAX ERROR");
        return false;
    }
    return true;
}

bool parseInsertInto(TokenList tokens) {
    if (tokens.size < 5) {
        // printError("Incomplete INSERT INTO statement");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkToken(tokens.tokens[0], KEYWORD, "INSERT") ||
        !checkToken(tokens.tokens[1], KEYWORD, "INTO")) {
        // printError("Expected INSERT INTO at the start of the statement");
        printError("SYNTAX ERROR");
        return false;
    }

    if (tokens.tokens[2].type != IDENTIFIER) {
        // printError("Expected table name after INSERT INTO");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkToken(tokens.tokens[3], KEYWORD, "VALUES")) {
        // printError("Expected VALUES after table name");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkToken(tokens.tokens[4], LEFT_BRACKET, "(")) {
        // printError("Expected '(' after VALUES");
        printError("SYNTAX ERROR");
        return false;
    }

    int i = 5;
    while (i < tokens.size - 2) {
        if (!checkTokenType(tokens.tokens[i], LITERAL) && !checkTokenType(tokens.tokens[i], STRING)) {
            // printError("Expected a value in the value list");
            printError("SYNTAX ERROR");
            return false;
        }
            // 判断token是不是字符串
        else if (checkTokenType(tokens.tokens[i], STRING)) {
            i++;
            // 字符串的长度不能大于256
            if (strlen(tokens.tokens[i].value) > 256) {
                // printError("Expected a string less than 256");
                printError("SYNTAX ERROR");
                return false;
            }
            i++;
        }
        i++;
        if (i < tokens.size - 2) {
            if (!checkToken(tokens.tokens[i], COMMA, ",")) {
                //printError("Expected ',' after a value in the value list");
                printError("SYNTAX ERROR");
                return false;
            }
            i++;
        }
    }

    if (!checkToken(tokens.tokens[i], RIGHT_BRACKET, ")")) {
        // printError("Expected ')' the statement");
        printError("SYNTAX ERROR");
        return false;
    }
    if (!hasSemicolon(tokens)) {
        // printError("Expected ';' at the end of the statement");
        printError("SYNTAX ERROR");
        return false;
    }
    return true;
}

bool parseDeleteFrom(TokenList tokens) {
    if (tokens.size < 5) {
        // printError("Incomplete DELETE FROM statement");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkToken(tokens.tokens[0], KEYWORD, "DELETE") ||
        !checkToken(tokens.tokens[1], KEYWORD, "FROM")) {
        // printError("Expected DELETE FROM at the start of the statement");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkTokenType(tokens.tokens[2], IDENTIFIER)) {
        // printError("Expected table name after DELETE FROM");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkToken(tokens.tokens[3], KEYWORD, "WHERE")) {
        // printError("Expected WHERE after table name");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkTokenType(tokens.tokens[4], IDENTIFIER) ||
        !checkTokenType(tokens.tokens[5], OPERATOR)) {
        // printError("Expected a valid expression after WHERE");
        printError("SYNTAX ERROR");
        return false;
    }

    // 判断token是不是字符串 或者 数字
    if (checkTokenType(tokens.tokens[6], STRING) || checkTokenType(tokens.tokens[6], LITERAL)) {
        // 判断token是不是字符串
        if (checkTokenType(tokens.tokens[6], STRING)) {
            // 字符串的长度不能大于256
            if (strlen(tokens.tokens[6].value) > 256) {
                // printError("Expected a string less than 256");
                printError("SYNTAX ERROR");
                return false;
            }
        }
            // 判断token是不是数字
        else if (checkTokenType(tokens.tokens[6], LITERAL)) {
            // 字面量的值不能大于256
            if (atoi(tokens.tokens[6].value) > 256) {
                // printError("Expected a number less than 256");
                printError("SYNTAX ERROR");
                return false;
            }
        }
    } else {
        // printError("Expected a valid expression after WHERE");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!hasSemicolon(tokens)) {
        // printError("Expected ';' at the end of the statement");
        printError("SYNTAX ERROR");
        return false;
    }
    return true;
}

bool parseUpdate(TokenList tokens) {
    if (tokens.size < 7) {
        // printError("Incomplete UPDATE statement");
        printError("SYNTAX ERROR");
        return false;
    }
    int i = 0;
    if (!checkToken(tokens.tokens[i], KEYWORD, "UPDATE")) {
        // printError("Expected UPDATE at the start of the statement");
        printError("SYNTAX ERROR");
        return false;
    }
    i++;

    if (!checkTokenType(tokens.tokens[i], IDENTIFIER)) {
        // printError("Expected table name after UPDATE");
        printError("SYNTAX ERROR");
        return false;
    }
    i++;

    if (!checkToken(tokens.tokens[i], KEYWORD, "SET")) {
        // printError("Expected SET after table name");
        printError("SYNTAX ERROR");
        return false;
    }
    i++;

    if (!checkTokenType(tokens.tokens[i], IDENTIFIER) || !checkTokenType(tokens.tokens[i + 1], OPERATOR)) {
        // printError("Expected a valid expression after SET");
        printError("SYNTAX ERROR");
        return false;
    }
    i += 2;

    // 判断token是不是字符串 或者 数字
    if (checkTokenType(tokens.tokens[i], STRING) || checkTokenType(tokens.tokens[i], LITERAL)) {
        // 判断token是不是字符串
        if (tokens.tokens[i].type == STRING) {
            // 字符串的长度不能大于256
            if (strlen(tokens.tokens[i + 1].value) > 256) {
                // printError("Expected a string less than 256");
                printError("SYNTAX ERROR");
                return false;
            }
            i += 3;
        }
            // 判断token是不是数字
        else if (tokens.tokens[i].type == LITERAL) {
            // 字面量的值不能大于256
            if (atoi(tokens.tokens[i].value) > 256) {
                // printError("Expected a number less than 256");
                printError("SYNTAX ERROR");
                return false;
            }
            i++;
        }
    } else {
        // printError("Expected a valid expression after SET");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!checkToken(tokens.tokens[i], KEYWORD, "WHERE")) {
        // printError("Expected WHERE after SET");
        printError("SYNTAX ERROR");
        return false;
    }
    i++;

    if (!checkTokenType(tokens.tokens[i], IDENTIFIER) || !checkTokenType(tokens.tokens[i + 1], OPERATOR)) {
        // printError("Expected a valid expression after WHERE");
        printError("SYNTAX ERROR");
        return false;
    }
    i += 2;

    // 判断token是不是字符串 或者 数字
    if (checkTokenType(tokens.tokens[i], STRING) || checkTokenType(tokens.tokens[i], LITERAL)) {
        // 判断token是不是字符串
        if (tokens.tokens[i].type == STRING) {
            // 字符串的长度不能大于256
            if (strlen(tokens.tokens[i].value) > 256) {
                // printError("Expected a string less than 256");
                printError("SYNTAX ERROR");
                return false;
            }
            i += 3;
        }
            // 判断token是不是数字
        else if (tokens.tokens[i].type == LITERAL) {
            // 字面量的值不能大于256
            if (atoi(tokens.tokens[i].value) > 256) {
                // printError("Expected a number less than 256");
                printError("SYNTAX ERROR");
                return false;
            }
            i++;
        }
    } else {
        // printError("Expected a valid expression after WHERE");
        printError("SYNTAX ERROR");
        return false;
    }

    if (!hasSemicolon(tokens)) {
        // printError("Expected ';' at the end of the statement");
        printError("SYNTAX ERROR");
        return false;
    }
    return true;
}

bool checkConditionClause(TokenList tokens, int *i) {
    if (*i >= tokens.size || tokens.tokens[*i].type != IDENTIFIER) {
        return false;
    }

    (*i)++;

    if (*i >= tokens.size || tokens.tokens[*i].type != OPERATOR) {
        return false;
    }

    (*i)++;

    if (*i >= tokens.size || (tokens.tokens[*i].type != LITERAL && tokens.tokens[*i].type != STRING)) {
        return false;
    }

    (*i)++;

    return true;
}

bool checkOrderByClause(TokenList tokens, int *i) {
    if (*i >= tokens.size || tokens.tokens[*i].type != IDENTIFIER) {
        return false;
    }

    (*i)++;

    if (*i >= tokens.size ||
        (strcmp(tokens.tokens[*i].value, "ASC") != 0 && strcmp(tokens.tokens[*i].value, "DESC") != 0)) {
        return false;
    }

    (*i)++;

    return true;
}

bool parseSelect(TokenList tokens) {
    if (tokens.size < 4) {
        printError("SYNTAX ERROR");
        return false;
    }
    int i = 0;

    if (tokens.tokens[i].type != KEYWORD || strcmp(tokens.tokens[0].value, "SELECT") != 0) {
        printError("SYNTAX ERROR");
        return false;
    }
    i++;

    if (tokens.tokens[i].type != OPERATOR || strcmp(tokens.tokens[i].value, "*") != 0) {
        // 判断token是不是标识符, 如果不是标识符, 那么就是语法错误
        if (tokens.tokens[i].type != IDENTIFIER) {
            printError("SYNTAX ERROR");
            return false;
        }
        // 跳过所有的标识符和逗号
        while (tokens.tokens[i].type == IDENTIFIER ||
                tokens.tokens[i].type == COMMA) {
            i++;
        }
    } else {
        i++;
    }

    if (tokens.tokens[i].type != KEYWORD || strcmp(tokens.tokens[i].value, "FROM") != 0) {
        printError("SYNTAX ERROR");
        return false;
    }
    i++;

    if (tokens.tokens[i].type != IDENTIFIER) {
        printError("SYNTAX ERROR");
        return false;
    }
    i++;

    if (checkToken(tokens.tokens[i], SEMICOLON, ";")) {
        return true;
    }
    while (i < tokens.size - 1) {
        if (checkToken(tokens.tokens[i], KEYWORD, "WHERE")) {
            // Check condition clause
            i++;
            if (!checkConditionClause(tokens, &i)) {
                printError("SYNTAX ERROR");
                return false;
            }
        } else if (checkToken(tokens.tokens[i], KEYWORD, "ORDER")
                   && checkToken(tokens.tokens[i + 1], KEYWORD, "BY")) {
            // Check order by clause
            i += 2;
            if (!checkOrderByClause(tokens, &i)) {
                printError("SYNTAX ERROR");
                return false;
            }
        } else {
            printError("SYNTAX ERROR");
            return false;
        }
    }

    if (!hasSemicolon(tokens)) {
        printError("SYNTAX ERROR");
        return false;
    }
    return true;
}