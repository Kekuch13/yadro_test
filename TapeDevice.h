#include "Tape.h"

#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <filesystem>

struct config {
    int readDelay = 0;
    int writeDelay = 0;
    int moveDelay = 0;
};

class TapeDevice : public Tape {
public:
    explicit TapeDevice(const std::string &path);
    TapeDevice(const std::string &path, const config &delays);
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

    config delays;

    std::vector<int> tape;
};
