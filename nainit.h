#ifndef NAINIT_H
#define NAINIT_H

#include <QtWidgets/QWizard>
#include <QtWidgets/QTextEdit>
#include <QVariant>
#include <QColor>
#include "ui_nainit.h"
#include "nadoinit.h"
namespace Ui {
class nainit;
}

class nainit : public QWizard
{
    Q_OBJECT
    
public:
    explicit nainit(QWidget *parent = 0);
    ~nainit();
    nadoinit doinit;
    QString LaTeX_Path, Gregorio_Path, Lilypond_Path, templateFolder, dictFolder, helpFolder;
public slots:
    void getPath(QString path);

private slots:

    void on_startButton_clicked();
    void getTextFromThread();

    void on_pushButton_clicked();

private:
    Ui::nainit *ui;
};

#endif // NAINIT_H
