#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Прототипы функций
void error();
void skip_spaces(const char **s);
int parse_number(const char **s);
int parse_factor(const char **s);
int parse_term(const char **s);
int parse_expression(const char **s);

// Функция для обработки ошибок
void error() {
    printf("error: unexpected symbol\n");
    exit(1);
}

// Функция для пропуска пробелов
void skip_spaces(const char **s) {
    while (isspace(**s)) (*s)++;
}

// Функция для разбора числа
int parse_number(const char **s) {
    int num = 0;
    while (isdigit(**s)) {
        num = num * 10 + (**s - '0');
        (*s)++;
    }
    return num;
}

// Функция для разбора выражения в скобках или числа
int parse_factor(const char **s) {
    skip_spaces(s);
    if (**s == '(') {
        (*s)++;
        int result = parse_expression(s);
        skip_spaces(s);
        if (**s != ')') error();
        (*s)++;
        return result;
    } else if (isdigit(**s)) {
        return parse_number(s);
    } else {
        error();
    }
}

// Функция для разбора умножения и деления
int parse_term(const char **s) {
    int result = parse_factor(s);
    skip_spaces(s);
    while (**s == '*' || **s == '/') {
        char op = **s;
        (*s)++;
        int next_factor = parse_factor(s);
        if (op == '*') {
            result *= next_factor;
        } else {
            if (next_factor == 0) {
                printf("error: division by zero\n");
                exit(1);
            }
            result /= next_factor;
        }
        skip_spaces(s);
    }
    return result;
}

// Функция для разбора сложения и вычитания
int parse_expression(const char **s) {
    int result = parse_term(s);
    skip_spaces(s);
    while (**s == '+' || **s == '-') {
        char op = **s;
        (*s)++;
        int next_term = parse_term(s);
        if (op == '+') {
            result += next_term;
        } else {
            result -= next_term;
        }
        skip_spaces(s);
    }
    return result;
}

int main() {
    char input[1024];
    if (fgets(input, sizeof(input), stdin)) {
        const char *s = input;
        int result = parse_expression(&s);
        skip_spaces(&s);
        if (*s != '\0' && *s != '\n') {
            error();
        }
        printf("%d\n", result);
    }
    return 0;
}
