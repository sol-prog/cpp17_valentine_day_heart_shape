// Draw a Valentine's Day heart shape
// ===================================

// Equation from http://mathworld.wolfram.com/HeartCurve.html
// x(t) = 16 * sin(t) ^ 3
// y(t) = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cost(4 * t)
// where t : [0, 2 * PI]

// Build examples:
// clang++ -std=c++17 -Wall -pedantic -O2 -march=native heart.cpp -o heart
// g++ -std=c++17 -Wall -pedantic -O2 -march=native heart.cpp -o heart
// cl /std:c++17 /EHsc /W4 /permissive- /O2 heart.cpp /link /out:heart.exe

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <tuple>

std::tuple<std::vector<double>, std::vector<double>> generate_points(const int no_pieces) {
    std::vector<double> vx, vy;

    constexpr double PI = 3.14159265358979323846264338327950288;
    double dt = 2.0 * PI / no_pieces;

    for(double t = 0.0; t <= 2.0 * PI ; t += dt) {
        vx.emplace_back(16.0 * sin(t) * sin(t) * sin(t));
        vy.emplace_back(13.0 * cos(t) - 5.0 * cos(2.0 * t) - 2.0 * cos(3.0 * t) - cos(4.0 * t));
    }

    return std::make_tuple(vx, vy);
}

void write_html_svg(std::ostream &out, const std::vector<double> &vx, const std::vector<double> &vy, const int width, const int height) {
    // Get the heart bounding box
    const auto [xmin, xmax] = std::minmax_element(std::begin(vx), std::end(vx));
    const auto [ymin, ymax] = std::minmax_element(std::begin(vy), std::end(vy));

    // HTML boilerplate
    out << "<!DOCTYPE html>\n" << "<html>\n" << "<body>\n\n";

    // Write the data in SVG format:
    // define a width x height area  and zoom in the data points using the data bounding box to fill the SVG vewBox
    out << "<svg height=\"" << height << "\" width=\"" << width << "\" viewBox=\"";

    // Use the "offset" variable to add some extra space around the shape
    const double offset = 5.0;

    out << *xmin - offset << ' ' << *ymin - offset << ' ' << (*xmax - *xmin) + offset << ' ' << (*ymax - *ymin) + offset;
    out << "\">\n";

    // Write the points in pairs and draw a SVG polyline
    out << "<polyline points=\"";

    for(size_t i = 0, no_points = vx.size(); i < no_points; ++i) {
        out << vx[i] << ',' << vy[i] << ' ';
    }

    // fill the polyline with red
    out << "\" style=\"fill:red;stroke:none;\" />\n";

    out << "</svg>\n";

    // HTML boilerplate
    out << "\n</body>\n" << "</html>\n";
}

int main() {
    // Generate the heart curve points
    int no_pieces = 300;
    auto [vx, vy] = generate_points(no_pieces);

    // Reverse Y axis direction
    std::for_each(std::begin(vy), std::end(vy), [](double &y){ y = -y; });

    // Write the output to stdout
    write_html_svg(std::cout, vx, vy, 500, 500);
}
