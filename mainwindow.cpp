#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackScreens->setCurrentIndex(0);

    auto createRow = [&](int row, const std::vector<QString>& keys) {
        for (int i = 0; i < keys.size(); ++i) {
            QPushButton* btn = new QPushButton(keys[i]);
            btn->setMinimumSize(45, 45);

            if (keys[i] == "Space") btn->setMinimumWidth(300);
            if (keys[i] == "Shift" || keys[i] == "Caps" || keys[i] == "Tab" || keys[i] == "Enter") btn->setMinimumWidth(80);

            ui->gridLayoutKeyboard->addWidget(btn, row, i);
        }
    };

    createRow(0, {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "←"});
    createRow(1, {"Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]"});
    createRow(2, {"Caps", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "Enter"});
    createRow(3, {"Shift", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Shift"});

    QPushButton* space = new QPushButton("Space");
    space->setMinimumHeight(45);
    ui->gridLayoutKeyboard->addWidget(space, 4, 0, 1, 14, Qt::AlignCenter);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_btnStartTraining_clicked() { ui->stackScreens->setCurrentIndex(1); }
void MainWindow::on_btnRestart_clicked() { ui->stackScreens->setCurrentIndex(1); }
void MainWindow::on_btnReturnToMain_clicked() { ui->stackScreens->setCurrentIndex(0); }
void MainWindow::on_actionExit_triggered() { close(); }
