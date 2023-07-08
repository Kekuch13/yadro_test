#include <string>
#include <algorithm>

#include "TapeDevice.h"

void sort(TapeDevice &inputTape, TapeDevice &outputTape, const int &RAM) {
    std::vector<int> elements;

    // храним временные ленты
//    std::vector<TapeDevice> tapes;
    std::vector<TapeDevice> tapes;

    while (!inputTape.eot()) {

        // считываем элементы с входной ленты
        // после очередного считанного элемента cдвигаем ленту вправо на одну позицию
        for (int i = 0; i < RAM && !inputTape.eot(); ++i) {
            int elem = inputTape.read();
            elements.push_back(elem);
            inputTape.moveTapeRight();
        }

        // сортируем только что считанные элементы и записываем их на временную ленту
        // после очередного записанного элемента сдвигаем ленту на одну позицию вправо
        std::sort(elements.rbegin(), elements.rend());
        tapes.emplace_back("tmp\\" + std::to_string(tapes.size() + 1) + ".txt");
        for (const auto &elem : elements) {
            tapes.back().write(elem);
            tapes.back().moveTapeRight();
        }
//        tapes.back().writeToFile();
        tapes.back().moveTapeLeft(); // сдвигаем ленту на одну позицию влево, т.к. вышли за её пределы

        // очищаем оперативную память
        elements.resize(0);
    }

    int tmp, min;
    while (true) {
        min = INT_MAX;
        auto minIt = tapes.begin();
        bool is_all_tapes = true;

        // Выбираем ленту с наименьшим элементом и запоминаем её
        // Записываем полученное значение на выходную ленту
        // Когда закончатся временные ленты, выходим из цикла
        for (auto it = tapes.begin(); it != tapes.end(); ++it) {
            if (!it->eot()) {
                tmp = it->read();
                if (tmp <= min) {
                    min = tmp;
                    minIt = it;
                }
                is_all_tapes = false;
            }
        }
        if (is_all_tapes) {
//            outputTape.writeToFile();
            break;
        } else {
            minIt->moveTapeLeft();
            outputTape.write(min);
            outputTape.moveTapeRight();
        }
    }
}

int main() {
    try {
        std::string inputFile, outputFile, configFile;
        int RAM;
        std::cout << "Enter the input tape file:";
        std::cin >> inputFile;
        std::cout << "Enter the output tape file:";
        std::cin >> outputFile;
//    std::cout << "Enter the config file:";
//    std:: cin >> configFile;
        std::cout << "Enter the amount of RAM:";
        std::cin >> RAM;

        TapeDevice inputTape(inputFile);
        TapeDevice outputTape(outputFile);

        sort(inputTape, outputTape, RAM);
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    return 0;
}
