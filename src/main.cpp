#include "MainWindow.h"
#include <gtkmm/application.h>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.example.hello-fft");

    return app->make_window_and_run<MainWindow>(argc, argv);
}
