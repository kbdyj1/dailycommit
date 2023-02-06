#include <QVarLengthArray>
#include <QVector>
#include <QDebug>
#include <memory>

namespace {  //================================================================

namespace _1 {

void test()
{
#if (1)
    auto v = QVarLengthArray<int, 64>{1, 2, 3, 4, 5, 6};
#else
    auto v = QVector<int>{1, 2, 3, 4, 5, 6};
#endif

    qDebug() << "size: " << v.size() << ", capacity: " << v.capacity();

    v.resize(10);

    for (auto i : v) {
        qDebug() << i << " ";
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

struct Student {
    int id;
    QString name;

    Student() : id(-1), name("Unnamed")
    {}
};

void test()
{
    auto v = QVarLengthArray<Student>(4);

    for (auto s : v) {
        qDebug() << "Student(" << s.id << ", " << s.name << ")";
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    auto p0 = std::make_unique<int[]>(100'000);
    qDebug() << "p0[0]: " << p0[0];

    auto p1 = std::unique_ptr<int[]>(new int[100'000]);
    qDebug() << "p1[0]: " << p1[0];
}

} //_3 --------------------------------------------------------------

namespace _4 {

struct CompilerFeature  {
    CompilerFeature(const char* name=nullptr, const char*  value=nullptr) : name(name), value(value)
    {}
    const char* name;
    const char* value;
};

#define COMPILER_FEATURE_VALUE(value) #value
#define COMPILER_FEATURE_ENTRY(name) { #name, COMPILER_FEATURE_VALUE(name) },

static CompilerFeature cxx_core[] = {
    COMPILER_FEATURE_ENTRY(__cpp_lib_make_unique)
    COMPILER_FEATURE_ENTRY(__cpp_lib_smart_ptr_for_overwrite)
};

void test()
{
    qDebug() << cxx_core[0].name << ": " << cxx_core[0].value;
    qDebug() << cxx_core[1].name << ": " << cxx_core[1].value;
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test()
{
    auto s = QString(10, Qt::Uninitialized);
    qDebug() << s;

    auto ba = QByteArray(10, Qt::Uninitialized);
    qDebug() << ba;
}

} //_5 --------------------------------------------------------------

} //===========================================================================

void test_qvarlengtharray()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
#endif

    _5::test();
}
