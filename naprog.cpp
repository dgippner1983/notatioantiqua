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

#include "naprog.h"
#include "ui_naprog.h"



NaProg::NaProg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NaProg)
{
    ui->setupUi(this);
    // create the main window and docking widgets
    createWorkspaces();
    // process command line arguments
    isArgument();
    // FileOpen signal throughput (at the moment only for Mac OS X)
    connect(static_cast<naapplication*>(QCoreApplication::instance()),SIGNAL(fileToOpen(QString&)),this,SLOT(fileOpen_request(QString&)));
}


NaProg::~NaProg()
{
    delete ui;
}

void NaProg::ausfuehren(QString program, QStringList arguments, QString work_path)
{
#if !defined (Q_OS_WIN)
    QString mydelim = ":";
#endif
#if defined (Q_OS_WIN) | defined (__MINGW32__)
    QString mydelim = ";";
#endif
    execProg = new QProcess(this);
    QStringList environment = QProcess::systemEnvironment();
    environment.replaceInStrings(QRegularExpression("^PATH=(.*)", QRegularExpression::CaseInsensitiveOption), "PATH=\\1"+mydelim+LaTeX_Path+mydelim+Lilypond_Path+mydelim+Gregorio_Path);
    execProg->setEnvironment(environment);
    execProg->setWorkingDirectory(work_path);
    execProg->setProcessChannelMode(QProcess::MergedChannels);
    connect(execProg,SIGNAL(readyRead()),this,SLOT(tolog()));
    execProg->start(program,arguments);
    static_cast<naapplication*>(QCoreApplication::instance())->processEvents();
    if (execProg->waitForFinished())
        if (pdfBox->isChecked()) {
            QString processedFile = arguments.last();
            QFileInfo openfile(processedFile); // extracting file name
            QString workingpath = openfile.absolutePath(); // strip closing /
            if (openfile.completeSuffix() == "gabc")
                QDesktopServices::openUrl(QUrl("file://"+workingpath+"/"+openfile.baseName()+"-main.pdf", QUrl::TolerantMode));
            else
                QDesktopServices::openUrl(QUrl("file://"+workingpath+"/"+openfile.baseName()+".pdf", QUrl::TolerantMode));
        }
}

void NaProg::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NaProg::info()
{
    QMessageBox::about(this, tr("Copyright-Informationen"),
                       tr("<h1>Notatio Antiqua %1 </h1>"
                          "&copy; 2009-2024 DGSOFTWARE<br /><br />"
                          "David Gippner M.A.<br />"
                          "Alte Hohl 3<br />"
                          "67659 Kaiserslautern<br />"
                          "Makes use of hyphen lib (part of hunspell) <a href='http://hunspell.sourceforge.net'>(homepage)</a> licensed dual under LGPL and MPL<br />"
                          "<a href='mailto:david@gippner.eu'>david@gippner.eu</a><br /><br />"
                          "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.<br /> "
                          "This program is distributed in the hope that it will be useful, "
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                          "GNU General Public License for more details. "
                          "You should have received a copy of the GNU General Public License "
                          "along with this program.  If not, see <a href=http://www.gnu.org/licenses/>http://www.gnu.org/licenses/</a>.").arg(Version::versionString()));
}


void NaProg::on_action_ber_2_triggered()
{
    info();
}

void NaProg::neu()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->showMaximized();
    child->activateWindow();
    child->document()->setUndoRedoEnabled(true);
    connect(ui->actionUndo,SIGNAL(triggered()),child->document(),SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),child->document(),SLOT(redo()));
    ui->actionSpeichern->setEnabled(1);
    ui->actionSpeichern_unter->setEnabled(1);
    ui->actionAusschneiden->setEnabled(1);
    ui->actionEinf_gen->setEnabled(1);
    ui->actionKopieren->setEnabled(1);
    ui->actionAufr_umen->setEnabled(1);
    ui->actionLyTeX_PDF->setEnabled(1);
    ui->actionGregorioTeX_PDF->setEnabled(1);
    ui->actionPDF_ansehen->setEnabled(1);
}

void NaProg::offen()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),"",
                                                    tr("Gregorio GABC (*.gabc);;"
                                                       "LyTeX Lilypond and LaTeX (*.lytex)"));
    openFile(fileName);
    ui->actionSpeichern->setEnabled(1);
    ui->actionSpeichern_unter->setEnabled(1);
    ui->actionAusschneiden->setEnabled(1);
    ui->actionEinf_gen->setEnabled(1);
    ui->actionKopieren->setEnabled(1);
    ui->actionAufr_umen->setEnabled(1);
    ui->actionLyTeX_PDF->setEnabled(1);
    ui->actionGregorioTeX_PDF->setEnabled(1);
    ui->actionPDF_ansehen->setEnabled(1);
}

void NaProg::offenLetzte()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openFile(action->data().toString());
    ui->actionSpeichern->setEnabled(1);
    ui->actionSpeichern_unter->setEnabled(1);
    ui->actionAusschneiden->setEnabled(1);
    ui->actionEinf_gen->setEnabled(1);
    ui->actionKopieren->setEnabled(1);
    ui->actionAufr_umen->setEnabled(1);
    ui->actionLyTeX_PDF->setEnabled(1);
    ui->actionGregorioTeX_PDF->setEnabled(1);
    ui->actionPDF_ansehen->setEnabled(1);
}

