/********************************************************************
 * Copyright (C) 2020 Piotr Pszczolkowski
 *-------------------------------------------------------------------
 * This file is part of Goedit.
 *
 * Goedit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Goedit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Goedit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *-------------------------------------------------------------------
 * AUTHOR : Piotr Pszczolkowski (piotr@beesoft.pl)
 * PROJECT: Goedit
 * FILE   : MainWindow.cpp
 * DATE   : 16.05.2020
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QApplication>
#include <QMenuBar>
#include <QSettings>
#include <QIcon>
#include "MainWindow.h"
#include "Shared/Shared.h"

/*------- local constants:
-------------------------------------------------------------------*/
const char* const MainWindow::MenuFile      = "File";
const char* const MainWindow::MenuEdit      = "Edit";
const char* const MainWindow::MenuSearch    = "Search";
const char* const MainWindow::MenuProject   = "Project";
const char* const MainWindow::MenuDebugger  = "Debugger";
const char* const MainWindow::MenuDocuments = "Documents";
const char* const MainWindow::MenuHelp      = "Help";

/********************************************************************
*                            MainWindow                        ctor *
********************************************************************/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // File menu subitems
    , _openFileAction         (new QAction(QIcon(":/img/OpenFileIcon"), "Open file ..."))
    , _newFileAction          (new QAction(QIcon(":/img/NewFileIcon"), "New file"))
    , _saveFileAction         (new QAction(QIcon(":/img/SaveFileIcon"), "Save file"))
    , _saveAsAction           (new QAction("Save as ..."))
    , _saveAllAction          (new QAction(QIcon(":/img/SaveAllIcon"), "Save all..."))
    , _printAction            (new QAction(QIcon(":/img/PrintIcon"), "Print ..."))
    , _lastOpenedFilesMenu    (new QMenu("Last opened files"))
    , _lastOpenedProjectsMenu (new QMenu("Last opened projects"))
    , _quitAction             (new QAction(QIcon(":/img/ExitIcon"), "Quit"))
    // Edit menu subitems
    , _undoAction             (new QAction(QIcon(":/img/UndoIcon"), "Undo"))
    , _redoAction             (new QAction(QIcon(":/img/RedoIcon"), "Redo"))
    , _cutAction              (new QAction(QIcon(":/img/CutIcon"), "Cut"))
    , _copyAction             (new QAction(QIcon(":/img/CopyIcon"), "Copy"))
    , _pasteAction            (new QAction(QIcon(":/img/PasteIcon"), "Paste"))
    , _deleteAction           (new QAction("Delete"))
    , _selectAllAction        (new QAction("Select all"))
    , _propertiesAction       (new QAction(QIcon(":/img/PropertiesIcon"), "Settings"))
    // Project menu subitems
    , _openProjectAction      (new QAction("Open project"))
    , _closeProjectAction     (new QAction("Close project"))
    , _newProjectAction       (new QAction("New project"))
    , _runAction              (new QAction(QIcon(":/img/RunIcon"), "Run"))
    , _buildAction            (new QAction(QIcon(":/img/MakeIcon"), "Build"))
    , _testAction             (new QAction("Test"))
    , _rebuildAction          (new QAction(QIcon(":/img/RebuildIcon"), "Rebuild"))
    , _breakAction            (new QAction(QIcon(":/img/BreakIcon"), "Break"))
{
    createMenu();
}

/********************************************************************
*                            ~MainWindow                       dtor *
********************************************************************/
MainWindow::~MainWindow() {
}

/********************************************************************
*                            createMenu                     private *
********************************************************************/
void MainWindow::createMenu() {
    QMenuBar* const bar = menuBar();
    bar->addMenu(createFileMenu());
    bar->addMenu(createEditMenu());


    if (auto action = bar->addMenu(new QMenu(MenuSearch)); action) {

    }

    bar->addMenu(createProjectMenu());

    if (auto action = bar->addMenu(new QMenu(MenuDebugger)); action) {

    }
    if (auto action = bar->addMenu(new QMenu(MenuDocuments)); action) {

    }
    if (auto action = bar->addMenu(new QMenu(MenuHelp)); action) {

    }

}

