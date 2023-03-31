#include <iostream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

namespace { //=================================================================

typedef uint32_t key_t;

class User
{
    key_t firstName;
    key_t lastName;

    static map<key_t, string> names;
    static key_t seed;
    static string empty;

public:
    User(const string& firstName, const string& lastName)
        : firstName(add(firstName))
        , lastName(add(lastName))
    {}

    static key_t add(const string& s)
    {
        for (auto iter=names.begin(); iter!=names.end(); iter++) {
            if (iter->second == s) {
                return iter->first;
            }
        }

        names.insert({++seed, s});
        return seed;
    }

    const string& getFirstName() const
    {
        auto iter = names.find(firstName);
        if (iter != names.end()) {
            return iter->second;
        } else {
            return empty;
        }
    }

    const string& getLastName() const
    {
        auto iter = names.find(lastName);
        if (iter != names.end()) {
            return iter->second;
        } else {
            return empty;
        }
    }
};

key_t User::seed = 0;
map<key_t, string> User::names;
string User::empty;

} // namespace ================================================================

void test_flyweight()
{
    auto u0 = User{"Hyein", "Lee"};
    auto u1 = User{"Haerin", "Kang"};
    auto u2 = User{"Minji", "Kim"};
    auto u3 = User{"Hanni", "Pham"};
    auto u4 = User{"Jihye", "Mo"};
    auto u5 = User{"Chaewon", "Kim"};
    auto u6 = User{"Sakura", "Miyawaki"};
    auto u7 = User{"Yunjin", "Huh"};
    auto u8 = User{"Kazuha", "Nakamura"};
    auto u9 = User{"Eunchae", "Hong"};
    auto u10= User{"Minju", "Kim"};
    auto u11= User{"Yonyoung", "Jang"};
    auto u12= User{"Eunbi", "Kwon"};
    auto u13= User{"Yuri", "Jo"};
    auto u14= User{"Hitomi", "Honda"};
    auto u15= User{"Hyewon", "Kang"};
    auto u16= User{"Yena", "Choi"};
    auto u17= User{"Yujin", "Ahn"};
    auto u18= User{"Nako", "Yabuki"};
    auto u19= User{"Chaeyeon", "Lee"};
    auto u20= User{"Gaeul", "Kim"};
    auto u21= User{"Rei", "Naoi"};
    auto u22= User{"Jiwon", "Kim"};
    auto u23= User{"Hyeonseo", "Lee"};

    cout << "Newjeans!!!\n";
    cout << u1.getFirstName() << " " << u1.getLastName() << "\n";

    cout << "\nLe serafim!!!\n";
    cout << u5.getFirstName() << " " << u5.getLastName() << "\n";

}
