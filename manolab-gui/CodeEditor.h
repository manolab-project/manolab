#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include "TextEditor.h"
#include "WindowBase.h"

class CodeEditor : public WindowBase
{
public:
    CodeEditor();

    void Draw(const char *title, bool *p_open);

    void Initialize();
private:
    TextEditor mEditor;
    std::string mFileToEdit;
};

#endif // CODE_EDITOR_H
