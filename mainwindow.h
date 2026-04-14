#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QKeyEvent>
#include "typingmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnStartTraining_clicked();
    void on_btnRestart_clicked();
    void on_btnReturnToMain_clicked();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    TypingModel model;

    void setupKeyboard();
    void updateDisplay();
    void loadLessonsList();
    void loadFileContent(const QString &fileName);


    void highlightKey(const QString &keyText);
};
#endif
