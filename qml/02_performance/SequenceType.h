#ifndef SEQUENCETYPE_H
#define SEQUENCETYPE_H

#include <QQuickItem>

class SequenceType : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<qreal> qrealList READ qrealList WRITE setQRealList NOTIFY qrealListChanged)

public:
    SequenceType() : QQuickItem() {
        mList << 1.1 << 1.2 << 1.3;
    }
    ~SequenceType()
    {}

    QList<qreal> qrealList() const {
        return mList;
    }
    void setQRealList(const QList<qreal>& list) {
        mList = list;
        emit qrealListChanged();
    }

signals:
    void qrealListChanged();

private:
    QList<qreal> mList;
};

#endif // SEQUENCETYPE_H
