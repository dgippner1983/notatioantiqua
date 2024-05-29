#include "najgabc.h"
#include "ui_najgabc.h"

najgabc::najgabc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::najgabc)
{
    ui->setupUi(this);
}

najgabc::~najgabc()
{
    delete ui;
}
