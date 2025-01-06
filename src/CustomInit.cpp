#include "CustomUI.hpp"
#include "ItemFunctions.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    void customInit()
    {
        initializeStatusObjects();

        initCustomUI();
        fillItemTable();
    }
}