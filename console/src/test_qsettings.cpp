#include <QSettings>
#include <QMetaEnum>
#include <QDir>
#include <QDebug>

namespace Setting
{
    Q_NAMESPACE
    enum Value {
        One = 1,
        Two,
        Three,
        Four
    };
    Q_ENUM_NS(Value)
}

class Settings : private QSettings
{
    Q_OBJECT

public:
    using QSettings::QSettings;

    template <Setting::Value key>
    auto value()
    {
        if constexpr (key == Setting::One) {
            return value(key).toInt();
        } else if constexpr (key == Setting::Two) {
            return value(key).toString();
        } else if constexpr (key == Setting::Three) {
            return value(key).toDouble();
        } else if constexpr (key == Setting::Four) {
            return value(key).toBool();
        }
    }

    void setValue(Setting::Value key, const QVariant &value)
    {
        const auto meta = QMetaEnum::fromType<Setting::Value>();
        const auto text = QString::fromLatin1(meta.valueToKey(key));

        QSettings::setValue(text, value);
    }
    QVariant value(Setting::Value key)
    {
        const auto meta = QMetaEnum::fromType<Setting::Value>();
        const auto text = QString::fromLatin1(meta.valueToKey(key));

        return QSettings::value(text);
    }
};



namespace internal
{

void test_setting_using_metaenum()
{
    Settings settings(QDir::tempPath() + "/daily.ini", QSettings::IniFormat);
    settings.setValue(Setting::One, 1);
    settings.setValue(Setting::Two, "2");
    settings.setValue(Setting::Three, 3.14);
    settings.setValue(Setting::Four, true);

    auto print = [&](Setting::Value key) {
        const auto meta = QMetaEnum::fromType<Setting::Value>();
        const auto text = QString::fromLatin1(meta.valueToKey(key));
        const auto value = settings.value(key);

        qDebug() << text << ":" << value;
    };
    print(Setting::One);
    print(Setting::Two);
    print(Setting::Three);
    print(Setting::Four);
}

void test_setting_using_template()
{
    Settings settings(QDir::tempPath() + "/daily.ini", QSettings::IniFormat);
    settings.setValue(Setting::One, 1);
    settings.setValue(Setting::Two, "2");
    settings.setValue(Setting::Three, 3.14);
    settings.setValue(Setting::Four, true);

    qDebug() << "One :" << settings.value<Setting::One>();
    qDebug() << "Two :" << settings.value<Setting::Two>();
    qDebug() << "Three :" << settings.value<Setting::Three>();
    qDebug() << "Four :" << settings.value<Setting::Four>();
}

}

void test_qsettings()
{
    internal::test_setting_using_metaenum();
}

#include "test_qsettings.moc"
