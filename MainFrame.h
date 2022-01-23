//
// Created by rhys on 22/01/2022.
//

#ifndef YOUTUBECLI_MAINFRAME_H
#define YOUTUBECLI_MAINFRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

#include "Table.h"
#include "Subscriptions.h"

class MainFrame : public wxFrame {
private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    Table* table;
    wxGrid* grid;

    Subscriptions subs;

    std::vector<Video> vids;

    void loadVids();

    void OnGridCellLeftDClick(wxGridEvent& evt);

    void VideoSelected(Video vid);

    void playVid(std::string link);

    void AutoSizeCols();

public:
    enum {
        ID_Table = 1
    };

    MainFrame();
};


#endif //YOUTUBECLI_MAINFRAME_H
