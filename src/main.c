#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Глобальная переменная для режима --float
int float_mode = 0;

// Прототипы функций
void error();
void skip_spaces(const char** s);
void validate_input(const char* s);
double parse_number(const char** s);
double parse_factor(const char** s);
double parse_term(const char** s);
double parse_expression(const char** s);

// Функция для обработки ошибок
void error()
{
    printf("error: invalid expression or unexpected symbol\n");
    exit(1);
}

// Функция для пропуска пробелов
void skip_spaces(const char** s)
{
    while (isspace(**s))
        (*s)++;
}

// Функция для проверки допустимых символов
void validate_input(const char* s)
{
    while (*s) {
        if (!isdigit(*s) && *s != '(' && *s != ')' && *s != '*' && *s != '+' && *s != '/' && *s != '-' && !isspace(*s)) {
            error();
        }
        s++;
    }
}

// Функция для разбора числа
double parse_number(const char** s)
{
    double num = 0;
    while (isdigit(**s)) {
        num = num * 10 + (**s - '0');
        (*s)++;
    }
    return num;
}

// Функция для разбора выражения в скобках или числа
double parse_factor(const char** s)
{
    skip_spaces(s);
    if (**s == '(') {
        (*s)++;
        double result = parse_expression(s);
        skip_spaces(s);
        if (**s != ')')
            error();
        (*s)++;
        return result;
    } else if (isdigit(**s)) {
        return parse_number(s);
    } else {
        error();
        return 0;
    }
}

// Функция для разбора умножения и деления
double parse_term(const char** s)
{
    double result = parse_factor(s);
    skip_spaces(s);
    while (**s == '*' || **s == '/') {
        char op = **s;
        (*s)++;
        double next_factor = parse_factor(s);
        if (op == '*') {
            result *= next_factor;
        } else {
            if (fabs(next_factor) < 1e-9) {
                printf("error: division by zero or too small number\n");
                exit(1);
            }
            if (float_mode) {
                result /= next_factor;
            } else {
                result = (int)(result / next_factor);
            }
        }
        skip_spaces(s);
    }
    return result;
}

// Функция для разбора сложения и вычитания
double parse_expression(const char** s)
{
    double result = parse_term(s);
    skip_spaces(s);
    while (**s == '+' || **s == '-') {
        char op = **s;
        (*s)++;
        double next_term = parse_term(s);
        if (op == '+') {
            result += next_term;
        } else {
            result -= next_term;
        }
        skip_spaces(s);
    }
    return result;
}

#ifndef TEST_MODE
int main(int argc, char* argv[])
{
    // Проверка флага --float
    if (argc == 2 && strcmp(argv[1], "--float") == 0) {
        float_mode = 1;
    } else if (argc > 1) {
        fprintf(stderr, "error: invalid flag\n");
        return 1;
    }

    char input[1024];
    if (fgets(input, sizeof(input), stdin)) {
        // Проверка допустимых символов
        validate_input(input);

        const char* s = input;
        double result = parse_expression(&s);
        skip_spaces(&s);

        // Проверка, что весь ввод был обработан
        if (*s != '\0' && *s != '\n') {
            error();
        }

        // Вывод результата
        if (float_mode) {
            printf("%.6f\n", result); // Вывод с точностью до 6 знаков
        } else {
            printf("%d\n", (int)result); // Вывод целого числа
        }
    }
    return 0;
}
#endif
