#include <iostream>
#include <string>

using namespace std;

namespace { //================================================================

namespace _1 {

struct Node {
    string position;
    Node* l;
    Node* r;
};

struct Org {
    Node* root;

    static Org createOrg(const string& position);
    static Node* find(Node* root, const string& position);

    bool add(const string& manager, const string& member);
};

Org Org::createOrg(const string& position)
{
    Org org;
    org.root = new Node{position, nullptr, nullptr};
    return org;
}

Node* Org::find(Node* root, const string& position)
{
    if (nullptr == root)
        return nullptr;

    if (root->position == position)
        return root;

    auto lFound = Org::find(root->l, position);
    if (lFound)
        return lFound;

    return Org::find(root->r, position);
}

bool Org::add(const string& manager, const string& member)
{
    auto managerNode = Org::find(root, manager);

    if (managerNode == nullptr) {
        cout << "Org::find() return nullptr\n";
        return false;
    }

    if (managerNode->l && managerNode->r) {
        cout << "position: " << member << ", ->l && ->r != nullptr\n";
        return false;
    }

    if (!managerNode->l) {
        managerNode->l = new Node{member, nullptr, nullptr};
    } else {
        managerNode->r = new Node{member, nullptr, nullptr};
    }

    return true;
}

void test()
{
    auto tree = Org::createOrg("CEO");
    tree.add("CEO", "VICE");
    tree.add("VICE", "IT VICE");
    tree.add("VICE", "Marketing");
    tree.add("IT VICE", "Security");
    tree.add("IT VICE", "App");
    tree.add("Marketing", "Distribution");
    tree.add("Marketing", "Promotion");
    tree.add("VICE", "Finance");
}

} //_1 --------------------------------------------------------------

} //==========================================================================

void test_ch_02()
{
    _1::test();
}
