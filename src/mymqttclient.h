#ifndef MYMQTTCLIENT_H
#define MYMQTTCLIENT_H

#include <QMqttClient>

class MyMqttClient : public QMqttClient
{
    Q_OBJECT
public:
    explicit MyMqttClient( QObject *parent = nullptr );
    QString getErrMessage() const;
    QString getConnectMessage() const;
private:
    QString errmsg;
    QString connectionmsg;
    quint16 m_port;

    QString myClientConnectionMessage( const QMqttClient::ClientState state ) const;
    QString myClientErrorMessage( const QMqttClient::ClientError state ) const;
};

#endif // MYMQTTCLIENT_H
