#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QDebug>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    trainingTimer = new QTimer(this);
    connect(trainingTimer, &QTimer::timeout, this, &MainWindow::updateTimer);

    loadLessonsList();
    loadSettings();
    setupKeyboard();

    ui->stackScreens->setCurrentIndex(0);
    this->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {
    saveSettings();
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (ui->stackScreens->currentIndex() != 1) return;

    QString enteredText = event->text();
    if (event->key() == Qt::Key_Space) enteredText = " ";
    if (enteredText.isEmpty()) return;

    QString expectedText = model.getRemainingText().left(1);

    if (enteredText == expectedText) {
        model.advance();
        updateDisplay();
        highlightKey(enteredText == " " ? "Space" : enteredText.toUpper());
    } else {
        model.addError();
        updateDisplay();
    }
}

void MainWindow::updateDisplay() {
    QString typed = model.getTypedText();
    QString remaining = model.getRemainingText();

    if (remaining.isEmpty() && !typed.isEmpty()) {
        trainingTimer->stop();
        if (ui->lblFinalStats) {
            ui->lblFinalStats->setText(QString("Помилок: %1 | Точність: %2% | Швидкість: %3 CPM")
                                           .arg(model.getErrors())
                                           .arg(model.getAccuracy(), 0, 'f', 1)
                                           .arg(model.getCPM(secondsElapsed)));
        }
        ui->stackScreens->setCurrentIndex(2);
        return;
    }

    QString currentChar = remaining.left(1);
    if (currentChar == " ") currentChar = "&nbsp;";

    QString html = QString("<span style='color: #4CAF50;'>%1</span>"
                           "<span style='background-color: #FFF59D; border-bottom: 2px solid black;'>%2</span>"
                           "<span style='color: #9E9E9E;'>%3</span>")
                       .arg(typed.toHtmlEscaped(), currentChar, remaining.mid(1).toHtmlEscaped());

    ui->textDisplay->setHtml(html);
    ui->progressBar->setValue(model.getProgress());

    if (ui->lblLiveStats) {
        ui->lblLiveStats->setText(QString("Час: %1с | Помилок: %2 | Точність: %3%")
                                      .arg(secondsElapsed)
                                      .arg(model.getErrors())
                                      .arg(model.getAccuracy(), 0, 'f', 1));
    }
}

void MainWindow::updateTimer() {
    secondsElapsed++;
    if (ui->lblLiveStats) {
        ui->lblLiveStats->setText(QString("Час: %1с | Помилок: %2 | Точність: %3%")
                                      .arg(secondsElapsed)
                                      .arg(model.getErrors())
                                      .arg(model.getAccuracy(), 0, 'f', 1));
    }
}

void MainWindow::on_btnStartTraining_clicked() {
    QString selected = ui->comboLesson->currentText();
    if (ui->comboLesson->currentIndex() == 0 && ui->comboLesson->count() > 1) {
        int idx = QRandomGenerator::global()->bounded(1, ui->comboLesson->count());
        selected = ui->comboLesson->itemText(idx);
    }

    if (!selected.isEmpty() && selected != "--- Випадковий урок ---") {
        loadFileContent(selected);
        secondsElapsed = 0;
        updateDisplay();
        ui->stackScreens->setCurrentIndex(1);
        trainingTimer->start(1000);

        this->setFocus();
        this->activateWindow();
    }
}

void MainWindow::loadSettings() {
    QSettings settings("Academy", "TypingTrainer");
    QString lastFile = settings.value("lastFile", "").toString();
    int index = ui->comboLesson->findText(lastFile);
    if (index != -1) ui->comboLesson->setCurrentIndex(index);
}

void MainWindow::saveSettings() {
    QSettings settings("Academy", "TypingTrainer");
    settings.setValue("lastFile", ui->comboLesson->currentText());
}

void MainWindow::highlightKey(const QString &keyText) {
    for (int i = 0; i < ui->gridLayoutKeyboard->count(); ++i) {
        QPushButton *btn = qobject_cast<QPushButton*>(ui->gridLayoutKeyboard->itemAt(i)->widget());
        if (btn && btn->text() == keyText) {
            btn->setStyleSheet("background-color: #81C784; color: white;");
            QTimer::singleShot(100, [btn]() { btn->setStyleSheet(""); });
            break;
        }
    }
}

void MainWindow::loadLessonsList() {
    ui->comboLesson->clear();
    ui->comboLesson->addItem("--- Випадковий урок ---");
    QDir dir(QDir::currentPath() + "/lessons");
    if (!dir.exists()) dir.mkpath(".");
    ui->comboLesson->addItems(dir.entryList({"*.txt"}, QDir::Files));
}

void MainWindow::loadFileContent(const QString &fileName) {
    QFile file(QDir::currentPath() + "/lessons/" + fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        model.setText(in.readAll().replace("\r\n", " ").replace("\n", " ").trimmed());
        file.close();
    }
}

void MainWindow::setupKeyboard() {
    auto addRow = [&](int row, const std::vector<QString>& keys) {
        for (int i = 0; i < (int)keys.size(); ++i) {
            QPushButton* btn = new QPushButton(keys[i]);
            btn->setMinimumSize(40, 40);
            btn->setFocusPolicy(Qt::NoFocus);
            if (keys[i] == "Space") btn->setMinimumWidth(300);
            ui->gridLayoutKeyboard->addWidget(btn, row, i);
        }
    };
    addRow(0, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"});
    addRow(1, {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"});
    addRow(2, {"A", "S", "D", "F", "G", "H", "J", "K", "L", ";"});
    addRow(3, {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"});
    addRow(4, {"Space"});
}

void MainWindow::on_btnRestart_clicked() { on_btnStartTraining_clicked(); }
void MainWindow::on_btnReturnToMain_clicked() { ui->stackScreens->setCurrentIndex(0); }
void MainWindow::on_actionExit_triggered() { close(); }
