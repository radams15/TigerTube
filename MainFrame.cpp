//
// Created by rhys on 22/01/2022.
//

#include "MainFrame.h"

#include "Net.h"

#ifdef __linux
#define PLAYER "flatpak run org.videolan.VLC"
#elif defined(__APPLE__)
#define PLAYER "VLC"
#endif

#define QUALITY ""

#define API_URL "http://therhys.co.uk/yt.php?url="

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Youtube") {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_AddSub, "&Add Subscription...\tCtrl-A",
                     "Add a new subscription");
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Welcome to youtube player!");


    wxPanel* panel = new wxPanel(this, -1);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(vbox);

    grid = new wxGrid(panel, ID_Table);
    grid->HideRowLabels();
    table = new Table();
    grid->SetTable(table);

    grid->Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &MainFrame::OnGridCellLeftDClick, this);

    vbox->Add(grid, 10, wxEXPAND|wxALL, 5);

    std::vector<wxString> headers = {"Title", "Author", "Published"};
    table->set_headers(headers);
    grid->SetTable(table);

    loadVids();

    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::AddSub, this, ID_AddSub);
}
void MainFrame::OnExit(wxCommandEvent& event){
    Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MainFrame::loadVids() {
    vids.clear();
    table->Clear();
    vids = subs.get_vids();
    for(auto vid : vids){
        std::vector<wxString> row = {
                vid.title,
                vid.channel_name,
                vid.publish_date
        };

        table->append(row);
    }

    grid->SetTable(table);
    AutoSizeCols();
}

void MainFrame::OnGridCellLeftDClick(wxGridEvent &evt) {
    Video vid = vids.at(evt.GetRow());
    VideoSelected(vid);
}

std::string chomp(std::string str){
    if(str.at(str.length()-1) == '\n'){
        str = str.substr(0, str.length()-1);
    }

    return str;
}

void MainFrame::playVid(std::string link) {
    std::string command;
#ifdef __linux
    command = PLAYER " '" + chomp(link) + "' &";
#elif defined(__APPLE__)
    system("osascript -e 'quit app \"" PLAYER "\"'");
    command = "open -a '" PLAYER "' '" + chomp(link) + "' &";
#else
#error "Unknown/Unsupported OS!"
#endif

    std::cout << command << std::endl;

    system(command.c_str());
}

void MainFrame::VideoSelected(Video vid) {
    std::string url = API_URL+vid.link+"&quality=best[height<=" + std::to_string(subs.quality) + "]";

    std::string raw = Net::get(url).content;

    playVid(raw);
}

void MainFrame::AutoSizeCols() {
    long num_cols=grid->GetNumberCols();
    long width=grid->GetRowLabelSize();

    for(int col=0;col<num_cols-1;col++){
        grid->AutoSizeColumn(col);
        width+=grid->GetColSize(col);
    }

    if (num_cols>0){
        width=GetClientSize().GetWidth()-1;
        std::cout << width << std::endl;
        grid->SetColSize(num_cols-1,width);
    }

}

void MainFrame::AddSub(wxCommandEvent& event) {
    wxTextEntryDialog dlg(this, "Channel ID");
    if ( dlg.ShowModal() == wxID_OK ){
        std::string value = (std::string) dlg.GetValue();

        std::cout << value << std::endl;

        Channel c(value);

        if(! c.get_vids().empty()){
            subs.add_sub(c);

            loadVids();

            subs.save();
        }
    }

}
