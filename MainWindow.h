//
// Created by Rhys on 01/02/2022.
//

#ifndef YOUTUBECLI_MAINWINDOW_H
#define YOUTUBECLI_MAINWINDOW_H

#include <QtGui/QMainWindow>


#include "Config.h"
#include "VideoTable.h"
#include "VideoPlayer.h"

#define API_URL "http://therhys.co.uk/yt.php?url="

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Config* conf);

public slots:
    void playClicked();

private:

    VideoTable* table;
    Config *conf;

    VideoPlayer* player;


    void videoSelected(Video vid);
};


#endif //YOUTUBECLI_MAINWINDOW_H
