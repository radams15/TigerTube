//
// Created by Rhys on 01/02/2022.
//

#include "MainWindow.h"
#include "Net.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>

#include <iostream>

MainWindow::MainWindow(Config *conf) {

    this->conf = conf;

    QHBoxLayout* layout = new QHBoxLayout;

    setWindowTitle("TigerTube");

    table = new VideoTable;

    QPushButton* playButton = new QPushButton("Play");

    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    layout->addWidget(table);
    layout->addWidget(playButton);


    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    // Set the widget to the centre of the window.
    setCentralWidget(widget);

    std::cout << conf->subs.size() << "\n";
    std::vector<Video> vids = conf->subs[0].get_vids();
    table->addVideos(vids);
}

std::string chomp(std::string str){
    if(str.at(str.length()-1) == '\n'){
        str = str.substr(0, str.length()-1);
    }

    return str;
}

void MainWindow::videoSelected(Video vid) {
    std::string url = API_URL + vid.link + "&quality=best[height<=" + std::to_string(conf->quality) + "]";

    std::string link = chomp(Net::get(url).content);

    //https://doc.qt.io/qt-5/qmediaplayer.html#details

    std::string command;
#ifdef __linux
    command = PLAYER " '" + link + "' &";
#elif defined(__APPLE__)
    system("osascript -e 'quit app \"" PLAYER "\"'");
    command = "open -a '" PLAYER "' '" + link + "' &";
#else
#error "Unknown/Unsupported OS!"
#endif

    std::cout << command << std::endl;

    system(command.c_str());
}


void MainWindow::playClicked(){
    Video* vid = table->getVid();

    if(vid == NULL){
        return;
    }

    videoSelected(*vid);
}