/********************************************************************
*                           createFileMenu                  private *
********************************************************************/
QMenu* MainWindow::createFileMenu() const {
    QMenu* menu = new QMenu(MenuFile);
    {
        _openFileAction->setShortcut(QKeySequence::Open);
        connect(_openFileAction, &QAction::triggered, this, &MainWindow::openFileHandler);
        menu->addAction(_openFileAction);
    }
    {
        _newFileAction->setShortcut(QKeySequence::New);
        connect(_newFileAction, &QAction::triggered, this, &MainWindow::newFileHandler);
        menu->addAction(_newFileAction);
    }
    {
        _saveFileAction->setShortcut(QKeySequence::Save);
        connect(_saveFileAction, &QAction::triggered, this, &MainWindow::saveFileHandler);
        menu->addAction(_saveFileAction);
    }
    {
        _saveAsAction->setShortcut(QKeySequence::SaveAs);
        connect(_saveAsAction, &QAction::triggered, this, &MainWindow::saveAsHandler);
        menu->addAction(_saveAsAction);
    }
    {
        connect(_saveAllAction, &QAction::triggered, this, &MainWindow::saveAllHandler);
        menu->addAction(_saveAllAction);
    }
    menu->addSeparator();
    {
        _printAction->setShortcut(QKeySequence::Print);
        connect(_printAction, &QAction::triggered, this, &MainWindow::printHandler);
        menu->addAction(_printAction);
    }
    menu->addSeparator();
    menu->addMenu(_lastOpenedFilesMenu);
    menu->addMenu(_lastOpenedProjectsMenu);
    menu->addSeparator();
    {
        _quitAction->setShortcut(QKeySequence::Quit);
        connect(_quitAction, &QAction::triggered, qApp, &QApplication::closeAllWindows);
        menu->addAction(_quitAction);
    }
    return menu;
}

/********************************************************************
*                        createProjectMenu                  private *
********************************************************************/
QMenu* MainWindow::createProjectMenu() const {
    QMenu* menu = new QMenu(MenuProject);
    {
        _openProjectAction->setShortcut(Qt::SHIFT | Qt::CTRL | Qt::Key_O);
        connect(_openProjectAction, &QAction::triggered, this, &MainWindow::openProjectHandler);
        menu->addAction(_openProjectAction);
    }
    {
        _closeProjectAction->setShortcut(Qt::SHIFT | Qt::CTRL | Qt::Key_C);
        connect(_closeProjectAction, &QAction::triggered, this, &MainWindow::closeProjectHandler);
        menu->addAction(_closeProjectAction);
    }
    {
        _newProjectAction->setShortcut(Qt::SHIFT | Qt::CTRL | Qt::Key_N);
        connect(_newProjectAction, &QAction::triggered, this, &MainWindow::newProjectHandler);
        menu->addAction(_newProjectAction);
    }
    menu->addSeparator();
    {
        _runAction->setShortcut(Qt::CTRL | Qt::Key_R);
        connect(_runAction, &QAction::triggered, this, &MainWindow::runHandler);
        menu->addAction(_runAction);
    }
    {
        _testAction->setShortcut(Qt::CTRL | Qt::Key_T);
        connect(_testAction, &QAction::triggered, this, &MainWindow::testHandler);
        menu->addAction(_testAction);
    }
    menu->addSeparator();
    {
        _buildAction->setShortcut(Qt::CTRL | Qt::Key_B);
        connect(_buildAction, &QAction::triggered, this, &MainWindow::buildHandler);
        menu->addAction(_buildAction);
    }
    {
        _rebuildAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_B);
        connect(_rebuildAction, &QAction::triggered, this, &MainWindow::rebuildHandler);
        menu->addAction(_rebuildAction);
    }
    menu->addSeparator();
    {
        _breakAction->setShortcut(Qt::CTRL | Qt::Key_C);
        connect(_breakAction, &QAction::triggered, this, &MainWindow::breakHandler);
        menu->addAction(_breakAction);
    }
    return menu;
}

