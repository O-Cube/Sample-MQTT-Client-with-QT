#ifndef MQTTDINTERFACE_H
#define MQTTDINTERFACE_H

#include "ui_uiclient.h"
#include <QMainWindow>

class QMqttTopicName;

class MyMqttClient;

class MQTTDatabaseClient : public QMainWindow
{
    Q_OBJECT
public:
    explicit MQTTDatabaseClient( QWidget *parent = nullptr );
    ~MQTTDatabaseClient();
private slots:
    void onSubscribedButton_clicked();
    void onPublishButton_clicked();
    void onRadioButton_clicked();
    void onQuery_status( const QString status );
    void onQuery_error( const QString error );
private:
    Ui::UIClient *ui;
    MyMqttClient *client;

    void processReceived_value( const QString &message, const QString &topic );
};

#endif // MQTTDINTERFACE_H
