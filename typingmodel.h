#ifndef TYPINGMODEL_H
#define TYPINGMODEL_H

#include <QString>

class TypingModel {
public:
    void setText(const QString &text);
    void advance();
    int getProgress() const;
    bool isFinished() const;
    QString getTypedText() const;
    QString getRemainingText() const;

private:
    QString fullText;
    int currentIndex = 0;
};

#endif
