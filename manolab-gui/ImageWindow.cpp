#include "ImageWindow.h"

ImageWindow::ImageWindow()
{

}

void ImageWindow::Initialize() {

    int my_image_width = 0;
    int my_image_height = 0;

    bool ret = Gui::LoadTextureFromFile("test/background.bmp", &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
}

void ImageWindow::Draw(const char *title, bool *p_open)
{
    if (!IsVisible())
    {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(626, 744), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(313, 367));

    float sz = ImGui::GetTextLineHeight();
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32(ImVec4(1.0, 1.0, 1.0, 1.0)));

    ImGui::End();
}
