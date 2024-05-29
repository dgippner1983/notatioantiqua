#include "nainit.h"
#include "ui_nainit.h"

nainit::nainit(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::nainit)
{
    ui->setupUi(this);
}

nainit::~nainit()
{
    delete ui;
}

void nainit::on_startButton_clicked()
{
    if (doinit.isRunning()) return;
    doinit.setValues(ui->basepathEdit->text());
    connect(&doinit,SIGNAL(finished()),this,SLOT(getTextFromThread()));
    connect(&doinit,SIGNAL(pathHasChanged(QString)),this,SLOT(getPath(QString)));
    ui->workingL->setText(tr("Working... please be patient."));
    doinit.start();


}
void nainit::getTextFromThread()
{
    ui->latexCB->addItems(doinit.LaTeX_Path);
    ui->lilyCB->addItems(doinit.Lilypond_Path);
    ui->gregorioCB->addItems(doinit.Gregorio_Path);
    ui->templateCB->addItems(doinit.templateFolder);
    ui->dictCB->addItems(doinit.dictFolder);
    ui->helpCB->addItems(doinit.helpFolder);
    if (!doinit.LaTeX_Path.isEmpty())
        ui->currentPath->setText(tr("Finished searching. Click 'Next' to go on."));
    else ui->currentPath->setText(tr("Finished searching. LaTeX has not been found. Please fill in the path yourself or click 'Search' again choosing another base file path."));
    ui->workingL->setText("");
}
void nainit::getPath(QString path)
{
    ui->currentPath->setText(tr("Current Path: %1").arg(path));
}

void nainit::on_pushButton_clicked()
{
    LaTeX_Path = ui->latexCB->currentText();
    Lilypond_Path = ui->lilyCB->currentText();
    Gregorio_Path = ui->gregorioCB->currentText();
    templateFolder = ui->templateCB->currentText();
    dictFolder = ui->dictCB->currentText();
    helpFolder = ui->helpCB->currentText();
    QSettings* preferences = new QSettings(QSettings::IniFormat,QSettings::UserScope,"DGSOFTWARE", "Notatio Antiqua"); // save ini file
    preferences->beginGroup("Paths");
    preferences->setValue("latexPath",LaTeX_Path);
    preferences->setValue("gregorioPath",Gregorio_Path);
    preferences->setValue("lilypondPath",Lilypond_Path);
    preferences->setValue("templateFolder",templateFolder);
    preferences->setValue("dictFolder",dictFolder);
    preferences->setValue("helpFolder",helpFolder);
    preferences->endGroup();
    preferences->beginGroup("Font");
    QFont saveFont;
    QFont docsFont;
    QFont docssFont;
    saveFont.setFamily("Ubuntu Mono");
    docsFont.setFamily("Times New Roman");
    docssFont.setFamily("Arial");
    preferences->setValue("standardFont",saveFont.family());
    preferences->setValue("standardSize",12);
    preferences->setValue("staffsize",17);
    preferences->setValue("fontsize",12);
    preferences->setValue("docserifFont",docsFont.family());
    preferences->setValue("docsansFont",docssFont.family());
    preferences->setValue("docinitFont",docsFont.family());
    preferences->setValue("doccsFont",docsFont.family());
    preferences->endGroup();
    preferences->beginGroup("Colors");
    preferences->setValue("keywords",QColor(Qt::darkBlue));
    preferences->setValue("pitches",QColor(Qt::darkRed));
    preferences->setValue("commentaries",QColor(Qt::darkGreen));
    preferences->setValue("translations",QColor(Qt::darkYellow));
    preferences->endGroup();
    ui->finishedit->setText(tr("We have found the following configuration:\nLuaLaTeX Path: %1\n"
                               "LilyPond Path: %2\n"
                               "Gregorio Path: %3\n"
                               "Template Folder: %4\n"
                               "Dictionaries Folder: %5\n"
                               "Help Folder: %6")
                            .arg(LaTeX_Path)
                            .arg(Lilypond_Path)
                            .arg(Gregorio_Path)
                            .arg(templateFolder)
                            .arg(dictFolder)
                            .arg(helpFolder));
}
