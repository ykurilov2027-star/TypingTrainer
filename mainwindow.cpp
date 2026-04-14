#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QDebug> // Для перевірки роботи
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackScreens->setCurrentIndex(0);

    loadLessonsList();
    setupKeyboard();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadLessonsList() {
    ui->comboLesson->clear();
    ui->comboLesson->addItem("--- Випадковий урок ---");

    // Шлях до папки з уроками
    QDir dir(QDir::currentPath() + "/lessons");
    if (!dir.exists()) {
        dir.mkpath(".");
        qDebug() << "Папка lessons створена за шляхом:" << dir.absolutePath();
    }

    QStringList files = dir.entryList({"*.txt"}, QDir::Files);
    ui->comboLesson->addItems(files);
}

void MainWindow::loadFileContent(const QString &fileName) {
    QFile file(QDir::currentPath() + "/lessons/" + fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        model.setText(in.readAll().trimmed());
        file.close();
    } else {
        qDebug() << "Не вдалося відкрити файл:" << fileName;
    }
}

// ГОЛОВНА КНОПКА
void MainWindow::on_btnStartTraining_clicked() {
    qDebug() << "Button Start Clicked!"; // Якщо бачиш це в консолі - кнопка підключена!

    int currentIndex = ui->comboLesson->currentIndex();
    int count = ui->comboLesson->count();
    QString selectedFile;

    if (currentIndex == 0) { // Випадковий вибір
        if (count > 1) {
            int randomIdx = QRandomGenerator::global()->bounded(1, count);
            selectedFile = ui->comboLesson->itemText(randomIdx);
        } else {
            qDebug() << "Файлів .txt не знайдено в папці /lessons/";
            return;
        }
    } else {
        selectedFile = ui->comboLesson->currentText();
    }

    if (!selectedFile.isEmpty()) {
        loadFileContent(selectedFile);
        updateDisplay();
        ui->stackScreens->setCurrentIndex(1); // Перехід на екран тренування
    }
}

void MainWindow::updateDisplay() {
    QString typed = model.getTypedText();
    QString remaining = model.getRemainingText();

    QString html = QString("<span style='color: #4CAF50;'>%1</span>"
                           "<span style='background-color: #FFF59D;'>%2</span>"
                           "<span style='color: #9E9E9E;'>%3</span>")
                       .arg(typed, remaining.left(1), remaining.mid(1));

    ui->textDisplay->setHtml(html);
    ui->progressBar->setValue(model.getProgress());

    if (model.isFinished()) ui->stackScreens->setCurrentIndex(2);
}

void MainWindow::setupKeyboard() {
    // Твій код генерації клавіш...
    auto addRow = [&](int row, const std::vector<QString>& keys) {
        for (int i = 0; i < (int)keys.size(); ++i) {
            QPushButton* btn = new QPushButton(keys[i]);
            btn->setMinimumSize(40, 40);
            ui->gridLayoutKeyboard->addWidget(btn, row, i);
        }
    };
    addRow(0, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"});
    addRow(1, {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"});
    addRow(2, {"A", "S", "D", "F", "G", "H", "J", "K", "L", ";"});
    addRow(3, {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"});
}

void MainWindow::on_btnRestart_clicked() { on_btnStartTraining_clicked(); }
void MainWindow::on_btnReturnToMain_clicked() { ui->stackScreens->setCurrentIndex(0); }
void MainWindow::on_actionExit_triggered() { close(); }
