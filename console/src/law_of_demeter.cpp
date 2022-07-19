#include <iostream>
#include <vector>

namespace { //=================================================================

// Rule (callable methods)
// 1. self
// 2. parameter
// 3. local
// 4. instance member

// Tell, don't ask !
// Don't talk to Strangers

class AbstractBuilding
{
public:
    virtual void sale() = 0;
    virtual std::string getQuality()  = 0;
};

class A : public AbstractBuilding
{
    std::string quality;
public:
    A() : quality("The high quality")
    {
    }

    void sale() override
    {
        std::cout << "A " << quality << " by selling !\n";
    }
    std::string getQuality() override
    {
        return quality;
    }
};

class B : public AbstractBuilding
{
    std::string quality;
public:
    B() : quality("The low quality")
    {
    }

    void sale() override
    {
        std::cout << "A " << quality << " by selling !\n";
    }
    std::string getQuality() override
    {
        return quality;
    }
};

class Mediator
{
    std::vector<AbstractBuilding*> buildings;
public:
    Mediator()
    {
        buildings.push_back(new A);
        buildings.push_back(new B);
    }
    ~Mediator()
    {
        for (auto iter = buildings.begin(); iter != buildings.end(); iter++) {
            auto* building = *iter;
            delete building;
        }
    }

    AbstractBuilding* find(const std::string quality)
    {
        for (auto iter = buildings.begin(); iter != buildings.end(); iter++) {
            auto* building = *iter;
            if (quality.compare(building->getQuality()) == 0) {
                return building;
            }
        }
        return nullptr;
    }
};

//*****************************************************************************

struct User {
    int id;
    std::string name;

    int getId() {
        return id;
    }
};

class DeletePostCommand {
    int id;
public:
    int getPostId() {
        return id;
    }
    User getDeleter() {
        return User{id, ""};
    }
};

struct Post {
    int id;
    std::string message;
    User getUser() {
        return User{id, ""};
    }
    int deleteAndSave() {
        return 0;
    }
    int deleteAndSave(User deleter) {
        if (getUser().getId() == deleter.getId()) {
            return 0;
        } else {
            return -1;
        }
    }
};

class PostRepository {
public:
    Post findByPostId(int id) {
        return Post{id, ""};
    }
    bool save(int id) {
        return true;
    }
};

class PostService {
    PostRepository messageRepository;

public:
    void deletePost(DeletePostCommand command) {
        auto post = messageRepository.findByPostId(command.getPostId());
#if (0)
        // Post, User, ...
        if (post.getUser().getId() == command.getDeleter().getId()) {
            messageRepository.save(post.deleteAndSave());
        }
#else
        messageRepository.save(post.deleteAndSave(command.getDeleter()));
#endif
    }
};

} // namespace ================================================================

void test_law_of_demeter()
{
    auto* mediator = new Mediator;
    auto* building = mediator->find("The low quality");
    if (building) {
        building->sale();
    } else {
        std::cout << "can't find building.\n";
    }
}
