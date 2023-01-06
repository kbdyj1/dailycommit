#include <iostream>
#include <string>
#include <queue>

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

    void preOrder(Node* node);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void levelOrder(Node* node);
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

void Org::preOrder(Node *node)
{
    if (node) {
        cout << node->position << ", ";

        preOrder(node->l);
        preOrder(node->r);
    }
}

void Org::inOrder(Node *node)
{
    if (node) {
        inOrder(node->l);
        cout << node->position << ", ";
        inOrder(node->r);
    }
}

void Org::postOrder(Node *node)
{
    if (node) {
        inOrder(node->l);
        inOrder(node->r);
        cout << node->position << ", ";
    }
}

void Org::levelOrder(Node *node)
{
    if (node) {
        auto q = std::queue<Node*>{};
        q.push(node);

        while (!q.empty()) {
            int size = q.size();

            for (auto i=0; i<size; i++) {
                auto current = q.front();
                q.pop();

                std::cout << current->position << ", ";
                if (current->l)
                    q.push(current->l);
                if (current->r)
                    q.push(current->r);
            }
            cout << endl;
        }
    }
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

    //tree.preOrder(tree.root);   //CEO, VICE, IT VICE, Security, App, Marketing, Distribution, Promotion
    //tree.inOrder(tree.root);    //Security, IT VICE, App, VICE, Distribution, Marketing, Promotion, CEO
    //tree.postOrder(tree.root);  //Security, IT VICE, App, VICE, Distribution, Marketing, Promotion, CEO
    tree.levelOrder(tree.root);
}

} //_1 --------------------------------------------------------------

} //==========================================================================

void test_ch_02()
{
    _1::test();
}
