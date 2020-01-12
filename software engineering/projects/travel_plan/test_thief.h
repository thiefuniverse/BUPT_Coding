#ifndef TEST_THIEF_H
#define TEST_THIEF_H

#include <QDialog>

namespace Ui {
class test_thief;
}

class test_thief : public QDialog
{
    Q_OBJECT

public:
    explicit test_thief(QWidget *parent = 0);
    ~test_thief();

private:
    Ui::test_thief *ui;
};

#endif // TEST_THIEF_H
