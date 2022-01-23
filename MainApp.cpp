//
// Created by rhys on 22/01/2022.
//

#include "MainApp.h"
#include "MainFrame.h"

bool MainApp::OnInit() {
    MainFrame *frame = new MainFrame;
    frame->Show(true);

    return true;
}
