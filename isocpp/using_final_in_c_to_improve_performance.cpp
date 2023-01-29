//=============================================================================
//
//  https://blog.feabhas.com/2022/11/using-final-in-c-to-improve-performance/
//
//  23/01/29
//
//=============================================================================

#include <iostream>

namespace  { //================================================================

namespace _1 {

class Buffer;

class DataLink {
public:
    virtual ~DataLink() = default;

    virtual void send(const Buffer&) = 0;
    virtual Buffer receive() = 0;
};

struct Message
{};

class PDOProtocol {
    DataLink* link;

public:
    PDOProtocol(DataLink& transport) : link(&transport)
    {}

    Message createMessage() {
        return Message{};
    }
    void sendMessage(const Message&);
    Message getMessage();
};

class CANBus : public DataLink {
public:
    CANBus() = default;
    ~CANBus() override = default;

protected:
    void send(const Buffer&) override;
    Buffer receive() override;
};

class RS422 : public DataLink {
public:
    RS422() = default;
    ~RS422() override = default;

protected:
    void send(const Buffer&) override;
    Buffer receive() override;
};

class Sensor {
public:
    virtual double get_value();
    virtual void set_id(int);
};

class RotaryEncoder final : public Sensor {
public:
    double get_value() override;
};

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_using_final_in_c_to_improve_performance()
{

}