void NaProg::offenWieder()
{
    if (activeMdiChild()) {
        QString fileName = QFileInfo(activeMdiChild()->curFile).absoluteFilePath();
        QMessageBox::StandardButton quest;
        quest = QMessageBox::question(this, tr("Notatio Antiqua"),
                                      tr("'%1' will be reopened.\n"
                                         "Do you really want to revert it to the last saved state?")
                                      .arg(fileName),
                                      QMessageBox::Yes | QMessageBox::No);
        if (quest == QMessageBox::Yes)
            openFile(fileName);
        else if (quest == QMessageBox::No)
            return;
    }
}

void NaProg::speichern()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("Saved."), 2000);
}

void NaProg::speichernunter()
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
        statusBar()->showMessage(tr("Saved."), 2000);
}

void NaProg::ausschneiden()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->cut();
}

void NaProg::kopieren()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->copy();
}

void NaProg::einfuegen()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->paste();
}



MdiChild *NaProg::createMdiChild()
{
    MdiChild *child = new MdiChild;
    ui->mdiArea->addSubWindow(child);
    return child;
}

void NaProg::on_actionKopieren_triggered()
{
    kopieren();
}

void NaProg::on_actionAusschneiden_triggered()
{
    ausschneiden();
}

void NaProg::on_actionEinf_gen_triggered()
{
    einfuegen();
}

MdiChild *NaProg::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *NaProg::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void NaProg::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void NaProg::on_actionNeu_triggered()
{
    neu();
}

void NaProg::on_action_ffnen_triggered()
{
    offen();
}

void NaProg::on_actionSpeichern_triggered()
{
    speichern();
}

void NaProg::on_actionSpeichern_unter_triggered()
{
    speichernunter();
}

void NaProg::updateMenus()
{

}

void NaProg::createWorkspaces()
{
    connect(ui->actionBeenden, SIGNAL(triggered()), static_cast<naapplication*>(QCoreApplication::instance()), SLOT(closeAllWindows()));
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(ui->mdiArea);
    setDockOptions(QMainWindow::ForceTabbedDocks);

    // Output dock on bottom
    nadock = new QDockWidget;
    logWindow = new QTextEdit;
    logWindow->setReadOnly(true);
    logWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    logWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    nadock->setWindowTitle(tr("Program output"));
    nadock->setAllowedAreas(Qt::BottomDockWidgetArea);
    nadock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    nadock->setWidget(logWindow);
    addDockWidget(Qt::BottomDockWidgetArea, nadock);

    // initialize mdi Area
    connect(ui->mdiArea, &QMdiArea::subWindowActivated,
            this, &NaProg::updateMenus);
    setWindowTitle(tr("Notatio Antiqua %1").arg(Version::versionString()));
    setUnifiedTitleAndToolBarOnMac(true);
    setStatusBar(statusBar());

    // get preferences from ini file
    QSettings* preferences = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                           "DGSOFTWARE", "Notatio Antiqua");
    QFileInfo areprefsexisting(preferences->fileName());
    if (!areprefsexisting.exists()) initialization();
    preferences->beginGroup("Paths");
    LaTeX_Path = preferences->value("latexPath").toString();
    Lilypond_Path = preferences->value("lilypondPath").toString();
    Gregorio_Path = preferences->value("gregorioPath").toString();
    templateFolder = preferences->value("templateFolder").toString();
    dictFolder = preferences->value("dictFolder").toString();
    preferences->endGroup();
    preferences->beginGroup("File");
    ui->menuDatei->addSeparator();
    lastOpenFile = new QAction(QFileInfo(preferences->value("lastOpen").toString()).fileName(),this);
    preferences->endGroup();

    // load all recent opened files into menu
    connect(ui->actionErneut_ffnen,SIGNAL(triggered()),this,SLOT(offenWieder()));
    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(on_actionFind_triggered()));
    menuRecent.setTitle(tr("Recently opened files"));
    for (int i = 0; i < MaxRecent; ++i) {
        recentAct[i] = new QAction(this);
        recentAct[i]->setVisible(false);
        connect(recentAct[i], SIGNAL(triggered()),
                this, SLOT(offenLetzte()));
        menuRecent.addAction(recentAct[i]);
    }
    ui->menuDatei->addMenu(&menuRecent);
    updateRecent();

    // create ribbon bar
    createRibbonBar();

    // Implementation of template folder
    QStringList templateItems;
    QStringList templateFilter;
    QDir templateDir(templateFolder);
    templateFilter << "*.natemplate";
    templateItems = templateDir.entryList(templateFilter);
    NaProg::templateBox->addItems(templateItems);

    // ready and up
    ui->statusBar->showMessage(tr("Startup completed"), 2000);
}

void NaProg::on_actionLyTeX_PDF_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active) {
        error_noopenfile();
        return;
    }
    else {
        QFileInfo openfile(active->curFile);
        QString workingpath = openfile.absolutePath();
        QStringList mitzugeben;
        mitzugeben << "--pdf --latex-program='lualatex --shell-escape -interaction=nonstopmode'" << activeMdiChild()->curFile;
        ausfuehren(Lilypond_Path+"/lilypond-book",mitzugeben,workingpath);
    }
}

