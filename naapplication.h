#ifndef NAAPPLICATION_H
#define NAAPPLICATION_H

#include <QtWidgets/QApplication>
#include <QtSvg>
#include <QString>
#include <QFileOpenEvent>
#include <QUrl>

class naapplication : public QApplication
{
    Q_OBJECT
public:
    naapplication(int & argc, char **argv);

signals:
    void fileToOpen(QString &file);
    void urlToOpen(QString &url);
public slots:

private:


protected:
    virtual bool event(QEvent *);
};

#endif // NAAPPLICATION_H
