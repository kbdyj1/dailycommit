#include <iostream>
#include <vector>

namespace { //=================================================================

struct ChatRoom;

struct Person {
    std::string name;
    ChatRoom* room = nullptr;
    std::vector<std::string> chatLog;

    Person(const std::string& name);

    void receive(const std::string& sender, const std::string& message);
    void send(const std::string& messagey) const;
    void dm(std::string& receiver, const std::string& message) const;
};

struct ChatRoom {
    std::vector<Person*> people;

    void join(Person* p);
    void broadcast(const std::string& sender, const std::string& message);
    void dm(const std::string& sender, const std::string& receiver, const std::string& message);
};

} //namespace =================================================================
