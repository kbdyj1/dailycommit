#include "big.h"
#include <QDebug>

Big::Big()
{
}

void Big::write(const QString &message)
{
    qDebug().noquote().nospace() << "Big::write(" << message << ")";
}
