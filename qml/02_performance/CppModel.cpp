#include "CppModel.h"
#include <QDebug>
#include <algorithm>

namespace {
constexpr int kDataSize{300};
}

CppModel::CppModel(QObject* parent) : QAbstractListModel(parent)
{
    mValues.reserve(kDataSize);

    std::fill_n(std::back_inserter(mValues), kDataSize, 2023);
}

int CppModel::rowCount(const QModelIndex &parent) const
{
    return mValues.count();
}

QVariant CppModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant{};
    }

    QVariant ret;
    if (role == Qt::DisplayRole) {
        ret = mValues.at(index.row());
    }

    return ret;
}

QHash<int, QByteArray> CppModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::DisplayRole, "text");

    return roles;
}

const QList<int>& CppModel::rawList() const
{
    return mValues;
}

void CppModel::prependValueToModel()
{
    beginInsertRows(QModelIndex{}, 0, 0);
    mValues.prepend(1993);
    endInsertRows();

    emit rawListChanged();
}
