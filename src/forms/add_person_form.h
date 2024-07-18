#ifndef ADD_PERSON_FORM_H
#define ADD_PERSON_FORM_H

#include <QWidget>
#include <QHash>
#include <QString>
#include <memory>

namespace Ui {
    class AddPersonForm;
}

class EntityInterface;
class IncomePeriod;

class AddPersonForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddPersonForm(QWidget *parent = nullptr);
    ~AddPersonForm();

    void setName(const QString &name);
    void setIncome(double income);
    void setIncomePeriod(const QString &period);
    bool isValid();

signals:
    void submitPerson(std::shared_ptr<EntityInterface> person);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_cancelBtn_clicked();
    void on_addBtn_clicked();

private:
    void setPeriodOptions();
    std::shared_ptr<IncomePeriod> fetchIncomePeriod(int id);

    Ui::AddPersonForm *ui;
};

#endif // ADD_PERSON_FORM_H
