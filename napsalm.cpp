/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2024 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software Foundation;
version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/
#include "napsalm.h"
#include "ui_napsalm.h"
#define BUFSIZE 1000
#define QT_USE_FAST_CONCATENATION
napsalm::napsalm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::napsalm)
{
    ui->setupUi(this);
    preparation();
}

napsalm::~napsalm()
{
    delete ui;
}

void napsalm::on_toneCBm_currentIndexChanged(int index)
{

}
void napsalm::preparation()
{
    // initialize psalms 1 to 150
    QStringList psalms;
    int number;
    for (number=1; number<=150; number++)
        psalms << QString::number(number);
    ui->psalmCBm->addItems(psalms);
    // get standard psalm tones from file
    QFile toneFile(":/napsalm.csv");
    if (!toneFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString standardtones;
    QTextStream standardtonesstream(&standardtones);
    standardtonesstream << toneFile.readAll();
    while (!standardtonesstream.atEnd()) {
        QString tonesline(standardtonesstream.readLine());
        QStringList toneslinelist;
        toneslinelist << tonesline.split(",");
        NaPsalmTone currenttone;
        currenttone.tone = toneslinelist.at(0).trimmed();
        currenttone.clef = toneslinelist.at(1).trimmed();
        currenttone.initium = toneslinelist.at(2).trimmed();
        currenttone.isolemn = toneslinelist.at(3).trimmed();
        currenttone.tuba = toneslinelist.at(4).trimmed();
        currenttone.flex = toneslinelist.at(5).trimmed();
        currenttone.mediant = toneslinelist.at(6).trimmed();
        currenttone.msolemn = toneslinelist.at(7).trimmed();
        int i = toneslinelist.at(8).toInt();
        int j = i*2;
        j = j+8;
        for (int it = 9; it <= j; it +=2 ) {
            currenttone.termination.insert(toneslinelist.at(it).trimmed(),toneslinelist.at(it+1).trimmed());
        }
        tones << currenttone;
    }
    toneFile.close();
    // add tones to Combo Box
    NaPsalmTone addtones;
    foreach (addtones,tones) ui->toneCBm->addItem(addtones.tone);
    // insert first tone's values as standard values
    ui->toneVariant->addItems(tones.at(0).termination.keys());
    ui->toneClef->setText(tr("Clef ")+tones.at(0).clef);
    ui->initText->setText(tones.at(0).initium);
    ui->tubaText->setText(tones.at(0).tuba);
    ui->flexText->setText(tones.at(0).flex);
    ui->mediationText->setText(tones.at(0).mediant);
    ui->terminationText->setText(tones.at(0).termination.value(ui->toneVariant->currentText()));
}
void napsalm::gettone()
{
    //tbd
}

void napsalm::on_toneCBm_currentIndexChanged(const QString &arg1)
{
    ui->toneVariant->clear();
    ui->toneVariant->addItems(tones.at(ui->toneCBm->currentIndex()).termination.keys());
    ui->toneClef->setText(tr("Clef ")+tones.at(ui->toneCBm->currentIndex()).clef);
    ui->initText->setText(tones.at(ui->toneCBm->currentIndex()).initium);
    ui->tubaText->setText(tones.at(ui->toneCBm->currentIndex()).tuba);
    ui->flexText->setText(tones.at(ui->toneCBm->currentIndex()).flex);
    ui->mediationText->setText(tones.at(ui->toneCBm->currentIndex()).mediant);
    ui->terminationText->setText(tones.at(ui->toneCBm->currentIndex()).termination.value(ui->toneVariant->currentText()));
}

void napsalm::on_toneVariant_currentIndexChanged(const QString &arg1)
{
    ui->terminationText->setText(tones.at(ui->toneCBm->currentIndex()).termination.value(ui->toneVariant->currentText()));
}

void napsalm::on_toneSolemn_stateChanged(int arg1)
{
    if (ui->toneSolemn->isChecked()) {
        ui->mediationText->setText(tones.at(ui->toneCBm->currentIndex()).msolemn);
        ui->initText->setText(tones.at(ui->toneCBm->currentIndex()).isolemn);
    }
    else {
        ui->mediationText->setText(tones.at(ui->toneCBm->currentIndex()).mediant);
        ui->initText->setText(tones.at(ui->toneCBm->currentIndex()).initium);
    }
}

void napsalm::on_closeButton_toggled(bool checked)
{

}

void napsalm::on_closeButton_clicked()
{
    napsalm::close();
}

void napsalm::on_psalmText_textChanged()
{
}

void napsalm::on_generateButton_clicked()
{
    ui->gabcPreviewE->clear();
    QStringList sentences;
    QStringList words;
    QStringList errors;
    QStringList out_words;
    QString out_text;
    bool isfirst = true;
    bool isclef = false;
    QSettings* preferences = new QSettings(QSettings::IniFormat,QSettings::UserScope,"DGSOFTWARE", "Notatio Antiqua");
    preferences->beginGroup("Paths");
    QString dictFolder;
    dictFolder = preferences->value("dictFolder").toString();
    preferences->endGroup();
    sentences << ui->psalmText->toPlainText().split(".");
    sentences.removeAll("");
    // re-add dot

    if (dictFolder.isEmpty())
    {
        QMessageBox::information(this,tr("Notatio Antiqua"),tr("Please select a dictionary for hyphenation.<br />"
                                                            "You may find them under contrib/dict."),QMessageBox::Ok);
        dictFolder = QFileDialog::getExistingDirectory(this,tr("Notatio Antiqua"),"",QFileDialog::ShowDirsOnly);
    }
    QStringList::const_iterator sentence_iterator;
    for (sentence_iterator = sentences.constBegin(); sentence_iterator != sentences.constEnd(); sentence_iterator++) {
        QString sentence = (*sentence_iterator) + ".";
        // reset word lists
        words.clear();
        out_words.clear();
        words << sentence.split(" ");
        for (int it = 0;it != words.size(); ++it)
        {
            QString punctuation;
            QString lword = words.at(it).trimmed();
            QString out_word;
            static QRegularExpression dot("^\\w*(\\.)$");
            static QRegularExpression comma("^\\w*(\\,)$");
            static QRegularExpression colon("^\\w*(\\:)$");
            static QRegularExpression semicolon("^\\w*(\\;)$");
            static QRegularExpression excl_mark("^\\w*(\\!)$");
            static QRegularExpression question_mark("^\\w*(\\?)$");
            int length = words.at(it).length();
            char *hyphens = new char[BUFSIZE+1];
            char *hyphword = new char[BUFSIZE * 2];
            char ** rep;
            int * pos;
            int * cut;
            rep = NULL;
            pos = NULL;
            cut = NULL;
            std::string dictionaryS = QString(dictFolder+"/hyph_de_DE.dic").toStdString();
            const char *dictionary = dictionaryS.c_str();
            if ((dict = hnj_hyphen_load(dictionary)) == NULL) {
                QMessageBox::critical(this,tr("Notatio Antiqua"),
                                      tr("We couldn't load required dictionary file."),QMessageBox::Ok,QMessageBox::NoButton);
                return;
            }
            if (lword.contains(dot)) {
                punctuation = ".";
                lword.remove(".",Qt::CaseInsensitive);
            }
            if (lword.contains(colon)) {
                punctuation = ":";
                lword.remove(":",Qt::CaseInsensitive);
            }
            if (lword.contains(comma)) {
                punctuation = ",";
                lword.remove(",",Qt::CaseInsensitive);
            }
            if (lword.contains(semicolon)) {
                punctuation = ";";
                lword.remove(";",Qt::CaseInsensitive);
            }
            if (lword.contains(excl_mark)) {
                punctuation = "!";
                lword.remove("!",Qt::CaseInsensitive);
            }
            if (lword.contains(question_mark)) {
                punctuation = "?";
                lword.remove("?",Qt::CaseInsensitive);
            }
            lword = lword.trimmed();
            std::string std_lword = lword.toStdString();
            const char *word = std_lword.c_str();
            if (hnj_hyphen_hyphenate3(dict,word,length,hyphens,hyphword,&rep,&pos,&cut,2,2,2,2)) {
                errors << tr("Error hyphenating %1").arg(QString(word));
                return;
            }
            if (!punctuation.isEmpty())
            { out_word = QString(hyphword)+punctuation; }
            else { out_word = QString(hyphword); }
            // if last syllable consists of one consonant and punctuation mark, undo hyphenation.
            if (out_word.lastIndexOf("=") == out_word.size()-2 && out_word.contains(".",Qt::CaseInsensitive)) out_word = out_word.remove(out_word.lastIndexOf("="),1);
            if (out_word.lastIndexOf("=") == out_word.size()-2 && out_word.contains(":",Qt::CaseInsensitive)) out_word = out_word.remove(out_word.lastIndexOf("="),1);
            if (out_word.lastIndexOf("=") == out_word.size()-2 && out_word.contains(",",Qt::CaseInsensitive)) out_word = out_word.remove(out_word.lastIndexOf("="),1);
            if (out_word.lastIndexOf("=") == out_word.size()-2 && out_word.contains(";",Qt::CaseInsensitive)) out_word = out_word.remove(out_word.lastIndexOf("="),1);
            if (out_word.lastIndexOf("=") == out_word.size()-2 && out_word.contains("!",Qt::CaseInsensitive)) out_word = out_word.remove(out_word.lastIndexOf("="),1);
            if (out_word.lastIndexOf("=") == out_word.size()-2 && out_word.contains("?",Qt::CaseInsensitive)) out_word = out_word.remove(out_word.lastIndexOf("="),1);
            // if "syllable" consists of one consonant only, undo hyphenation.
            if (out_word.lastIndexOf("=") == out_word.size()-1) out_word = out_word.remove(out_word.lastIndexOf("="),1);
            QStringList syllables = out_word.split("=");
            if (lword == "*") out_words << lword;
            else if (lword == "+") out_words << lword;
            else out_words << syllables;
            out_words << " ";
        }
        //out_words.removeLast();
        // split initium, mediation and termination to single notes
        QStringList initiumGABC = ui->initText->toPlainText().split(" ");
        QStringList mediationGABC = ui->mediationText->toPlainText().split(" ");
        QStringList terminationGABC = ui->terminationText->toPlainText().split(" ");
        QString tuba = ui->tubaText->toPlainText();
        int syl = 0;
        if (isfirst) for (int i = 0; i != initiumGABC.count(); i++) {
            if (out_words[syl] == " ") syl++;
            out_words[syl].append("("+initiumGABC[i]+")");
            syl++;
        }
    }
    // output of complete input
    if (!isclef) {
        out_text = "("+tones[ui->toneCBm->currentIndex()].clef+") ";
        isclef = true;
    }
    for (int i = 0; i != out_words.count(); ++i)
        out_text = out_text+out_words[i];
    ui->gabcPreviewE->append(out_text);
    out_text = "";

}
