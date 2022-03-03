#include "ExtraFilterPlugin.h"
#include <QImage>

QStringList ExtraFilterPlugin::filters() const
{
    return QStringList{};
}

QImage ExtraFilterPlugin::filterImage(const QString &filter, const QImage &image)
{
    Q_UNUSED(filter)

    return image;
}