void NaProg::on_actionGregorioTeX_PDF_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active) {
        error_noopenfile();
        return;
    }
    else {
        gregorio_extract("-main.tex");
        QStringList mitzugeben;
        if (!active) { return; } else {
            QFileInfo openfile(active->curFile);
            QString workingpath = openfile.absolutePath();
            mitzugeben << "--shell-escape" <<  "-interaction=nonstopmode" << workingpath+"/"+openfile.baseName()+"-main.tex";
            ausfuehren(LaTeX_Path+"/lualatex",mitzugeben,workingpath);
        }
    }
}
void NaProg::on_actionPDF_ansehen_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active) { error_noopenfile();
        return; }
    else {
        QFileInfo openfile(active->curFile); // extracting file name
        QString workingpath = openfile.absolutePath(); // strip closing /
        if (openfile.completeSuffix() == "gabc")
            QDesktopServices::openUrl(QUrl("file://"+workingpath+"/"+openfile.baseName()+"-main.pdf", QUrl::TolerantMode));
        else
            QDesktopServices::openUrl(QUrl("file://"+workingpath+"/"+openfile.baseName()+".pdf", QUrl::TolerantMode));
    }
}
void NaProg::on_actionEinstellungen_2_triggered()
{
    NASettings* settingsdlg = new NASettings;
    MdiChild *active = activeMdiChild();
    if (settingsdlg->exec() == QDialog::Accepted)
    {
    }
    if (!active) return;
    QSettings* preferences = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                           "DGSOFTWARE", "Notatio Antiqua");
    preferences->beginGroup("Font");
    active->selectAll();
    if (preferences->value("standardFont") != "")
        active->setFontFamily(preferences->value("standardFont").toString());
    if (preferences->value("standardSize") != "")
        active->setFontPointSize(preferences->value("standardSize").toReal());
    preferences->endGroup();
    active->moveCursor(QTextCursor::End);
    active->moveCursor(QTextCursor::Left);
    active->moveCursor(QTextCursor::Right);
    active->highlighter->rehighlight();
}

void NaProg::on_actionAufr_umen_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    QFileInfo openfile(activeMdiChild()->curFile);
    QString workingpath = openfile.absolutePath();
    QStringList garbage;
    garbage << ".dep" << ".aux" << ".log" <<  ".bbl" <<  ".ilg" << ".sav" << ".gtex"
            << ".bak" << "-blx.bib" << ".out" << ".toc" << "*.blg" << ".ind" << ".idx"
            << ".gaux" << "~" << ".backup" << "tmp" << "snippet" << "-auto.tex";
    QDirIterator garbage_collector(workingpath,QDir::Files,QDirIterator::NoIteratorFlags);
    while (garbage_collector.hasNext())
    {
        garbage_collector.next();
        foreach (QString str, garbage)
        {
            if (garbage_collector.fileInfo().fileName().contains(str))
            {
                QDir::setCurrent(workingpath);
                QFile *removefile = new QFile(garbage_collector.fileInfo().canonicalFilePath());
                logWindow->append(tr("Cleaning up %1").arg(garbage_collector.fileInfo().fileName()));
                removefile->remove();
            }
        }

    }
}

void NaProg::gregorio_extract(QString suffix)
{
    QString file_content;
    QTextStream gabc_content(&file_content);
    QStringList gregorio_header;
    gabc_content << activeMdiChild()->toPlainText();
    int info_count = 0;
    while (!gabc_content.atEnd()) // read header until %%
    {
        gregorio_header << gabc_content.readLine();
        if (gregorio_header[info_count] == "%%" || gregorio_header[info_count].isEmpty()) break;
        ++info_count;
    }
    if (!gregorio_header.isEmpty())
    {
        gregorio_header.removeLast(); // %% is removed from StringList
        foreach (QString element, gregorio_header) // filling variable with header information
        {
            element.resize(element.size() - 1);
            if (element.contains("name:"))
                hi.name = element.section(":",1,1).simplified();

            if (element.contains("arranger:"))
                hi.arranger = element.section(":",1,1).simplified();

            if (element.contains("office-part:"))
                hi.officepart = element.section(":",1,1).simplified();

            if (element.contains("mode:"))
                hi.mode = element.section(":",1,1).simplified();

            if (element.contains("annotation:"))
            {
                if (hi.annotation1.isEmpty()) hi.annotation1 = element.section(":",1,1).simplified();
                else hi.annotation2 = element.section(":",1,1).simplified();
            }

            if (element.contains("commentary:"))
                hi.commentary = element.section(":",1,1).simplified();

            if (element.contains("gregoriotex-font:"))
                hi.font = element.section(":",1,1).simplified();

            if (element.contains("gabc-copyright:"))
                hi.gabccopy = element.section(":",1,1).simplified();

            if (element.contains("score-copyright:"))
                hi.scorecopy = element.section(":",1,1).simplified();

            if (element.contains("occasion:"))
                hi.occasion = element.section(":",1,1).simplified();

            if (element.contains("meter:"))
                hi.meter = element.section(":",1,1).simplified();

            if (element.contains("author:"))
                hi.author = element.section(":",1,1).simplified();

            if (element.contains("date:"))
                hi.date = element.section(":",1,1).simplified();

            if (element.contains("manuscript:"))
                hi.manuscript = element.section(":",1,1).simplified();

            if (element.contains("manuscript-reference:"))
                hi.mreference = element.section(":",1,1).simplified();

            if (element.contains("book:"))
                hi.book = element.section(":",1,1).simplified();

            if (element.contains("transcriber:"))
                hi.transcriber = element.section(":",1,1).simplified();

            if (element.contains("transcription-date:"))
                hi.tdate = element.section(":",1,1).simplified();

            if (element.contains("initial-style:"))
                hi.instyle = element.section(":",1,1).simplified();

            if (element.contains("user-notes:"))
                hi.notes = element.section(":",1,1).simplified();
        }
    }
    gregorio_prepare(suffix);
}

