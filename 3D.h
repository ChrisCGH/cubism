#ifndef _3D_H
#define _3D_H

#include <vector>
#include <map>
#include <algorithm>

struct Point3D
{
    Point3D() : x_(0), y_(0), z_(0) {}
    Point3D(double x, double y, double z) : x_(x), y_(y), z_(z) {}
    friend Point3D operator+(const Point3D& p, const Point3D& q)
    {
        Point3D r(p);
        r.x_ += q.x_;
        r.y_ += q.y_;
        r.z_ += q.z_;
        return r;
    }
    friend Point3D operator-(const Point3D& p, const Point3D& q)
    {
        Point3D r(p);
        r.x_ -= q.x_;
        r.y_ -= q.y_;
        r.z_ -= q.z_;
        return r;
    }
    Point3D rotate_about_x_axis(double theta) const
    {
        Point3D result(*this);
        // Rotate theta about x axis anti-clockwise
        // x -> x
        // y -> y cos(theta) - z sin(theta)
        // z -> z cos(theta) + y sin(theta)
        /*
         /y\ -> / cos(theta) -sin(theta) \ /y\
         \z/    \ sin(theta)  cos(theta> / \z/
         */
        double c = std::cos(theta);
        double s = std::sin(theta);
        result.y_ = c * y_ - s * z_;
        result.z_ = s * y_ + c * z_;
        return result;
    }

    Point3D rotate_about_x_axis(double theta, const Point3D& origin) const
    {
        Point3D new_point = *this - origin;
        new_point = new_point.rotate_about_x_axis(theta);
        Point3D result = new_point + origin;
        return result;
    }

    Point3D rotate_about_y_axis(double theta) const
    {
        Point3D result(*this);
        // Rotate theta about y axis anti-clockwise
        // x -> x cos(theta) + z sin(theta)
        // y -> y
        // z -> z cos(theta) - x sin(theta)
        /*
         /z\ -> / cos(theta) -sin(theta) \ /z\
         \x/    \ sin(theta)  cos(theta> / \x/
         */
        double c = std::cos(theta);
        double s = std::sin(theta);
        result.z_ = c * z_ - s * x_;
        result.x_ = s * z_ + c * x_;
        return result;
    }

    Point3D rotate_about_y_axis(double theta, const Point3D& origin) const
    {
        Point3D new_point = *this - origin;
        new_point = new_point.rotate_about_y_axis(theta);
        Point3D result = new_point + origin;
        return result;
    }

    Point3D rotate_about_z_axis(double theta) const
    {
        Point3D result(*this);
        // Rotate theta about z axis anti-clockwise
        // x -> x cos(theta) - y sin(theta)
        // y -> y cos(theta) + x sin(theta)
        // z -> z
        /*
         /x\ -> / cos(theta) -sin(theta) \ /x\
         \y/    \ sin(theta)  cos(theta> / \y/
         */
        double c = std::cos(theta);
        double s = std::sin(theta);
        result.x_ = c * x_ - s * y_;
        result.y_ = s * x_ + c * y_;
        return result;
    }

    Point3D rotate_about_z_axis(double theta, const Point3D& origin) const
    {
        Point3D new_point = *this - origin;
        new_point = new_point.rotate_about_z_axis(theta);
        Point3D result = new_point + origin;
        return result;
    }
    double x_;
    double y_;
    double z_;
};

class Polygon3D
{
    public: 
        Polygon3D(const std::vector<Point3D>& points, int colour)
            : points_(points), colour_(colour), min_z_set_(false) {}
        Polygon3D(int n, const Point3D* points, int colour)
            : colour_(colour), min_z_set_(false)
        {
            for (int i = 0; i < n; ++i)
            {
                points_.push_back(points[i]);
            }
        }
        int colour() const { return colour_; }
        const std::vector<Point3D>& points() const { return points_; }
        int min_z() const 
        { 
            if (!min_z_set_)
            {
                min_z_ = 100000000;
                for (auto& point : points_)
                {
                    if (point.z_ < min_z_)
                    {
                        min_z_ = point.z_;
                    }
                }
                min_z_set_ = true;
            }
            return min_z_; 
        }

    private:
        std::vector<Point3D> points_;
        int colour_;
        mutable bool min_z_set_;
        mutable int min_z_;
};

typedef std::vector<Polygon3D> polygon_list_type;

