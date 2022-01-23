#include <iostream>
#include "Channel.h"
#include "Config.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


int main(int argc, char *argv[]){
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();

    /*Config conf;

    conf.save();*/

    return 0;
}
