#ifndef LOGIN_H
#define LOGIN_H

#include "View.h"

class LoginPrivate;

class Login : public View
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Login)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    static void registerMetaType();

    enum LoginAction {
        Created = Action::User,
        CheckUser
    };
    Q_ENUM(LoginAction)

    Login(QObject *parent = nullptr);
    virtual ~Login();

    bool subscribe(int action, const QVariant &param) Q_DECL_OVERRIDE;

    QString title() const;
    void setTitle(const QString &title);

Q_SIGNALS:
    void titleChanged();

private:
    Q_D(Login);
};

#endif // LOGIN_H
