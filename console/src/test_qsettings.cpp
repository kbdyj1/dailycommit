#include <QSettings>
#include <QMetaEnum>
#include <QDir>

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

void test_setting_class()
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

}

void test_qsettings()
{
    internal::test_setting_class();
}

#include "test_qsettings.moc"
