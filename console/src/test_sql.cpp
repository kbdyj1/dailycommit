#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QHostInfo>

/******************************************************************************
 * install Sqlite3
 *
 * sudo apt-get update
 * sudo apt-get install sqlite3
 */

namespace internal
{

void test_sqldatabase()
{
    const auto dbName = QLatin1String{"sample-db"};
    const auto tableName = QLatin1String{"customer"};
    const auto driverName = QLatin1String{"QSQLITE"};
    const auto hostName = QHostInfo::localHostName();

    auto drivers = QSqlDatabase::drivers();

    qDebug() << "supported drivers:" << drivers;
    qDebug() << "hostName:" << hostName;

    if (drivers.contains(driverName)) {
        auto db = QSqlDatabase::addDatabase(driverName);
        db.setHostName(hostName);
        db.setDatabaseName(dbName);

        qDebug() << "hasFeature(QuerySize) :" << db.driver()->hasFeature(QSqlDriver::QuerySize);
        qDebug() << "hasFeature(Transactions) :" << db.driver()->hasFeature(QSqlDriver::Transactions);

        auto ok = db.open();
        if (ok) {
            qDebug() << db.databaseName() << "open :" << ok;

            auto tables = db.tables();
            qDebug() << "tables:" << tables;

            if (!tables.contains(tableName))
            {
                auto createTable = QString{"create table %1 (id integer primary key, name varchar(64))"}.arg(tableName);
                auto query = QSqlQuery{};
                auto result = query.exec(createTable);

                qDebug() << createTable << ":" << result;
                if (!result) {
                    qDebug() << query.lastError();
                }
            }
            {
                auto insert = QLatin1String{"insert into customer (id, name) values (:id, :name)"};
                auto query = QSqlQuery{};
                query.prepare(insert);
                query.bindValue(":id", 1000);
                query.bindValue(":name", "qt6");
                auto result = query.exec();

                qDebug() << "insert :" << result;
                if (!result) {
                    qDebug() << query.lastError();
                }
            }

            {
                auto select = QString{"select * from %1"}.arg(tableName);
                auto query = QSqlQuery{select};
                auto result = query.exec();

                qDebug() << "query.result:" << result << "<-" << query.executedQuery();
                qDebug() << "query.size:" << query.size();  // sqlite3 return -1. (for databases that support it)
                qDebug() << "query.isSelect:" << query.isSelect();
                qDebug() << "query.isValid:" << query.isValid();

                while (query.next()) {
                    qDebug() << "id:" << query.value(0).toInt() << ", name:" << query.value(1).toString();
                }
            }
        } else {
            auto error = db.lastError();
            qDebug() << db.databaseName() << "fail :" << error;
        }
    }
}

} //internal

void test_sql()
{
    internal::test_sqldatabase();
}
