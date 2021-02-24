#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "TextEditor.h"

class CodeEditor
{
public:
    CodeEditor();

    void Draw(const char *title, bool *p_open);

    void Initialize();
private:
    TextEditor mEditor;
    std::string mFileToEdit;
};

#endif // CODEEDITOR_H
