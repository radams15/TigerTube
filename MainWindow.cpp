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

QBuffer* vidBuf = NULL;

MainWindow::MainWindow(Config *conf) {

    this->conf = conf;

    QHBoxLayout* layout = new QHBoxLayout;

    setWindowTitle("TigerTube");

    table = new VideoTable;
    player = new Phonon::VideoPlayer();

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

void streamWrite(void* data, size_t len){
    if(vidBuf != NULL){
        vidBuf->write((const char*) data, (qint64) len);

        std::cout << vidBuf->size() << "\n";
    }
}

void MainWindow::videoSelected(Video vid) {
    if(vidBuf != NULL) {
        delete vidBuf;
    }
    vidBuf = new QBuffer;
    vidBuf->setBuffer(new QByteArray);
    vidBuf->open(QIODevice::ReadWrite);

    std::string url = API_URL + vid.link + "&quality=best[height<=" + std::to_string(conf->quality) + "]";

    std::string link = chomp(Net::get(url).content);

    std::cout << link << std::endl;

    Net::stream(link, streamWrite);

    Phonon::MediaSource source(vidBuf);

    player->play(source);
}


void MainWindow::playClicked(){
    Video* vid = table->getVid();

    if(vid == NULL){
        return;
    }

    videoSelected(*vid);
}