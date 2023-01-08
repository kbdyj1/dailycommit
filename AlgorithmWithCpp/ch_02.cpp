#include <iostream>
#include <string>
#include <queue>
#include <functional>

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

namespace _2 { //BST

template <typename T>
class Tree {
public:
    struct Node {
        T value;
        Node* l;
        Node* r;
    };

private:
    Node* root;

    void insertInternal(Node* node, Node* insert) {
        auto nodeValue = node->value;
        auto insertValue = insert->value;
        if (insertValue < nodeValue) {
            if (node->l) {
                insertInternal(node->l, insert);
            } else {
                node->l = insert;
            }
        } else if (insertValue > nodeValue) {
            if (node->r) {
                insertInternal(node->r, insert);
            } else {
                node->r = insert;
            }
        } else {
            std::cout << "same value exist: " << insertValue << "\n";

            delete insert;
        }
    }

    Node* removeInternal(Node* node, T value) {
        if (node == nullptr)
            return nullptr;

        if (node->value == value) {
            if (!node->l) {
                auto tmp = node->r;
                delete node;
                return tmp;
            }
            if (!node->r) {
                auto tmp = node->l;
                delete node;
                return tmp;
            }

            auto s = succssor(node);
            node->value = s->value;

            node->r = removeInternal(node->r, s->value);
        } else if (node->value < value) {
            if (node->r) {
                node->r = removeInternal(node->r, value);
            }
        } else if (node->value > value) {
            if (node->l) {
                node->l = removeInternal(node->l, value);
            }
        }
        return node;
    }
    Node* succssor(Node* node) {
        Node* current = node->r;
        while (current->l) {
            current = current->l;
        }
        return current;
    }

public:
    Tree() : root(nullptr)
    {}
    ~Tree()
    {
        postOrder(root, [](Node* node){
            std::cout << "del(" << node->value << "), ";
            delete node;
        });
        std::cout << "\n";
    }

    Node* rootNode() const {
        return root;
    }

    void insert(T value) {
        auto node = new Node{ value, nullptr, nullptr };

        if (root) {
            insertInternal(root, node);
        } else {
            root = node;
        }
    }
    void remove(T value) {
        if (root) {
            root = removeInternal(root, value);
        }
    }
    void preOrder(Node* node, const std::function<void(Node*)>& func) {
        if (node) {
            func(node);
            preOrder(node->l, func);
            preOrder(node->r, func);
        }
    }
    void inOrder(Node* node, const std::function<void(Node*)>& func) {
        if (node) {
            inOrder(node->l, func);
            func(node);
            inOrder(node->r, func);
        }
    }
    void postOrder(Node* node, const std::function<void(Node*)>& func) {
        if (node) {
            postOrder(node->l, func);
            postOrder(node->r, func);
            func(node);
        }
    }
    void print() {
        inOrder(root, [](Tree<int>::Node* node){
            std::cout << node->value << ", ";
        });
        std::cout << "\n";
    }
};

void test()
{
    auto tree = Tree<int>{};

    tree.insert(3);
    tree.insert(4);
    tree.insert(8);
    tree.insert(5);
    tree.insert(1);
    tree.insert(9);
    tree.insert(7);
    tree.insert(2);
    tree.insert(6);

    tree.print();

    std::cout << "tree.remove(8)\n";
    tree.remove(8);
    tree.print();

    std::cout << "tree.insert(8)\n";
    tree.insert(8);
    tree.print();

    std::cout << "tree.remove(3)\n";
    tree.remove(3);
    tree.print();
}

} //_2 --------------------------------------------------------------

} //==========================================================================

void test_ch_02_heap();

void test_ch_02()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    test_ch_02_heap();
}
