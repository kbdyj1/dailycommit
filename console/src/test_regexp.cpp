#include <QRegularExpression>

namespace { //=================================================================

// Validating
// Searching
// Search and Replace
// String Splitting

// QRegularExpression implements Perl-compatible regular expressions

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

} // namespace ================================================================

void test_regexp()
{
    test_extracting_captured_substrings();
}
