#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

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
        if (iss.fail() && iss.eof()) continue;

        while (iss >> x) {
            if (x == 0) {
                std::cout << "Error: деление на 0 в строке " << line_number << '\n';
                exit(2);
            }
            res /= x;
        }
        if (iss.fail() && !iss.eof()) {
            std::cout << "Error: неправильный ввод в строке " << line_number << '\n';
            exit(3);
        }

        std::cout << res << std::endl; //std::endl делает flash автоматически
    }

    return 0;
}