#include "naapplication.h"
#include "naprog.h"
#define naApp (static_cast<naapplication *>(QCoreApplication::instance()))

naapplication::naapplication(int &argc, char **argv) :
    QApplication(argc, argv)
{

}
bool naapplication::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
    {
        QString fto = static_cast<QFileOpenEvent*>(event)->file();
        if (!fto.isEmpty()) emit fileToOpen(fto);
        QString uto = static_cast<QFileOpenEvent*>(event)->url().toString();
        if ( !uto.isEmpty()) emit urlToOpen(uto);
        event->accept();
        return true;
    }
    default:
        return QApplication::event(event);
    }
}
