#include "CustomUI.hpp"
#include "ItemFunctions.hpp"
#include "extern/dw1.hpp"
#include "Partner.hpp"
#include "Timestamp.hpp"

extern "C"
{
    void customInit()
    {
        initializeStatusObjects();

        initTimestamp();
        initCustomUI();
        fillItemTable();
    }
}