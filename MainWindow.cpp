//
// Created by Rhys on 01/02/2022.
//

#include "MainWindow.h"
#include "Net.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtCore/Qurl>
#include <QtCore/QBuffer>
#include <Phonon/MediaSource>

#include <iostream>


MainWindow::MainWindow(Config *conf) {

    this->conf = conf;

    QHBoxLayout* layout = new QHBoxLayout;

    setWindowTitle("TigerTube");

    table = new VideoTable;

    player = new VideoPlayer;

    QPushButton* playButton = new QPushButton("Play");

    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    layout->addWidget(table);
    layout->addWidget(playButton);
    layout->addWidget(player);


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

    std::cout << link << std::endl;

    player->playVideo(link);
}


void MainWindow::playClicked(){
    Video* vid = table->getVid();

    if(vid == NULL){
        return;
    }

    videoSelected(*vid);
}