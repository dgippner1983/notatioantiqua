#ifndef NAHELP_H
#define NAHELP_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtHelp>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QSplitter>
#include <QSettings>


namespace Ui {
class NAHelp;
}

class NAHelp : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit NAHelp(QWidget *parent = 0);
    ~NAHelp();
    void createWidgets();
    
private slots:


private:
    Ui::NAHelp *ui;

};

class QHelpBrowser : public QTextBrowser
{
    Q_OBJECT
 public:
    QHelpBrowser(QString collectionFile, QWidget *parent);
    void showHelpForKeyword(const QString &id);
 private:
    QVariant loadResource(int type, const QUrl &name);
    QHelpEngineCore *m_helpEngine;
};

#endif // NAHELP_H
