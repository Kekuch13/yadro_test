#include <string>
#include <algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "TapeDevice.h"

void sort(TapeDevice &inputTape, TapeDevice &outputTape, const int &RAM) {
    std::cout << "\nSorting started\n";

    std::vector<int> elements;
    std::vector<TapeDevice> tapes;

    while (!inputTape.eot()) {

        // считываем элементы с входной ленты
        // после очередного считанного элемента cдвигаем ленту вправо на одну позицию
        for (int i = 0; i < RAM && !inputTape.eot(); ++i) {
            int elem = inputTape.read();
            elements.push_back(elem);
            inputTape.moveTapeRight();
        }

        // сортируем считанные элементы и записываем их на временную ленту
        // после очередного записанного элемента сдвигаем ленту на одну позицию вправо
        std::sort(elements.rbegin(), elements.rend());
        tapes.emplace_back("tmp\\" + std::to_string(tapes.size() + 1) + ".txt");
        for (const auto &elem : elements) {
            tapes.back().write(elem);
            tapes.back().moveTapeRight();
        }
        tapes.back().moveTapeLeft(); // сдвигаем ленту на одну позицию влево, т.к. головка указывает на пустой элемент

        elements.resize(0); // очищаем оперативную память
    }

    std::cout << "Sorting in progress\n";

    int tmp, min;
    while (true) {
        min = INT_MAX;
        auto minIt = tapes.begin();
        bool end_of_all_tapes = true;

        // выбираем ленту с наименьшим элементом
        // записываем минимальное значение на выходную ленту
        // когда закончатся временные ленты, выходим из цикла
        for (auto it = tapes.begin(); it != tapes.end(); ++it) {
            if (!it->eot()) {
                tmp = it->read();
                if (tmp <= min) {
                    min = tmp;
                    minIt = it;
                }
                end_of_all_tapes = false;
            }
        }
        if (end_of_all_tapes) {
            break;
        } else {
            minIt->moveTapeLeft();
            outputTape.write(min);
            outputTape.moveTapeRight();
        }
    }
    std::cout << "Sorting is finished\n";
}

int main() {
    try {
        std::string inputFile, outputFile, configFile;
        int RAM;
        Config delays;

        std::cout << "Enter the path to the input tape file:";
        std::cin >> inputFile;
        std::cout << "Enter the path to the output tape file:";
        std::cin >> outputFile;
        std::cout << "Enter the path to Config file:";
        std:: cin >> configFile;
        std::cout << "Enter the amount of RAM:";
        std::cin >> RAM;

        boost::property_tree::ptree root;
        if (std::filesystem::exists(configFile)) {
            boost::property_tree::read_json(configFile, root);
        } else {
            std::cerr << "\nCannot open Config file\n";
        }

        delays.readDelay = root.get_optional<int>("readDelay").get_value_or(0);
        delays.writeDelay = root.get_optional<int>("writeDelay").get_value_or(0);
        delays.moveDelay = root.get_optional<int>("moveDelay").get_value_or(0);

        TapeDevice inputTape(inputFile, delays);
        TapeDevice outputTape(outputFile, delays);

        sort(inputTape, outputTape, RAM);
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    return 0;
}
