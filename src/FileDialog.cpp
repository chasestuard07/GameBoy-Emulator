#include "FileDialog.h"
#include <nfd.h>
#include <iostream>

#include "FileDialog.h"
#include <nfd.h>

bool OpenROMDialog(std::string& path)
{

    nfdu8char_t* outPath = nullptr;

    nfdu8filteritem_t filters[] = {
        { "Game Boy ROM", "gb" }
    };

    nfdresult_t result = NFD_OpenDialog(
        &outPath,
        filters,
        1,
        nullptr
    );

    if (result == NFD_OKAY)
    {
        path = outPath;
        NFD_FreePath(outPath);
        return true;
    }

    return false;
}
