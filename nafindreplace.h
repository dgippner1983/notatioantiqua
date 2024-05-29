#ifndef NAFINDREPLACE_H
#define NAFINDREPLACE_H

#include <QtWidgets/QDockWidget>

namespace Ui {
class nafindreplace;
}

class nafindreplace : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit nafindreplace(QWidget *parent = 0);
    ~nafindreplace();
    
private:
    Ui::nafindreplace *ui;
signals:
    void findText(const QString &find, bool caseS, bool words, bool regex);
    void replaceText(const QString &find, const QString &replace, bool caseS, bool words, bool regex, bool repall);
private slots:
    void on_replaceE_textChanged(const QString &arg1);
    void find();
    void replace();
    void replaceAll();
    void on_findE_textChanged(const QString &arg1);
};

#endif // NAFINDREPLACE_H
