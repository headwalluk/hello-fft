#pragma once

#include <gtkmm/drawingarea.h>
#include <vector>
#include <string>

class HistogramWidget : public Gtk::DrawingArea {
public:
    HistogramWidget();
    virtual ~HistogramWidget();

    void set_data(const std::vector<double>& data);
    
    // Export functions
    void save_as_csv(const std::string& filename);
    void save_as_png(const std::string& filename);

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

    std::vector<double> m_data;
};
