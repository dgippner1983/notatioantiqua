cmake_minimum_required(VERSION 3.16)
project(notatioantiqua VERSION 1.2 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE__OSX_ARCHITECTURES arm64;x86_64)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Pdf Help PrintSupport Svg Widgets)

qt_standard_project_setup()
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/images/notantiq.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
qt_add_executable(notatioantiqua WIN32 MACOSX_BUNDLE
    hnjalloc.c hnjalloc.h
    hyphen.c hyphen.h
    main.cpp
    naapplication.cpp naapplication.h
    naclefselect.cpp naclefselect.h naclefselect.ui
    nadoinit.cpp nadoinit.h
    nafindreplace.cpp nafindreplace.h nafindreplace.ui
    naheaderwizard.cpp naheaderwizard.h naheaderwizard.ui
    nahelp.cpp nahelp.h nahelp.ui
    nainit.cpp nainit.h nainit.ui
    najgabc.cpp najgabc.h najgabc.ui
    namdi.cpp namdi.h
    naprog.cpp naprog.h naprog.ui
    napsalm.cpp napsalm.h napsalm.ui
    nasettings.cpp nasettings.h nasettings.ui
    nasyntax.cpp nasyntax.h
    version.h
)
target_include_directories(notatioantiqua PRIVATE
    .
)

target_link_libraries(notatioantiqua PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Help
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
    Qt6::Pdf
)


# Resources:
set(i18n_resource_files
    "NaProg-de.qm"
    "NaProg-en.qm"
    "NaProg-fr.qm"
)

qt_add_resources(notatioantiqua "i18n"
    PREFIX
        "/"
    FILES
        ${i18n_resource_files}
)
set(naprog_resource_files
    "help/notantiq-de.qhc"
    "help/notantiq-fr.qhc"
    "help/notantiq.qch"
    "help/notantiq.qhc"
    "help/notantiq.qhcp"
    "images/acrobat.svg"
    "images/action-unavailable-symbolic.svg.png"
    "images/antiphon.svg"
    "images/arrow-left.svg"
    "images/arrow-right.svg"
    "images/celeriter.svg"
    "images/clef.svg"
    "images/cross.svg"
    "images/dagger.svg"
    "images/dgs.png"
    "images/document-edit-symbolic Kopie.svg.png"
    "images/document-export-symbolic.svg.png"
    "images/document-import-symbolic.svg.png"
    "images/document-new-symbolic.svg.png"
    "images/document-open-recent-symbolic.svg.png"
    "images/document-open-symbolic.svg.png"
    "images/document-page-setup-symbolic.svg.png"
    "images/document-print-preview-symbolic.svg.png"
    "images/document-print-symbolic.svg.png"
    "images/document-properties-symbolic.svg.png"
    "images/document-revert-symbolic.svg.png"
    "images/document-save-as-symbolic.svg.png"
    "images/document-save-symbolic.svg.png"
    "images/document-send-symbolic.svg.png"
    "images/dominican_bars.svg"
    "images/double_bar.svg"
    "images/edit-clear-all-symbolic.svg.png"
    "images/edit-clear-symbolic-rtl.svg.png"
    "images/edit-clear-symbolic.svg.png"
    "images/edit-copy-symbolic.svg.png"
    "images/edit-cut-symbolic.svg.png"
    "images/edit-delete-symbolic.svg.png"
    "images/edit-find-replace-symbolic.svg.png"
    "images/edit-find-symbolic.svg.png"
    "images/edit-paste-symbolic.svg.png"
    "images/edit-redo-symbolic.svg.png"
    "images/edit-select-all-symbolic.svg.png"
    "images/edit-select-symbolic.svg.png"
    "images/edit-symbolic.svg.png"
    "images/edit-tag-symbolic.svg.png"
    "images/edit-undo-symbolic.svg.png"
    "images/exspecta.svg"
    "images/format-justify-center-symbolic.svg.png"
    "images/format-justify-fill-symbolic.svg.png"
    "images/format-justify-left-symbolic.svg.png"
    "images/format-justify-right-symbolic.svg.png"
    "images/format-text-bold-symbolic.svg.png"
    "images/format-text-italic-symbolic.svg.png"
    "images/format-text-larger-symbolic.svg.png"
    "images/format-text-remove-formatting-symbolic.svg.png"
    "images/format-text-smaller-symbolic.svg.png"
    "images/format-text-strikethrough-symbolic.svg.png"
    "images/format-text-underline-symbolic.svg.png"
    "images/full_bar.svg"
    "images/gabctopdf.png"
    "images/half_bar.svg"
    "images/help-about.svg"
    "images/lytexpdf.png"
    "images/mediocriter.svg"
    "images/non.svg"
    "images/notantiq-dmgb.png"
    "images/notantiq-icon.png"
    "images/notantiq-icon.xcf"
    "images/notantiq.icns"
    "images/notantiq.jpg"
    "images/preferences-system-symbolic.svg.png"
    "images/quarter_bar.svg"
    "images/question-symbolic.svg.png"
    "images/responsa.svg"
    "images/tenere.svg"
    "images/toolbarBackground.png"
    "images/user-home-symbolic.svg"
    "images/versus.svg"
    "images/virgula.svg"
    "images/window-close-1.svg"
    "images/window-maximize.svg"
    "images/window-minimize.svg"
    "images/window-restore.svg"
    "images/windows-tile.svg"
    "napsalm.csv"
    "notantiq.css"
    "notantiq.icns"
    "notantiq.ico"
)

qt_add_resources(notatioantiqua "naprog"
    PREFIX
        "/"
    FILES
        ${naprog_resource_files}
)
set(naprog1_resource_files
    "font/Ubuntu-B.ttf"
    "font/Ubuntu-BI.ttf"
    "font/Ubuntu-C.ttf"
    "font/Ubuntu-L.ttf"
    "font/Ubuntu-LI.ttf"
    "font/Ubuntu-M.ttf"
    "font/Ubuntu-MI.ttf"
    "font/Ubuntu-R.ttf"
    "font/Ubuntu-RI.ttf"
    "font/UbuntuMono-B.ttf"
    "font/UbuntuMono-BI.ttf"
    "font/UbuntuMono-R.ttf"
    "font/UbuntuMono-RI.ttf"
)

qt_add_resources(notatioantiqua "naprog1"
    PREFIX
        "/fonts"
    FILES
        ${naprog1_resource_files}
)

install(TARGETS notatioantiqua
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET notatioantiqua
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
