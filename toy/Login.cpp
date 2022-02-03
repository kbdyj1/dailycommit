#include "Login.h"
#include "qqml.h"

/******************************************************************************
 * Private Class
 *
 */
class LoginPrivate
{
public:
    QString mTitle;
};

/******************************************************************************
 * Login Class
 *
 */
void Login::registerMetaType()
{
    qRegisterMetaType<Login>();
    qmlRegisterType<Login>("Login", 1, 0, "Login");
}

Login::Login(QObject *parent)
    : View("Login", parent)
    , d(new LoginPrivate)
{
}

Login::~Login()
{
    delete d;
}

bool Login::subscribe(int action, const QVariant &param)
{
    switch (action) {
    case Action::Created:
        qDebug() << "Action.Login Created";
        setQmlProperty("title", "Hello, Qt!");
        break;

    case Login::Created:
        qDebug() << "Login.Created.";
        break;

    case Login::CheckUser:
        {
            auto m = param.toMap();
            auto id = m["id"].toString();
            auto password = m["password"].toString();

            qDebug() << "Login::CheckUser(id:" << id << ", password:" << password << ")";
        }
        break;
    }
    return false;
}

QString Login::title() const
{
    return d->mTitle;
}

void Login::setTitle(const QString &title)
{
    if (d->mTitle != title) {
        d->mTitle = title;
        emit titleChanged();
    }
}
