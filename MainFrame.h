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
#include "Config.h"

class MainFrame : public wxFrame {
private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    Table* table;
    wxGrid* grid;

    Config conf;

    std::vector<Video> vids;

    void loadVids_t();

    void OnGridCellLeftClick(wxGridEvent& evt);
    void OnGridCellLeftDClick(wxGridEvent& evt);

    void VideoSelected(Video vid);

    void playVid(std::string link);

    void AutoSizeCols();

    void AddSub(wxCommandEvent& event);

    void ChangeQuality(wxCommandEvent& event);

    void PlayButtonPressed(wxCommandEvent& event);

    Video* selectedVid = NULL;
	
	wxButton* playButton;
	
	wxDECLARE_EVENT_TABLE()

public:
    enum {
        ID_Table = 1,
        ID_AddSub = 2,
        ID_ChangeQuality = 3
    };

    void loadVids();

    MainFrame();

    ~MainFrame();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(ID_AddSub, MainFrame::AddSub)
	EVT_BUTTON(playButton, MainFrame::PlayButtonPressed)
wxEND_EVENT_TABLE()



#endif //YOUTUBECLI_MAINFRAME_H
