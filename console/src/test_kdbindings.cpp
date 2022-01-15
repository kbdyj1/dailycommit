#include <iostream>
#include <string>

#include <binding.h>
#include <property.h>
#include <signal.h>
#include <binding_evaluator.h>

namespace internal
{

using namespace KDBindings;

/*
 * Simple connection
 *
 */
void testKDBindingsSimpleConnection()
{
    Signal<std::string, int> signal;

    signal.connect([](std::string arg0, int arg1){
        std::cout << "arg0 : " << arg0 << ", arg1 : " << arg1 << std::endl;
    });
    signal.connect([](std::string arg0, int arg1){
        std::cout << "lambda(" << arg0 << ", " << arg1 << ")" << std::endl;
    });

    signal.emit("The answer", 42);
}

/*
 * Signal member
 *
 */
class Button
{
public:
    Signal<> clicked;
};

class Message
{
public:
    void display() const
    {
        std::cout << "Message.display()" << std::endl;
    }
};

/*
 * Member arguments
 *
 */
void testSignalMember()
{
    Button button;
    Message message;

    button.clicked.connect(&Message::display, &message);
    button.clicked.emit();
}

class Person
{
public:
    Person(std::string const &name)
        : mName(name)
    {}
    Signal<std::string const&> speak;

    void listen(std::string const &message)
    {
        std::cout << mName << " received: " << message << std::endl;
    }
private:
    std::string mName;
};

void testPerson()
{
    Person alice("Alice");
    Person bob("Bob");

    alice.speak.connect(&Person::listen, &bob);
    bob.speak.connect(&Person::listen, &alice);

    alice.speak.emit("Have a nice day!!!");
    bob.speak.emit("Thank you");
}

/*
 * Simple property
 *
 */
class Widget
{
public:
    Widget(std::string const &name)
        : mValue(0)
        , mName(name)
    {}

    Property<int> mValue;

private:
    std::string mName;
};

void testWidget()
{
    Widget w("Widget");

    w.mValue.valueChanged().connect([](int newValue){
        std::cout << "The new value is " << newValue << std::endl;
    });

    w.mValue = 10;
    w.mValue = 20;

    std::cout << "Property value is : " << w.mValue.get() << std::endl; // compile error ???
}

/*
 * Property bindings
 *
 */
BindingEvaluator eval;

class Image
{
public:
    const int bytesPerPixel = 4;
    Property<int> width{ 800 };
    Property<int> height{ 600 };
    const Property<int> byteSize = makeBoundProperty(bytesPerPixel * width * height);
    const Property<int> lazyByteSize = makeBoundProperty(eval, bytesPerPixel * width * height);
};

void testImage()
{
    Image image;
    std::cout << "the initialize size of the image = " << image.byteSize.get() << " bytes" << std::endl;

    image.byteSize.valueChanged().connect([](const int &newValue){
        std::cout << "the new size of the image = " << newValue << " bytes" << std::endl;
    });

    image.width = 1920;
    image.height = 1080;
}

void testImageWithLazyPropertyBindings()
{

    Image image;
    std::cout << "the initialize size of the image = " << image.byteSize.get() << " bytes" << std::endl;

    image.lazyByteSize.valueChanged().connect([](const int &newValue){
        std::cout << "the new size of the image = " << newValue << " bytes" << std::endl;
    });

    image.width = 1920;
    image.height = 1080;

    eval.evaluateAll();
}

/*
 * Advanced connections
 *
 */
void display()
{
    std::cout << "display()" << std::endl;
}

void displayLabelled(const std::string &label, int value)
{
    std::cout << label << ": " << value << std::endl;
}

class SignalHandler
{
public:
    bool received = 0;

    void receive()
    {
        received = true;
    }
};

void testAdvancedConnections()
{
    Signal<int> signal;

    signal.connect(display);
    signal.connect(displayLabelled, "emitted value");

    SignalHandler handler;
    signal.connect(&SignalHandler::receive, &handler);

    signal.emit(5);

    std::cout << std::boolalpha << handler.received << std::endl;
}

}

void testKDBindings()
{
    //internal::testKDBindingsSimpleConnection();
    //internal::testSignalMember();
    //internal::testPerson();
    //nternal::testWidget();
    //internal::testImage();
    //internal::testImageWithLazyPropertyBindings();
    internal::testAdvancedConnections();
}