void NaProg::gregorio_prepare(QString suffix)
{
    QFileInfo openfile(activeMdiChild()->curFile);
    tmplName = templateFolder+"/"+templateBox->currentText();
    if (tmplName.isEmpty())
    {
       tmplName = QFileDialog::getOpenFileName(this,tr("Open Template"),"", "Notatio Antiqua Template (*.natemplate)");
    }
    QString gabcName = openfile.fileName();
    QString outputName = openfile.absolutePath()+"/"+openfile.baseName()+suffix;
    logWindow->append(tr("Using template file: %1").arg(tmplName));
    QFile tmpl(tmplName);
    if (!tmpl.exists()) return;
    QFile output(outputName);
    if (QFile::exists(outputName))
    {
        if (QMessageBox::question(this,tr("Notatio Antiqua"),tr("You seem to already have compiled your GABC to PDF. Do you want to overwrite the old TeX-File (Backup will be made)?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::No) return;
        QString bak = outputName;
        bak.append("~");
        QFile::copy(outputName,bak);
        QFile::remove(outputName);
    }
    if (!output.exists())
    {
        QStringList tmplcontent;
        tmpl.open(QFile::ReadOnly | QFile::Text);
        QTextStream in(&tmpl);
        int i = 0;
        while (!in.atEnd())
        {
            tmplcontent << in.readLine();
            ++i;
        }
        QSettings* preferences = new QSettings(QSettings::IniFormat,QSettings::UserScope,"DGSOFTWARE", "Notatio Antiqua");
        preferences->beginGroup("Font");
        QString docsFont;
        QString docssFont;
        QString doccsFont;
        QString docinitFont;
        QString staffsize;
        QString fontsize;
        docsFont = preferences->value("docserifFont").toString();
        docssFont = preferences->value("docsansFont").toString();
        doccsFont = preferences->value("doccsFont").toString();
        docinitFont = preferences->value("docinitFont").toString();
        staffsize = preferences->value("staffsize").toString();
        fontsize = preferences->value("fontsize").toString();
        preferences->endGroup();
        if (hi.font == "" ) hi.font = "greciliae"; // Standard
        tmplcontent.replaceInStrings(QRegularExpression("VAR:NAME"), hi.name);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:ANN1"), hi.annotation1);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:ANN2"), hi.annotation2);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:COMMENTARY"), hi.commentary);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:FILENAME"), gabcName);
        if ( QMessageBox::question(this,tr("Notatio Antiqua"),tr("Do you want to include the User Notes in your output PDF?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes )
        {
            tmplcontent.replaceInStrings(QRegularExpression("VAR:USERNOTES"),hi.notes);
        }
        else {
            tmplcontent.replaceInStrings("User Notes:\\par","",Qt::CaseInsensitive);
            tmplcontent.replaceInStrings("VAR:USERNOTES"," ");
        }
        tmplcontent.replaceInStrings(QRegularExpression("VAR:SERIFFONT"),docsFont);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:SANSFONT"),docssFont);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:CSFONT"),doccsFont);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:INITFONT"),docinitFont);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:STAFFSIZE"),staffsize);
        tmplcontent.replaceInStrings(QRegularExpression("VAR:FONTSIZE"),fontsize+"pt");
        tmplcontent.replaceInStrings(QRegularExpression("VAR:FONT"), hi.font);
        i = 0;
        output.open(QFile::ReadWrite | QFile::Text);
        QTextStream out(&output);
        while (i <= tmplcontent.count()-1)
        {
            out << tmplcontent[i] << "\n";
            ++i;
        }
        reset_headers();
        logWindow->append(tr("Created file: %1 successfully.").arg(outputName));
    }
}

void NaProg::reset_headers()
{
    hi.name.clear();
    hi.font="";
    hi.annotation1="";
    hi.annotation2="";
    hi.arranger="";
    hi.author="";
    hi.book="";
    hi.commentary="";
    hi.date="";
    hi.gabccopy="";
    hi.instyle="";
    hi.manuscript="";
    hi.meter="";
    hi.mode="";
    hi.mreference="";
    hi.notes="";
    hi.occasion="";
    hi.officepart="";
    hi.scorecopy="";
    hi.tdate="";
    hi.transcriber="";
}

void NaProg::tolog()
{
    QByteArray raw_data = execProg->readAllStandardOutput();
    logWindow->append(QString(raw_data));
}

void NaProg::on_action_ber_triggered()
{
    NAHelp* helpwindow = new NAHelp;
    helpwindow->showMaximized();
}

void NaProg::error_noopenfile()
{
    QMessageBox::critical(this,tr("Notatio Antiqua"),
                          tr("No document window open, you can't run the desired action."),QMessageBox::Ok,QMessageBox::NoButton);
}

