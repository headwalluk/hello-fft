#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/popover.h>
#include <gtkmm/filechooserdialog.h>
#include "HistogramWidget.h"

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();
    virtual ~MainWindow();

protected:
    // Signals
    void on_open_button_clicked();
    void on_export_csv_clicked();
    void on_export_png_clicked();

    // Widgets
    Gtk::Box m_box;
    Gtk::Box m_button_box;
    Gtk::Button m_open_button;
    
    // Menu Button approach
    Gtk::MenuButton m_export_menu_button;
    Gtk::Popover m_export_popover;
    Gtk::Box m_export_box;
    Gtk::Button m_export_csv_button;
    Gtk::Button m_export_png_button;

    HistogramWidget m_histogram_widget;
    
    // Helper to save file
    void save_file(const std::string& title, const std::string& filter_name, const std::string& pattern, bool is_csv);
};
