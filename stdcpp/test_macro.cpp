#include <string>
#include <map>

using namespace std;

namespace { //=================================================================


#define W_MACRO_EXPAND(...) __VA_ARGS__
#define W_MACRO_DELAY(X,...) W_MACRO_EXPAND(X(__VA_ARGS__))
#define W_MACRO_DELAY2(X,...) W_MACRO_EXPAND(X(__VA_ARGS__))
#define W_MACRO_TAIL(A,...) __VA_ARGS__

#define W_MACRO_REMOVE_PAREN(A) W_MACRO_DELAY(W_MACRO_REMOVE_PAREN2, W_MACRO_REMOVE_PAREN_HELPER A)
#define W_MACRO_REMOVE_PAREN2(...) W_MACRO_DELAY2(W_MACRO_TAIL, W_MACRO_REMOVE_PAREN_HELPER_##__VA_ARGS__)
#define W_MACRO_REMOVE_PAREN_HELPER(...) _ , __VA_ARGS__
#define W_MACRO_REMOVE_PAREN_HELPER_W_MACRO_REMOVE_PAREN_HELPER

#define DECL_GETTER(TYPE, NAME) W_MACRO_REMOVE_PAREN(TYPE) get_##NAME()

namespace _1 {

class A
{
public:
    DECL_GETTER((map<string,int>), m1);

private:
    std::map<std::string, int> m1;
};

void test()
{
}

} //_1 --------------------------------------------------------------

} //===========================================================================
void test_macro()
{

}
