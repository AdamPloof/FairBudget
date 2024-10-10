#ifndef UTILS_H
#define UTILS_H

#include <QString>

namespace FairBudget::utils {
    QString sentenceCase(QString str);
    QString asCurrency(double val);
}

#endif // UTILS_H
