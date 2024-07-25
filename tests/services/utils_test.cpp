#include <gtest/gtest.h>
#include <QString>
#include "../../src/services/utils.h"

using namespace FairBudget::utils;

TEST(UtilsTest, Simple) {
    QString wordTest = "basic";
    QString wordAnswer = "Basic";
    EXPECT_EQ(sentenceCase(wordTest).toStdString(), wordAnswer.toStdString());

    QString sentenceTest = "basic_sentence";
    QString sentenceAnswer = "Basic sentence";
    EXPECT_EQ(sentenceCase(sentenceTest).toStdString(), sentenceAnswer.toStdString());
}

TEST(UtilsTest, ExtraSpaces) {
    QString wordTest = "_basic_";
    QString wordAnswer = "Basic";
    EXPECT_EQ(sentenceCase(wordTest).toStdString(), wordAnswer.toStdString());

    QString sentenceTest = "_basic___sentence_";
    QString sentenceAnswer = "Basic sentence";
    EXPECT_EQ(sentenceCase(sentenceTest).toStdString(), sentenceAnswer.toStdString());
}

TEST(UtilsTest, AlphaNumeric) {
    QString wordTest = "basic123";
    QString wordAnswer = "Basic123";
    EXPECT_EQ(sentenceCase(wordTest).toStdString(), wordAnswer.toStdString());

    QString sentenceTest = "basic_123_sentence";
    QString sentenceAnswer = "Basic 123 sentence";
    EXPECT_EQ(sentenceCase(sentenceTest).toStdString(), sentenceAnswer.toStdString());
}

TEST(UtilsTest, WithSymbols) {
    QString wordTest = "basic*%@";
    QString wordAnswer = "Basic*%@";
    EXPECT_EQ(sentenceCase(wordTest).toStdString(), wordAnswer.toStdString());

    QString sentenceTest = "basic_*%@_sentence";
    QString sentenceAnswer = "Basic *%@ sentence";
    EXPECT_EQ(sentenceCase(sentenceTest).toStdString(), sentenceAnswer.toStdString());
}
