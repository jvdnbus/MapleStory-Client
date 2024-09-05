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

    /// @end interface

private:
    /// @begin impl
    bool isOpen = true;

    /// @end impl
};

extern DebugUI debugUI;
