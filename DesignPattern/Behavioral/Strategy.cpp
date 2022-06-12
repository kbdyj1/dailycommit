#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

namespace { //=================================================================

enum OutputFormat {
    Markdown = 0,
    Html
};

struct ListStrategy {
    virtual void addListItem(std::ostringstream&, const std::string&) {}
    virtual void start(std::ostringstream&) {}
    virtual void end(std::ostringstream&) {}
};

struct HtmlListStrategy : ListStrategy {
    void start(std::ostringstream& os) override {
        os << "<ul>" << "\n";
    }
    void end(std::ostringstream& os) override {
        os << "</ul>" << "\n";
    }
    void addListItem(std::ostringstream& os, const std::string& item) override {
        os << "<li>" << item << "</li>" << "\n";
    }
};

struct MarkdownListStrategy : ListStrategy {
    void addListItem(std::ostringstream& os, const std::string& item) override {
        os << " * " << item << "\n";
    }
};

class TextProcessor {
    std::ostringstream os;
    std::unique_ptr<ListStrategy> listStrategy;

public:
    void clear() {
        os.str("");
        os.clear();
    }

    void appendList(const std::vector<std::string>& items) {
        listStrategy->start(os);
        for (auto& item: items) {
            listStrategy->addListItem(os, item);
        }
        listStrategy->end(os);
    }

    void setOutputFormat(OutputFormat format) {
        switch (format) {
        case Markdown:
            listStrategy = std::make_unique<MarkdownListStrategy>();
            break;
        case Html:
            listStrategy = std::make_unique<HtmlListStrategy>();
            break;
        }
    }

    std::string str() const {
        return os.str();
    }
};

void test_text_process()
{
    auto tp = TextProcessor();
    tp.setOutputFormat(Markdown);
    tp.appendList({"foo", "bar", "baz"});
    std::cout << tp.str() << "\n";

    tp.clear();
    tp.setOutputFormat(Html);
    tp.appendList({"foo", "bar", "baz"});
    std::cout << tp.str() << "\n";
}

template <typename T>
class Character {
    T combat;
public:
    void attack() {
        combat.prepare();
        combat.attack();
    }
};

class Combat {
protected:
    virtual void prepare() = 0;
    virtual void attack() = 0;
};

class ArcherCombat : public Combat {
public:
    void prepare() override {
        std::cout << "ArcherCombat::prepare()\n";
    }
    void attack() override {
        std::cout << "ArcherCombat::attack()\n";
    }
};

class WorriorCombat : public Combat {
public:
    void prepare() override {
        std::cout << "WorriorCombat::prepare()\n";
    }
    void attack() override {
        std::cout << "WorriorCombat::attack()\n";
    }
};

void test_combat()
{
    auto archer = Character<ArcherCombat>();
    archer.attack();

    auto worrior = Character<WorriorCombat>();
    worrior.attack();
}

} //namespace =================================================================

void test_strategy()
{
    //test_text_process();
    test_combat();
}
