#include "databasemanagement.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QObject>


DatabaseManagement::DatabaseManagement()
    : db( new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "mysampledatabase" ) ) )
    , m_status( "" )
{
    db->setHostName( "172.30.224.1" );
    db->setPort( 3306 );
    db->setDatabaseName("mysampledatabase");
    db->setUserName( "client01" );
    db->setPassword( "rootRoot55" );

    if ( !db->open() ) {
        m_status = db->lastError().text();
    }
    m_status = "Database opens successfully";
}

DatabaseManagement::~DatabaseManagement()
{
    db->close();
}

QStringList DatabaseManagement::tableList() const
{
    return db->tables();
}

void DatabaseManagement::beginTransaction() const
{
    db->transaction();
}

void DatabaseManagement::commitTransaction() const
{
    db->commit();
}

QSqlDatabase *DatabaseManagement::getDatabase() const
{
    return db.get();
}

QString DatabaseManagement::getDB_status() const
{
    return m_status;
}
