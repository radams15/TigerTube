#include <iostream>
#include "Channel.h"
#include "Config.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


int main(int argc, char *argv[]){
    /*wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();*/

    Config conf;

    time_t start = clock();

    auto vids = conf.get_vids();

    time_t end = clock();

    std::cout << (end-start)/100000.0f << std::endl;

    return 0;
}
