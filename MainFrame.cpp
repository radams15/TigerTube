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

#define API_URL "http://therhys.co.uk/yt.php?url="

wxString toString(const char* in){
    return wxString::Format("%s", in);
}

wxString toString(std::string in){
    return toString(in.c_str());
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, toString("Youtube")) {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_AddSub, toString("&Add Subscription...\tCtrl-A"),toString("Add a new subscription"));
    menuFile->Append(ID_ChangeQuality, toString("&Change Quality...\tCtrl-U"),toString("Change default video quality"));
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, toString("&File"));
    menuBar->Append(menuHelp, toString("&Help"));
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText(toString("Welcome to youtube player!"));


    wxPanel* panel = new wxPanel(this, -1);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(vbox);

    playButton = new wxButton(panel, -1, toString("Play"));
    playButton->Bind(wxEVT_BUTTON, &MainFrame::PlayButtonPressed, this);


    grid = new wxGrid(panel, ID_Table);
    grid->HideRowLabels();
    table = new Table();
    grid->SetTable(table);

    grid->Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &MainFrame::OnGridCellLeftDClick, this);
    grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &MainFrame::OnGridCellLeftClick, this);

    vbox->Add(playButton, 2, wxALL, 5);
    vbox->Add(grid, 8, wxEXPAND|wxALL, 5);

    std::vector<wxString> headers = {toString("Title"), toString("Author"), toString("Published")};
    table->set_headers(headers);
    grid->SetTable(table);

    loadVids_t();

    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::AddSub, this, ID_AddSub);
    Bind(wxEVT_MENU, &MainFrame::ChangeQuality, this, ID_ChangeQuality);
}
void MainFrame::OnExit(wxCommandEvent& event){
    Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox(toString("This is a wxWidgets Hello World example"),
                 toString("About Hello World"), wxOK | wxICON_INFORMATION);
}

void MainFrame::loadVids() {
    vids.clear();
    table->Clear();

    vids = conf.get_vids();
    for(auto vid : vids){
        std::vector<wxString> row = {
                toString(vid.title),
				toString(vid.channel_name),
                toString(vid.publish_date)
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

    system(command.c_str());
}

void MainFrame::VideoSelected(Video vid) {
    std::string url = API_URL + vid.link + "&quality=best[height<=" + std::to_string(conf.quality) + "]";

    std::string raw = Net::get(url).content;

    playVid(raw);
}

void MainFrame::AutoSizeCols() {
    wxMilliSleep(800);
    long num_cols=grid->GetNumberCols();
    long width=grid->GetRowLabelSize();

    for(int col=0;col<num_cols-1;col++){
        grid->AutoSizeColumn(col);
        width+=grid->GetColSize(col);
    }

    if (num_cols>0){
        width=GetClientSize().GetWidth()-1;
        grid->SetColSize(num_cols-1,width);
    }

}

void MainFrame::AddSub(wxCommandEvent& event) {
    wxTextEntryDialog dlg(this, toString("Channel Name"));
    if ( dlg.ShowModal() == wxID_OK ){
        std::string value = std::string(dlg.GetValue().mb_str());

        Channel c = *Channel::from_name(value);

        if(! c.get_vids().empty()){
            conf.add_sub(c);

            loadVids_t();

            conf.save();
        }
    }

}

void MainFrame::ChangeQuality(wxCommandEvent &event) {
    wxArrayString choices;

    choices.push_back(toString("144"));
    choices.push_back(toString("240"));
    choices.push_back(toString("480"));
    choices.push_back(toString("720"));
    choices.push_back(toString("1024"));

    wxSingleChoiceDialog dlg(this, toString("Quality"), toString("Select Quality"), choices);

    auto it = std::find(choices.begin(), choices.end(), toString(std::to_string(conf.quality)));
	
    if(it != choices.end()){
        int index = it-choices.begin();
        dlg.SetSelection(index);
    }else{
        dlg.SetSelection(2);
    }

    if ( dlg.ShowModal() == wxID_OK ) {
        int value = atoi((choices[dlg.GetSelection()].mb_str()));

        conf.quality = value;
    }
}

MainFrame::~MainFrame() {
    conf.save();
}

void* loadVids_s(void* frame){
    ((MainFrame*)frame)->loadVids();
}

void MainFrame::loadVids_t() {
    pthread_t thread;

    pthread_create(&thread, NULL, loadVids_s, this);
}

void MainFrame::PlayButtonPressed(wxCommandEvent& event) {
    if(selectedVid != NULL){
        VideoSelected(*selectedVid);
    }
}

void MainFrame::OnGridCellLeftClick(wxGridEvent &evt) {
    selectedVid = &vids.at(evt.GetRow());
}
