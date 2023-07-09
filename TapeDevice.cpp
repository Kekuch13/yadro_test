#include "TapeDevice.h"

TapeDevice::TapeDevice(const std::string &path) : path(path) {
    if (std::filesystem::exists(path)) {
        file.open(path, std::fstream::in | std::fstream::out);
    } else {
        file.open(path, std::fstream::in | std::fstream::out | std::fstream::trunc);
    }

    if (file.is_open()) {
        readFromFile();
    } else {
        std::cerr << "Cannot open file with path: " << path << std::endl;
    }

    file.exceptions(std::fstream::badbit | std::fstream::failbit);
}

TapeDevice::TapeDevice(const std::string &path, const Config &delays) : TapeDevice(path) {
    this->delays = delays;
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
    for (size_t i = 0; i < tape.size() - 1; ++i) {
        file << tape[i] << "\n";
    }
    file << tape.back();
}

int &TapeDevice::read() {
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.readDelay));
    return tape[currPos];
}

void TapeDevice::write(const int &elem) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.writeDelay));
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
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.moveDelay));
    ++currPos;
}

void TapeDevice::moveTapeLeft() {
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.moveDelay));
    --currPos;
}
