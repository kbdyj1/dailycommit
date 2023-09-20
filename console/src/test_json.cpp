#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>

#define TAB_SIZE    3

namespace internal
{

void print_bracket(char c, int depth)
{
    auto str = QString{};
    str.fill(' ', depth*TAB_SIZE).append(c);
    qDebug().noquote() << str;
}
void print_key(const QString &key, int depth)
{
    auto str = QString{};
    str.fill(' ', depth*TAB_SIZE).append(key).append(":");
    qDebug().noquote() << str;
}
void print_key_value(const QString &key, const QString &value, int depth)
{
    auto str = QString{};
    if (key.length()) {
        str.fill(' ', depth*TAB_SIZE).append(key).append(": ").append(value);
    } else {
        str.fill(' ', depth*TAB_SIZE).append(value);
    }
    qDebug().noquote() << str;
}

void parse_object(const QJsonObject &object, int depth);
void parse_array(const QJsonArray &array, int depth);

void parse_value(const QString &key, const QJsonValue &value, int depth) {
    auto valueText = QString{};
    switch (value.type()) {
    case QJsonValue::Bool:
        valueText = QString{"%1"}.arg(value.toBool());
        break;
    case QJsonValue::Double:
        valueText = QString{"%1"}.arg(value.toDouble());
        break;
    case QJsonValue::String:
        valueText = QString{"%1"}.arg(value.toString());
        break;
    case QJsonValue::Array:
        {
            print_key(key, depth);
            auto array = value.toArray();
            parse_array(array, depth);
        }
        break;
    case QJsonValue::Object:
        {
            print_key(key, depth);
            auto object = value.toObject();
            parse_object(object, depth);
        }
        break;
    case QJsonValue::Null:
        valueText = QString{"Null"};
        break;
    case QJsonValue::Undefined:
        valueText = QString{"Undefined"};
        break;
    }
    if (valueText.length()) {
        print_key_value(key, valueText, depth);
    }
}

void parse_object(const QJsonObject &object, int depth) {
    print_bracket('{', depth);

    for (auto key : object.keys()) {
        auto value = object.value(key);
        parse_value(key, value, depth+1);
    }
    print_bracket('}', depth);
}

void parse_array(const QJsonArray &array, int depth) {
    print_bracket('[', depth);

    auto key = QString{};
    for (auto iter = array.begin(); iter != array.end(); iter++) {
        auto value = *iter;
        parse_value(key, value, depth+1);
    }

    print_bracket(']', depth);
}

void test_json_doc()
{
    auto file = QFile{":/person.json"};
    auto result = file.open(QFile::ReadOnly);
    qDebug() << file.fileName() << "open :" << result;
    if (result) {
        auto ba = file.readAll();
        auto error = QJsonParseError{};
        auto doc = QJsonDocument::fromJson(ba, &error);
        auto depth = 0;
        if (!doc.isNull()) {
            if (doc.isObject()) {
                auto object = doc.object();
                parse_object(object, depth);
            } else {
                auto array = doc.array();
                parse_array(array, depth);
            }
        } else {
            qDebug() << error.errorString();
        }
    }
}

}

void test_json()
{
/****************************************************************************** internal::test_json_doc() output
":/person.json" open : true
{
   Address:
   {
      City: London
      Country: Great Britain
      Street: Downing Street 10
   }
   Age: 43
   FirstName: John
   LastName: Doe
   Phone numbers:
   [
      +44 1234567
      +44 2345678
   ]
}
*/
    internal::test_json_doc();
}
