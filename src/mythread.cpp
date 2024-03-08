#include "mythread.h"
#include "databasemanagement.h"

#include <QMutex>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

static QMutex myMutex;

MyThread::MyThread( const QString &message, const QString &topic, QObject *parent )
    : m_message( message )
    , m_topic( topic )
    , QThread( parent )
{}

MyThread::~MyThread()
{

}

void MyThread::run()
{
    myMutex.lock();

    DatabaseManagement db;
    emit db_status( db.getDB_status() );
    db.beginTransaction();
    QStringList tableList = db.tableList();
    bool tableExist = false;
    for ( auto table : tableList ) {
        if ( table == m_topic.replace( '/', '_' ) ) {
            tableExist = true;
            break;
        }
    }

    if ( tableExist ) {
        QSqlQuery query( *db.getDatabase() );
        query.prepare( "INSERT INTO " + m_topic.replace( '/', '_' ) + " ( Data, Time )"
                          + "VALUES ( :Data, :Time );" );
        query.bindValue( 0, m_message.toInt() );
        query.bindValue( 1, QDateTime::currentDateTime() );

        if ( !query.exec() ) {
            emit queryError( query.lastError().text() );
        }
        db.commitTransaction();
        emit queryStatus( "Data written into " + m_topic.replace( '/', '_' ) + " was successful!" );
        myMutex.unlock();
    } else {
        QSqlQuery query( *db.getDatabase() );
        query.prepare( "CREATE TABLE " + m_topic.replace( '/', '_' ) + "("
                        + "id INT AUTO_INCREMENT,"
                        + "Data INT NOT NULL,"
                        + "Time DATETIME NOT NULL,"
                        + "PRIMARY KEY(id) );" );
        query.exec();
        emit queryError( query.lastError().text() );
        query.prepare( "INSERT INTO " + m_topic.replace( '/', '_' ) + "(Data, Time) VALUES( :Data, :Time );" );
        query.bindValue(0, m_message.toInt() );
        query.bindValue(1, QDateTime::currentDateTime() );
        if ( !query.exec() ) {
            emit queryError( query.lastError().text() );
        } else {
            emit queryStatus( "Data written into " + m_topic.replace(  '/', '_' ) + " was successful!" );
        }
        db.commitTransaction();
        myMutex.unlock();
    }
}
