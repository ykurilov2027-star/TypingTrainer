#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackScreens->setCurrentIndex(0);

    lessons << "jfj j jff ffjjjj fjjf"
            << "The quick brown fox jumps over the lazy dog"
            << "int main() { return 0; }"
            << "1234 5678 !@#$ %^&*";

    setupKeyboard();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupKeyboard() {
    auto addRow = [&](int row, const std::vector<QString>& keys) {
        for (int i = 0; i < (int)keys.size(); ++i) {
            QPushButton* btn = new QPushButton(keys[i]);
            btn->setMinimumSize(40, 40);
            if (keys[i] == "Space") btn->setMinimumWidth(250);
            ui->gridLayoutKeyboard->addWidget(btn, row, i);
        }
    };
    addRow(0, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"});
    addRow(1, {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"});
    addRow(2, {"A", "S", "D", "F", "G", "H", "J", "K", "L", ";"});
    addRow(3, {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"});
}

void MainWindow::startLesson(int index) {
    if (index >= 0 && index < lessons.size()) {
        model.setText(lessons.at(index));
        updateDisplay();
    }
}

void MainWindow::updateDisplay() {
    QString typed = model.getTypedText();
    QString remaining = model.getRemainingText();

    QString html = QString("<span style='color: green;'>%1</span>"
                           "<span style='background-color: yellow;'>%2</span>"
                           "<span>%3</span>")
                       .arg(typed, remaining.left(1), remaining.mid(1));

    ui->textDisplay->setHtml(html);
    ui->progressBar->setValue(model.getProgress());

    if (model.isFinished()) {
        ui->stackScreens->setCurrentIndex(2);
    }
}

void MainWindow::on_btnStartTraining_clicked() {
    startLesson(ui->comboLesson->currentIndex());
    ui->stackScreens->setCurrentIndex(1);
}

void MainWindow::on_btnRestart_clicked() {
    startLesson(ui->comboLesson->currentIndex());
}

void MainWindow::on_btnReturnToMain_clicked() {
    ui->stackScreens->setCurrentIndex(0);
}

void MainWindow::on_actionExit_triggered() {
    close();
}
