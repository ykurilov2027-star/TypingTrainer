#ifndef TYPINGMODEL_H
#define TYPINGMODEL_H

#include <QString>

class TypingModel {
public:
    void setText(const QString &text);
    void advance();
    void addError();

    int getProgress() const;
    bool isFinished() const;
    QString getTypedText() const;
    QString getRemainingText() const;

    int getErrors() const;
    double getAccuracy() const;
    int getCPM(int elapsedSeconds) const;

private:
    QString fullText;
    int currentIndex = 0;
    int errorCount = 0;
};

#endif
