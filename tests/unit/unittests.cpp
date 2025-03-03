#include <cmath>
#include <cstdlib>
#include <cstring>
#include <gtest/gtest.h>

// Включаем функции из main.c
extern "C" {
void skip_spaces(const char** s);
void validate_input(const char* s);
double parse_number(const char** s);
double parse_factor(const char** s);
double parse_term(const char** s);
double parse_expression(const char** s);
}

// Тесты для skip_spaces
TEST(SkipSpacesTest, SkipsLeadingSpaces)
{
    const char* input = "   123";
    skip_spaces(&input);
    EXPECT_EQ(*input, '1');
}

TEST(SkipSpacesTest, HandlesNoSpaces)
{
    const char* input = "123";
    skip_spaces(&input);
    EXPECT_EQ(*input, '1');
}

// Тесты для validate_input
TEST(ValidateInputTest, ValidInput)
{
    const char* input = "123 + 456 * (789 - 10)";
    EXPECT_NO_THROW(validate_input(input));
}

TEST(ValidateInputTest, InvalidInput)
{
    const char* input = "123 + abc";
    testing::internal::CaptureStdout(); // Захватываем stdout
    EXPECT_EXIT(validate_input(input), ::testing::ExitedWithCode(1), ""); // Проверяем только код завершения
    std::string output = testing::internal::GetCapturedStdout(); // Получаем захваченный вывод
    EXPECT_TRUE(output.find("error: invalid expression or unexpected symbol") != std::string::npos);
}

// Тесты для parse_number
TEST(ParseNumberTest, ParsesSingleDigit)
{
    const char* input = "5";
    EXPECT_EQ(parse_number(&input), 5);
}

TEST(ParseNumberTest, ParsesMultipleDigits)
{
    const char* input = "123";
    EXPECT_EQ(parse_number(&input), 123);
}

// Тесты для parse_factor
TEST(ParseFactorTest, ParsesNumber)
{
    const char* input = "42";
    EXPECT_EQ(parse_factor(&input), 42);
}

TEST(ParseFactorTest, ParsesExpressionInParentheses)
{
    const char* input = "(42)";
    EXPECT_EQ(parse_factor(&input), 42);
}

// Тесты для parse_term
TEST(ParseTermTest, HandlesMultiplication)
{
    const char* input = "2 * 3";
    EXPECT_EQ(parse_term(&input), 6);
}

TEST(ParseTermTest, HandlesDivision)
{
    const char* input = "6 / 2";
    EXPECT_EQ(parse_term(&input), 3);
}

// Тесты для parse_expression
TEST(ParseExpressionTest, HandlesAddition)
{
    const char* input = "2 + 3";
    EXPECT_EQ(parse_expression(&input), 5);
}

TEST(ParseExpressionTest, HandlesSubtraction)
{
    const char* input = "5 - 3";
    EXPECT_EQ(parse_expression(&input), 2);
}

TEST(ParseExpressionTest, HandlesComplexExpression)
{
    const char* input = "2 + 3 * 4";
    EXPECT_EQ(parse_expression(&input), 14);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
