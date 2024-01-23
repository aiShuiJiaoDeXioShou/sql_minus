#include "lexer.h"

#define MAX_STATEMENT_LENGTH 1024
char *KEY_WORD[KEY_WORD_SIZE] = {"INT", "CHAR", "CREATE", "TABLE", "PRIMARY", "KEY",
                                 "NOT", "NULL", "UNIQUE", "INSERT", "INTO", "VALUES",
                                 "DELETE", "FROM", "WHERE", "UPDATE", "ORDER", "BY",
                                 "ASC", "DESC", "AND", "OR", "BETWEEN", "IS",
                                 "SET","ORDER", "BY", "ASC", "DESC","SELECT"};

// 初始化 TokenList
TokenList initTokenList() {
    TokenList list;
    list.tokens = (Token *) malloc(sizeof(Token));
    list.size = 0;
    return list;
};

// 销毁 TokenList
void destroyTokenList(TokenList *list) {
    free(list->tokens);
    list->tokens = NULL;
    list->size = 0;
}


// 添加 Token 到 TokenList
void addToken(TokenList *list, Token token) {
    list->tokens = realloc(list->tokens, (list->size + 1) * sizeof(Token));
    if (list->tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->tokens[list->size++] = token;
}

// 判断一个字符串是否为关键字
int isKeyword(const char *str) {
    for (int i = 0; i < KEY_WORD_SIZE; i++) {
        if (strcmp(str, KEY_WORD[i]) == 0) {
            return 1;
        }
    }
    return 0;
}


// 词法分析器函数实现
TokenList tokenize(char *input) {
    // 初始化 TokenList
    TokenList list = initTokenList();
    // 获取input里面的每一个字符
    char *p = input;
    while (*p != '\0') {
        // 跳过空白字符
        if (isspace(*p)) {
            p++;
            continue;
        }
        // 处理数字
        if (isdigit(*p)) {
            Token token;
            token.type = LITERAL;
            char *q = token.value;
            while (isdigit(*p)) {
                *q++ = *p++;
            }
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 处理标识符
        if (isalpha(*p) || *p == '_') {
            Token token;
            token.type = IDENTIFIER;
            char *q = token.value;
            while (isalnum(*p) || *p == '_') {
                *q++ = *p++;
            }
            *q = '\0';
            // 判断是否为关键字
            if (isKeyword(token.value)) {
                token.type = KEYWORD;
            }
            addToken(&list, token);
            continue;
        }
        // 处理运算符
        if (*p == '=' || *p == '<' || *p == '>') {
            Token token;
            token.type = OPERATOR;
            char *q = token.value;
            *q++ = *p++;
            if (*p == '=') {
                *q++ = *p++;
            }
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 处理分号
        if (*p == ';') {
            Token token;
            token.type = SEMICOLON;
            char *q = token.value;
            *q++ = *p++;
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 处理(
        if (*p == '(') {
            Token token;
            token.type = LEFT_BRACKET;
            char *q = token.value;
            *q++ = *p++;
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 处理)
        if (*p == ')') {
            Token token;
            token.type = RIGHT_BRACKET;
            char *q = token.value;
            *q++ = *p++;
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 处理,
        if (*p == ',') {
            Token token;
            token.type = COMMA;
            char *q = token.value;
            *q++ = *p++;
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 处理 *
        if (*p == '*') {
            Token token;
            token.type = OPERATOR;
            char *q = token.value;
            *q++ = *p++;
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 处理 ' 处理字符串
        if (*p == '\'') {
            Token token;
            token.type = STRING;
            char *q = token.value;
            *q++ = *p++;
            *q = '\0';
            addToken(&list, token);
            continue;
        }
        // 未知字符
        fprintf(stderr, "Unknown character %c\n", *p);
        exit(EXIT_FAILURE);
    }
    return list;
}

// 输入一个文件, 使用;分割利用tokenize函数解析每一条语句, 返回TokenList
TokenList* parseFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_STATEMENT_LENGTH];
    char statement[MAX_STATEMENT_LENGTH * 10] = "";
    while (fgets(line, sizeof(line), file) != NULL) {
        strcat(statement, line);
    }

    fclose(file);

    char* tokenStr;
    TokenList* tokenLists = NULL;
    int size = 0;

    tokenStr = strtok(statement, ";");
    while (tokenStr != NULL) {
        tokenLists = realloc(tokenLists, (size + 1) * sizeof(TokenList));
        if (tokenLists == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        tokenLists[size++] = tokenize(tokenStr);
        tokenStr = strtok(NULL, ";");
    }

    return tokenLists;
}