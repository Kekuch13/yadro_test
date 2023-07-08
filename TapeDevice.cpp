#include "TapeDevice.h"

TapeDevice::TapeDevice(const std::string &filename) : path(filename) {
    std::cout << filename << " ===\n";
    if (std::filesystem::exists(path)) {
        file.open(path, std::fstream::in | std::fstream::out);
    } else {
        std::cout << "Created file with path: " << path << std::endl;
        file.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    }

    if (file.is_open()) {
        std::cout << "File successfully opened\n";
        readFromFile();
    } else {
        std::cerr << "Cannot open file with path: " << filename << std::endl;
    }

    file.exceptions(std::fstream::badbit | std::fstream::failbit);
}

TapeDevice::~TapeDevice() {
    if (file.is_open()) {
        file.close();
        file.open(path, std::fstream::out | std::fstream::trunc);
        file.clear();
        writeToFile();
        file.close();
    }
}

void TapeDevice::readFromFile() {
    if (file.peek() == std::fstream::traits_type::eof()) {
        file.clear();
        return;
    }

    file.seekg(std::ios::beg);
    int element;
    while (!file.eof()) {
        file >> element;
        tape.push_back(element);
    }
    file.clear();
}

void TapeDevice::writeToFile() {
    for (const auto &elem : tape) {
        file << elem << "\n";
    }
}

int& TapeDevice::read() {
    std::this_thread::sleep_for(std::chrono::milliseconds(readDelay));
    return tape[currPos];
}

void TapeDevice::write(const int &elem) {
    std::this_thread::sleep_for(std::chrono::milliseconds(writeDelay));
    if (currPos >= tape.size()) {
        tape.push_back(elem);
    } else {
        tape[currPos] = elem;
    }
}

bool TapeDevice::eot() const {
    return currPos >= tape.size() || currPos < 0;
}

void TapeDevice::moveTapeRight() {
    std::this_thread::sleep_for(std::chrono::milliseconds((moveDelay)));
    ++currPos;
}

void TapeDevice::moveTapeLeft() {
    std::this_thread::sleep_for(std::chrono::milliseconds((moveDelay)));
    --currPos;
}
