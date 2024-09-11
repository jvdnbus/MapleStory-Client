// Generated with ImRAD 0.7
// visit https://github.com/tpecholt/imrad

#pragma once
#include "imrad.h"

class DebugUI
{
public:
    /// @begin interface
    void Open();
    void Close();
    void Draw();

    float value1;
    float value2;
    int value3;
    bool value4 = false;
    bool value5 = false;
    bool value6 = false;
    bool value7 = false;
    bool value8 = false;
    bool value9 = false;
    int PhObjCols = 4;
    /// @end interface

private:
    /// @begin impl
    bool isOpen = true;

    /// @end impl
};

extern DebugUI debugUI;
