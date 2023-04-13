#ifndef CPPLOGICTEST_H
#define CPPLOGICTEST_H

#include <QObject>
#include <QList>

class CppLogicTest : public QObject
{
    Q_OBJECT

public:
    CppLogicTest(QObject* parent = nullptr);

    Q_INVOKABLE const QList<double> values() const;
    Q_INVOKABLE QList<double> calculateSqrt();
    Q_INVOKABLE QList<double> calculateSqrtAsync();

private:
    unsigned int mValuesCount = 1000000;
    QList<double> mValues;
};

#endif // CPPLOGICTEST_H
