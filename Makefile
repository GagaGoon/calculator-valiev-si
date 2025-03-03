# Компилятор и флаги
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11 -lm
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread
GTEST_DIR = ./googletest
GTEST_BUILD_DIR = $(GTEST_DIR)/build
GTEST_INCLUDE = -isystem $(GTEST_DIR)/googletest/include
GTEST_LIBS = -L$(GTEST_BUILD_DIR)/lib -lgtest -lgtest_main -pthread

# Директории
SRC_DIR = ./src
TESTS_DIR = ./tests
BUILD_DIR = ./build
VENV_DIR = ./venv

# Цели по умолчанию
all: $(BUILD_DIR)/app.exe $(BUILD_DIR)/unit-tests.exe

# Сборка основного приложения
$(BUILD_DIR)/app.exe: $(SRC_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Сборка unit-тестов
$(BUILD_DIR)/unit-tests.exe: $(BUILD_DIR)/main.o $(BUILD_DIR)/unittests.o $(GTEST_BUILD_DIR)/lib/libgtest.a
	$(CXX) $(CXXFLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/unittests.o -o $@ $(GTEST_LIBS)

# Компиляция main.c с флагом -DTEST_MODE
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -DTEST_MODE $(CFLAGS) -o $@ $<

# Компиляция unittests.cpp
$(BUILD_DIR)/unittests.o: $(TESTS_DIR)/unit/unittests.cpp $(GTEST_BUILD_DIR)/lib/libgtest.a
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -c -o $@ $<

# Запуск app.exe (целочисленный режим)
run-int: $(BUILD_DIR)/app.exe
	./$(BUILD_DIR)/app.exe

# Запуск app.exe с флагом --float (режим с плавающей точкой)
run-float: $(BUILD_DIR)/app.exe
	./$(BUILD_DIR)/app.exe --float

# Запуск unit-тестов
run-unit-test: $(BUILD_DIR)/unit-tests.exe
	./$(BUILD_DIR)/unit-tests.exe

# Создание виртуального окружения и установка pytest
venv: $(VENV_DIR)/bin/activate

$(VENV_DIR)/bin/activate:
	@echo "Создание виртуального окружения..."
	python3 -m venv $(VENV_DIR)
	@echo "Установка pytest..."
	. $(VENV_DIR)/bin/activate && pip install pytest

# Запуск интеграционных тестов
run-integration-tests: $(BUILD_DIR)/app.exe venv
	@echo "Запуск интеграционных тестов..."
	. $(VENV_DIR)/bin/activate && python3 $(TESTS_DIR)/integration/integration_tests.py

# Очистка артефактов
clean:
	rm -rf $(BUILD_DIR) $(VENV_DIR) $(GTEST_DIR)

# Клонирование и сборка Google Test
$(GTEST_DIR)/CMakeLists.txt:
	@echo "Клонирование Google Test..."
	git clone https://github.com/google/googletest.git $(GTEST_DIR)

$(GTEST_BUILD_DIR)/lib/libgtest.a: $(GTEST_DIR)/CMakeLists.txt
	@echo "Сборка Google Test..."
	@mkdir -p $(GTEST_BUILD_DIR)
	cd $(GTEST_BUILD_DIR) && cmake .. && make

# Псевдоцели
.PHONY: all clean run-int run-float run-unit-test run-integration-tests venv
