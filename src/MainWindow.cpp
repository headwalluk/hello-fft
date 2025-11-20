#include "MainWindow.h"
#include "CsvReader.h"
#include "FftProcessor.h"
#include <iostream>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>

MainWindow::MainWindow()
    : m_box(Gtk::Orientation::VERTICAL),
      m_button_box(Gtk::Orientation::HORIZONTAL),
      m_open_button("Open CSV"),
      m_export_box(Gtk::Orientation::VERTICAL),
      m_export_csv_button("Export CSV"),
      m_export_png_button("Export PNG"),
      m_exit_button("Exit") {

    set_title("Hello FFT");
    set_default_size(800, 600);

    // Layout
    set_child(m_box);
    m_box.append(m_button_box);
    m_box.append(m_histogram_widget);

    // Histogram expands to fill space
    m_histogram_widget.set_expand(true);

    // Buttons
    m_button_box.append(m_open_button);
    
    // Export Menu Button
    m_export_menu_button.set_label("Export");
    m_button_box.append(m_export_menu_button);
    
    // Exit button on the far right
    m_exit_button.set_hexpand(true);
    m_exit_button.set_halign(Gtk::Align::END);
    m_button_box.append(m_exit_button);
    
    // Export Popover
    m_export_popover.set_child(m_export_box);
    m_export_box.append(m_export_csv_button);
    m_export_box.append(m_export_png_button);
    m_export_menu_button.set_popover(m_export_popover);

    // Signals
    m_open_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_open_button_clicked));
    m_export_csv_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_export_csv_clicked));
    m_export_png_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_export_png_clicked));
    m_exit_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_exit_button_clicked));
}

MainWindow::~MainWindow() {
}

void MainWindow::on_open_button_clicked() {
    auto dialog = new Gtk::FileChooserDialog("Please choose a CSV file", Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::ACCEPT);

    // Filter for CSV
    auto filter_csv = Gtk::FileFilter::create();
    filter_csv->set_name("CSV Files");
    filter_csv->add_mime_type("text/csv");
    filter_csv->add_pattern("*.csv");
    dialog->add_filter(filter_csv);

    dialog->signal_response().connect([this, dialog](int response_id) {
        if (response_id == Gtk::ResponseType::ACCEPT) {
            std::string filepath = dialog->get_file()->get_path();
            try {
                auto timeData = CsvReader::read(filepath);
                auto freqData = FftProcessor::computeFft(timeData);
                m_histogram_widget.set_data(freqData);
            } catch (const std::exception& e) {
                 auto err_dialog = new Gtk::MessageDialog(*this, "Error parsing file", false, Gtk::MessageType::ERROR, Gtk::ButtonsType::OK, true);
                 err_dialog->set_secondary_text(e.what());
                 err_dialog->signal_response().connect([err_dialog](int){
                     delete err_dialog;
                 });
                 err_dialog->show();
            }
        }
        delete dialog;
    });

    dialog->show();
}

void MainWindow::on_export_csv_clicked() {
    m_export_popover.popdown();
    save_file("Export CSV", "CSV Files", "*.csv", true);
}

void MainWindow::on_export_png_clicked() {
    m_export_popover.popdown();
    save_file("Export PNG", "PNG Images", "*.png", false);
}

void MainWindow::save_file(const std::string& title, const std::string& filter_name, const std::string& pattern, bool is_csv) {
    auto dialog = new Gtk::FileChooserDialog(title, Gtk::FileChooser::Action::SAVE);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Save", Gtk::ResponseType::ACCEPT);

    auto filter = Gtk::FileFilter::create();
    filter->set_name(filter_name);
    filter->add_pattern(pattern);
    dialog->add_filter(filter);
    
    dialog->set_current_name(is_csv ? "histogram.csv" : "histogram.png");

    dialog->signal_response().connect([this, dialog, is_csv](int response_id) {
        if (response_id == Gtk::ResponseType::ACCEPT) {
            std::string filepath = dialog->get_file()->get_path();
            try {
                if (is_csv) {
                    m_histogram_widget.save_as_csv(filepath);
                } else {
                    m_histogram_widget.save_as_png(filepath);
                }
            } catch (const std::exception& e) {
                 auto err_dialog = new Gtk::MessageDialog(*this, "Error saving file", false, Gtk::MessageType::ERROR, Gtk::ButtonsType::OK, true);
                 err_dialog->set_secondary_text(e.what());
                 err_dialog->signal_response().connect([err_dialog](int){
                     delete err_dialog;
                 });
                 err_dialog->show();
            }
        }
        delete dialog;
    });

    dialog->show();
}

void MainWindow::on_exit_button_clicked() {
    close();
}
