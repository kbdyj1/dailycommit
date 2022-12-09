#include <iostream>
#include <functional>

namespace { //=================================================================

namespace _1 {

template <typename SourceMessageType, typename MessageType>
class Actor
{
    std::function<void(MessageType&&)> emit;

public:
    using value_type = MessageType;

    void processMessage(SourceMessageType&& message);

    template<typename EmitFunction>
    void setMessageHandler(EmitFunction emit);
};

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_ch_12()
{

}