class PolygonList
{
    public:
        PolygonList() {}
        void add(const Polygon3D& polygon)
        {
            polygon_list_.push_back(polygon);
            min_z_list_.push_back(std::make_pair<int, int>(polygon.min_z(), polygon_list_.size() - 1));
        }
        polygon_list_type get_sorted_list() const
        {
            std::sort(min_z_list_.begin(), min_z_list_.end());
            polygon_list_type sorted_polygon_list;
            for (std::vector<std::pair<int, int> >::const_reverse_iterator rit = min_z_list_.rbegin();
                 rit != min_z_list_.rend();
                 ++rit)
            {
                sorted_polygon_list.push_back(polygon_list_[rit->second]); 
            }
            return sorted_polygon_list;
        }

    private:
        std::vector<Polygon3D> polygon_list_;
        mutable std::vector<std::pair<int, int> > min_z_list_;
};

class Shape3D
{
    public:
        Shape3D() {}
        virtual ~Shape3D() {}
        virtual polygon_list_type get_polygon_list() const = 0;
        virtual void rotate_about_x_axis(double theta) = 0;
        virtual void rotate_about_x_axis(double theta, const Point3D& origin) = 0;
        virtual void rotate_about_y_axis(double theta) = 0;
        virtual void rotate_about_y_axis(double theta, const Point3D& origin) = 0;
        virtual void rotate_about_z_axis(double theta) = 0;
        virtual void rotate_about_z_axis(double theta, const Point3D& origin) = 0;
        virtual void move(const Point3D& v) = 0;
        virtual Shape3D* clone() const = 0;

        static int add_to_polygon_list(int n, const Point3D* points, std::vector<std::vector<Point3D> >& polygon_list)
        {
            int min_z = 10000000;
            std::vector<Point3D> polygon;
            for (int i = 0; i < n; ++i)
            {
                polygon.push_back(points[i]);
                if (points[i].z_ < min_z)
                {
                    min_z = points[i].z_;
                }
            }
            polygon_list.push_back(polygon);
            return min_z;
        }
};

class Object3D
{
    public:
        Object3D() {}
        ~Object3D()
        {
            for (auto& shape : shape_list_)
            {
                delete shape;
            }
            shape_list_.clear();
        }
        polygon_list_type get_polygon_list() const
        {
            PolygonList polygon_list;
            for (auto& shape : shape_list_)
            {
                polygon_list_type shape_polygon_list = (shape)->get_polygon_list();
                for (auto& polygon : shape_polygon_list)
                {
                    polygon_list.add(polygon);
                }
            }
            return polygon_list.get_sorted_list();
        }
        void rotate_about_x_axis(double theta)
        {
            for (auto& shape : shape_list_)
            {
                shape->rotate_about_x_axis(theta);
            }
        }

        void rotate_about_x_axis(double theta, const Point3D& origin) 
        {
            for (auto& shape : shape_list_)
            {
                shape->rotate_about_x_axis(theta, origin);
            }
        }

        void rotate_about_x_axis(int id, double theta, const Point3D& origin) 
        {
            shape_map_[id]->rotate_about_x_axis(theta, origin);
        }

        void rotate_about_y_axis(double theta)
        {
            for (auto& shape : shape_list_)
            {
                shape->rotate_about_y_axis(theta);
            }
        }

        void rotate_about_y_axis(double theta, const Point3D& origin) 
        {
            for (auto& shape : shape_list_)
            {
                shape->rotate_about_y_axis(theta, origin);
            }
        }

        void rotate_about_y_axis(int id, double theta, const Point3D& origin) 
        {
            shape_map_[id]->rotate_about_y_axis(theta, origin);
        }

        void rotate_about_z_axis(double theta)
        {
            for (auto& shape : shape_list_)
            {
                shape->rotate_about_z_axis(theta);
            }
        }

        void rotate_about_z_axis(double theta, const Point3D& origin) 
        {
            for (auto& shape : shape_list_)
            {
                shape->rotate_about_z_axis(theta, origin);
            }
        }

        void rotate_about_z_axis(int id, double theta, const Point3D& origin) 
        {
            shape_map_[id]->rotate_about_z_axis(theta, origin);
        }

        void move(const Point3D& v)
        {
            for (auto& shape : shape_list_)
            {
                shape->move(v);
            }
        }
        void add_shape(const Shape3D& shape)
        {
            shape_list_.push_back(shape.clone());
            shape_map_.insert(std::map<int, Shape3D*>::value_type(shape_list_.size() - 1, shape_list_[shape_list_.size() - 1]));
        }
        void add_shape(const Shape3D& shape, int id)
        {
            shape_list_.push_back(shape.clone());
            shape_map_.insert(std::map<int, Shape3D*>::value_type(id, shape_list_[shape_list_.size() - 1]));
        }
       
    private:
        std::vector<Shape3D*> shape_list_;
        std::map<int, Shape3D*> shape_map_;
};