void NaProg::initialization()
{
    nainit* inicreate = new nainit;
    inicreate->exec();
}

void NaProg::on_actionInitialisierung_triggered()
{
    initialization();
}

void NaProg::on_actionClef_triggered()
{
    QString retclef;
    NAClefSelect clefchoice;
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    if( clefchoice.exec() == QDialog::Accepted )
    {
        retclef = clefchoice.clefS;
    }
    if (!retclef.isEmpty())
        active->insertFromMenuWizard(retclef);

}

void NaProg::on_actionHeader_Wizard_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active) {
        on_actionNeu_triggered();
        headerWiz();
    } else headerWiz();

}

void NaProg::on_actionCeleriter_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("[cs:c]");
}

void NaProg::on_actionTenere_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("[cs:t]");
}

void NaProg::on_actionMediocriter_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("[cs:m]");
}

void NaProg::on_actionNon_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("[cs:n]");
}

void NaProg::on_actionExspecta_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("[cs:x]");
}

void NaProg::on_actionVirgula_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("(`)");
}

void NaProg::on_actionQuarter_Bar_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("(,)");
}

void NaProg::on_actionHalf_Bar_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("(;)");
}

void NaProg::on_actionDouble_Bar_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("(::)");
}

void NaProg::on_actionFull_Bar_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("(:)");
}

void NaProg::on_actionDagger_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("+");
}

void NaProg::on_actionCross_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("<v>\\grecross</v>");
}

void NaProg::on_actionVerse_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("<sp>V/</sp>");
}

void NaProg::on_actionResponse_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("<sp>R/</sp>");
}

void NaProg::on_actionAntiphon_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("<sp>A/</sp>");
}

void NaProg::on_actionAccented_triggered()
{
    MdiChild *active = activeMdiChild();
    if(!active)
    {
        error_noopenfile();
        return;
    }
    activeMdiChild()->insertFromMenuWizard("<sp>'æ</sp>");
}

void NaProg::processFile(bool isGabc)
{
    MdiChild *active = activeMdiChild();
    if (!active)
    {
        error_noopenfile();
        return;
    }
    if (active->document()->isModified()) {
        active->save();
    }
    if (isGabc)
    {
        if (templateBox->currentText() == "inclusion.natemplate") {
            MdiChild *active = activeMdiChild();
            if(!active) {
                error_noopenfile();
                return;
            }
            else
                gregorio_extract("-inc.tex");
            QMessageBox::information(this,tr("TeX for inclusion"),tr("Your TeX file for inclusion in other projects has been written. This file is not standalone, so no PDF produced."),QMessageBox::Ok,QMessageBox::NoButton);
        } else
            on_actionGregorioTeX_PDF_triggered();
    }
    else
    {
        if (templateBox->currentText() == "inclusion.natemplate") {
            QMessageBox::information(this,tr("TeX for inclusion"),tr("Your TeX file for inclusion in other projects has been written. This file is not standalone, so no PDF produced."),QMessageBox::Ok,QMessageBox::NoButton);
        } else
            on_actionLyTeX_PDF_triggered();
    }
}

