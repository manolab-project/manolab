#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include "Gui.h"

class ImageWindow
{
public:
    ImageWindow();

    void Initialize();
    void Draw(const char* title, bool* p_open);

private:

    GLuint my_image_texture = 0;
};

#endif // IMAGEWINDOW_H
