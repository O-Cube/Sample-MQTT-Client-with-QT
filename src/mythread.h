#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(const QString &message, const QString &topic, QObject *parent = nullptr);
    ~MyThread();
    void run() override;
signals:
    void queryStatus( const QString status );
    void queryError( const QString error );
    void db_status( const QString status );
private:
    QString m_message;
    QString m_topic;
};

#endif // MYTHREAD_H
