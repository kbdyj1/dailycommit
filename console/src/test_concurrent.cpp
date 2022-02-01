#include <QtConcurrent>
#include <QFuture>

namespace internal //==========================================================
{

/******************************************************************************
 * task
 *
 */
void test_concurrent_task()
{
#if (0)
    auto s = QString{"Hello,"};
    QtConcurrent::task([](QString &s){
        s.append("Qt");
    })
    .withArguments(std::ref(s))
    .spawn();

    qDebug() << s;
#else
    auto task = [](const QString &s){ qDebug().noquote() << "Hello, " << s; };
    QtConcurrent::task(std::move(task))
            .withArguments("Qt")
            .spawn();
#endif
}
/******************************************************************************
 * map
 *
 */
int square(int v)
{
    return v * v;
}
void sum(int &result, int value)
{
    result += value;
    qDebug() << "thread(" << QThread::currentThreadId() << ") -> sum(" << result << ", " << value << ")";
}

void test_concurrent_map()
{
    auto vlist = std::vector { 1, 2, 3, 4, 5 };
    auto sqlist = QtConcurrent::mapped(vlist, square);

    for (auto v : sqlist) {
        qDebug() << v;
    }

    auto final = QtConcurrent::blockingMappedReduced(vlist, square, sum);
    qDebug() << "\nfinal:" << final;
}

/******************************************************************************
 * filter
 *
 */
bool allLowerCase(const QString &s)
{
#if (0)
    auto rx = QRegularExpression{"[A-Z]"};

    return !rx.match(s).hasMatch();
#else
    return s.toLower() == s;
#endif
}

void test_concurrent_filter()
{
    auto slist = QStringList {
            QLatin1String{"hello, Qt"},
            QLatin1String{"hello, github"},
            QLatin1String{"hello, WSL"},
            QLatin1String{"hello, osx"},
            QLatin1String{"hello, c++2a"}
    };
    auto filtered = QtConcurrent::blockingFiltered(slist, allLowerCase);

    qDebug() << "all lower-case strings.";
    for (const auto &s : filtered) {
        qDebug() << s;
    }
}

} // internal =================================================================

void test_concurrent()
{
    //internal::test_concurrent_map();
    //internal::test_concurrent_filter();
    internal::test_concurrent_task();
}
