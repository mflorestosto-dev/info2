#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>

#define PORTsERVER 10234

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_LedGreen_clicked();
    void on_pushButton_LedRed_clicked();
    void on_pushButton_LedBlue_clicked();
    void onNewConnection();
    void onReadyRead();
    QString generateChecksum(const QString &data);
    void onStateChanged(QAbstractSocket::SocketState state);
    void MessageReceived(const QString &msg);

    void on_paradaButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer server;
    QList<QTcpSocket*> sockets;
};

#endif // MAINWINDOW_H
