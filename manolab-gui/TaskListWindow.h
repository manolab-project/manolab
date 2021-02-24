#ifndef TASKLISTWINDOW_H
#define TASKLISTWINDOW_H

#include "Gui.h"
#include "ImGuiFileDialog.h"

class TaskListWindow
{
public:
    TaskListWindow();

    void Initialize();
    void Draw(const char* title, bool* p_open);

private:

};

#endif // TASKLISTWINDOW_H
