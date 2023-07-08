#include "Tape.h"

#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <filesystem>

class TapeDevice : public Tape {
public:
    explicit TapeDevice(const std::string &filename);
    ~TapeDevice();

    TapeDevice(TapeDevice &&other) = default;
    TapeDevice& operator=(TapeDevice &&other) = default;

    int &read() override;
    void write(const int &elem) override;
    bool eot() const override;
    void moveTapeRight() override;
    void moveTapeLeft() override;

private:
    void readFromFile();
    void writeToFile();

private:
    std::string path;
    std::fstream file;
    int currPos = 0;

    int readDelay = 0;
    int writeDelay = 0;
    int moveDelay = 0;

    std::vector<int> tape;
};
