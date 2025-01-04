#include "CustomUI.hpp"
#include "ItemFunctions.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    void customInit()
    {
        printf("Custom Init\n");
        initializeStatusObjects();

        initCustomUI();
        fillItemTable();
    }
}