//
// Created by Rhys on 02/02/2022.
//

#ifndef YOUTUBECLI_VIDEOPLAYER_H
#define YOUTUBECLI_VIDEOPLAYER_H

#include <QtGui/QWidget>
#include <Phonon/VideoPlayer>

class VideoPlayer : public QWidget{
private:

    Phonon::VideoPlayer* player;

public:

    VideoPlayer();

    void playVideo(std::string link);
};


#endif //YOUTUBECLI_VIDEOPLAYER_H
