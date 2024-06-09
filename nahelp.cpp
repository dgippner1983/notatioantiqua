#include "nahelp.h"
#include "ui_nahelp.h"
#include <QMessageBox>

NAHelp::NAHelp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NAHelp)
{
    ui->setupUi(this);
    createWidgets();
    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(close()));
}

NAHelp::~NAHelp()
{
    delete ui;
}

void NAHelp::createWidgets()
{
    QString help_file;
    QFile help_stylesheet(":/help/nahelp.css");
    if (!help_stylesheet.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Notatio Antiqua"),
                             tr("Can't read style sheet:\n%1.")
                                 .arg(help_stylesheet.errorString()));
        }
    QTextStream help_stylesheet_contents(&help_stylesheet);
    QSettings* preferences = new QSettings(QSettings::IniFormat,QSettings::UserScope,"DGSOFTWARE", "Notatio Antiqua");
    preferences->beginGroup("Paths");
    QString helpFolder = preferences->value("helpFolder").toString();
    preferences->endGroup();
    help_file = helpFolder+tr("/notantiq.qhc");
    QHelpEngine *helpEngine = new QHelpEngine(help_file,this);
    helpEngine->setupData();
    QHelpBrowser *helpBrowser = new QHelpBrowser(help_file,this);
    helpBrowser->setSource(QUrl("qthelp://com.dgsoftware.notatioantiqua.12/doc/index.html"));
    helpBrowser->createStandardContextMenu();
    helpBrowser->document()->setLayoutEnabled(1);
    helpBrowser->document()->setDefaultStyleSheet(help_stylesheet_contents.readAll());
    QSplitter *helpPanel = new QSplitter(Qt::Horizontal);
    helpPanel->insertWidget(0, helpEngine->contentWidget());
    helpPanel->insertWidget(1, helpBrowser);
    helpPanel->widget(0)->setWindowTitle(tr("Contents"));
    helpPanel->widget(0)->setMaximumWidth(200);
    ui->gridLayout->addWidget(helpPanel);
    connect(helpEngine->contentWidget(), SIGNAL(linkActivated(const QUrl &)), helpBrowser, SLOT(setSource(const QUrl &)));
    connect(ui->actionHome,SIGNAL(triggered()),helpBrowser,SLOT(home()));
    connect(ui->actionBack,SIGNAL(triggered()),helpBrowser,SLOT(backward()));
    connect(ui->actionForward,SIGNAL(triggered()),helpBrowser,SLOT(forward()));
}
QHelpBrowser::QHelpBrowser(QString collectionFile, QWidget *parent)
     : QTextBrowser(parent)
 {
     m_helpEngine = new QHelpEngineCore(collectionFile, this);
     if (!m_helpEngine->setupData()) {
         delete m_helpEngine;
         m_helpEngine = nullptr;
     }
 }
QVariant QHelpBrowser::loadResource(int type, const QUrl &name)
{
    QByteArray ba;
    if (type < 4 && m_helpEngine) {
        QUrl url(name);
        if (name.isRelative())
            url = source().resolved(url);
        ba = m_helpEngine->fileData(url);
    }
    return ba;
}
void QHelpBrowser::showHelpForKeyword(const QString &id)
 {
     if (m_helpEngine) {
         QList<QHelpLink> links = m_helpEngine->documentsForIdentifier(id);
         {
             if (links.count())
                 setSource(links.first().url);
         }
     }
 }
