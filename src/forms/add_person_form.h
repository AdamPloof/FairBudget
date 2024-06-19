#ifndef ADD_PERSON_FORM_H
#define ADD_PERSON_FORM_H

#include <QWidget>
#include <memory>

namespace Ui {
    class AddPersonForm;
}

class EntityInterface;

class AddPersonForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddPersonForm(QWidget *parent = nullptr);
    ~AddPersonForm();

signals:
    void submitPerson(std::shared_ptr<EntityInterface> person);

private slots:
    void on_cancelBtn_clicked();
    void on_addBtn_clicked();

private:
    bool isValid();

    Ui::AddPersonForm *ui;
};

#endif // ADD_PERSON_FORM_H
