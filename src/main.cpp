#include <iostream>
#include <gtkmm.h>

#include "windows/mainwindow.hpp"

int main(const int argc, char* argv[])
{
    const auto app = Gtk::Application::create("cone.team.onemotorgui.base");
    return app->make_window_and_run<OneMotorGUI::MainWindow>(argc, argv);
}
