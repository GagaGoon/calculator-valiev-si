import subprocess
import unittest

def run_calculator(input_data):
    """
    Запускает app.exe с входными данными и возвращает результат.
    """
    result = subprocess.run(
        ["./build/app.exe"],  # Путь к app.exe
        input=input_data,      # Входные данные
        text=True,             # Использовать текстовый режим
        capture_output=True    # Захватить вывод
    )
    return result.stdout.strip()  # Возвращаем результат без лишних пробелов

def run_calculator_float(input_data):
    """
    Запускает app.exe с флагом --float и входными данными, возвращает результат.
    """
    result = subprocess.run(
        ["./build/app.exe", "--float"],
        input=input_data,
        text=True,
        capture_output=True
    )
    return result.stdout.strip()

class TestCalculator(unittest.TestCase):
    def test_addition(self):
        """Проверка сложения."""
        result = run_calculator("2 + 2\n")
        self.assertEqual(result, "4", "2 + 2 should be 4")

    def test_subtraction(self):
        """Проверка вычитания."""
        result = run_calculator("5 - 3\n")
        self.assertEqual(result, "2", "5 - 3 should be 2")

    def test_multiplication(self):
        """Проверка умножения."""
        result = run_calculator("3 * 4\n")
        self.assertEqual(result, "12", "3 * 4 should be 12")

    def test_division(self):
        """Проверка деления."""
        result = run_calculator("10 / 2\n")
        self.assertEqual(result, "5", "10 / 2 should be 5")

    def test_division_by_zero(self):
        """Проверка деления на ноль."""
        result = run_calculator("10 / 0\n")
        self.assertIn("error: division by zero or too small number", result.lower(), "Division by zero should return an error")

    def test_complex_expression(self):
        """Проверка сложного выражения со скобками."""
        result = run_calculator("(2 + 2) * 3\n")
        self.assertEqual(result, "12", "(2 + 2) * 3 should be 12")

    def test_large_numbers(self):
        """Проверка работы с большими числами."""
        result = run_calculator("123456789 + 987654321\n")
        self.assertEqual(result, "1111111110", "123456789 + 987654321 should be 1111111110")

    def test_invalid_input(self):
        """Проверка некорректного ввода."""
        result = run_calculator("abc\n")
        self.assertIn("error: invalid expression or unexpected symbol", result.lower(), "Invalid input should return an error")

    def test_empty_input(self):
        """Проверка пустого ввода."""
        result = run_calculator("\n")
        self.assertIn("error: invalid expression or unexpected symbol", result.lower(), "Empty input should return an error")

    def test_spaces_in_expression(self):
        """Проверка выражения с пробелами."""
        result = run_calculator(" 2   + 2 \n")
        self.assertEqual(result, "4", "2 + 2 with spaces should be 4")

    def test_float_division(self):
        """Проверка деления с результатом во float."""
        result = run_calculator_float("10 / 3\n")
        self.assertEqual(result, "3.333333", "10 / 3 should be 3.333333")

if __name__ == "__main__":
    unittest.main()
