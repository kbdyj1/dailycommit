#include <QRegularExpression>
#include <QDebug>

namespace { //=================================================================

// Validating
// Searching
// Search and Replace
// String Splitting

// QRegularExpression implements Perl-compatible regular expressions

void print_matched(const QRegularExpressionMatch& match)
{
//    qDebug() << "match.hasMatch(): " << match.hasMatch();
//    qDebug() << "match.hasPartialMatch(): " << match.hasPartialMatch();
    qDebug() << match;
}

void print_matched_all(QRegularExpressionMatchIterator& iter)
{
    while (iter.hasNext()) {
        auto value = iter.next();
        qDebug() << value;
    }
}

void test_regexp_construct()
{
    auto rx0 = QRegularExpression("a pattern");
    auto rx1 = QRegularExpression("\\d\\d \\w+");
    auto rx2 = QRegularExpression(R"(\d\d \w+)");   // raw string literal

    auto match = rx2.match("abc123 def");
    if (match.hasMatch()) {
        auto start = match.capturedStart();
        auto end = match.capturedEnd();
        auto len = match.capturedLength();
        auto s = match.captured(0);

        qDebug() << "start: " << start << ", end: " << end << ", len: " << len << ", captured: " << s;
    }
}

void test_extracting_captured_substrings()
{
    auto rx = QRegularExpression(R"(^(?<day>\d\d)/(?<month>\d\d)/(?<year>\d\d\d\d)$)");
    auto match = rx.match("08/12/2022");
#if (0)
    auto day = match.captured(1);
    auto month = match.captured(2);
    auto year = match.captured(3);
#else
    auto day = match.captured("day");
    auto month = match.captured("month");
    auto year = match.captured("year");
#endif

    qDebug() << "day: " << day << ", month: " << month << ", year: " << year;
    qDebug() << "captured(0): " << match.captured(0);
    for (auto i=1; i<=3; i++) {
        qDebug() << "(" << i << ") : start: " << match.capturedStart(i) << ", end: " << match.capturedEnd(i);
    }
}

void test_pattern_option()
{
    auto rx = QRegularExpression("Qt", QRegularExpression::CaseInsensitiveOption);
    auto match = rx.match("qt company");
    if (match.hasMatch()) {
        qDebug() << match.captured();
    }
}

void test_match_use_offset()
{
    auto rx = QRegularExpression(R"(\d\d \w+)");
    auto match = rx.match("12 abc 45 def", 1);
    if (match.hasMatch()) {
        qDebug() << match.captured();
    }
}

void test_global_matching()
{
    auto rx = QRegularExpression(R"(\w+)");
    auto i = rx.globalMatch("The qt company");
#if (1)
    while (i.hasNext()) {
        qDebug() << "peekNext: " << i.peekNext();
        auto match = i.next();
        qDebug() << "match.captured(): " << match.captured();
    }
#else
    for (const auto& match : i) {
        qDebug() << match.captured();
    }
#endif
}

void test_partial_matching()
{
    //#1
    {
        auto rx = QRegularExpression(R"(\d\d\d\d)");
        auto match = rx.match("123 456", 0, QRegularExpression::PartialPreferCompleteMatch);
        qDebug() << "match.hasMatch(): " << match.hasMatch();
        qDebug() << "match.hasPartialMatch(): " << match.hasPartialMatch();
        if (match.hasPartialMatch()) {
            qDebug() << match.captured();
            qDebug() << match.captured(1);
        }
    }
    //#2
    {
        auto rx = QRegularExpression(R"(abc\w+X|def)");
        auto match = rx.match("abcdef abc00X", 0, QRegularExpression::PartialPreferCompleteMatch);
        print_matched(match);
    }
}

void test_incremental_multi_segment_matching()
{
    //#1
    {
        auto rx = QRegularExpression("abc|ab");
        auto match = rx.match("ab", 0, QRegularExpression::PartialPreferFirstMatch);
        print_matched(match);
    }
    //#2
    {
        auto rx = QRegularExpression("abc(def)?");
        auto match = rx.match("abc", 0, QRegularExpression::PartialPreferFirstMatch);
        print_matched(match);
    }
    //#3
    {
        auto rx = QRegularExpression("(abc)*");
        auto match = rx.match("abc", 0, QRegularExpression::PartialPreferFirstMatch);
        print_matched(match);
    }
}

void test_error_handling()
{
    auto rx = QRegularExpression("(unmatched|parenthesis");
    if (!rx.isValid()) {
        qDebug().noquote() << rx.errorString();
        qDebug().noquote() << rx.pattern();
        auto position = QString(rx.patternErrorOffset(), ' ');
        position += "^";
        qDebug().noquote() << position;
    }
}

#define STR \
    "sales1.xls\n" \
    "orders3.xls\n" \
    "sales2.xls\n" \
    "sales3.xls\n" \
    "apac1.xls\n" \
    "europe2.xls\n" \
    "sam.xls\n" \
    "na1.xls\n" \
    "na2.xls\n" \
    "sa1.xls\n" \
    "ca1.xls"

void test_dot()
{
    auto s = QString(STR);
    //auto rx = QRegularExpression("[ns]a.\.xls");
    //auto rx = QRegularExpression("[ns]a[0123456789]\.xls");
    //auto rx = QRegularExpression("[ns]a[0-9]\.xls");
    //auto rx = QRegularExpression("[ns]a[^0-9]\.xls");
    auto rx = QRegularExpression("[ns]a[[:digit:]]\.xls");  // POSIX character class
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_email_address()
{
    auto s = QString("Hello, Qt!!! .gildong.hong@test.company.com is my e-mail address");
    auto rx = QRegularExpression(R"(\w+[\w.]*@[\w.]+\.\w+)");
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_url()
{
    auto s = QString("company url : https://git.company.com");
    auto rx = QRegularExpression(R"(https?:\/\/[\w.]+\.\w+)");
    auto match = rx.match(s);
    if (match.hasMatch()) {
        qDebug() << match.captured();
    }
}

void test_word_boundary()
{
    {
        auto s = QString{"The cat scattered his food all over the room."};
        //auto rx = QRegularExpression{R"(\bcat\b)"};
        auto rx = QRegularExpression{R"(cat)"};
        auto iter = rx.globalMatch(s);
        print_matched_all(iter);
    }
    {
        auto s = QString{"zero-digit invalid - hypen."};
        auto rx0 = QRegularExpression{R"(\b-\b)"};
        auto m0 = rx0.match(s);
        qDebug() << rx0;
        print_matched(m0);

        auto rx1 = QRegularExpression{R"(\B-\B)"};
        auto m1 = rx1.match(s);
        qDebug() << rx1;
        print_matched(m1);
    }
}

void test_xml()
{
    {
        auto rx = QRegularExpression{R"(<\?xml.*\?>)"};
        auto s = QString{R"(<?xml version="1.0" encoding="UTF-8 ?>")"};
        auto match = rx.match(s);
        if (match.hasMatch()) {
            print_matched(match);
        }
    }
    {
        auto rx = QRegularExpression{R"(^\s*<\?xml.*\?>)"};
        auto s = QString{R"(no xml<?xml version="1.0" encoding="UTF-8 ?>")"};
        auto match = rx.match(s);
        if (match.hasMatch()) {
            print_matched(match);
        }
    }
}

void test_multiline()
{
    auto s = QString{
        "<script>\n"
        "function spellcheck(form, field){\n"
        "   // no empty\n"
        "   if (field.value == '') {\n"
        "       return false;\n"
        "   }\n"
        "   // init\n"
        "   var windowName='spellWindow'\n"
        "   var spellCheckUrl='spell.cfm?formname=comment&fieldname='+field.name\n"
        "   // done\n"
        "   return true;\n"
        "}\n"
        "</script>"
    };
    //auto rx = QRegularExpression{R"(^\s*\/\/.*$)"};
    auto rx = QRegularExpression{R"((?m)^\s*\/\/.*$)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_blank()
{
    auto s = QString{"Hello, Qt6.&nbsp;multi platform&nbsp;&nbsp; library"};
    auto rx = QRegularExpression{"(&nbsp;){2,}"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_ipv4_expression()
{
    auto s = QString{"192.168.0.1 255.255.255.128"};
    auto rx = QRegularExpression{R"((\d{1,3}\.){3}\d{1,3})"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_valid_ipv4_expression()
{
    auto s = QString{"192.168.0.1 255.255.255.128 1.0.256.3 300.1.2.244"};
    auto rx = QRegularExpression{R"(((\d{1,2}|1\d{2}|2([0-4]\d|5[0-5]))\.){3}(\d{1,2}|1\d{2}|2([0-4]\d|5[0-5])))"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_html_tag()
{
    auto s = QString{
        "<body>\n"
        "<h1>title</h1>\n"
        "contents 1\n"
        "<h2>Qt</h2>\n"
        "version 6.0\n"
        "<h2>illigal expression</h3>\n"
        "<h2>c++21</h2>\n"
        "new c++ language\n"
        "</body>"
    };
    auto rx = QRegularExpression{R"(<[hH](\d)>.*?</[hH]\1>)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_dereference()
{
    auto s = QString{
        "This is a block of of text,\n"
        "servral words here are are\n"
        "repeated, and and they\n"
        "should not be."
    };
    auto rx = QRegularExpression{R"([ ]+(\w+)[ ]+\1)"}; // javascript /1, perl $1
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_date_expression()
{
    auto s = QString{
            "ID: 008\n"
            "SEX: M\n"
            "BIRTH: 1975-01-01\n"
            "STATUS: ACTIVE"
    };
    auto rx = QRegularExpression{R"((19|20)\d{2})"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_replace()
{
    auto s = QString{"Hello, yuri.cho@bbb.com"};
    auto rx = QRegularExpression{R"((\w+[\w\.]*@\w+[\w\.]+\w+))"};
    auto m = rx.match(s);
    if (m.isValid()) {
        auto html = QString{R"(<a href="mailto:%1">%1</a>)"}.arg(m.captured());
        qDebug().noquote() << html;
    }
}

void test_lookahead()
{
    auto s = QString{
        "http://www.aaa.com\n"
        "https://mail.aaa.com\n"
        "ftp://ftp.aaa.com"
    };
#if (1)
    auto rx = QRegularExpression{".+(?=:)"};
#else
    auto rx = QRegularExpression{".+:"};
#endif
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_lookbehind()
{
    auto s = QString{
        "ABC: $23.45\n"
        "EFG: $10.00\n"
        "HIJ: $34.90\n"
        "KLM: $80.15\n"
        "Total 4 items"
    };
    auto rx = QRegularExpression{R"((?<=\$)[\d.]+)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_look_ahead_and_behind()
{
    auto s = QString{"<head><title>Regular expression example</title></head>"};
    auto rx = QRegularExpression{"(?<=<[Tt][Ii][Tt][Ll][Ee]>).*(?=</[Tt][Ii][Tt][Ll][Ee]>)"};
    auto m = rx.match(s);
    print_matched(m);
}

void test_negative_look()
{
    auto s = QString{
        "I paid $30 for 100 apples,\n"
        "50 oranges, and 60 pears.\n"
        "I saved $5 on this order"
    };
    auto rx = QRegularExpression{R"(\b(?<!\$)\d+)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_condition()
{
    auto s = QString{
        "123-456-7890\n"
        "(123)456-7890\n"
        "(123)-456-7890\n"
        "(123-456-7890\n"
        "1234567890\n"
        "123 456 7890\n"
    };
#if (0)
    auto rx = QRegularExpression{R"(\(?\d{3}\)?-?\d{3}-\d{4})"};
#else
    auto rx = QRegularExpression{R"((\()?\d{3}(?(1)\)|-)\d{3}-\d{4})"};
#endif
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_dereference_condition()
{
    auto s = QString{
        R"("
        "<!-- Nav bar -->\n"
        "<div>\n"
        "<a href="/home"><img src="/images/home.gif"></a>\n"
        "<img src="/images/spacer.gif">\n"
        "<a href="/search"><img src="/images/search.gif"></a>\n"
        "<img src="/images/spacer.gif">\n"
        "<a href="/help"><img src="/images/help.gif"></a>\n"
        "</div>
        ")"
    };
    auto rx = QRegularExpression{R"((<[Aa]\s+[^>]+>\s*)?<[Ii][Mm][Gg]\s+[^>]+>(?(1)\s*<\/[Aa]>))"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_ahead_behind_condition()
{
    auto s = QString{
        "11111\n"
        "22222\n"
        "33333-\n"
        "44444-4444"
    };
    auto rx = QRegularExpression{R"(\d{5}(?(?=-)-\d{4}))"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_na_phone_number()
{
    auto s = QString{
        "A.Aaa : 012-345-6789\n"
        "B.Bbb : (234) 567-8901\n"
        "C.Ccc : (456)777-8888\n"
    };
    auto rx = QRegularExpression{R"(\(?[2-9]\d\d\)?[ -]?[2-9]\d\d-\d{4})"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_na_zip_code()
{
    auto s = QString{
        "999 1st Aveneu, Bigtown, NY, 11122\n"
        "123 High Street, Any City, MI 40000-1234"
    };
    auto rx = QRegularExpression{R"(\d{5}(-\d{4})?)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_ca_postal_code()
{
    auto s = QString{
        "123 4th Street, Toronto, Ontario, M1A 1A2\n"
        "567 8th, MOntreal, Quebec, H9Z 9Z9"
    };
    auto rx = QRegularExpression{R"([ABCEGHJKLMNPRSTVXY]\d[A-Z] \d[A-Z]\d)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_uk_postal_code()
{
    auto s = QString{
        "171 Kyverdale Road, London N16 6PS\n"
        "33 Main Street, Portsmouth, P01 3AX\n"
        "17 High Street, London NW11 8AB"
    };
    auto rx = QRegularExpression{R"([A-Z]{1,2}\d[A-Z\d]? \d[ABDEFGHJLNPQRSTUWXYZ]{2})"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_us_social_code()
{
    auto s = QString{
        "Hungmin.Son: 123-45-6789\n"
        "HarryCane: 275-00-8444"
    };
    auto rx = QRegularExpression{R"(\d{3}-\d{2}-\d{4})"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_perfect_url()
{
    auto s = QString{
        "http://www.forta.com/blog\n"
        "https://www.forta.com:80/blog/index.html\n"
        "http://www.forta.com\n"
        "http://ben:password@www.forta.com\n"
        "http://localhost/index.php?ab=1&c=2\n"
        "https://localhost:8080/"
    };
#if (0)
    auto rx = QRegularExpression{R"(https?:\/\/[-\w.]+(:\d+)?(\/([\w\/_.]*)?)?)"};
#else
    auto rx = QRegularExpression{R"(https?:\/\/(\w+:\w+@)?[-\w.]+(:\d+)?(\/[\w\/_.]*(\?\S+)?)?)"};
#endif
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_email()
{
    auto s = QString{R"(my e-mail is heungmin.son@tottenham.com)"};
    auto rx = QRegularExpression{R"((\w+.)*\w+@(\w+\.)+[A-Za-z]+)"};
    auto m = rx.match(s);
    print_matched(m);
}

void test_html_comment()
{
    auto s = QString{R"("
        "<!-- Start of page --->\n"
        "<html>\n"
        "<!-- Start of head -->\n"
        "<head>\n"
        "<title>Hello, Qt 6</title><!-- Page title -->\n"
        "</head>\n"
        "<!-- Body -->\n"
        "<body>"
    ")"};
    auto rx = QRegularExpression{R"(<!-{2,}.*?-{2,}>)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_javascript_comment()
{
    auto s = QString{R"("
        "<scrpt language="JavaScript">\n"
        "//turn off fields used only by replace\n"
        "function replaces(){\n"
        "   document.getElementById('RegExPlace').disabled=true;\n"
        "   document.getElementById('replaceheader').disabled=true;\n"
        "}\n"
        "// Turn on fields\n"
        "function showReplaceFields(){\n"
        "   document.getElementById('RegExPlace').disabled=false;\n"
        "   document.getElementById('replaceheader').disabled=false;\n"
        "}\n"
    ")"};
    auto rx = QRegularExpression{R"(\/\/.*)"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_credit_card()
{
    auto s = QString{
        "MasterCard: 5212345678901234\n"
        "Visa 1: 4123456789012\n"
        "Visa 2: 4123456789012345\n"
        "Amex: 371234567890123\n"
        "Discover: 601112345678901234\n"
        "Diners Club: 38812345678901\n"
        "Diners Club: 30109876543219"
    };
    qDebug() << "*** Master Card ***";
    auto masterCard = QRegularExpression{R"(5[1-5]\d{14})"};
    auto iter = masterCard.globalMatch(s);
    print_matched_all(iter);

    qDebug() << "\n*** Visa Card ***";
    auto visaCard = QRegularExpression{R"(4\d{12}(\d{3})?)"};
    iter = visaCard.globalMatch(s);
    print_matched_all(iter);

    qDebug() << "\n*** Amex Card ***";
    auto amexCard = QRegularExpression{R"(3[4|7]\d{13})"};
    iter = amexCard.globalMatch(s);
    print_matched_all(iter);

    qDebug() << "\n*** Discover Card ***";
    auto discoverCard = QRegularExpression{R"(6011\d{14})"};
    iter = discoverCard.globalMatch(s);
    print_matched_all(iter);

    qDebug() << "\n*** Diners Card ***";
    auto dinersCard = QRegularExpression{R"(((30[0-5]|36\d|38\d)\d{11}))"};
    iter = dinersCard.globalMatch(s);
    print_matched_all(iter);
}

void test_resident_registration_number()
{
    auto s = QString{
        "450815-1001234\n"
        "909999-2345678\n"
        "325-120"
    };
    auto rx = QRegularExpression{R"(\d{2}(0\d|1[0-2])(0\d|[12]\d|3[01])-[1-4]\d{6})"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_iso_8691_date_format()
{
    auto s = QString{"08/14/1979"};
    auto rx = QRegularExpression{R"(([01]\d)/([012]\d|3[01])/(\d{4}))"};
    auto m = rx.match(s);
    if (m.hasMatch()) {
        auto yyyy = m.captured(3);
        auto mm = m.captured(1);
        auto dd = m.captured(2);

        qDebug().noquote().nospace() << "original: " << s << "\n"
                                     << "ISO-8691: " << yyyy << "-" << mm << "-" << dd << "\n";
    }
}

void test_replace_function()
{
    auto s = QString{R"("
        "def get_id(account):\n"
        "   return account['id']\n"
        "\n"
        "def get_useridle_time():\n"
        "   conf = load_conf('default.conf')\n"
        "   return conf['idle_time']"
    ")"};
    auto rx = QRegularExpression{"([_'])(id)(['(])"};
    auto iter = rx.globalMatch(s);
    print_matched_all(iter);
}

void test_uni_code()
{
    auto s = QString{"Copyright © 2022 The Qt Company"};
    auto rx = QRegularExpression{"\u00a9"};
    auto m = rx.match(s);
    print_matched(m);
}

} // namespace ================================================================

void test_regexp()
{
    test_uni_code();
}
