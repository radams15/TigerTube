/** @file main.cpp
 * This is the main entry point of the program.
 */
#include <iostream>

#include <QtGui/QApplication>

#include "MainWindow.h"
#include "Config.h"

/** @brief Main GUI method.
 * This method runs the QT GUI code and
 * returns the status code of it.
 */
int guiMain(int argc, char** argv, Config* conf){
    QApplication a(argc, argv);
    MainWindow w(conf);
    w.show();
    return QApplication::exec();
}

/** @brief Main method.
 * This is the main program entry point, which for now just starts the guiMain
 * method and returns the output to the caller.
 */
int main(int argc, char** argv){
    Config* c = new Config();
    int out = guiMain(argc, argv, c);

    c->save();

    return out;
}
