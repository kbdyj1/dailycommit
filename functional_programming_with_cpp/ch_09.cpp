#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <assert.h>

namespace { //=================================================================

namespace _1 {

void test_greater_than() {
    int l = 3;
    int r = 2;

    auto result = std::greater<int>()(l, r);
    std::cout << "greater(2, 3) ? " << result << "\n";

    result = std::greater<int>()(l, l);
    std::cout << "greater(2, 2) ? " << result << "\n";
}

void test() {
    test_greater_than();
}

} //_1 --------------------------------------------------------------

namespace _2 {

struct TestData {
    int l;
    int r;
    bool expected;
};

std::map<std::string, TestData> testData{
    { "2 is greater than 1", { 2, 1, true } },
    { "2 is not greater than 2", { 2, 2, false } },
    { "2 is not greater than 3", { 2, 3, false } }
};

void test() {
    for (auto iter = testData.begin(); iter != testData.end(); iter++) {
        auto data = iter->second;
        auto result = std::greater<int>()(data.l, data.r);
        std::cout << iter->first << " -> " << result << ", expected(" << data.expected << ")\n";
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

struct Card {
    int type;   //0[♣], 1[♥], 2[♦], 3[♠]
    char value;  //2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A
};
bool operator<(const Card& l, const Card& r) {
    if (l.value == r.value) {
        return l.type < r.type;
    } else {
        return l.value < r.value;
    }
}
bool operator>(const Card& l, const Card& r) {
    return r < l;
}

typedef std::vector<Card> Cards;


auto has5Cards =[](const auto& hand) {
    return hand.size() == 5;
};

auto isSameSuit = [](const auto& hand) {
    return std::equal(hand.begin()+1, hand.end(), hand.begin(), [](const auto& l, const auto& r){
        return l.type == r.type;
    });
};

auto allValuesInOrder = [](const auto& hand) {
    auto seq = std::string("23456789TJQKA");
    auto cards = std::string{};
    for (const auto& card : hand) {
        cards += card.value;
    }
    //auto pos = seq.find(cards);
    //auto notFound = pos == std::string::npos;
    //std::cout << "cards: " << cards << ", notFound: " << notFound << "\n";

    return seq.find(cards) != std::string::npos;
};

auto isStraightFlush = [](const auto& hand) {
    return has5Cards(hand) &&
           isSameSuit(hand) &&
           allValuesInOrder(hand);
};

auto compareStraightFlushes = [](const auto& l, const auto& r) {
    auto lValue = l.front();
    auto rValue = r.front();
    if (lValue > rValue) {
        return 1;
    } else if (lValue < rValue) {
        return -1;
    } else {
        return 0;
    }
};

auto comparePokerHands = [](const auto& l, const auto& r) {
    if (isStraightFlush(l) && isStraightFlush(r)) {
        return compareStraightFlushes(l, r);
    } else if (isStraightFlush(l)) {
        return 1;
    } else if (isStraightFlush(r)) {
        return -1;
    }
    return 0;
};
void test_win_with_straight_flush() {
    Cards r{ {0,'2'}, {2,'4'}, {1,'7'}, {3,'9'}, {1,'A'} };

    //2 based straigt flush
    {
        Cards l{ {3,'2'}, {3,'3'}, {3,'4'}, {3,'5'}, {3,'6'} };

        auto result = comparePokerHands(l, r);
        assert(result == 1);
    }
    //3 based straigt flush
    {
        Cards l{ {3,'3'}, {3,'4'}, {3,'5'}, {3,'6'}, {3,'7'} };

        auto result = comparePokerHands(l, r);
        assert(result == 1);
    }
    //10 based straigt flush
    {
        Cards l{ {3,'T'}, {3,'J'}, {3,'Q'}, {3,'K'}, {3,'A'} };

        auto result = comparePokerHands(l, r);
        assert(result == 1);
    }
    //right win
    {
        Cards l{ {3,'2'}, {3,'3'}, {3,'4'}, {3,'5'}, {3,'9'} };
        Cards r{ {0,'2'}, {0,'3'}, {0,'4'}, {0,'5'}, {0,'6'} };

        auto result = comparePokerHands(l, r);
        assert(result == -1);
    }
    //right win
    {
        Cards l{ {3,'2'}, {3,'3'}, {3,'4'}, {3,'5'}, {3,'6'} };
        Cards r{ {0,'3'}, {0,'4'}, {0,'5'}, {0,'6'}, {0,'7'} };

        auto result = comparePokerHands(l, r);
        assert(result == -1);
    }
    std::cout << "test_win_with_straight_flush : PASS\n";
}

void test_is_straight_flush() {
    {
        Cards hand{ {0,'2'}, {0,'3'}, {0,'4'}, {0,'5'}, {0,'6'} };
        auto result = isStraightFlush(hand);
        assert(result == true);
    }

    {
        Cards hand{ {1,'2'}, {0,'3'}, {0,'4'}, {0,'5'}, {0,'6'} };
        auto result = isStraightFlush(hand);
        assert(result == false);
    }
    {
        Cards hand{ {0,'2'}, {0,'3'}, {0,'4'}, {0,'5'}, {0,'7'} };
        auto result = isStraightFlush(hand);
        assert(result == false);
    }
    //empty hand
    {
        Cards hand{};
        auto result = isStraightFlush(hand);
        assert(result == false);
    }

    std::cout << "test_is_straight_flush : PASS\n";
}

void test() {
    test_win_with_straight_flush();
    test_is_straight_flush();
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_09()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
