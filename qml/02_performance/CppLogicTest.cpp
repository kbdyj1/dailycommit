#include "CppLogicTest.h"
#include <QRandomGenerator>
#include <QtConcurrent>

CppLogicTest::CppLogicTest(QObject *parent) : QObject{parent}
{
    for (auto i=0u; i<mValuesCount; i++) {
        mValues.append(QRandomGenerator::global()->generateDouble());
    }
}

const QList<double> CppLogicTest::values() const {
    return mValues;
}

QList<double> CppLogicTest::calculateSqrt() {
    QList<double> ret;

    for (const auto& val : mValues) {
        ret.append(sqrt(val));
    }

    return ret;
}

QList<double> CppLogicTest::calculateSqrtAsync()
{
    auto ret = values();
    QtConcurrent::blockingMap(ret, [=](double& value){
        value = sqrt(value);
    });

    return ret;
}
