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
    helpBrowser->document()->setDefaultStyleSheet("h1 { "
                                                  "color: #003333;"
                                                  "font-size: 160%;"
                                                  "font-weight: bold;"
                                                  "}"
                                                  "h2 {"
                                                  "color: #000066;"
                                                  "font-size: 140%;"
                                                  "font-weight: bold;"
                                                  "}"
                                                  "h3 {"
                                                  "color: #003366;"
                                                  "font-size: 120%"
                                                  "font-weight: bold;"
                                                  "}"
                                                  "p {"
                                                  "align: justified;"
                                                  "}"
                                                  "body {"
                                                  "background-color: #fff;"
                                                  "width: 1050px;"
                                                  "}"
                                                  "#copy {"
                                                  "background-color: #663366;"
                                                  "color: #fff;"
                                                  "}"
                                                  "a {"
                                                  "color: #006600;"
                                                  "font-weight: bold;"
                                                  "text-decoration: none;"
                                                  "}"
                                                  "#header {"
                                                  "background: url('qrc:/images/notantiq.jpg') no-repeat;"
                                                  "height: 150px;"
                                                  "width: 100%;"
                                                  "padding: 0 0;"
                                                  "}");
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
         m_helpEngine = 0;
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
       /* QMap<QString, QUrl> links = m_helpEngine->documentsForIdentifier(id);
       if (links.count())
       setSource(links.constBegin().value()); */
     }
 }