void NaProg::on_actionJsgabc_triggered()
{
    if (QMessageBox::warning(this,tr("Notatio Antiqua Warning"),tr("This feature is highly experimental and not functioning as expected. Proceed?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::No) return;
    napsalm* psalmW = new napsalm;
    psalmW->show();
}

void NaProg::on_actionQuelle_drucken_triggered()
{
    MdiChild *active = activeMdiChild();
    if (!active)
    {
        error_noopenfile();
        return;
    }
    QPrinter sourcePrint;
    QPrintDialog printSetting(&sourcePrint,this);
    if (printSetting.exec() == QDialog::Accepted)
        active->document()->print(&sourcePrint);
}
void NaProg::isArgument()
/**
 * @brief Open file with command line arguments
 * @abstract When called from command line or in similar ways, open all arguments with gabc or lytex file extension at start.
 *
 **/
{
    QStringList startArguments;
    QStringList filesToOpen;
    for (int i=1; i < static_cast<naapplication*>(QCoreApplication::instance())->arguments().count();i++)
        startArguments << static_cast<naapplication*>(QCoreApplication::instance())->arguments().at(i);
    foreach(QString str,startArguments)
        if (str.contains("gabc") || str.contains("lytex"))
            filesToOpen << str;
    foreach(QString fileName,filesToOpen)
    {
        openFile(fileName);
    }
}
void NaProg::openFile(QString fileName)
/**
 * @brief Open file with filename fileName
 * @abstract Open file. When there is an open file with same name, set focus on it. Otherwise open.
 *
 **/
{
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            ui->mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File opened."), 2000);
            child->showMaximized();
            child->setFocus();
        } else {
            child->close();
        }
        addToRecent(fileName);
    }
}
void NaProg::fileOpen_request(QString &fORq)
{
    openFile(fORq);
}
void NaProg::addToRecent(QString fileName)
{
    QSettings* preferences = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                           "DGSOFTWARE", "Notatio Antiqua");
    preferences->beginGroup("File");
    QStringList recentFiles = preferences->value("recentFiles").toStringList();
    // converting existing entries of last used files and deleting lastOpen entry
    QString lastOpen = preferences->value("lastOpen").toString();
    if (!lastOpen.isEmpty()) {
        recentFiles << lastOpen;
        preferences->remove("lastOpen");
    }
    // removing duplicates, if any
    recentFiles.removeDuplicates();
    if (recentFiles.count() >= MaxRecent) {
        int overfull = recentFiles.count() - MaxRecent + 1;
        if (overfull > 0) for (int i = 0; i < overfull; i++) recentFiles.removeAt(i);
    }
    recentFiles << fileName;
    // removing duplicates, if any (again)
    recentFiles.removeDuplicates();
    preferences->setValue("recentFiles",recentFiles);
    preferences->endGroup();
    updateRecent();
}
void NaProg::updateRecent()
{
    QSettings* preferences = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                           "DGSOFTWARE", "Notatio Antiqua");
    preferences->beginGroup("File");
    QStringList recentFiles = preferences->value("recentFiles").toStringList();
    int numRecent = qMin(recentFiles.size(), (int)MaxRecent);
    for (int i = 0; i < numRecent; ++i) {
        QString actionText = tr("&%1 %2").arg(i+1).arg(QFileInfo(recentFiles[i]).fileName());
        recentAct[i]->setText(actionText);
        recentAct[i]->setData(recentFiles[i]);
        recentAct[i]->setVisible(true);
    }
    preferences->endGroup();
}
void NaProg::headerWiz()
{
    MdiChild *active = activeMdiChild();
    if (!active) {
        error_noopenfile();
        return;
    }
    NAHeaderWizard wiz;
    if (wiz.exec() == QDialog::Accepted)
    {
        QStringList::const_iterator it;
        for (it= wiz.header.constBegin(); it!= wiz.header.constEnd();++it)
        {
            active->append(*it);
        }
        if (QMessageBox::question(this,tr("Notatio Antiqua"),tr("Do you want to create an initial clef using clef wizard now?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes) on_actionClef_triggered();
    }
}

void NaProg::on_actionFind_triggered()
{
    nafindreplace* nf = new nafindreplace;
    connect(nf,SIGNAL(findText(QString,bool,bool,bool)),this,SLOT(documentFind(QString,bool,bool,bool)));
    connect(nf,SIGNAL(replaceText(QString,QString,bool,bool,bool,bool)),this,SLOT(documentReplace(QString,QString,bool,bool,bool,bool)));
    addDockWidget(Qt::LeftDockWidgetArea,nf);
}
void NaProg::documentFind(QString findText,bool caseS, bool words, bool direction)
{
    // getting correct find flags first
    QTextDocument::FindFlags queryflag = QTextDocument::FindFlag();
    if (caseS) queryflag |= QTextDocument::FindCaseSensitively;
    if (words) queryflag |= QTextDocument::FindWholeWords;
    if (direction) queryflag |= QTextDocument::FindBackward;
    MdiChild *active = activeMdiChild(); // is any window/document open?
    if (!active) {
        error_noopenfile();
        return;
    }
    active->setFocus(); // set focus on active document
    if (findText.isEmpty()) // Is there any search string?
        QMessageBox::information(this, tr("Empty Search Field"), tr("The search field is empty. Please enter a word and click Find."),QMessageBox::Ok);
    bool found = false; // no, at the moment we haven't found anything
    QTextCursor highlightCursor(active->document());
    QTextCursor cursor(active->document());
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(cursor.charFormat());
    active->highlighter->rehighlight();
    cursor.clearSelection();
    highlightCursor.clearSelection(); // ensure nothing is highlighted
    cursor.beginEditBlock(); // now the searching is done
    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setBackground(QColor::fromCmyk(100,80,0,0,80));

    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = active->document()->find(findText, highlightCursor,
                                         queryflag);

        if (!highlightCursor.isNull()) {
            found = true;
            highlightCursor.movePosition(QTextCursor::WordRight,
                                         QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);
        }
    }
    cursor.endEditBlock();
    if (!found) // still nothing
        QMessageBox::information(this, tr("Word not found"), tr("Could not find the requested phrase."),QMessageBox::Ok);
}
void NaProg::documentReplace(QString findText, QString replaceText, bool caseS, bool words, bool direction, bool repall)
{
    // getting correct find flags first
    QTextDocument::FindFlags queryflag = QTextDocument::FindFlag();
    if (caseS) queryflag |= QTextDocument::FindCaseSensitively;
    if (words) queryflag |= QTextDocument::FindWholeWords;
    if (direction) queryflag |= QTextDocument::FindBackward;
    // getting active document
    MdiChild *active = activeMdiChild();
    if (!active) {
        error_noopenfile();
        return;
    }
    active->setFocus(); // set focus on active document
    if (findText.isEmpty()) // Is there any search string?
        QMessageBox::information(this, tr("Empty Search Field"), tr("The search field is empty. Please enter a word and click Find."),QMessageBox::Ok);
    bool found = false; // no, at the moment we haven't found anything
    bool abort = false; // we want to proceed with replacing
    QTextCursor highlightCursor(active->document());
    QTextCursor cursor(active->document());
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(cursor.charFormat());
    active->highlighter->rehighlight();
    cursor.clearSelection();
    highlightCursor.clearSelection(); // ensure nothing is highlighted
    cursor.beginEditBlock(); // now the searching is done
    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setBackground(QColor::fromCmyk(100,80,0,0,80));

    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = active->document()->find(findText, highlightCursor,
                                                   queryflag);

        if (!highlightCursor.isNull() && !abort) {
            found = true;
            highlightCursor.insertText(replaceText);
            if (!repall)
            {
                replaceQuestion.setText(tr("Replacing string: ")+findText+tr(" with ")+replaceText);
                replaceQuestion.setInformativeText(tr("Replaced string. Do you want to proceed to replacing the next string?"));
                replaceQuestion.setIconPixmap(QPixmap(":/images/question-symbolic.svg.png"));
                replaceQuestion.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No);
                int ret = replaceQuestion.exec();
                switch (ret)
                {
                case QMessageBox::Yes:
                    break;
                case QMessageBox::YesToAll:
                    repall = true;
                    break;
                case QMessageBox::No:
                    abort = true; // last replacing
                    break;
                }
            }
        }
    }
    cursor.endEditBlock();
    if (!found) // still nothing
        QMessageBox::information(this, tr("Word not found"), tr("Could not find the requested phrase."),QMessageBox::Ok);
}
void NaProg::createRibbonBar()
{
    // ribbon-style menu on top
    naribbonBar = new QToolBar;
    naribbons = new QTabWidget;
    naribbonBar->setFont(QFont("Ubuntu",12));
    naribbonBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // File ribbon
    fileBox = new QGroupBox;
    fileToolBar = new QToolBar;
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileToolBar->addAction(ui->actionNeu);
    fileToolBar->addSeparator();
    fileToolBar->addAction(ui->action_ffnen);
    fileToolBar->addAction(ui->actionErneut_ffnen);
    fileToolBar->addSeparator();
    fileToolBar->addAction(ui->actionSpeichern);
    fileToolBar->addAction(ui->actionSpeichern_unter);
    fileToolBar->addSeparator();
    fileToolBar->addAction(ui->actionClose);
    fileToolBar->addAction(ui->actionClose_All);
    fileToolBar->addSeparator();
    fileToolBar->addAction(ui->actionQuelle_drucken);
    fileBox->setTitle(tr("File operations"));
    fileBox->setToolTip(tr("Select file operation"));
    fileBox->setStatusTip(fileBox->toolTip());
    fileLayout = new QHBoxLayout;
    fileLayout->addWidget(fileToolBar);
    fileBox->setLayout(fileLayout);
    naribbons->insertTab(1,fileBox,QIcon(":/images/document-revert-symbolic.svg.png"),tr("File"));

    // Edit ribbon
    editBox = new QGroupBox;
    nafindreplace* nf = new nafindreplace;
    connect(nf,SIGNAL(findText(QString,bool,bool,bool)),this,SLOT(documentFind(QString,bool,bool,bool)));
    connect(nf,SIGNAL(replaceText(QString,QString,bool,bool,bool,bool)),this,SLOT(documentReplace(QString,QString,bool,bool,bool,bool)));
    editToolBar = new QToolBar;
    editToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    editToolBar->addAction(ui->actionKopieren);
    editToolBar->addAction(ui->actionAusschneiden);
    editToolBar->addAction(ui->actionEinf_gen);
    editToolBar->addSeparator();
    editToolBar->addAction(ui->actionRedo);
    editToolBar->addAction(ui->actionUndo);
    editLayout = new QHBoxLayout;
    editLayout->addWidget(editToolBar);
    editLayout->addWidget(nf);
    editBox->setLayout(editLayout);
    editBox->setTitle(tr("Edit file"));
    editBox->setToolTip(tr("Cut, Copy, Paste"));
    editBox->setStatusTip(editBox->toolTip());
    naribbons->insertTab(2,editBox,QIcon(":/images/edit-symbolic.svg.png"),tr("Edit"));

    // insert ribbon
    insertBox = new QGroupBox;
    insertToolBar = new QToolBar;
    insertToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    insertToolBar->addAction(ui->actionVirgula);
    insertToolBar->addAction(ui->actionQuarter_Bar);
    insertToolBar->addAction(ui->actionHalf_Bar);
    insertToolBar->addAction(ui->actionFull_Bar);
    insertToolBar->addAction(ui->actionDouble_Bar);
    insertToolBar->addAction(ui->actionDominican_Bars);
    insertToolBar->addSeparator();
    insertToolBar->addAction(ui->actionCross);
    insertToolBar->addAction(ui->actionDagger);
    insertToolBar->addAction(ui->actionAccented);
    insertToolBar->addSeparator();
    insertToolBar->addAction(ui->actionCeleriter);
    insertToolBar->addAction(ui->actionMediocriter);
    insertToolBar->addAction(ui->actionExspecta);
    insertToolBar->addAction(ui->actionTenere);
    insertToolBar->addAction(ui->actionNon);
    insertToolBar->addSeparator();
    insertToolBar->addAction(ui->actionAntiphon);
    insertToolBar->addAction(ui->actionResponse);
    insertToolBar->addAction(ui->actionVerse);
    insertToolBar->addSeparator();
    insertToolBar->addAction(ui->actionHeader_Wizard);
    insertToolBar->addAction(ui->actionClef);
    insertToolBar->addAction(ui->actionJsgabc);
    insertLayout = new QHBoxLayout;
    insertLayout->addWidget(insertToolBar);
    insertBox->setLayout(insertLayout);
    insertBox->setTitle(tr("Inserting Divisions, Symbols, Choral Signs - Assistants"));
    insertBox->setToolTip(tr("Insert bars, divisions, symbols and choral signs or use an assistant."));
    insertBox->setStatusTip(insertBox->toolTip());
    naribbons->insertTab(3,insertBox,QIcon(":/images/edit-paste-symbolic.svg.png"),tr("Insert / Assistants"));

    // processing ribbon
    runBox = new QGroupBox;
    runLayout = new QHBoxLayout;
    pdfBox = new QCheckBox;
    templateBox = new QComboBox;
    gabcProc = new QToolButton;
    lytexProc = new QToolButton;
    cleanUp = new QToolButton;
    templateText = new QLabel;
    pdfBox->setText(tr("View PDF after creation"));
    pdfBox->setCheckable(true);
    pdfBox->setChecked(true);
    templateText->setText(tr("Process file with template"));
    gabcProc->setText(tr("GABC\u2192PDF"));
    gabcProc->setIcon(QIcon(":/images/gabctopdf.png"));
    gabcProc->setIconSize(QSize(32,32));
    gabcProc->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    lytexProc->setText(tr("LyTeX\u2192PDF"));
    lytexProc->setIcon(QIcon(":/images/lytexpdf.png"));
    lytexProc->setIconSize(QSize(32,32));
    lytexProc->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    cleanUp->setText(tr("Aufräumen"));
    cleanUp->setIcon(QIcon(":/images/edit-delete-symbolic.svg.png"));
    cleanUp->setIconSize(QSize(32,32));
    cleanUp->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(gabcProc,SIGNAL(clicked()),this,SLOT(gabcP()));
    connect(lytexProc,SIGNAL(clicked()),this,SLOT(lytexP()));
    connect(cleanUp,SIGNAL(clicked()),this,SLOT(on_actionAufr_umen_triggered()));
    runLayout->addWidget(templateText);
    runLayout->addWidget(templateBox);
    runLayout->addWidget(pdfBox);
    runLayout->addWidget(gabcProc);
    runLayout->addWidget(lytexProc);
    runLayout->addWidget(cleanUp);
    runLayout->addStretch(15);
    runBox->setLayout(runLayout);
    runBox->setTitle(tr("Process with options"));
    runBox->setToolTip(tr("Set processing options and generate PDF"));
    runBox->setStatusTip(runBox->toolTip());
    naribbons->insertTab(4,runBox,QIcon(":/images/document-send-symbolic.svg.png"),tr("Process"));

    // settings ribbon
    prefsBox = new QGroupBox;
    prefsToolBar = new QToolBar;
    prefsLayout = new QHBoxLayout;
    prefsToolBar->addAction(ui->actionEinstellungen_2);
    prefsToolBar->addAction(ui->actionInitialisierung);
    prefsToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    prefsLayout->addWidget(prefsToolBar);
    prefsBox->setLayout(prefsLayout);
    prefsBox->setTitle(tr("Settings and Configuration"));
    prefsBox->setToolTip(tr("Set preferences for the editor, set program options, configure Notatio Antiqua"));
    prefsBox->setStatusTip(prefsBox->toolTip());
    naribbons->insertTab(5,prefsBox,QIcon(":/images/preferences-system-symbolic.svg.png"),tr("Settings"));

    // help ribbon
    helpBox = new QGroupBox;
    helpToolBar = new QToolBar;
    helpLayout = new QHBoxLayout;
    helpToolBar->addAction(ui->action_ber);
    helpToolBar->addAction(ui->action_ber_2);
    helpToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    helpLayout->addWidget(helpToolBar);
    helpBox->setLayout(helpLayout);
    helpBox->setTitle(tr("Help"));
    helpBox->setToolTip(tr("Help and information"));
    helpBox->setStatusTip(helpBox->toolTip());
    naribbons->insertTab(6,helpBox,QIcon(":/images/help-about.svg"),tr("Help"));

    // window ribbon
    windowBox = new QGroupBox;
    windowToolBar = new QToolBar;
    windowLayout = new QHBoxLayout;
    windowToolBar->addAction(ui->action_Previous_Window);
    windowToolBar->addAction(ui->action_Next_Window);
    windowToolBar->addSeparator();
    windowToolBar->addAction(ui->actionCascade);
    windowToolBar->addAction(ui->actionTile);
    windowToolBar->addSeparator();
    windowToolBar->addAction(ui->actionMaximize);
    windowToolBar->addAction(ui->actionRestore);
    windowToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    windowLayout->addWidget(windowToolBar);
    windowBox->setLayout(windowLayout);
    windowBox->setTitle(tr("Windows"));
    windowBox->setToolTip(tr("Change window size and behaviour"));
    windowBox->setStatusTip(windowBox->toolTip());
    naribbons->insertTab(7,windowBox,QIcon(":/images/window-maximize.svg"),tr("Windows"));

    // putting everything together and do some styling
    naribbons->setDocumentMode(1); // for ribbonlike style
    naribbonBar->autoFillBackground();
    naribbonBar->addWidget(naribbons); // magic: toolbar with ribbons :-)
    addToolBar(naribbonBar);
}
void NaProg::gabcP()
{
    processFile(1);
}
void NaProg::lytexP()
{
    processFile(0);
}
