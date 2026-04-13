#include "typingmodel.h"

void TypingModel::setText(const QString &text) {
    fullText = text;
    currentIndex = 0;
}

void TypingModel::advance() {
    if (currentIndex < fullText.size()) {
        currentIndex++;
    }
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
