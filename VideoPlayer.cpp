//
// Created by Rhys on 02/02/2022.
//

#include "VideoPlayer.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>

#include <Phonon/SeekSlider>

#include <iostream>

#include "Net.h"

QBuffer* vidBuf = NULL;

VideoPlayer::VideoPlayer() {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* controlsLayout = new QHBoxLayout;

    player = new Phonon::VideoPlayer();

    Phonon::SeekSlider* seekSlider = new Phonon::SeekSlider;
    seekSlider->setMediaObject(player->mediaObject());

    QPushButton* playBtn = new QPushButton("Play");
    QPushButton* pauseBtn = new QPushButton("Pause");

    connect(playBtn, SIGNAL(clicked()), player, SLOT(play()));
    connect(pauseBtn, SIGNAL(clicked()), player, SLOT(pause()) );

    controlsLayout->addWidget(playBtn);
    controlsLayout->addWidget(pauseBtn);

    mainLayout->addWidget(player);
    mainLayout->addWidget(seekSlider);
    mainLayout->addLayout(controlsLayout);

    setLayout(mainLayout);
}

void streamWrite(void* data, size_t len){
    if(vidBuf != NULL){
        vidBuf->write((const char*) data, (qint64) len);

        std::cout << vidBuf->size() << "\n";
    }
}

void VideoPlayer::playVideo(std::string link) {
    if(vidBuf != NULL) {
        delete vidBuf;
    }

    vidBuf = new QBuffer;
    vidBuf->setBuffer(new QByteArray);
    vidBuf->open(QIODevice::ReadWrite);

    Net::stream(link, streamWrite);

    Phonon::MediaSource source(vidBuf);

    player->play(source);
}