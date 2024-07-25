#include <QStringList>
#include "utils.h"

QString FairBudget::utils::sentenceCase(QString str) {
    QStringList parts = str.split(u'_', Qt::SkipEmptyParts);
    QString sentence;
    bool isFirst = true;

    for (QString word : parts) {
        word = word.toLower();
        if (isFirst) {
            word.replace(0, 1, word.at(0).toUpper());
            isFirst = false;
        }
        sentence.append(word + " ");
    }

    // call removeLast to drop the space at the end
    return sentence.removeLast();
}
