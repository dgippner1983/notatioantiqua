#include "nadoinit.h"

void nadoinit::run()
{
    isinit = false;
    QStringList dirFilter;
    QString dirsIt;
    #if !defined Q_OS_WIN
        dirFilter << "notantiq.qhc" << "hyph_de_DE.dic" << "*.natemplate" << "lualatex" << "gregorio" << "lilypond-book";
    #endif
    #if defined Q_OS_WIN | defined __MINGW32__
        dirFilter << "notantiq.qhc" << "hyph_de_DE.dic" << "*.natemplate" << "lualatex.exe" << "gregorio.exe" << "lilypond-book.py";
    #endif
        if (!basePath.isEmpty())
            dirsIt = basePath;
        else {
            #if !defined Q_OS_WIN
                dirsIt = "/";
            #endif
            #if defined Q_OS_WIN | defined __MINGW32__
                dirsIt = "C:/";
            #endif
             }
        QDirIterator findPaths(dirsIt,dirFilter,QDir::Files,QDirIterator::Subdirectories);
        while(findPaths.hasNext())
        {
            QString dirsSearched = findPaths.next();
            QString currDir = findPaths.filePath();
            emit pathHasChanged(currDir);
            currentPath = tr("Current Directory: %1").arg(currDir);
            if (dirsSearched.contains("hyph_de_DE.dic"))
            {

                    dictFolder << findPaths.fileInfo().canonicalPath();

            }
            if (dirsSearched.contains("notantiq.qhc"))
            {

                    helpFolder << findPaths.fileInfo().canonicalPath();

            }
            if (dirsSearched.contains("article.natemplate")) {

                    templateFolder << findPaths.fileInfo().canonicalPath();

                }
    #if !defined Q_OS_WIN
            if (dirsSearched.contains("lualatex")) {
    #endif
    #if defined Q_OS_WIN | defined __MINGW32__
                if (dirsSearched.contains("lualatex.exe")) {
    #endif


                    LaTeX_Path << findPaths.fileInfo().canonicalPath();

    }
    #if !defined Q_OS_WIN
        if (dirsSearched.contains("lilypond-book")) {
    #endif
    #if defined Q_OS_WIN | defined __MINGW32__
            if (dirsSearched.contains("lilypond-book.py")) {
    #endif
                Lilypond_Path << findPaths.fileInfo().canonicalPath();
            }
    #if !defined Q_OS_WIN
                if (dirsSearched.contains("gregorio")) {
    #endif
    #if defined Q_OS_WIN | defined __MINGW32__
                if (dirsSearched.contains("gregorio.exe")) {
    #endif
                Gregorio_Path << findPaths.fileInfo().canonicalPath();

    }
    }
}


void nadoinit::setValues(QString path)
{
    basePath = path;
}
