//
// Created by Rhys on 01/02/2022.
//

#include "VideoTable.h"

#include <QtGui/QTableWidgetItem>
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>

#include <iostream>

VideoTable::VideoTable(QWidget* parent) : QTableWidget(parent) {
    setColumnCount(3);

    QStringList headers;

    headers << "Title" << "Author" << "Published";
    setHorizontalHeaderLabels(headers);

    verticalHeader()->setVisible(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

void VideoTable::addVideo(Video vid) {
    setRowCount(numRows+1);

    vids.push_back(vid);

    std::cout << vid.title << std::endl;

    setItem(numRows, 0, new QTableWidgetItem(QString::fromStdString(vid.title)));
    setItem(numRows, 1, new QTableWidgetItem(QString::fromStdString(vid.channel_name)));
    setItem(numRows, 2, new QTableWidgetItem(QString::fromStdString(vid.publish_date)));

    numRows++;
}

void VideoTable::addVideos(std::vector<Video> vids){
    foreach(Video vid, vids){
        addVideo(vid);
    }
}

Video* VideoTable::getVid(){
    QItemSelectionModel *select = selectionModel();

    QList<QModelIndex> rows = select->selectedRows();

    if(rows.length() != 0){
        return & vids[rows[0].row()];
    }else{
        return NULL;
    }
}