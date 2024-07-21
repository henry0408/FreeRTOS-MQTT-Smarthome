#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QList>
#include <QByteArray>
#include <QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QUdpSocket* udpSocket;
    QHostAddress    m_hostAddr;     //保存目标的地址对象
    quint16         m_port;         //保存目标的端口号(类型一致)

    bool bind(QString ip, ushort port);

    //通过该函数发送数据
    void sendData(QString data);

    //设置目标主机的ip和端口号
    void setTargetInfo(QString ip, quint16 port);

private slots:
    void on_pushButtonConnect_clicked();

    void on_pushButtonSend_clicked();

    void readPendingDatagrams();

    void on_pushButtonDiscon_clicked();

    void on_pushButtonLightOn_clicked();

    void on_pushButtonLightOff_clicked();

    void on_pushButtonFanOn_clicked();

    void on_pushButtonFanOff_clicked();

    void on_pushButtonLeaveHome_clicked();

    void on_pushButtonReturnHome_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