class Cube3D : public Shape3D
{
    public:
        Cube3D() {}
        Cube3D(const Point3D& centre, int edge)
            : centre_(centre)
        {
            /*

                         TopBack
                            |
                            v
                            x
              TopLeft-> x       x <-TopRight
                            x <-----TopFront
                            .
           BottomLeft-> x       x <-BottomRight
                            x
                            ^
                            |
                       BottomFront

             */
            corners_[Cube::TopLeft] = centre + Point3D(-edge / 2, edge / 2, -edge / 2);
            corners_[Cube::TopBack] = centre + Point3D(-edge / 2, edge / 2, edge / 2);
            corners_[Cube::TopRight] = centre + Point3D(edge / 2, edge / 2, edge / 2); 
            corners_[Cube::TopFront] = centre + Point3D(edge / 2, edge / 2, -edge / 2);
            corners_[Cube::BottomLeft] = centre + Point3D(-edge / 2, -edge / 2, -edge / 2);
            corners_[Cube::BottomBack] = centre + Point3D(-edge / 2, -edge / 2, edge / 2);
            corners_[Cube::BottomRight] = centre + Point3D(edge / 2, -edge / 2, edge / 2);
            corners_[Cube::BottomFront] = centre + Point3D(edge / 2, -edge / 2, -edge / 2);
            for (int i = 0; i < Cube::NumberOfFaces; ++i)
            {
                colours_[i] = i;
            }
            faces();
        }

        void set_face_colour(const Cube::Faces face, int colour)
        {
            colours_[face] = colour;
        }

        Shape3D* clone() const
        {
            return new Cube3D(*this);
        }

        void move(const Point3D& v)
        {
            centre_ = centre_ + v;
            for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
            {
                corners_[i] = corners_[i] + v;
            }
            faces();
        }

        void rotate_about_x_axis(double theta)
        {
            for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
            {
                corners_[i] = (corners_[i] - centre_).rotate_about_x_axis(theta) + centre_;
            }
            faces();
        }

        void rotate_about_x_axis(double theta, const Point3D& origin)
        {
            centre_ = centre_.rotate_about_x_axis(theta, origin);
            for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
            {
                corners_[i] = corners_[i].rotate_about_x_axis(theta, origin);
            }
            faces();
        }
    
        void rotate_about_y_axis(double theta)
        {
            for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
            {
                corners_[i] = (corners_[i] - centre_).rotate_about_y_axis(theta) + centre_;
            }
            faces();
        }
    
        void rotate_about_y_axis(double theta, const Point3D& origin)
        {
            centre_ = centre_.rotate_about_y_axis(theta, origin);
            for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
            {
                corners_[i] = corners_[i].rotate_about_y_axis(theta, origin);
            }
            faces();
        }
    
        void rotate_about_z_axis(double theta)
        {
            for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
            {
                corners_[i] = (corners_[i] - centre_).rotate_about_z_axis(theta) + centre_;
            }
            faces();
        }
    
        void rotate_about_z_axis(double theta, const Point3D& origin)
        {
            centre_ = centre_.rotate_about_z_axis(theta, origin);
            for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
            {
                corners_[i] = corners_[i].rotate_about_z_axis(theta, origin);
            }
            faces();
        }
    
        polygon_list_type get_polygon_list() const
        {
            PolygonList polygon_list;
            for (int i = 0; i < Cube::NumberOfFaces; ++i)
            {
                polygon_list.add(Polygon3D(4, faces_[i], colours_[i]));
            }
            return polygon_list.get_sorted_list();
        }

    private:
        void faces()
        {
            // Mapping between faces and corners of face
            static Cube::CornerPieces face_corner_mapping[Cube::NumberOfFaces][4] = 
            {
                { Cube::TopLeft, Cube::TopBack, Cube::TopRight, Cube::TopFront },
                { Cube::BottomRight, Cube::BottomBack, Cube::BottomLeft, Cube::BottomFront },
                { Cube::TopFront, Cube::TopRight, Cube::BottomRight, Cube::BottomFront },
                { Cube::TopBack, Cube::TopRight, Cube::BottomRight, Cube::BottomBack },
                { Cube::TopLeft, Cube::TopFront, Cube::BottomFront, Cube::BottomLeft },
                { Cube::TopLeft, Cube::TopBack, Cube::BottomBack, Cube::BottomLeft }
            };

            for (int i = Cube::TopFace; i < Cube::NumberOfFaces; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    faces_[i][j] = corners_[face_corner_mapping[i][j]];
                }
            }
        }

        Point3D centre_;
        Point3D corners_[Cube::NumberOfCornerPieces];
        Point3D faces_[Cube::NumberOfFaces][4];
        int colours_[Cube::NumberOfFaces];
};

#endif
