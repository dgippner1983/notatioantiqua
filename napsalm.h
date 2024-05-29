#ifndef NAPSALM_H
#define NAPSALM_H

#include <QFrame>
#include <QTextStream>
#include <QMultiMap>
#include <QDesktopServices>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include "hyphen.h"

namespace Ui {
class napsalm;
}

class napsalm : public QFrame
{
    Q_OBJECT
    
public:
    explicit napsalm(QWidget *parent = 0);
    ~napsalm();
    
private slots:
    void on_toneCBm_currentIndexChanged(int index);
    void preparation();
    void gettone();

    void on_toneCBm_currentIndexChanged(const QString &arg1);

    void on_toneVariant_currentIndexChanged(const QString &arg1);

    void on_toneSolemn_stateChanged(int arg1);

    void on_closeButton_toggled(bool checked);

    void on_closeButton_clicked();

    void on_psalmText_textChanged();

    void on_generateButton_clicked();

private:
    Ui::napsalm *ui;
    struct NaPsalmTone {
        QString tone;
        QString clef;
        QString initium;
        QString isolemn;
        QString tuba;
        QString flex;
        QString mediant;
        QString msolemn;
        QMap< QString, QString > termination;
        };
    QList<NaPsalmTone> tones;
    HyphenDict *dict;
};

#endif // NAPSALM_H
