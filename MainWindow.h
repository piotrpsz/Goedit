#ifndef GOEDIT_MAIN_WINDOW_H
#define GOEDIT_MAIN_WINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

// local constants
    // Menu items
    static const char* const MenuFile;
    static const char* const MenuEdit;
    static const char* const MenuSearch;
    static const char* const MenuProject;
    static const char* const MenuDebugger;
    static const char* const MenuDocuments;
    static const char* const MenuHelp;
    // Toolbars
    // File menu subitems

// members
    // File menu subitems
    QAction* const _openFileAction;
    QAction* const _newFileAction;
    QAction* const _saveFileAction;
    QAction* const _saveAsAction;
    QAction* const _saveAllAction;
    QAction* const _printAction;
    QMenu* const _lastOpenedFilesMenu;
    QMenu* const _lastOpenedProjectsMenu;
    QAction* const _quitAction;
    // Edit menu subitems
    QAction* const _undoAction;
    QAction* const _redoAction;
    QAction* const _cutAction;
    QAction* const _copyAction;
    QAction* const _pasteAction;
    QAction* const _deleteAction;
    QAction* const _selectAllAction;
    QAction* const _propertiesAction;
    // Project menu subitems
    QAction* const _openProjectAction;
    QAction* const _closeProjectAction;
    QAction* const _newProjectAction;
    QAction* const _runAction;
    QAction* const _buildAction;
    QAction* const _testAction;
    QAction* const _rebuildAction;
    QAction* const _breakAction;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenu();
    QMenu* createFileMenu() const;
    QMenu* createEditMenu() const;
    QMenu* createProjectMenu() const;

    void showEvent(QShowEvent*) override;
    void closeEvent(QCloseEvent*) override;
private slots:
    // File menu subitems handlers
    void newFileHandler();
    void openFileHandler();
    void saveFileHandler();
    void saveAsHandler();
    void saveAllHandler();
    void printHandler();
    void lastOpenedFilesHandler();
    void lastOpenedProjectsHandler();
    // Edit menu subitems handlers
    void undoHandler();
    void redoHandler();
    void cutHandler();
    void copyHandler();
    void pasteHandler();
    void deleteHandler();
    void selectAllHandler();
    void propertiesHandler();
    // Project menu subitems handlers
    void openProjectHandler();
    void closeProjectHandler();
    void newProjectHandler();
    void runHandler();
    void buildHandler();
    void testHandler();
    void rebuildHandler();
    void breakHandler();
};

#endif // GOEDIT_MAIN_WINDOW_H
