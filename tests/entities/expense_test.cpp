 #include <gtest/gtest.h>
#include <QHash>
#include <QString>
#include <QVariant>
#include "../../src/entities/expense.h"

TEST(ExpenseTest, DataIsSet) {
    // EXPECT_EQ(1, 1);
    Expense expense;
    expense.setData("id", 42);
    expense.setData("description", "Forty two");
    expense.setData("amount", 4.2);

    QHash<QString, QVariant> data = expense.getData();
    EXPECT_EQ(42, data["id"].toInt());
    EXPECT_EQ("Forty two", data["description"].toString().toStdString());
    EXPECT_DOUBLE_EQ(4.2, data["amount"].toDouble());
}
