#ifndef EXTRAFILTERPLUGIN_H
#define EXTRAFILTERPLUGIN_H

#include "ExtraFilter_global.h"
#include <QObject>
#include "FilterInterface.h"

class EXTRAFILTER_EXPORT ExtraFilterPlugin : public QObject, public FilterInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID FilterInterface_iid FILE "extrafilters.json")
    Q_INTERFACES(FilterInterface)

public:
    QStringList filters() const;
    QImage filterImage(const QString &filter, const QImage &image);
};

#endif // EXTRAFILTERPLUGIN_H
