#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QStringList>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>


class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(const QString &clientName, double initialBalance, QWidget *parent = nullptr);
    double getClientBalance() const; // Declaración del método

signals:
    void sessionEnded(); // Para regresar a la ventana de bienvenida

private slots:
    void startWashingCentrifuge();
    void startWashingNormal();
    void startWashingQuick();
    void stopWashing();
    void updateProgress();
    void showWashHistory();
    void showHelp(); // Ayuda para el cliente
    void endSession(); // Finalizar la sesión

private:
    void startWashing(const QString &mode, double time, double cost);
    void updateBalance();

    void saveHistoryToJson(); // Guarda el historial en un archivo JSON
    void loadHistoryFromJson(); // Carga el historial desde un archivo JSON (opcional, si deseas cargar datos previos)

    QLabel *statusLabel;
    QLabel *clientNameLabel;
    QLabel *balanceLabel;
    QLabel *timeRemainingLabel;
    QLabel *thankYouLabel; // Mensaje de agradecimiento
    QProgressBar *progressBar;
    QPushButton *stopButton;
    QPushButton *centrifugeButton;
    QPushButton *normalButton;
    QPushButton *quickButton;
    QPushButton *showHistoryButton;
    QPushButton *helpButton; // Botón de ayuda
    QPushButton *endSessionButton; // Botón para finalizar sesión
    QTimer *timer;

    QList<QString> washHistory; // Historial de lavados

    double clientBalance; // Saldo actual del cliente
    double washingTime;   // Tiempo total del lavado actual
    double timeLeft;      // Tiempo restante del lavado actual
    double lastCost;      // Costo del último lavado
    QString clientName;

    // Reproductor de música
    QMediaPlayer *audioPlayer;
    QAudioOutput *audioOutput;
};

#endif // MAINWINDOW_H
