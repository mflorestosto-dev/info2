#include "mainwindow.h"

MainWindow::MainWindow(const QString &clientName, double initialBalance, QWidget *parent)
    : QWidget(parent), clientBalance(initialBalance), clientName(clientName) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Información del cliente
    clientNameLabel = new QLabel(QString("Cliente: %1").arg(clientName), this);
    clientNameLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #0047AB; text-align: center;");
    clientNameLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(clientNameLabel);

    balanceLabel = new QLabel(this);
    updateBalance(); // Actualiza el texto y color del saldo
    balanceLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(balanceLabel);

    // Estado del sistema
    statusLabel = new QLabel("Seleccione un modo de lavado:", this);
    statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    layout->addWidget(statusLabel);

    // Cargar el historial al iniciar
    loadHistoryFromJson();

    // Botones para seleccionar el tipo de lavado
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    quickButton = new QPushButton("Lavado Rápido ($2.00)", this);
    quickButton->setIcon(QIcon(":/icons/quick.png"));
    connect(quickButton, &QPushButton::clicked, this, &MainWindow::startWashingQuick);
    buttonsLayout->addWidget(quickButton);

    normalButton = new QPushButton("Lavado Normal ($3.00)", this);
    normalButton->setIcon(QIcon(":/icons/normal.png"));
    connect(normalButton, &QPushButton::clicked, this, &MainWindow::startWashingNormal);
    buttonsLayout->addWidget(normalButton);

    centrifugeButton = new QPushButton("Centrifugado ($1.50)", this);
    centrifugeButton->setIcon(QIcon(":/icons/centrifuge.png"));
    connect(centrifugeButton, &QPushButton::clicked, this, &MainWindow::startWashingCentrifuge);
    buttonsLayout->addWidget(centrifugeButton);

    layout->addLayout(buttonsLayout);

    // Configuración del reproductor de música
    audioPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    audioPlayer->setAudioOutput(audioOutput);
    audioPlayer->setSource(QUrl::fromLocalFile("C:/Users/Frave/Desktop/interfaz/sonido.mp3"));
    audioOutput->setVolume(0.5);
    audioPlayer->setLoops(QMediaPlayer::Infinite);

    // Barra de progreso
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; } "
                               "QProgressBar::chunk { background-color: violet; width: 20px; }");
    layout->addWidget(progressBar);

    timeRemainingLabel = new QLabel("Tiempo restante: 0 segundos", this);
    timeRemainingLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(timeRemainingLabel);

    // Mensaje de agradecimiento
    thankYouLabel = new QLabel("Gracias por confiar en nosotros.", this);
    thankYouLabel->setAlignment(Qt::AlignCenter);
    thankYouLabel->setStyleSheet("font-size: 16px; color: #1E90FF;");
    thankYouLabel->hide(); // Inicialmente oculto
    layout->addWidget(thankYouLabel);

    // Botón para detener el lavado
    stopButton = new QPushButton("Detener Lavado", this);
    stopButton->setEnabled(false);
    stopButton->setStyleSheet("background-color: red; color: white; font-size: 16px;");
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopWashing);
    layout->addWidget(stopButton);

    // Botón para mostrar el historial
    showHistoryButton = new QPushButton("Ver Historial de Lavados", this);
    connect(showHistoryButton, &QPushButton::clicked, this, &MainWindow::showWashHistory);
    layout->addWidget(showHistoryButton);

    // Botón de ayuda
    helpButton = new QPushButton("Ayuda", this);
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::showHelp);
    helpButton->setStyleSheet("font-size: 14px; background-color: #0078D7; color: white; border-radius: 5px;");
    layout->addWidget(helpButton);

    // Botón para finalizar sesión
    endSessionButton = new QPushButton("Finalizar Sesión", this);
    connect(endSessionButton, &QPushButton::clicked, this, &MainWindow::endSession);
    endSessionButton->setStyleSheet("font-size: 14px; background-color: #FF4500; color: white; border-radius: 5px;");
    layout->addWidget(endSessionButton);

    // Temporizador para actualizar el progreso
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);

    setLayout(layout);
}

void MainWindow::startWashingQuick() {
    startWashing("Lavado Rápido", 20.0, 2.0);
}

void MainWindow::startWashingNormal() {
    startWashing("Lavado Normal", 40.0, 3.0);
}

void MainWindow::startWashingCentrifuge() {
    startWashing("Centrifugado", 15.0, 1.5);
}

