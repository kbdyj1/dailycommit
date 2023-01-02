#include <iostream>
#include <string>
#include <map>
#include <list>
#include <functional>

namespace { //=================================================================


class User {
public:
    int id;
    std::string handle;

    User() : id(0)
    {}
    User(int id, const std::string& handle) : id(id), handle(handle)
    {}

    bool operator==(const User& other) const
    {
        return id == other.id && handle == other.handle;
    }
};

class DataStore
{
public:
    std::list<User> users;
};

typedef std::map<std::string, std::string> Event;

auto filterEvent = [](Event event, const std::string& eventType) {
    return event["type"] == eventType;
};

template <typename Entity>
auto playEvents = [](const auto& events, const auto& eventType, auto playEvent) {
    std::list<Event> allEvents;
    auto filterEventByThisEventType = std::bind(filterEvent, std::placeholders::_1, eventType);

    std::copy_if(events.begin(), events.end(), std::back_inserter(allEvents), filterEventByThisEventType);
    std::list<Entity> entities(allEvents.size());
    std::transform(allEvents.begin(), allEvents.end(), entities.begin(), playEvent);

    return entities;
};

class EventStore : public std::list<Event>
{
public:
    EventStore() : std::list<Event>()
    {}

    DataStore play(){
        DataStore dataStore;
        auto createUserFromEvent = [](Event event) {
            return User(std::stoi(event["id"]), event["handle"]);
        };
#if (0)
        for (auto event : *this) {
            if (event["type"] == "createUser") {
                dataStore.users.push_back(createUserFromEvent(event));
            }
        }
#else
        dataStore.users = playEvents<User>(*this, "createUser", createUserFromEvent);
#endif
        return dataStore;
    }
};

auto makeCreateUserEvent = [](const std::string& handle, const int id) {
    return Event {
        {"type", "createUser"},
        {"handle", handle},
        {"id", std::to_string(id)}
    };
};

int id = 1;

auto createUser = [](std::string handle, EventStore& eventStore) {
    eventStore.push_back(makeCreateUserEvent(handle, id));
    return id;
};

auto makePostMessageEvent = [](const int userId, const std::string& message, int id){
    return Event{
        {"type", "postMessage"},
        {"userId", std::to_string(userId)},
        {"message", message},
        {"id", std::to_string(id)}
    };
};

auto postMessage = [](const int userId, const std::string& message, EventStore& eventStore) {
    eventStore.push_back(makePostMessageEvent(userId, message, id));
    return id;
};

namespace _1 { //----------------------------------------------------

void testCreateUser()
{
    auto eventStore = EventStore{};

    auto handle = "Haerin";
    auto haerinId = createUser(handle, eventStore);
    auto expectedEvent = makeCreateUserEvent(handle, haerinId);
    auto event = eventStore.back();

    std::cout << "testCreateUser : expectedEvent == event ? " << (expectedEvent == event) << "\n";
}

void testPostMessage()
{
    auto handle = "Hani";
    auto message = "Hello, Ditto!!!";

    auto eventStore = EventStore{};

    auto haniId = createUser(handle, eventStore);
    auto messageId = postMessage(haniId, message, eventStore);
    auto expectedEvent = makePostMessageEvent(haniId, message, messageId);
    auto event = eventStore.back();

    std::cout << "testPostMessage : expectedEvent == event ? " << (expectedEvent == event) << "\n";
}

void testUserStore()
{
    auto handle = "Hyein";
    auto eventStore = EventStore{};

    auto hyeinId = createUser(handle, eventStore);
    auto dataStore = eventStore.play();

    std::cout << "testUserStore: " << (dataStore.users.back() == User(hyeinId, handle));
}

void test()
{
    testCreateUser();
    testPostMessage();
    testUserStore();
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_13()
{
    _1::test();
}
