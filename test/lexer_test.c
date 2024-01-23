#include "../lexer.h"

void test_tokenize(char* input) {
    printf("Testing tokenize with input: %s\n", input);
    printf("coding... \n");
    TokenList list = tokenize(input);
    for (int i = 0; i < list.size; i++) {
        Token token = list.tokens[i];
        printf("Token %d: type = %d, value = %s\n", i, token.type, token.value);
    }
    printf("\n");
}

// gcc -o lexer_test lexer_test.c lexer.c
int main() {
//     test_tokenize("CREATE TABLE students (\n"
//                   "\tid INT PRIMARY KEY, \n"
//                   "\tname CHAR(30)\n"
//                   ");");
//     test_tokenize("DELETE FROM students WHERE id = 1;");
//     test_tokenize("DELETE FROM students WHERE id = 1;");
//     test_tokenize("UPDATE students SET name = 'Bob' WHERE id = 1;");
     test_tokenize("SELECT * FROM students ORDER BY id ASC;");
     test_tokenize("SELECT * FROM students WHERE id = 1;");
//     // 测试TokenList* parseFile(const char* filename)
//     printf("Testing parseFile with input: test.sql\n");
//     TokenList* list = parseFile("test.sql");
//     // list是一个双链表吧
//     for (int i = 0; i < 5; i++) {
//         TokenList t = list[i];
//         for (int j = 0; j < t.size; ++j) {
//             Token token = t.tokens[j];
//             printf("Token %d: type = %d, value = %s\n", j, token.type, token.value);
//         }
//         printf("\n");
//     }
     return 0;
}