#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H

class WindowBase
{
public:
    WindowBase()
    {
        mDisplay = false;
    }

    bool IsVisible() const {
        return mDisplay;
    }
private:
    bool mDisplay;
};

#endif // WINDOW_BASE_H

