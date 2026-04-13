#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "typingmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStartTraining_clicked();
    void on_btnRestart_clicked();
    void on_btnReturnToMain_clicked();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    TypingModel model;
    QStringList lessons;
    void setupKeyboard();
    void updateDisplay();
    void startLesson(int index);
};
#endif
