#ifndef DATABASEMANAGEMENT_H
#define DATABASEMANAGEMENT_H

#include <memory>
#include <QSqlDatabase>

//class QString;

class DatabaseManagement
{
public:
    DatabaseManagement();
    ~DatabaseManagement();

    QStringList tableList() const;
    void beginTransaction() const;
    void commitTransaction() const;
    QSqlDatabase *getDatabase() const;
    QString getDB_status() const;
signals:
    void databaseStatus( const QString &status );

private:
    std::unique_ptr<QSqlDatabase> db;
    QString m_status;
};

#endif // DATABASEMANAGEMENT_H
