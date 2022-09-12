#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std::string_literals;

namespace { //=================================================================

void test_create_locale()
{
    auto locale = std::locale{};    // default

    auto locale_us = std::locale{"en_US.utf8"};

    auto locale0 = std::locale{locale, new std::collate<wchar_t>};

    auto locale1 = std::locale{locale, locale_us, std::locale::collate};
}

void test_locale_using_name(std::locale locale)
{
    auto now = std::chrono::system_clock::now();
    auto st = std::chrono::system_clock::to_time_t(now);
    auto lt = std::localtime(&st);



    std::cout.imbue(locale);

    std::cout << "1000.50: " << 1000.50 << "\n";

    std::cout << std::showbase << std::put_money(1050) << "\n";

    std::cout << std::put_time(lt, "%c") << "\n";

    auto names = std::vector<std::string>{
        "John", "adele", "øinvind", "Francois", "Robert", "Ake"
    };

    auto sortAndPrint = [](std::vector<std::string> v, const std::locale& loc) {
        std::sort(std::begin(v), std::end(v), loc);
        std::copy(std::begin(v), std::end(v), std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << "\n";
    };

    sortAndPrint(names, locale);
}

void test_locale()
{
    //---------------------------------------------------------------
    // sudo apt-get install locales
    // sudo locale-gen de_DE.UTF8
    //---------------------------------------------------------------

    std::locale::global(std::locale{"sv_SE.utf8"});

    //auto locale = std::locale{"de_DE.utf8"};
    //auto locale = std::locale{""};
    //auto locale = std::locale{};
    auto locale = std::locale::classic();

    test_locale_using_name(locale);
}

void test_manip()
{
#if (0)
    std::cout << std::right << std::setw(10) << "right" << "\n";
    std::cout << std::setw(10) << "text" << "\n";
    std::cout << std::left << std::setw(10) << "left" << "\n";
#endif

    std::cout << std::right << std::setfill('.') << std::setw(15) << "fixed: " << std::fixed << 0.25 << "\n";
    std::cout << std::right << std::setfill('.') << std::setw(15) << "scientific: " << std::scientific << 0.25 << "\n";
    std::cout << std::right << std::setfill('.') << std::setw(15) << "hexfloat: " << std::hexfloat << 0.25 << "\n";
    std::cout << std::right << std::setfill('.') << std::setw(15) << "defaultfloat: " << std::defaultfloat << 0.25 << "\n\n";

    std::cout << "oct: " << std::oct << 42 << "\n";
    std::cout << "hex: " << std::hex << 42 << "\n";
    std::cout << "dec: " << std::dec << 42 << "\n";

    std::cout << std::fixed << std::setprecision(2) << 123.4567 << "\n";    // 123.46
}

void test_currency()
{
    auto locale = std::locale{"en_GB.utf8"};
    auto money = 12345.67;
    auto strMoney = "12345.67"s;

    auto international = false;

    std::cout.imbue(locale);
    std::cout << std::showbase << std::put_money(money, international) << "\n";
    std::cout << std::showbase << std::put_money(strMoney, international) << "\n";

    long double v0;
    std::string v1;
    auto in = std::istringstream{"$123.45 987.65 USD"};
    in.imbue(std::locale{"en_US.utf8"});
    in >> std::get_money(v0, international) >> std::get_money(v1, international);

    std::cout << std::showbase << "v0: " << v0 << ", v1: " << v1 << std::endl;
}

void test_time()
{
    auto tt = std::time(nullptr);
    auto lt = std::localtime(&tt);

    std::cout.imbue(std::locale{"en_GB.utf8"});
    // ISO 8601
    std::cout << std::put_time(lt, "%F") << "\n";       // 2022-09-02
    std::cout << std::put_time(lt, "%T") << "\n";       // 23:39:09
    std::cout << std::put_time(lt, "%FT%T%z") << "\n";  // 2022-09-02T23:40:45+0900 (ISO 8601 UTC)
    std::cout << std::put_time(lt, "%Y-W%V") << "\n";   // 2022-W35
    std::cout << std::put_time(lt, "%Y-W%V-%u") << "\n";// 2022-W35-5
    std::cout << std::put_time(lt, "%Y-%j") << "\n";    // 2022-245

    auto time = std::tm{};
    auto str = std::istringstream{"2022-09-02T23:45:05+0900"};
    str >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S");

    std::cout.imbue(std::locale{"en_US.utf8"});
    if (!str.fail()) {
        std::cout << (1900+time.tm_year) << ", " << (1+time.tm_mon) << ", " << time.tm_mday << "\n";
        std::cout << time.tm_hour << ", " << time.tm_min << ", " << time.tm_sec << "\n";
    } else {
        std::cout << "str.fail()\n";
    }
}

} //namespace =================================================================

void test_ch_07_stream()
{
#if (0)
    test_create_locale();
    test_locale();
    test_manip();
    test_currency();
#endif

    test_time();
}
