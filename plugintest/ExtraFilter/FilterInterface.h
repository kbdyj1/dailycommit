#ifndef FILTERINTERFACE_H
#define FILTERINTERFACE_H

#include <QString>
#include <QImage>

class FilterInterface
{
public:
    virtual ~FilterInterface()
    {}

    virtual QStringList filters() const = 0;
    virtual QImage filterImage(const QString &filter, const QImage &image) = 0;
};

#define FilterInterface_iid "com.github.kbdyj1.dailyproject.plugintest.extrafilter.FilterInterface/1.0"
Q_DECLARE_INTERFACE(FilterInterface, FilterInterface_iid)

#endif // FILTERINTERFACE_H
