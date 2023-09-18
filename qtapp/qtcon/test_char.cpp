#include <QChar>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test()
{
    auto c = QChar{u'\U0000D55C'};
    qDebug() << c;
    qDebug() << c.category();   //QChar::Letter_Other
    qDebug() << "cell: " << c.cell();
    qDebug() << "row : " << c.row();
    qDebug() << "combiningClass: " << c.combiningClass();
    qDebug() << "currentUnicodeVersion: " << c.currentUnicodeVersion();
    qDebug() << "decomposition: " << c.decomposition();
    qDebug() << "decompositionTag: " << c.decompositionTag();
    qDebug() << "digitValue: " << c.digitValue();
    qDebug() << "direction: " << c.direction();
    qDebug() << "isLetter: " << c.isLetter();
    qDebug() << "isLower: " << c.isLower();
    qDebug() << "isMark: " << c.isMark();
    qDebug() << "isNonCharacter: " << c.isNonCharacter();
    qDebug() << "isPrint: " << c.isPrint();
    qDebug() << "isPunct: " << c.isPunct();
    qDebug() << "isSymbol: " << c.isSymbol();
    qDebug() << "isTitleCase: " << c.isTitleCase();
    qDebug() << "joiningType: " << c.joiningType();
    qDebug() << "script: " << c.script();
    qDebug() << "toLatin1: " << c.toLatin1();
    qDebug() << "unicode: " << c.unicode();
    qDebug() << "unicodeVersion: " << c.unicodeVersion();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_char()
{
    _1::test();
}
