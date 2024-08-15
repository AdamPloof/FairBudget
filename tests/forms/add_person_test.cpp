#include <gtest/gtest.h>
#include <QApplication>
#include <memory>

#include "../../src/forms/ui_add_person_form.h"
#include "../../src/forms/add_person_form.h"
#include "../../src/services/entity_manager.h"

class AddPersonFormTest : public ::testing::Test {
protected:
    void SetUp() override {
        int argc = 0;
        char **argv = nullptr;
        std::shared_ptr<EntityManager> em = std::make_shared<EntityManager>(EntityManager());
        app = new QApplication(argc, argv);
        form = new AddPersonForm(em);
    }

    void TearDown() override {
        delete app;
        delete form;
    }

    QApplication *app;
    AddPersonForm *form;
};

TEST_F(AddPersonFormTest, FormIsValid) {
    form->setName("Peter");
    form->setIncome(50000.0);
    form->setIncomePeriod("Annually");

    EXPECT_TRUE(form->isValid());
}

TEST_F(AddPersonFormTest, MissingNameIsInvalid) {
    form->setName("");
    form->setIncome(50000.0);
    form->setIncomePeriod("Annually");

    EXPECT_FALSE(form->isValid());
}

TEST_F(AddPersonFormTest, MissingIncomeIsInvalid) {
    form->setName("Peter");
    form->setIncome(0);
    form->setIncomePeriod("Annually");

    EXPECT_FALSE(form->isValid());
}

TEST_F(AddPersonFormTest, MissingAllFieldsIsInvalid) {
    form->setName("");
    form->setIncome(0);
    form->setIncomePeriod("broken");

    EXPECT_FALSE(form->isValid());
}
