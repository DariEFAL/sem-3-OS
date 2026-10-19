#include <iostream>
#include <sstream>
#include <string>
#include <limits> // Обязательно для std::numeric_limits 
// Если произошло переполнение в переменную записывается std::numeric_limits<double>::infinity()

int main() {
    //число число число<endline>
    // Дочерний процесс производит деление первого числа команда, на последующие числа в команде
    std::string line;
    int line_number = 0;

    while (std::getline(std::cin, line)) {
        line_number++;

        std::istringstream iss(line);
        double res = 0, x = 0;
        iss >> res;
        if (iss.fail() && res == std::numeric_limits<double>::max()) {
            /*
            для C++11 и выше
            Если число в строке превышает лимиты double, поток обязан выставить флаг ошибки iss.fail().
            При этом в переменную res записывается не бесконечность (inf), а std::numeric_limits<double>::max() 
            факт: При переполнении в переменную записывается std::numeric_limits<double>::infinity(). это может проверить std::isinf(res)) это работает только при арифмитических операциях
            */
            std::cout << "Error: неправильный ввод в строке " << line_number << ", произошло переполнение\n";
            return 3;
        }
        if (iss.fail() && iss.eof()) continue; //строка пустая
        //если ввели: a . iss.fail() = true, а iss.eof() = false, тк поток остановился перед a и сразу бросил ошибку, 
        //потому проверка на ввод букв дальше. И чтение из потока, который броисил ошибку, не возможно. поток блокируется

        while (iss >> x) {
            if (x == 0) {
                std::cout << "Error: деление на 0 в строке " << line_number << '\n';
                return 2;
            }
            res /= x;
        }
        if (iss.fail() && x == std::numeric_limits<double>::max()) {
            std::cout << "Error: неправильный ввод в строке " << line_number << ", произошло переполнение\n";
            return 3;
        }
        if (iss.fail() && !iss.eof()) {
            std::cout << "Error: неправильный ввод в строке " << line_number << '\n';
            return 3;
        }

        std::cout << res << std::endl; //std::endl делает flash автоматически
    }

    return 0;
}