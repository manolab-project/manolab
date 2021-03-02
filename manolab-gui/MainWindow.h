#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Gui.h"
#include "ConsoleWindow.h"
#include "CodeEditor.h"
#include "ProcessEngine.h"
#include "ImageWindow.h"
#include "TaskListWindow.h"
#include "Settings.h"
#include "TableWindow.h"

class MainWindow
{
public:
    MainWindow();
    ~MainWindow();

    void Initialize();
    void Loop();

private:
    ProcessEngine engine;
    Gui gui;
    ImageWindow imgWindow;
    ConsoleWindow console;
    CodeEditor editor;
    TaskListWindow taskList;
    Settings mSettings;
    TableWindow tableWindow;
    ImGuiFileDialog fileDialog;

    void SetupFileMenu();
    void SetupMainMenuBar();
    void EngineEvents(int signal, const std::vector<Value> &args);
};

#endif // MAINWINDOW_H
