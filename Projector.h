#ifndef _PROJECTOR_H
#define _PROJECTOR_H
#include <vector>
#include "wx/wx.h"
class Projector
{
    public:
        Projector(int screen_width, int screen_height, int Z = 2000)
            : Z_(Z), screen_width_(screen_width), screen_height_(screen_height) {}
        ~Projector() {}

        void project(double x, double y, int z, int& px, int& py) const
        {
            px = 0;
            py = 0;
            double u = static_cast<double>(Z_) / static_cast<double>(Z_ + z);
            // Coordinates in projected_polygon are (X, Y) assuming (0, 0) is centre of the screen
            // and Y coordinate increases vertically up.
            // Screen coordinates (x, y) have origin is at top left corner of screen and y increases
            // vertically down.
            // To convert to screen coordinates:
            // (X, Y) = (X, Y) -> (screen_width / 2 + X, screen_height / 2 - Y);
            px = screen_width_ / 2 + static_cast<int>(u * x);
            py = screen_height_ / 2 - static_cast<int>(u * y);
        }
        void project(const Point3D& p, wxPoint& pp) const
        {
            project(p.x_, p.y_, p.z_, pp.x, pp.y);
        }
        void project(const Point3D* p_ptr, wxPoint* pp_ptr, int count) const
        {
            for (int i = 0; i < count; ++i)
            {
                project(*(p_ptr + i), *(pp_ptr + i));
            }
        }
        void project(const std::vector<Point3D>& points, std::vector<wxPoint>& wx_points) const
        {
            wx_points.clear();
            for (auto& point : points)
            {
                wxPoint wx_point;
                project(point, wx_point);
                wx_points.push_back(wx_point);
            }
        }

    private:
        int Z_;
        int screen_width_;
        int screen_height_;
};
#endif
