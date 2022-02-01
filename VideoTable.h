//
// Created by Rhys on 01/02/2022.
//

#ifndef YOUTUBECLI_VIDEOTABLE_H
#define YOUTUBECLI_VIDEOTABLE_H

#include <QtGui/QTableWidget>
#include <QtGui/QWidget>
#include <vector>
#include "Video.h"

class VideoTable : public QTableWidget{
public:

    VideoTable(QWidget* parent = NULL);

    void addVideo(Video vid);
    void addVideos(std::vector<Video> vids);

    Video* getVid();

private:

    std::vector<Video> vids;

    int numRows = 0;
};


#endif //YOUTUBECLI_VIDEOTABLE_H