/********************************************************************
*                           createEditMenu                  private *
********************************************************************/
QMenu* MainWindow::createEditMenu() const {
    QMenu* menu = new QMenu(MenuEdit);
    {
        _undoAction->setShortcut(QKeySequence::Undo);
        connect(_undoAction, &QAction::triggered, this, &MainWindow::undoHandler);
        menu->addAction(_undoAction);
    }
    {
        _redoAction->setShortcut(QKeySequence::Redo);
        connect(_redoAction, &QAction::triggered, this, &MainWindow::redoHandler);
        menu->addAction(_redoAction);
    }
    {
        _cutAction->setShortcut(QKeySequence::Cut);
        connect(_cutAction, &QAction::triggered, this, &MainWindow::cutHandler);
        menu->addAction(_cutAction);
    }
    {
        _copyAction->setShortcut(QKeySequence::Copy);
        connect(_copyAction, &QAction::triggered, this, &MainWindow::copyHandler);
        menu->addAction(_copyAction);
    }
    {
        _pasteAction->setShortcut(QKeySequence::Paste);
        connect(_pasteAction, &QAction::triggered, this, &MainWindow::pasteHandler);
        menu->addAction(_pasteAction);
    }
    {
        _deleteAction->setShortcut(QKeySequence::Delete);
        connect(_deleteAction, &QAction::triggered, this, &MainWindow::deleteHandler);
        menu->addAction(_deleteAction);
    }
    menu->addSeparator();
    {
        _selectAllAction->setShortcut(QKeySequence::SelectAll);
        connect(_selectAllAction, &QAction::triggered, this, &MainWindow::selectAllHandler);
        menu->addAction(_selectAllAction);
    }
    menu->addSeparator();
    {
        connect(_propertiesAction, &QAction::triggered, this, &MainWindow::propertiesHandler);
        menu->addAction(_propertiesAction);
    }
    return menu;
}


/********************************************************************
*                             showEvent                     private *
********************************************************************/
void MainWindow::showEvent(QShowEvent*) {
    QSettings settings;

    if (auto rect = settings.value("mainWindow/geometry", QRect()).toRect(); rect.isNull()) {
         const int screenIndex = settings.value("mainWindow/screenIndex", -1).toInt();
        Shared::resize(this, 75, 75, screenIndex);
        Shared::moveToCenter(this, screenIndex);
    } else {
        setGeometry(rect);
    }
}

/********************************************************************
*                             closeEvent                    private *
********************************************************************/
void MainWindow::closeEvent(QCloseEvent*) {
    QSettings settings;
    settings.setValue("mainWindow/screenIndex", Shared::currentScreenIndex(this));
    settings.setValue("mainWindow/geometry", geometry());
}


void MainWindow::newFileHandler() {
}

void MainWindow::openFileHandler() {
}

void MainWindow::saveFileHandler() {
}

void MainWindow::saveAsHandler() {
}

void MainWindow::saveAllHandler() {
}

void MainWindow::printHandler() {
}

void MainWindow::lastOpenedFilesHandler() {
}

void MainWindow::lastOpenedProjectsHandler() {
}

void MainWindow::undoHandler() {}
void MainWindow::redoHandler() {}
void MainWindow::cutHandler() {}
void MainWindow::copyHandler() {}
void MainWindow::pasteHandler() {}
void MainWindow::deleteHandler() {}
void MainWindow::selectAllHandler() {}
void MainWindow::propertiesHandler() {}

void MainWindow::openProjectHandler() {}
void MainWindow::closeProjectHandler() {}
void MainWindow::newProjectHandler() {}
void MainWindow::runHandler() {}
void MainWindow::buildHandler() {}
void MainWindow::testHandler() {}
void MainWindow::rebuildHandler() {}
void MainWindow::breakHandler() {}

