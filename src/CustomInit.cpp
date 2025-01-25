#include "CustomUI.hpp"
#include "ItemFunctions.hpp"
#include "extern/dw1.hpp"
#include "Partner.hpp"

extern "C"
{
    void customInit()
    {
        initializeStatusObjects();

        initCustomUI();
        fillItemTable();
    }
}