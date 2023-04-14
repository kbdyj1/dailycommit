#ifndef CPPMODEL_H
#define CPPMODEL_H

#include <QAbstractListModel>

class CppModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QList<int> rawList READ rawList NOTIFY rawListChanged)

public:
    explicit CppModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    const QList<int>& rawList() const;
    Q_INVOKABLE void prependValueToModel();

signals:
    void rawListChanged();

private:
    QList<int> mValues;
};

#endif // CPPMODEL_H
