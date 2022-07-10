#include <QRegularExpression>

namespace { //=================================================================

// Validating
// Searching
// Search and Replace
// String Splitting

// QRegularExpression implements Perl-compatible regular expressions

void print_matched(const QRegularExpressionMatch& match)
{
    qDebug() << "match.hasMatch(): " << match.hasMatch();
    qDebug() << "match.hasPartialMatch(): " << match.hasPartialMatch();
    qDebug() << match.captured();
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
#if (0)
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

} // namespace ================================================================

void test_regexp()
{
    test_error_handling();
}
