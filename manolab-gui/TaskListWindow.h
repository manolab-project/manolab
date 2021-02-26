#ifndef TASKLISTWINDOW_H
#define TASKLISTWINDOW_H

#include "Gui.h"
#include "ImGuiFileDialog.h"
#include "IProcessEngine.h"

class TaskListWindow
{
public:
    TaskListWindow(IProcessEngine &engine);

    void Initialize();
    void Draw(const char* title, bool* p_open);

    void ScanWorkspace();
    void RefreshList();
private:
    IProcessEngine &mEngine;

    // Workspace stuff
    std::vector<std::string> files;
};

#endif // TASKLISTWINDOW_H
