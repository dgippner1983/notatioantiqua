#ifndef NAJGABC_H
#define NAJGABC_H

#include <QDialog>

namespace Ui {
class najgabc;
}

class najgabc : public QDialog
{
    Q_OBJECT

public:
    explicit najgabc(QWidget *parent = 0);
    ~najgabc();

private:
    Ui::najgabc *ui;
};

#endif // NAJGABC_H