void MainWindow::startWashing(const QString &mode, double time, double cost) {
    if (clientBalance < cost) {
        QMessageBox::warning(this, "Saldo insuficiente", "No tienes suficiente saldo para este lavado.");
        return;
    }

    // Actualizar saldo
    clientBalance -= cost;
    updateBalance();

    // Registrar el lavado en el historial
    QString timestamp = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString historyEntry = QString("[%1] %2 - $%3").arg(timestamp).arg(mode).arg(cost, 0, 'f', 2);
    washHistory.append(historyEntry);

    // Guardar el historial en JSON
    saveHistoryToJson();

    // Configuración del lavado
    washingTime = time;
    timeLeft = washingTime;

    progressBar->setValue(0);
    statusLabel->setText("Modo seleccionado: " + mode);
    timer->start(1000);

    // Mostrar mensaje de agradecimiento
    thankYouLabel->show();

    // Deshabilitar botones
    quickButton->setEnabled(false);
    normalButton->setEnabled(false);
    centrifugeButton->setEnabled(false);
    stopButton->setEnabled(true);

    // Configuración de música
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioPlayer->play();
}

void MainWindow::stopWashing() {
    timer->stop();

    // Actualizar barra de progreso y mensaje de estado
    progressBar->setValue(0);
    progressBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; } "
                               "QProgressBar::chunk { background-color: red; width: 20px; }");

    thankYouLabel->hide();

    quickButton->setEnabled(true);
    normalButton->setEnabled(true);
    centrifugeButton->setEnabled(true);
    stopButton->setEnabled(false);

    statusLabel->setText("Lavado detenido.");
    statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: red;");

    // Cálculo del reembolso parcial
    double completedRatio = (washingTime - timeLeft) / washingTime; // Porcentaje completado
    double refundAmount = (1.0 - completedRatio) * lastCost; // Reembolso proporcional
    if (refundAmount > 0) {
        clientBalance += refundAmount;
        updateBalance();
        QMessageBox::information(this, "Reembolso parcial",
                                 QString("Se ha devuelto $%1 a su saldo.").arg(refundAmount, 0, 'f', 2));
    }
    audioPlayer->stop(); // Detener la música
}



void MainWindow::updateProgress() {
    timeLeft -= 1.0;
    int progress = static_cast<int>((1.0 - timeLeft / washingTime) * 100.0);
    progressBar->setValue(progress);

    timeRemainingLabel->setText(QString("Tiempo restante: %1 segundos").arg(static_cast<int>(timeLeft)));

    if (timeLeft <= 0) {
        timer->stop();
        stopButton->setEnabled(false);
        quickButton->setEnabled(true);
        normalButton->setEnabled(true);
        centrifugeButton->setEnabled(true);

        thankYouLabel->hide(); // Ocultar mensaje de agradecimiento
        statusLabel->setText("Lavado completado.");
        QMessageBox::information(this, "Lavado completado", "El lavado ha finalizado.");
    }
}

void MainWindow::updateBalance() {
    balanceLabel->setText(QString("Saldo: $%1").arg(clientBalance, 0, 'f', 2));
    if (clientBalance < 2.0) {
        balanceLabel->setStyleSheet("font-size: 16px; color: red;");
    } else if (clientBalance <= 5.0) {
        balanceLabel->setStyleSheet("font-size: 16px; color: orange;");
    } else {
        balanceLabel->setStyleSheet("font-size: 16px; color: green;");
    }
}

void MainWindow::showWashHistory() {
    if (washHistory.isEmpty()) {
        QMessageBox::information(this, "Historial de Lavados", "No hay lavados registrados.");
    } else {
        QString history = washHistory.join("\n");
        QMessageBox::information(this, "Historial de Lavados", history);
    }
}


void MainWindow::showHelp() {
    QMessageBox::information(this, "Ayuda",
                             "1. Verifique que tiene saldo suficiente.\n"
                             "2. Seleccione un modo de lavado.\n"
                             "3. Puede detener el lavado en cualquier momento.\n"
                             "4. Si tiene dudas, contacte con el personal.");
}

void MainWindow::saveHistoryToJson() {
    QFile file("wash_history.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray historyArray;
        for (const auto &entry : washHistory) {
            historyArray.append(entry);
        }
        QJsonDocument jsonDoc(historyArray);
        file.write(jsonDoc.toJson());
        file.close();
        qDebug() << "Historial guardado correctamente en JSON.";
    } else {
        qDebug() << "No se pudo abrir el archivo para guardar el historial.";
    }
}

double MainWindow::getClientBalance() const {
    return clientBalance;
}

void MainWindow::loadHistoryFromJson() {
    QFile file("wash_history.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonArray historyArray = jsonDoc.array();

        washHistory.clear();
        for (const auto &entry : historyArray) {
            washHistory.append(entry.toString());
        }

        file.close();
        qDebug() << "Historial cargado desde JSON.";
    } else {
        qDebug() << "No se pudo abrir el archivo de historial. Se iniciará vacío.";
    }
}


void MainWindow::endSession() {
    emit sessionEnded(); // Emitir señal para cerrar sesión
    close();             // Cerrar MainWindow
}
