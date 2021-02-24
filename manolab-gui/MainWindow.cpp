#include "MainWindow.h"
#include <filesystem>
#include <Util.h>
#include "ImGuiFileDialog.h"

MainWindow::MainWindow()
{

}

MainWindow::~MainWindow()
{

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
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            SetupFileMenu();
            ImGui::EndMenu();
        }
        /*
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        */
        ImGui::EndMainMenuBar();
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


    std::string p = engine.GetWorkspace() + "/files";


    files = Util::GetDirectoryFiles(p, "js");

    std::function< void(int, const std::vector<Value>&) > cb = std::bind( &MainWindow::EngineEvents, this, std::placeholders::_1 , std::placeholders::_2 );
    engine.RegisterEventEmitter(cb);

    for (const auto & f : files)
    {
        std::cout << f << std::endl;
    }
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

        gui.EndFrame();
    }

    gui.Destroy();
}


void MainWindow::EngineEvents(int signal, const std::vector<Value> &args)
{
    switch (signal)
    {

    case ProcessEngine::SIG_DELAY_1S:
        if (args.size() > 0)
        {
            //            QMetaObject::invokeMethod(this, "sigDelay", Qt::QueuedConnection,
            //                                      Q_ARG(int, args[0].GetInteger()));

        }
        break;
    case ProcessEngine::SIG_TEST_NUMBER:
        if (args.size() > 1)
        {
            //            QMetaObject::invokeMethod(this, "sigTest", Qt::QueuedConnection,
            //                                      Q_ARG(int, args[0].GetInteger()),
            //                                      Q_ARG(int, args[1].GetInteger())
            //                                      );
        }
        break;
    case ProcessEngine::SIG_STEP_NUMBER:
        if (args.size() >= 2)
        {
            //            QMetaObject::invokeMethod(this, "sigStep", Qt::QueuedConnection,
            //                                      Q_ARG(QString, args[0].GetString().c_str()),
            //                                      Q_ARG(bool, args[1].GetBool()));
        }
        break;
    case ProcessEngine::SIG_TEST_FINISHED:
        //        QMetaObject::invokeMethod(this, "sigFinished", Qt::QueuedConnection);

        break;

    case ProcessEngine::SIG_MESSAGE:
        if (args.size() > 0)
        {
            std::cout << args[0].GetString() << std::endl;
            //            QMetaObject::invokeMethod(this, "sigMessage", Qt::QueuedConnection,
            //                                      Q_ARG(QString, args[0].GetString().c_str()));
        }
        break;

    case ProcessEngine::SIG_LOADED:
        //        QMetaObject::invokeMethod(this, "sigScriptLoaded", Qt::QueuedConnection);
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
        //        QMetaObject::invokeMethod(this, "sigTestEnded", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_TEST_ERROR:
        //        QMetaObject::invokeMethod(this, "sigTestError", Qt::QueuedConnection);
        break;

    default:
        TLogWarning("Un-managed signal: " + std::to_string(signal));
        break;
    }

    //QMetaObject::invokeMethod(this, "sigRunningChanged", Qt::QueuedConnection);
}
