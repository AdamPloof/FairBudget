#include <gtest/gtest.h>
#include <QApplication>
#include "../../src/forms/ui_add_expense_form.h"
#include "../../src/forms/add_expense_form.h"

class AddExpenseFormTest : public ::testing::Test {
protected:
    void SetUp() override {
        int argc = 0;
        char **argv = nullptr;
        app = new QApplication(argc, argv);
        form = new AddExpenseForm();
    }
    
    void TearDown() override {
        delete form;
        delete app;
    }

    QApplication *app;
    AddExpenseForm *form;
};

TEST_F(AddExpenseFormTest, FormIsValid) {
    form->setDescription("Foo");
    form->setAmount(42.0);

    EXPECT_TRUE(form->isValid());
}

TEST_F(AddExpenseFormTest, MissingDescriptionIsInvalid) {
    form->setDescription("");
    form->setAmount(28.8);

    EXPECT_FALSE(form->isValid());
}

TEST_F(AddExpenseFormTest, MissingAmountIsInvalid) {
    form->setDescription("Something");
    form->setAmount(0);

    EXPECT_FALSE(form->isValid());
}

TEST_F(AddExpenseFormTest, MissingAllFieldsIsInvalid) {
    form->setDescription("");
    form->setAmount(0);

    EXPECT_FALSE(form->isValid());
}
