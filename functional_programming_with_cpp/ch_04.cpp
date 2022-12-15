#include <iostream>
#include <functional>

namespace { //=================================================================

namespace _1 {

std::function<int(int)> compose(std::function<int(int)> f, std::function<int(int)> g)
{
    return [f, g](auto value) {
        return f(g(value));
    };
}

void test() {
    auto increment = [](const int value) {
        return value + 1;
    };
    auto incrementTwice = [increment](int value) {
        return increment(increment(value));
    };

    auto result = incrementTwice(1);

    std::cout << "result == 3 ? " << result << "\n";

    auto square = [](int value) {
        return value * value;
    };
    auto incrementSquare = [=](int value) {
        return increment(square(value));
    };

    std::cout << "incrementSquare(3): " << incrementSquare(3) << "\n";

    std::cout << "compose(increment, square)(3): " << compose(increment, square)(3) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

int increment(int value) {
    return value + 1;
}
int square(int value) {
    return value * value;
}

template <typename F, typename G>
auto compose(F f, G g) {
    return [=](auto value) {
        return f(g(value));
    };
}

template <typename F>
auto decomposeToOneParameter(F f) {
    return [=](auto first) {
        return [=](auto second) {
            return f(first, second);
        };
    };
}

template <typename T>
T multiply(T first, T second) {
    return first * second;
}

template <typename F, typename G>
auto composeWithTwoParameters(F f, G g) {
    return [=](auto first, auto second) {
        return compose(f, decomposeToOneParameter(g)(first))(second);
    };
}

void test()
{
    auto incrementTwice = compose(increment, increment);
    std::cout << "incrementTwice(2): " << incrementTwice(2) << "\n";

    auto incrementSquare = compose(increment, square);
    std::cout << "incrementSquare(2): " << incrementSquare(2) << "\n";

    auto multiplyDecomposed = [](const int first) {
        return [=](int second) {
            return first * second;
        };
    };

    std::cout << "multiplyDecomposed(2)(3): " << multiplyDecomposed(2)(3) << "\n";

    std::cout << "decomposeToOneParameter(multiply<int>)(2)(3): " << decomposeToOneParameter(multiply<int>)(2)(3) << "\n";

    {
        auto incrementResultOfMultiplication = compose(increment, multiplyDecomposed(4));
        std::cout << "incrementResultOfMultiplication(5): " << incrementResultOfMultiplication(5) << "\n";
    }
    {
        auto incrementResultOfMultiplication = [](int first, int second) {
            return compose(increment, decomposeToOneParameter(multiply<int>)(first))(second);
        };
        std::cout << "incrementResultOfMultiplication(4, 5): " << incrementResultOfMultiplication(4, 5) << "\n";
    }
    {
        auto multiplyIncrementedValues = [](int first, int second) {
            return multiply(increment(first), increment(second));
        };
        std::cout << "multiplyIncrementedValues(2, 3): " << multiplyIncrementedValues(2, 3) << "\n";
    }
    {
        auto multiplyIncrementedValues = [=](int first, int second) {
            return multiplyDecomposed(increment(first))(increment(second));
        };
        std::cout << "multiplyIncrementedValues(4, 5): " << multiplyIncrementedValues(4, 5) << "\n";
    }
    {
        auto multiplyIncrementedValues = [=](int first, int second) {
            return compose(decomposeToOneParameter(multiply<int>), increment)(first)(increment(second));
        };
        std::cout << "multiplyIncrementedValues(2, 7): " << multiplyIncrementedValues(2, 7) << "\n";
    }

    std::cout << "composeWithTwoParameters(increment, multiply<int>)(3, 3): " << composeWithTwoParameters(increment, multiply<int>)(3, 3) << "\n";
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_04()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
