#include "HistogramWidget.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>

HistogramWidget::HistogramWidget() {
    set_draw_func(sigc::mem_fun(*this, &HistogramWidget::on_draw));
}

HistogramWidget::~HistogramWidget() {
}

void HistogramWidget::set_data(const std::vector<double>& data) {
    m_data = data;
    queue_draw(); // Schedule a redraw
}

void HistogramWidget::save_as_csv(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "Index,Magnitude\n";
    for (size_t i = 0; i < m_data.size(); ++i) {
        file << i << "," << m_data[i] << "\n";
    }
    file.close();
}

void HistogramWidget::save_as_png(const std::string& filename) {
    int width = get_width();
    int height = get_height();
    
    if (width <= 0 || height <= 0) {
        // Fallback if widget is not realized or hidden
        width = 800;
        height = 600;
    }

    auto surface = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, width, height);
    auto cr = Cairo::Context::create(surface);

    // Reuse the drawing logic
    on_draw(cr, width, height);

    surface->write_to_png(filename);
}

void HistogramWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    // Clear background
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();

    if (m_data.empty()) {
        return;
    }

    // Margins
    const double margin_left = 50.0;
    const double margin_bottom = 40.0;
    const double margin_top = 20.0;
    const double margin_right = 20.0;

    const double plot_width = width - margin_left - margin_right;
    const double plot_height = height - margin_top - margin_bottom;

    // Draw Axes
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->set_line_width(2.0);

    // Y Axis
    cr->move_to(margin_left, margin_top);
    cr->line_to(margin_left, height - margin_bottom);
    
    // X Axis
    cr->move_to(margin_left, height - margin_bottom);
    cr->line_to(width - margin_right, height - margin_bottom);
    
    cr->stroke();

    // Find max value for scaling
    double max_val = 0.0;
    for (double val : m_data) {
        if (val > max_val) max_val = val;
    }

    if (max_val <= 0.0) max_val = 1.0; // Avoid division by zero

    // Draw bars
    // We limit the number of bars we draw if there are too many, or Cairo gets slow/messy
    // But for < 2000 points usually fine.
    
    double bar_width = plot_width / m_data.size();

    cr->set_source_rgb(0.2, 0.4, 0.8); // Blue color
    cr->set_line_width(1.0);

    for (size_t i = 0; i < m_data.size(); ++i) {
        double normalized_height = m_data[i] / max_val;
        double bar_height = normalized_height * plot_height;

        double x = margin_left + i * bar_width;
        double y = (height - margin_bottom) - bar_height;

        cr->rectangle(x, y, bar_width, bar_height);
        cr->fill();
    }
    
    // Draw Labels (Simple implementation)
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->select_font_face("Sans", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::NORMAL);
    cr->set_font_size(10.0);

    // X Axis Labels (Indices)
    int num_ticks_x = 5;
    for(int i = 0; i <= num_ticks_x; ++i) {
        double fraction = (double)i / num_ticks_x;
        double x = margin_left + fraction * plot_width;
        double y = height - margin_bottom + 15.0;
        
        size_t index = (size_t)(fraction * (m_data.size() - 1));
        std::string label = std::to_string(index);
        
        Cairo::TextExtents extents;
        cr->get_text_extents(label, extents);
        cr->move_to(x - extents.width/2, y);
        cr->show_text(label);
    }
    
    // Y Axis Labels (Magnitude)
    int num_ticks_y = 5;
    for(int i = 0; i <= num_ticks_y; ++i) {
        double fraction = (double)i / num_ticks_y;
        double val = fraction * max_val;
        double y = (height - margin_bottom) - (fraction * plot_height);
        double x = margin_left - 5.0;
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << val;
        std::string label = ss.str();
        
        Cairo::TextExtents extents;
        cr->get_text_extents(label, extents);
        cr->move_to(x - extents.width - 2, y + extents.height/2);
        cr->show_text(label);
    }
}
