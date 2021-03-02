#include "MainWindow.h"
#include <filesystem>
#include <Util.h>
#include "ImGuiFileDialog.h"
#include "imgui_internal.h"

MainWindow::MainWindow()
    : taskList(engine)
{

}

MainWindow::~MainWindow()
{
    engine.Quit();
}

void MainWindow::SetupFileMenu()
{
    if (ImGui::MenuItem("Choose workspace"))
    {
        fileDialog.OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, ".");
    }
//    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
//    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
//    if (ImGui::MenuItem("Save As..")) {}
}

void MainWindow::SetupMainMenuBar()
{
    bool showAboutPopup = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            SetupFileMenu();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                showAboutPopup = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (showAboutPopup)
    {
        ImGui::OpenPopup("AboutPopup");
    }

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    //ImVec2 parent_pos = ImGui::GetWindowPos();
    //ImVec2 parent_size = ImGui::GetWindowSize();
    //ImVec2 center(parent_pos.x + parent_size.x * 0.5f, parent_pos.y + parent_size.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("AboutPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
        ImGui::Text("%s", SDL_GetPlatform());
        ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
        ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();

        ImGui::SameLine(300);
        if (ImGui::Button("Close", ImVec2(120, 40)))
        {
           ImGui::CloseCurrentPopup();
        }
       ImGui::EndPopup();
    }

    // display
    if (fileDialog.Display("ChooseDirDlgKey"))
    {
        // action if OK
        if (fileDialog.IsOk())
        {
          std::string filePathName = fileDialog.GetFilePathName();
          std::string filePath = fileDialog.GetCurrentPath();
          // action
        }

        // close
        fileDialog.Close();
    }
}

void MainWindow::Initialize()
{
    gui.Initialize();
    gui.ApplyTheme();

    editor.Initialize();
    imgWindow.Initialize();
    taskList.Initialize();

    mSettings.ReadSettings(engine);
    mSettings.WriteSettings(engine);


    taskList.ScanWorkspace();

    std::function< void(int, const std::vector<Value>&) > cb = std::bind( &MainWindow::EngineEvents, this, std::placeholders::_1 , std::placeholders::_2 );
    engine.RegisterEventEmitter(cb);


}

//engine.LoadScript(filePath.toStdString());

void MainWindow::Loop()
{
    // Main loop
    bool done = false;

    while (!done)
    {
        done = gui.PollEvent();

        gui.StartFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        SetupMainMenuBar();

        console.Draw("Console", nullptr);
        imgWindow.Draw("ImageWindow", nullptr);
        editor.Draw("Code Editor", nullptr);
        taskList.Draw("Test list", nullptr);
        tableWindow.Draw("Table View", nullptr);

        gui.EndFrame();
    }

    gui.Destroy();
}

/**
 * @brief MainWindow::EngineEvents
 *
 *  ATTENTION, POTENTIELLEMENT MULTI THREADÉ | PRÉVOIR DES MUTEX
 *
 * @param signal
 * @param args
 */
void MainWindow::EngineEvents(int signal, const std::vector<Value> &args)
{
    switch (signal)
    {

    case ProcessEngine::SIG_DELAY_1S:
        if (args.size() > 0)
        {
            console.AddMessage("[TEST] Delay: " + std::to_string(args[0].GetInteger()));
            //            QMetaObject::invokeMethod(this, "sigDelay", Qt::QueuedConnection,
            //                                      Q_ARG(int, args[0].GetInteger()));

        }
        break;
    case ProcessEngine::SIG_TEST_NUMBER:
        if (args.size() > 1)
        {
            console.AddMessage("[TEST] Test number: " + std::to_string(args[0].GetInteger())
                               + " / " + std::to_string(args[1].GetInteger()));
            //            QMetaObject::invokeMethod(this, "sigTest", Qt::QueuedConnection,
            //                                      Q_ARG(int, args[0].GetInteger()),
            //                                      Q_ARG(int, args[1].GetInteger())
            //                                      );
        }
        break;
    case ProcessEngine::SIG_STEP_NUMBER:
        if (args.size() >= 2)
        {
            std::string enabled = args[1].GetBool() ? "true" : "false";
            console.AddMessage("[TEST] Test step: " + args[0].GetString() + " " + enabled);
            //            QMetaObject::invokeMethod(this, "sigStep", Qt::QueuedConnection,
            //                                      Q_ARG(QString, .c_str()),
            //                                      Q_ARG(bool, args[1].GetBool()));
        }
        break;
    case ProcessEngine::SIG_TEST_FINISHED:
        console.AddMessage("[TEST] Finished");
        //        QMetaObject::invokeMethod(this, "sigFinished", Qt::QueuedConnection);

        break;

    case ProcessEngine::SIG_MESSAGE:
        if (args.size() > 0)
        {
            console.AddMessage(args[0].GetString());
//            std::cout << args[0].GetString() << std::endl;
            //            QMetaObject::invokeMethod(this, "sigMessage", Qt::QueuedConnection,
            //                                      Q_ARG(QString, args[0].GetString().c_str()));
        }
        break;

    case ProcessEngine::SIG_LOADED:
        //        QMetaObject::invokeMethod(this, "sigScriptLoaded", Qt::QueuedConnection);
        console.AddMessage("Script loaded");
        taskList.RefreshList();
        break;

    case ProcessEngine::SIG_INPUT_TEXT:
        if (args.size() >= 2)
        {
            //            QMetaObject::invokeMethod(this, "sigInputText", Qt::QueuedConnection,
            //                                      Q_ARG(QString, args[0].GetString().c_str()),
            //                                      Q_ARG(bool, args[1].GetBool()));
        }
        break;

    case ProcessEngine::SIG_AUTO_TEST_FINISHED:
        console.AddMessage("Script finished");
        //        BuildComChannelModel(); // refresh model
        //        // THEN, update UI
        //        QMetaObject::invokeMethod(this, "sigAutoTestFinished", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_SHOW_IMAGE:
        //        QMetaObject::invokeMethod(this, "sigShowImage", Qt::QueuedConnection,
        //                                  Q_ARG(bool, args[0].GetBool()));
        break;

    case ProcessEngine::SIG_TEST_SKIPPED:
        //        QMetaObject::invokeMethod(this, "sigTestSkipped", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_TEST_ENDED:
        console.AddMessage("Test ended");
        //        QMetaObject::invokeMethod(this, "sigTestEnded", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_TEST_ERROR:
        console.AddMessage("[TEST] Error");
        //        QMetaObject::invokeMethod(this, "sigTestError", Qt::QueuedConnection);
        break;
    case ProcessEngine::SIG_TABLE_ACTION:
        tableWindow.ParseAction(args);
        break;
    default:
        TLogWarning("Un-managed signal: " + std::to_string(signal));
        break;
    }

    //QMetaObject::invokeMethod(this, "sigRunningChanged", Qt::QueuedConnection);
}
