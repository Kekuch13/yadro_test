class Tape {
public:
    virtual int &read() = 0;
    virtual void write(const int &elem) = 0;
    virtual void moveTapeRight() = 0;
    virtual void moveTapeLeft() = 0;
    virtual bool eot() const = 0;
};
