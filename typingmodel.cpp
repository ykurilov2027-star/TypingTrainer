#include "typingmodel.h"

void TypingModel::setText(const QString &text) {
    fullText = text;
    currentIndex = 0;
    errorCount = 0;
}

void TypingModel::advance() {
    if (currentIndex < fullText.size()) currentIndex++;
}

void TypingModel::addError() {
    errorCount++;
}

int TypingModel::getErrors() const {
    return errorCount;
}

double TypingModel::getAccuracy() const {
    int totalAttempts = currentIndex + errorCount;
    if (totalAttempts == 0) return 100.0;
    return (currentIndex * 100.0) / totalAttempts;
}

int TypingModel::getCPM(int elapsedSeconds) const {
    if (elapsedSeconds <= 0) return 0;
    return static_cast<int>((currentIndex * 60.0) / elapsedSeconds);
}

int TypingModel::getProgress() const {
    if (fullText.isEmpty()) return 0;
    return (currentIndex * 100) / fullText.size();
}

bool TypingModel::isFinished() const {
    return currentIndex >= fullText.size();
}

QString TypingModel::getTypedText() const {
    return fullText.left(currentIndex);
}

QString TypingModel::getRemainingText() const {
    return fullText.mid(currentIndex);
}
