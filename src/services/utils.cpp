#include <QStringList>
#include <cmath>
#include <sstream>
#include <string>
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

QString FairBudget::utils::asCurrency(double val) {
    double rounded = std::round(val * 100) / 100;
    std::stringstream ss;
    ss << "$" << rounded;
    std::string curStr = ss.str();
    size_t decimalPos = curStr.find(".");

    if (decimalPos == std::string::npos) {
        ss << ".00";
    } else if (decimalPos == curStr.length() - 2) {
        ss << "0";
    }

    return QString(ss.str().c_str());
}
