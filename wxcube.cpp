#define _USE_MATH_DEFINES
#include <cmath>
#include "wx/wxprec.h"
#include "wx/wx.h"
#include "wx/button.h"
#include "wx/graphics.h"
#include "cube.h"
#include "3D.h"
#include "Projector.h"
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <typeinfo>

class CubeApp : public wxApp
{
    public:
        virtual bool OnInit();

        virtual int OnExit()
        {
            return 0;
        }

    protected:
};

class CubeCanvas;

class CubeFrame : public wxFrame
{
    public:
        CubeFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

        // event handlers (these functions should _not_ be virtual)
        void OnQuit(wxCommandEvent& event);

        CubeCanvas* canvas_;
        Cube cube_;

    private:
        DECLARE_EVENT_TABLE()

};

class CubeCanvas : public wxPanel
{
    public:
        CubeCanvas(CubeFrame* parent, int screen_width, int screen_height);
        ~CubeCanvas();

        void OnPaint(wxPaintEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnLeftButtonDClick(wxMouseEvent& event);
        void OnButtonRandomize(wxCommandEvent& event);
        void OnButtonSolve(wxCommandEvent& event);
        void OnButtonUndo(wxCommandEvent& event);
        void OnButtonTest(wxCommandEvent& event);
        void OnButtonSwapCorners(wxCommandEvent& event);
        void OnButtonFlipEdges(wxCommandEvent& event);
        void OnButtonRotateThreeEdges(wxCommandEvent& event);
    private:
        enum directions { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };
        void draw_cube(wxDC& dc);
        void draw_shape3D(wxDC& dc, const Projector& projector, const Shape3D& shape, const wxPoint& origin);
        void draw_object3D(wxDC& dc, const Projector& projector, const Object3D& object, const wxPoint& origin);
        void perform_move(const Cube::Move& move, bool animate = true);
        Cube::move_sequence_type move_history_;

        void perform_move(const Cube::Axis axis, const Cube::Slice slice, const Cube::Rotation rotation, int line_number);
        void perform_move(const Cube::Axis axis, const Cube::Rotation rotation, int line_number);
        typedef std::map<Cube::Cells, wxPoint> face_cell_map_type;
        double get_move_angle(const wxPoint& p1, const wxPoint& p2);
        directions get_direction(const Cube::Faces& face, double theta);
        void move_cube(const Cube::Faces& face, const Cube::Cells& cell, const wxPoint& p1, const wxPoint& p2);
        void move_cube(const Cube::Faces& face, const Cube::Cells& cell, directions direction);
        void log(const std::string& msg);
        std::ofstream* log_;

        class CubeModel
        {
            public:
                CubeModel(const wxPoint& origin, int unit = 50);
                bool right_of_top_face(const wxPoint& point);
                bool above_top_face(const wxPoint& point);
                bool left_of_left_face(const wxPoint& point);
                bool right_of_right_face(const wxPoint& point);
                bool below_left_face(const wxPoint& point);
                bool below_right_face(const wxPoint& point);
                bool get_cell(const wxPoint& point, Cube::Faces& face, Cube::Cells& cell);
                void draw_cube(wxDC& dc, wxBrush** brushes, const Cube& cube);
            private:
                face_cell_map_type top_face_cell_map_;
                face_cell_map_type left_face_cell_map_;
                face_cell_map_type right_face_cell_map_;
                wxPoint top_diamond_[4];
                wxPoint left_side_diamond_[4];
                wxPoint right_side_diamond_[4];
                wxPoint origin_;
                int a_;
                int b_;
                bool inside_top_face(const wxPoint& point);
                bool inside_left_face(const wxPoint& point);
                bool inside_right_face(const wxPoint& point);
                bool inside_top_cell(const wxPoint& point, const wxPoint& cell_origin);
                bool inside_left_cell(const wxPoint& point, const wxPoint& cell_origin);
                bool inside_right_cell(const wxPoint& point, const wxPoint& cell_origin);
        };

        class Cube3DModel
        {
            // Build a cube out of 26 sub-cubes:
            // 8 corner cubes
            // 12 edge cubes
            // 6 centre cubes
            public:
                Cube3DModel(const Cube& cube, int edge = 140) : edge_(edge), cube_(cube)
                {
                    int gap = 2;
                    corner_cubes_[Cube::TopBack] = Cube3D(Point3D(-edge_, edge_, edge_), edge_-gap);
                    corner_cubes_[Cube::TopBack].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    corner_cubes_[Cube::TopBack].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    corner_cubes_[Cube::TopBack].set_face_colour(Cube::BottomFace, Cube::Black);

                    corner_cubes_[Cube::TopRight] = Cube3D(Point3D(edge_, edge_, edge_), edge_-gap);
                    corner_cubes_[Cube::TopRight].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    corner_cubes_[Cube::TopRight].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    corner_cubes_[Cube::TopRight].set_face_colour(Cube::BottomFace, Cube::Black);

                    corner_cubes_[Cube::TopFront] = Cube3D(Point3D(edge_, edge_, -edge_), edge_-gap);
                    corner_cubes_[Cube::TopFront].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    corner_cubes_[Cube::TopFront].set_face_colour(Cube::RightBackFace, Cube::Black);
                    corner_cubes_[Cube::TopFront].set_face_colour(Cube::BottomFace, Cube::Black);

                    corner_cubes_[Cube::TopLeft] = Cube3D(Point3D(-edge_, edge_, -edge_), edge_-gap);
                    corner_cubes_[Cube::TopLeft].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    corner_cubes_[Cube::TopLeft].set_face_colour(Cube::RightBackFace, Cube::Black);
                    corner_cubes_[Cube::TopLeft].set_face_colour(Cube::BottomFace, Cube::Black);

                    corner_cubes_[Cube::BottomBack] = Cube3D(Point3D(-edge_, -edge_, edge_), edge_-gap);
                    corner_cubes_[Cube::BottomBack].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    corner_cubes_[Cube::BottomBack].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    corner_cubes_[Cube::BottomBack].set_face_colour(Cube::TopFace, Cube::Black);

                    corner_cubes_[Cube::BottomRight] = Cube3D(Point3D(edge_, -edge_, edge_), edge_-gap);
                    corner_cubes_[Cube::BottomRight].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    corner_cubes_[Cube::BottomRight].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    corner_cubes_[Cube::BottomRight].set_face_colour(Cube::TopFace, Cube::Black);

                    corner_cubes_[Cube::BottomFront] = Cube3D(Point3D(edge_, -edge_, -edge_), edge_-gap);
                    corner_cubes_[Cube::BottomFront].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    corner_cubes_[Cube::BottomFront].set_face_colour(Cube::RightBackFace, Cube::Black);
                    corner_cubes_[Cube::BottomFront].set_face_colour(Cube::TopFace, Cube::Black);

                    corner_cubes_[Cube::BottomLeft] = Cube3D(Point3D(-edge_, -edge_, -edge_), edge_-gap);
                    corner_cubes_[Cube::BottomLeft].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    corner_cubes_[Cube::BottomLeft].set_face_colour(Cube::RightBackFace, Cube::Black);
                    corner_cubes_[Cube::BottomLeft].set_face_colour(Cube::TopFace, Cube::Black);

                    edge_cubes_[Cube::TopLeftFrontEdge] = Cube3D(Point3D(0, edge_, -edge_), edge_-gap);
                    edge_cubes_[Cube::TopLeftFrontEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::TopLeftFrontEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::TopLeftFrontEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::TopLeftFrontEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::TopRightFrontEdge] = Cube3D(Point3D(edge_, edge_, 0), edge_-gap);
                    edge_cubes_[Cube::TopRightFrontEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::TopRightFrontEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::TopRightFrontEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::TopRightFrontEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::TopLeftBackEdge] = Cube3D(Point3D(-edge_, edge_, 0), edge_-gap);
                    edge_cubes_[Cube::TopLeftBackEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::TopLeftBackEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::TopLeftBackEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::TopLeftBackEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::TopRightBackEdge] = Cube3D(Point3D(0, edge_, edge_), edge_-gap);
                    edge_cubes_[Cube::TopRightBackEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::TopRightBackEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::TopRightBackEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::TopRightBackEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::MiddleBackEdge] = Cube3D(Point3D(-edge_, 0, edge_), edge_-gap);
                    edge_cubes_[Cube::MiddleBackEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::MiddleBackEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::MiddleBackEdge].set_face_colour(Cube::TopFace, Cube::Black);
                    edge_cubes_[Cube::MiddleBackEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::MiddleRightEdge] = Cube3D(Point3D(edge_, 0, edge_), edge_-gap);
                    edge_cubes_[Cube::MiddleRightEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::MiddleRightEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::MiddleRightEdge].set_face_colour(Cube::TopFace, Cube::Black);
                    edge_cubes_[Cube::MiddleRightEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::MiddleFrontEdge] = Cube3D(Point3D(edge_, 0, -edge_), edge_-gap);
                    edge_cubes_[Cube::MiddleFrontEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::MiddleFrontEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::MiddleFrontEdge].set_face_colour(Cube::TopFace, Cube::Black);
                    edge_cubes_[Cube::MiddleFrontEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::MiddleLeftEdge] = Cube3D(Point3D(-edge_, 0, -edge_), edge_-gap);
                    edge_cubes_[Cube::MiddleLeftEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::MiddleLeftEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::MiddleLeftEdge].set_face_colour(Cube::TopFace, Cube::Black);
                    edge_cubes_[Cube::MiddleLeftEdge].set_face_colour(Cube::BottomFace, Cube::Black);

                    edge_cubes_[Cube::BottomLeftFrontEdge] = Cube3D(Point3D(0, -edge_, -edge_), edge_-gap);
                    edge_cubes_[Cube::BottomLeftFrontEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::BottomLeftFrontEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::BottomLeftFrontEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::BottomLeftFrontEdge].set_face_colour(Cube::TopFace, Cube::Black);

                    edge_cubes_[Cube::BottomRightFrontEdge] = Cube3D(Point3D(edge_, -edge_, 0), edge_-gap);
                    edge_cubes_[Cube::BottomRightFrontEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::BottomRightFrontEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::BottomRightFrontEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::BottomRightFrontEdge].set_face_colour(Cube::TopFace, Cube::Black);

                    edge_cubes_[Cube::BottomLeftBackEdge] = Cube3D(Point3D(-edge_, -edge_, 0), edge_-gap);
                    edge_cubes_[Cube::BottomLeftBackEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::BottomLeftBackEdge].set_face_colour(Cube::RightBackFace, Cube::Black);
                    edge_cubes_[Cube::BottomLeftBackEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::BottomLeftBackEdge].set_face_colour(Cube::TopFace, Cube::Black);

                    edge_cubes_[Cube::BottomRightBackEdge] = Cube3D(Point3D(0, -edge_, edge_), edge_-gap);
                    edge_cubes_[Cube::BottomRightBackEdge].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    edge_cubes_[Cube::BottomRightBackEdge].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    edge_cubes_[Cube::BottomRightBackEdge].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    edge_cubes_[Cube::BottomRightBackEdge].set_face_colour(Cube::TopFace, Cube::Black);

                    centre_cubes_[Cube::TopFace] = Cube3D(Point3D(0, edge_, 0), edge_-gap);
                    centre_cubes_[Cube::TopFace].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    centre_cubes_[Cube::TopFace].set_face_colour(Cube::RightBackFace, Cube::Black);
                    centre_cubes_[Cube::TopFace].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    centre_cubes_[Cube::TopFace].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    centre_cubes_[Cube::TopFace].set_face_colour(Cube::BottomFace, Cube::Black);

                    centre_cubes_[Cube::BottomFace] = Cube3D(Point3D(0, -edge_, 0), edge_-gap);
                    centre_cubes_[Cube::BottomFace].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    centre_cubes_[Cube::BottomFace].set_face_colour(Cube::RightBackFace, Cube::Black);
                    centre_cubes_[Cube::BottomFace].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    centre_cubes_[Cube::BottomFace].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    centre_cubes_[Cube::BottomFace].set_face_colour(Cube::TopFace, Cube::Black);

                    centre_cubes_[Cube::LeftFrontFace] = Cube3D(Point3D(0, 0, -edge_), edge_-gap);
                    centre_cubes_[Cube::LeftFrontFace].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    centre_cubes_[Cube::LeftFrontFace].set_face_colour(Cube::RightBackFace, Cube::Black);
                    centre_cubes_[Cube::LeftFrontFace].set_face_colour(Cube::BottomFace, Cube::Black);
                    centre_cubes_[Cube::LeftFrontFace].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    centre_cubes_[Cube::LeftFrontFace].set_face_colour(Cube::TopFace, Cube::Black);

                    centre_cubes_[Cube::RightFrontFace] = Cube3D(Point3D(edge_, 0, 0), edge_-gap);
                    centre_cubes_[Cube::RightFrontFace].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    centre_cubes_[Cube::RightFrontFace].set_face_colour(Cube::RightBackFace, Cube::Black);
                    centre_cubes_[Cube::RightFrontFace].set_face_colour(Cube::BottomFace, Cube::Black);
                    centre_cubes_[Cube::RightFrontFace].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    centre_cubes_[Cube::RightFrontFace].set_face_colour(Cube::TopFace, Cube::Black);

                    centre_cubes_[Cube::LeftBackFace] = Cube3D(Point3D(-edge_, 0, 0), edge_-gap);
                    centre_cubes_[Cube::LeftBackFace].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    centre_cubes_[Cube::LeftBackFace].set_face_colour(Cube::RightBackFace, Cube::Black);
                    centre_cubes_[Cube::LeftBackFace].set_face_colour(Cube::BottomFace, Cube::Black);
                    centre_cubes_[Cube::LeftBackFace].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    centre_cubes_[Cube::LeftBackFace].set_face_colour(Cube::TopFace, Cube::Black);

                    centre_cubes_[Cube::RightBackFace] = Cube3D(Point3D(0, 0, edge_), edge_-gap);
                    centre_cubes_[Cube::RightBackFace].set_face_colour(Cube::LeftFrontFace, Cube::Black);
                    centre_cubes_[Cube::RightBackFace].set_face_colour(Cube::LeftBackFace, Cube::Black);
                    centre_cubes_[Cube::RightBackFace].set_face_colour(Cube::BottomFace, Cube::Black);
                    centre_cubes_[Cube::RightBackFace].set_face_colour(Cube::RightFrontFace, Cube::Black);
                    centre_cubes_[Cube::RightBackFace].set_face_colour(Cube::TopFace, Cube::Black);

                    // Add cubes to big_cube_ and set up mapping between position in big_cube shape_ list and slices
                    for (int i = 0; i < Cube::NumberOfCornerPieces; ++i)
                    {
                        big_cube_.add_shape(corner_cubes_[i]);
                    }
                    for (int i = 0; i < Cube::NumberOfEdgePieces; ++i)
                    {
                        big_cube_.add_shape(edge_cubes_[i]);
                    }
                    for (int i = 0; i < Cube::NumberOfFaces; ++i)
                    {
                        big_cube_.add_shape(centre_cubes_[i]);
                    }
                    big_cube_.rotate_about_y_axis(M_PI/4.0, Point3D(0, 0, 0));
                    big_cube_.rotate_about_x_axis(-M_PI/5.0, Point3D(0, 0, 0));
                    big_cube_origin_ = Point3D(0, 58, 200);
                    big_cube_.move(big_cube_origin_);
                }

                const Object3D& get_object() const { return big_cube_; }
                void rotate_about_x_axis(double theta)
                {
                    big_cube_.rotate_about_x_axis(theta, big_cube_origin_);
                }
                void rotate_about_y_axis(double theta)
                {
                    big_cube_.rotate_about_y_axis(theta, big_cube_origin_);
                }
                void rotate_about_z_axis(double theta)
                {
                    big_cube_.rotate_about_z_axis(theta, big_cube_origin_);
                }
                void move_slice_by_angle(const Cube::Axis axis, const Cube::Slice slice, double angle_radians)
                {
                    // slice_map gives the mapping from axis/slice -> index into big_cube_ shape list
                    // Each shape in the list is a small cube
                    static int slice_map[Cube::NumberOfAxes][Cube::NumberOfSlices][10] = 
                    {
                        // TopBottom
                        {
                            // TopSlice
                            { 9, Cube::TopLeft, 
                                 Cube::TopBack, 
                                 Cube::TopRight, 
                                 Cube::TopFront, 
                                 Cube::NumberOfCornerPieces+Cube::TopRightFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::TopRightBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::TopLeftFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::TopLeftBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::TopFace },
                            // MiddleSlice
                            { 8, Cube::NumberOfCornerPieces+Cube::MiddleLeftEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleRightEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::RightFrontFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::RightBackFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::LeftFrontFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::LeftBackFace },
                            // BottomSlice
                            { 9, Cube::BottomLeft, 
                                 Cube::BottomBack, 
                                 Cube::BottomRight, 
                                 Cube::BottomFront, 
                                 Cube::NumberOfCornerPieces+Cube::BottomRightFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomRightBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomLeftFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomLeftBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::BottomFace }
                        },
                        // RightFrontLeftBack
                        {
                            // TopSlice
                            { 9, Cube::TopRight, 
                                 Cube::TopFront, 
                                 Cube::BottomRight, 
                                 Cube::BottomFront, 
                                 Cube::NumberOfCornerPieces+Cube::TopRightFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleRightEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomRightFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::RightFrontFace },
                            // MiddleSlice
                            { 8, Cube::NumberOfCornerPieces+Cube::TopRightBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::TopLeftFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomRightBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomLeftFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::TopFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::RightBackFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::LeftFrontFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::BottomFace },
                            // BottomSlice
                            { 9, Cube::TopLeft, 
                                 Cube::TopBack, 
                                 Cube::BottomLeft, 
                                 Cube::BottomBack, 
                                 Cube::NumberOfCornerPieces+Cube::TopLeftBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleLeftEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomLeftBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::LeftBackFace }
                        },
                        // LeftFrontRightBack
                        {
                            // TopSlice
                            { 9, Cube::TopLeft, 
                                 Cube::TopFront, 
                                 Cube::BottomLeft, 
                                 Cube::BottomFront, 
                                 Cube::NumberOfCornerPieces+Cube::TopLeftFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomLeftFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleLeftEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::LeftFrontFace },
                            // MiddleSlice
                            { 8, Cube::NumberOfCornerPieces+Cube::TopRightFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::TopLeftBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomRightFrontEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomLeftBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::TopFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::RightFrontFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::LeftBackFace, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::BottomFace },
                            // BottomSlice
                            { 9, Cube::TopBack, 
                                 Cube::TopRight, 
                                 Cube::BottomBack, 
                                 Cube::BottomRight, 
                                 Cube::NumberOfCornerPieces+Cube::TopRightBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleRightEdge, 
                                 Cube::NumberOfCornerPieces+Cube::MiddleBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::BottomRightBackEdge, 
                                 Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::RightBackFace }
                        }
                    };

                    switch (axis)
                    {
                        case Cube::TopBottom:
                        {
                            switch (slice)
                            {
                                case Cube::TopSlice:
                                {
                                    Point3D origin = big_cube_origin_ + Point3D(0, edge_, 0);
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_y_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                                case Cube::MiddleSlice:
                                {
                                    Point3D origin = big_cube_origin_;
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_y_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                                case Cube::BottomSlice:
                                {
                                    Point3D origin = big_cube_origin_ + Point3D(0, -edge_, 0);
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_y_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                            }
                        }
                        break;
                        case Cube::RightFrontLeftBack:
                        {
                            switch (slice)
                            {
                                case Cube::TopSlice:
                                {
                                    Point3D origin = big_cube_origin_ + Point3D(edge_, 0, 0);
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_x_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                                case Cube::MiddleSlice:
                                {
                                    Point3D origin = big_cube_origin_;
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_x_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                                case Cube::BottomSlice:
                                {
                                    Point3D origin = big_cube_origin_ + Point3D(-edge_, 0, 0);
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_x_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                            }
                        }
                        break;
                        case Cube::LeftFrontRightBack:
                        {
                            switch (slice)
                            {
                                case Cube::TopSlice:
                                {
                                    Point3D origin = big_cube_origin_ + Point3D(0, 0, -edge_);
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_z_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                                case Cube::MiddleSlice:
                                {
                                    Point3D origin = big_cube_origin_;
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_z_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                                case Cube::BottomSlice:
                                {
                                    Point3D origin = big_cube_origin_ + Point3D(0, 0, edge_);
                                    int count = slice_map[axis][slice][0];
                                    for (int i = 0; i < count; ++i)
                                    {
                                        int index = cube_.identify_piece(slice_map[axis][slice][i + 1]);
                                        big_cube_.rotate_about_z_axis(index, angle_radians, origin);
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                void move_slice(const Cube::Axis axis, const Cube::Slice slice, const Cube::Rotation rotation)
                {
                    move_slice_by_angle(axis, slice, slice_angle(axis, rotation));
                }
                void rotate_by_angle(const Cube::Axis axis, double angle)
                {
                    switch (axis)
                    {
                        case Cube::TopBottom:
                            big_cube_.rotate_about_y_axis(angle, big_cube_origin_);
                            break;
                        case Cube::RightFrontLeftBack:
                            big_cube_.rotate_about_x_axis(angle, big_cube_origin_);
                            break;
                        case Cube::LeftFrontRightBack:
                            big_cube_.rotate_about_z_axis(angle, big_cube_origin_);
                            break;
                    }
                }
                void rotate(const Cube::Axis axis, const Cube::Rotation rotation)
                {
                    rotate_by_angle(axis, slice_angle(axis, rotation));
                }

                void perform_move(const Cube::Move& move) 
                {
                    big_cube_.rotate_about_x_axis(VIEWING_ANGLE_X, big_cube_origin_);
                    big_cube_.rotate_about_y_axis(-VIEWING_ANGLE_Y, big_cube_origin_);
                    try
                    {
                        const Cube::SliceMove& slice_move = dynamic_cast<const Cube::SliceMove&>(move);
                        move_slice(slice_move.axis_, slice_move.slice_, slice_move.rotation_);
                    }
                    catch (const std::bad_cast& e)
                    {
                        try 
                        {
                            const Cube::RotateMove& rotate_move = dynamic_cast<const Cube::RotateMove&>(move);
                            rotate(rotate_move.axis_, rotate_move.rotation_);
                        }
                        catch (const std::bad_cast& e)
                        {
                        }
                    }
                    big_cube_.rotate_about_y_axis(VIEWING_ANGLE_Y, big_cube_origin_);
                    big_cube_.rotate_about_x_axis(-VIEWING_ANGLE_X, big_cube_origin_);
                }

                void perform_inverse_move(const Cube::Move& move) 
                {
                    big_cube_.rotate_about_x_axis(VIEWING_ANGLE_X, big_cube_origin_);
                    big_cube_.rotate_about_y_axis(-VIEWING_ANGLE_Y, big_cube_origin_);
                    try
                    {
                        const Cube::SliceMove& slice_move = dynamic_cast<const Cube::SliceMove&>(move);
                        Cube::SliceMove inverse_move(slice_move);
                        inverse_move.rotation_ = Cube::invert(inverse_move.rotation_);
                        move_slice(inverse_move.axis_, inverse_move.slice_, inverse_move.rotation_);
                    }
                    catch (const std::bad_cast& e)
                    {
                        try 
                        {
                            const Cube::RotateMove& rotate_move = dynamic_cast<const Cube::RotateMove&>(move);
                            Cube::RotateMove inverse_move(rotate_move);
                            inverse_move.rotation_ = Cube::invert(inverse_move.rotation_);
                            rotate(inverse_move.axis_, inverse_move.rotation_);
                        }
                        catch (const std::bad_cast& e)
                        {
                        }
                    }
                    big_cube_.rotate_about_y_axis(VIEWING_ANGLE_Y, big_cube_origin_);
                    big_cube_.rotate_about_x_axis(-VIEWING_ANGLE_X, big_cube_origin_);
                }

                void animate_move(const Cube::Move& move, wxWindow* refresh_window,
                                  Cube3DModel* secondary_model = nullptr,
                                  int n_steps = ANIMATION_STEPS,
                                  int step_delay_ms = ANIMATION_STEP_DELAY_MS)
                {
                    const Cube::SliceMove* slice_move = dynamic_cast<const Cube::SliceMove*>(&move);
                    const Cube::RotateMove* rotate_move = dynamic_cast<const Cube::RotateMove*>(&move);

                    if (!slice_move && !rotate_move)
                    {
                        perform_move(move);
                        if (secondary_model) secondary_model->perform_move(move);
                        return;
                    }

                    Cube::Axis axis = slice_move ? slice_move->axis_ : rotate_move->axis_;
                    Cube::Rotation rotation = slice_move ? slice_move->rotation_ : rotate_move->rotation_;
                    double step_angle = slice_angle(axis, rotation) / n_steps;

                    // Un-rotate the viewing angle so the slice/cube origins are canonical
                    unrotate_viewing_angle(*this);
                    if (secondary_model) unrotate_viewing_angle(*secondary_model);

                    for (int step = 0; step < n_steps; ++step)
                    {
                        if (slice_move)
                        {
                            move_slice_by_angle(slice_move->axis_, slice_move->slice_, step_angle);
                            if (secondary_model)
                                secondary_model->move_slice_by_angle(slice_move->axis_, slice_move->slice_, step_angle);
                        }
                        else
                        {
                            rotate_by_angle(rotate_move->axis_, step_angle);
                            if (secondary_model)
                                secondary_model->rotate_by_angle(rotate_move->axis_, step_angle);
                        }

                        // Re-apply viewing angle so the repaint shows the correct perspective
                        rerotate_viewing_angle(*this);
                        if (secondary_model) rerotate_viewing_angle(*secondary_model);

                        refresh_window->Refresh();
                        refresh_window->Update();
                        wxMilliSleep(step_delay_ms);

                        if (step < n_steps - 1)
                        {
                            // Un-rotate again for the next animation step
                            unrotate_viewing_angle(*this);
                            if (secondary_model) unrotate_viewing_angle(*secondary_model);
                        }
                    }
                    // After the final step both models are already in viewing orientation
                }

                void animate_inverse_move(const Cube::Move& move, wxWindow* refresh_window,
                                          Cube3DModel* secondary_model = nullptr,
                                          int n_steps = ANIMATION_STEPS,
                                          int step_delay_ms = ANIMATION_STEP_DELAY_MS)
                {
                    const Cube::SliceMove* slice_move = dynamic_cast<const Cube::SliceMove*>(&move);
                    const Cube::RotateMove* rotate_move = dynamic_cast<const Cube::RotateMove*>(&move);

                    if (slice_move)
                    {
                        Cube::SliceMove inverse(*slice_move);
                        inverse.rotation_ = Cube::invert(inverse.rotation_);
                        animate_move(inverse, refresh_window, secondary_model, n_steps, step_delay_ms);
                    }
                    else if (rotate_move)
                    {
                        Cube::RotateMove inverse(*rotate_move);
                        inverse.rotation_ = Cube::invert(inverse.rotation_);
                        animate_move(inverse, refresh_window, secondary_model, n_steps, step_delay_ms);
                    }
                    else
                    {
                        perform_inverse_move(move);
                        if (secondary_model) secondary_model->perform_inverse_move(move);
                    }
                }

                // Put model into canonical (un-viewed) space ready for incremental slice drags.
                void begin_slice_drag() { unrotate_viewing_angle(*this); }
                // Restore the viewing angle after incremental slice drag steps.
                void end_slice_drag()   { rerotate_viewing_angle(*this); }

                // Public accessor for the signed rotation angle (radians) for a given axis/rotation.
                static double get_slice_angle(const Cube::Axis axis, const Cube::Rotation rotation)
                {
                    return slice_angle(axis, rotation);
                }

            private:
                // Remove the viewing angle from a model so rotations act in canonical space.
                static void unrotate_viewing_angle(Cube3DModel& m)
                {
                    m.big_cube_.rotate_about_x_axis(VIEWING_ANGLE_X, m.big_cube_origin_);
                    m.big_cube_.rotate_about_y_axis(-VIEWING_ANGLE_Y, m.big_cube_origin_);
                }
                // Re-apply the viewing angle after a canonical-space rotation step.
                static void rerotate_viewing_angle(Cube3DModel& m)
                {
                    m.big_cube_.rotate_about_y_axis(VIEWING_ANGLE_Y, m.big_cube_origin_);
                    m.big_cube_.rotate_about_x_axis(-VIEWING_ANGLE_X, m.big_cube_origin_);
                }

                // Compute the signed rotation angle in radians for a given axis/rotation.
                // LeftFrontRightBack uses the opposite hand convention from the other two axes.
                // The result is normalized to (-π, π] so that animation always takes the
                // shortest angular path: e.g. ThreeQuarters becomes -π/2, not +3π/2.
                static double slice_angle(const Cube::Axis axis, const Cube::Rotation rotation)
                {
                    double angle = static_cast<int>(rotation) * M_PI / 2.0;
                    if (axis == Cube::LeftFrontRightBack) angle = -angle;
                    if (angle > M_PI)  angle -= 2.0 * M_PI;
                    else if (angle < -M_PI) angle += 2.0 * M_PI;
                    return angle;
                }

                static const int ANIMATION_STEPS = 10;         // sub-steps per quarter turn
                static const int ANIMATION_STEP_DELAY_MS = 10; // ms between steps (~100ms/quarter)
                static constexpr double VIEWING_ANGLE_X = M_PI / 5.0;
                static constexpr double VIEWING_ANGLE_Y = M_PI / 4.0;
                Cube3D corner_cubes_[Cube::NumberOfCornerPieces];
                Cube3D edge_cubes_[Cube::NumberOfEdgePieces];
                Cube3D centre_cubes_[Cube::NumberOfFaces];
                Object3D big_cube_;
                Point3D big_cube_origin_;
                int edge_;
                const Cube& cube_;
        };
        Cube3DModel cube3d_model_;
        // Cube viewed from each angle
        Cube3DModel cube3d_model_small_;
        int screen_width_;
        int screen_height_;
        CubeModel cube_model_;

        void get_slice_move(const Cube::Faces& face, const Cube::Cells& cell, directions direction,
                            Cube::Axis& axis, Cube::Slice& slice, Cube::Rotation& rotation);

        static constexpr double DRAG_LOCK_THRESHOLD_PX    = 5.0;   // pixels before direction is locked
        static constexpr double DRAG_PIXELS_PER_QTR_TURN  = 150.0; // pixels per 90° of slice rotation
        static constexpr double DRAG_SNAP_EPSILON          = 1e-9;  // treat snapped angle as zero below this
        static constexpr double DRAG_ANGLE_MATCH_TOL       = 0.1;   // tolerance for matching rotations (rad)

        wxBrush* brushes_[Cube::NumberOfColours];
        CubeFrame* parent_;
        wxPoint* start_drag_point_;
        wxPoint cube_drag_offset_;
        wxPoint last_mouse_pos_;
        bool is_translating_cube_;
        Cube::Faces drag_face_;
        Cube::Cells drag_cell_;
        bool drag_axis_locked_;
        Cube::Axis drag_axis_;
        Cube::Slice drag_slice_;
        directions drag_locked_direction_;
        double drag_angle_per_pixel_;
        double drag_current_angle_;
        bool drag_is_whole_cube_;
        wxButton* randomize_button_;
        wxButton* solve_button_;
        wxButton* undo_button_;
        wxButton* test_button_;
        wxButton* swap_corners_button_;
        wxButton* flip_edges_button_;
        wxButton* rotate_three_edges_button_;
        DECLARE_EVENT_TABLE()
};

enum
{
    File_Quit = wxID_EXIT
};

CubeFrame::CubeFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(0, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(File_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _T("&File"));

    SetMenuBar(menuBar);

    wxStatusBar* statusBar = CreateStatusBar();
    SetStatusBar(statusBar);

    canvas_ = new CubeCanvas(this, size.x, size.y);
}

void CubeFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

BEGIN_EVENT_TABLE(CubeFrame, wxFrame)
    EVT_MENU      (File_Quit,     CubeFrame::OnQuit)
END_EVENT_TABLE()

// control ids
enum
{
    Button_Solve = wxID_HIGHEST,
    Button_Randomize,
    Button_Undo,
    Button_Test,
    Button_Swap_Corners,
    Button_Flip_Edges,
    Button_Rotate_Three_Edges
};

CubeCanvas::CubeModel::CubeModel(const wxPoint& origin, int unit)
    : a_(unit), b_(static_cast<int>(static_cast<double>(a_) * std::sqrt(3.0))), origin_(origin)
{
    top_diamond_[0] = wxPoint(0, 0);
    top_diamond_[1] = wxPoint(b_, a_);
    top_diamond_[2] = wxPoint(0, 2*a_);
    top_diamond_[3] = wxPoint(-b_, a_);

    left_side_diamond_[0] = wxPoint(0, 0);
    left_side_diamond_[1] = wxPoint(b_, a_);
    left_side_diamond_[2] = wxPoint(b_, 3*a_);
    left_side_diamond_[3] = wxPoint(0, 2*a_);

    right_side_diamond_[0] = wxPoint(0, 0);
    right_side_diamond_[1] = wxPoint(b_, -a_);
    right_side_diamond_[2] = wxPoint(b_, a_);
    right_side_diamond_[3] = wxPoint(0, 2*a_);

    top_face_cell_map_[Cube::TopLeftCorner] = origin_;
    top_face_cell_map_[Cube::LeftEdge] = origin_ + wxPoint(b_, a_);
    top_face_cell_map_[Cube::BottomLeftCorner] = origin_ + wxPoint(2*b_, 2*a_);
    top_face_cell_map_[Cube::TopEdge] = origin_ + wxPoint(b_, -a_);
    top_face_cell_map_[Cube::Centre] = origin_ + wxPoint(2*b_, 0);
    top_face_cell_map_[Cube::BottomEdge] = origin_ + wxPoint(3*b_, a_);
    top_face_cell_map_[Cube::TopRightCorner] = origin_ + wxPoint(2*b_, -2*a_);
    top_face_cell_map_[Cube::RightEdge] = origin_ + wxPoint(3*b_, -a_);
    top_face_cell_map_[Cube::BottomRightCorner] = origin_ + wxPoint(4*b_, 0);

    left_face_cell_map_[Cube::TopLeftCorner] = origin_ + wxPoint(-b_, a_);
    left_face_cell_map_[Cube::TopEdge] = origin_ + wxPoint(0, 2*a_);
    left_face_cell_map_[Cube::TopRightCorner] = origin_ + wxPoint(b_, 3*a_);
    left_face_cell_map_[Cube::LeftEdge] = origin_ + wxPoint(-b_, 3*a_);
    left_face_cell_map_[Cube::Centre] = origin_ + wxPoint(0, 4*a_);
    left_face_cell_map_[Cube::RightEdge] = origin_ + wxPoint(b_, 5*a_);
    left_face_cell_map_[Cube::BottomLeftCorner] = origin_ + wxPoint(-b_, 5*a_);
    left_face_cell_map_[Cube::BottomEdge] = origin_ + wxPoint(0, 6*a_);
    left_face_cell_map_[Cube::BottomRightCorner] = origin_ + wxPoint(b_, 7*a_);

    right_face_cell_map_[Cube::TopLeftCorner] = origin_ + wxPoint(2*b_, 4*a_);
    right_face_cell_map_[Cube::TopEdge] = origin_ + wxPoint(3*b_, 3*a_);
    right_face_cell_map_[Cube::TopRightCorner] = origin_ + wxPoint(4*b_, 2*a_);
    right_face_cell_map_[Cube::LeftEdge] = origin_ + wxPoint(2*b_, 6*a_);
    right_face_cell_map_[Cube::Centre] = origin_ + wxPoint(3*b_, 5*a_);
    right_face_cell_map_[Cube::RightEdge] = origin_ + wxPoint(4*b_, 4*a_);
    right_face_cell_map_[Cube::BottomLeftCorner] = origin_ + wxPoint(2*b_, 8*a_);
    right_face_cell_map_[Cube::BottomEdge] = origin_ + wxPoint(3*b_, 7*a_);
    right_face_cell_map_[Cube::BottomRightCorner] = origin_ + wxPoint(4*b_, 6*a_);
}

CubeCanvas::CubeCanvas(CubeFrame* parent, int screen_width, int screen_height)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(screen_width, screen_height), wxNO_FULL_REPAINT_ON_RESIZE), 
      screen_width_(screen_width), screen_height_(screen_height), cube_model_(wxPoint(265,100)),
      start_drag_point_(0), cube_drag_offset_(0, 0), last_mouse_pos_(0, 0), is_translating_cube_(false),
      drag_face_(Cube::TopFace), drag_cell_(Cube::TopLeftCorner),
      drag_axis_locked_(false), drag_axis_(Cube::TopBottom), drag_slice_(Cube::TopSlice),
      drag_locked_direction_(UP), drag_angle_per_pixel_(0.0), drag_current_angle_(0.0),
      drag_is_whole_cube_(false),
      log_(0), move_history_(), cube3d_model_(parent->cube_), cube3d_model_small_(parent->cube_, 20)
{
    parent_ = parent;
//    wxGraphicsRenderer* renderer = wxGraphicsRenderer::GetDefaultRenderer();
//    wxGraphicsContext* context = renderer->CreateContext(this);
    randomize_button_ = new wxButton(this, Button_Randomize, wxT("Randomize"), wxPoint(10, 10), wxDefaultSize);
    solve_button_= new wxButton(this, Button_Solve, wxT("Solve"), wxPoint(10, 40), wxDefaultSize);
    undo_button_= new wxButton(this, Button_Undo, wxT("Undo"), wxPoint(10, 70), wxDefaultSize);
    test_button_= new wxButton(this, Button_Test, wxT("Test"), wxPoint(10, 100), wxDefaultSize);
    swap_corners_button_= new wxButton(this, Button_Swap_Corners, wxT("Swap Corners"), wxPoint(10, 130), wxDefaultSize);
    flip_edges_button_= new wxButton(this, Button_Flip_Edges, wxT("Flip Edges"), wxPoint(10, 160), wxDefaultSize);
    rotate_three_edges_button_= new wxButton(this, Button_Rotate_Three_Edges, wxT("Rotate Three Edges"), wxPoint(10, 190), wxDefaultSize);

    wxBrush* red_brush = new wxBrush(*wxRED);
    wxBrush* blue_brush = new wxBrush(*wxBLUE);
    wxBrush* green_brush = new wxBrush(*wxGREEN);
    wxBrush* black_brush = new wxBrush(*wxBLACK);
    wxBrush* white_brush = new wxBrush(*wxWHITE);
    wxBrush* cyan_brush = new wxBrush(*wxCYAN);
    brushes_[Cube::Red] = red_brush;
    brushes_[Cube::Blue] = blue_brush;
    brushes_[Cube::Green] = green_brush;
    brushes_[Cube::Black] = black_brush;
    brushes_[Cube::White] = white_brush;
    brushes_[Cube::Cyan] = cyan_brush;
}

void CubeCanvas::log(const std::string& msg)
{
    if (!log_)
    {
        log_ = new std::ofstream("cubism.log");
    }
    *log_ << msg << std::endl;
}

CubeCanvas::~CubeCanvas()
{
    for (int i = 0; i < Cube::NumberOfColours; ++i)
    {
        delete brushes_[i];
    }
    delete log_;
    for (auto& move: move_history_)
    {
        delete move;
    }
    move_history_.clear();
}

void CubeCanvas::OnLeftButtonDClick(wxMouseEvent& event)
{
    wxUnusedVar(event);
}

void CubeCanvas::OnButtonUndo(wxCommandEvent& WXUNUSED(event))
{
    if (move_history_.empty())
    {
        return;
    }
    Cube::Move* last_move = *(move_history_.rbegin());
    last_move->perform_inverse(parent_->cube_);
    cube3d_model_.animate_inverse_move(*last_move, this, &cube3d_model_small_);
    move_history_.erase(move_history_.end() - 1);
    parent_->GetStatusBar()->SetStatusText(wxString::FromAscii(last_move->inverse_to_string().c_str()));
    delete last_move;
    parent_->Refresh();
    parent_->Update();
}

void CubeCanvas::OnButtonSolve(wxCommandEvent& WXUNUSED(event))
{
    Cube::move_sequence_type solution;
    parent_->cube_.solve(solution);
    for (auto& move : solution)
    {
        perform_move(*move);  // animation provides the visual delay
        delete move;
    }

}

void CubeCanvas::OnButtonRandomize(wxCommandEvent& WXUNUSED(event))
{
    Cube::move_sequence_type random_sequence;
    parent_->cube_.randomize(random_sequence);
    for (auto& move : random_sequence)
    {
        perform_move(*move, false);  // skip animation for batch scrambling
        delete move;
    }
}

void CubeCanvas::OnButtonRotateThreeEdges(wxCommandEvent& event)
{
    std::vector<std::string> sequence = { "RT2", "LM1", "RT2", "LM3" };
    for (auto& s: sequence)
    {
        Cube::Move* move = Cube::make_move(s);
        perform_move(*move);
        delete move;
    }
}

void CubeCanvas::OnButtonFlipEdges(wxCommandEvent& event)
{
    std::vector<std::string> sequence = { "RT3", "LM2", "RT2", "LM1", "RT3", "LT2", "RT1", "LM3", "RT2", "LM2", "RT1", "LT2" };
    for (auto& s: sequence)
    {
        Cube::Move* move = Cube::make_move(s);
        perform_move(*move);
        delete move;
    }
}

void CubeCanvas::OnButtonSwapCorners(wxCommandEvent& event)
{
    std::vector<std::string> sequence = { "RT3", "TT1", "RB1", "TB2", "RB3", "TT3", "RT1" };
    for (auto& s: sequence)
    {
        Cube::Move* move = Cube::make_move(s);
        perform_move(*move);
        delete move;
    }
}

void CubeCanvas::OnButtonTest(wxCommandEvent& event)
{
    try
    {
        for (int i = 0; i < 10000; ++i)
        {
            OnButtonRandomize(event);  // already no animation
            Cube::move_sequence_type solution;
            parent_->cube_.solve(solution);
            for (auto& move : solution)
            {
                perform_move(*move, false);  // no animation for stress test
                delete move;
            }
        }
    }
    catch (const CubeException& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
}

void CubeCanvas::perform_move(const Cube::Move& move, bool animate)
{
    move.perform(parent_->cube_);
    if (animate)
        cube3d_model_.animate_move(move, this, &cube3d_model_small_);
    else
    {
        cube3d_model_.perform_move(move);
        cube3d_model_small_.perform_move(move);
    }
    move_history_.push_back(move.clone());
    parent_->GetStatusBar()->SetStatusText(wxString::FromAscii(move.to_string().c_str()));
    parent_->Refresh();
    parent_->Update();
}

void CubeCanvas::perform_move(const Cube::Axis axis, const Cube::Slice slice, const Cube::Rotation rotation, int line_number)
{
    perform_move(Cube::SliceMove(axis, slice, rotation, line_number));
}

void CubeCanvas::perform_move(const Cube::Axis axis, const Cube::Rotation rotation, int line_number)
{
    perform_move(Cube::RotateMove(axis, rotation, line_number));
}

bool CubeCanvas::CubeModel::right_of_top_face(const wxPoint& point)
{
    wxPoint top_face[4];
    top_face[0] = wxPoint(2*b_, -2*a_) + origin_;
    top_face[1] = wxPoint(5*b_, a_) + origin_;
    top_face[2] = wxPoint(2*b_, 4*a_) + origin_;
    top_face[3] = wxPoint(-b_, a_) + origin_;

    int x_min = top_face[3].x;
    int x_max = top_face[1].x;
    int y_min = top_face[0].y;
    int y_max = top_face[1].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    // Line through points 0 and 1 is given by
    // y = A x + B
    // where A = (0.y - 1.y) / (0.x - 1.x)
    // and   B = (0.x 1.y - 0.y 1.x) / (0.x - 1.x)
    double A = (double)(top_face[0].y - top_face[1].y) / (double)(top_face[0].x - top_face[1].x);
    double B = (double)(top_face[0].x * top_face[1].y - top_face[0].y * top_face[1].x) / (double)(top_face[0].x - top_face[1].x);
    if (point.y > A * point.x + B)
        return false;

    return true;
}

bool CubeCanvas::CubeModel::above_top_face(const wxPoint& point)
{
    wxPoint top_face[4];
    top_face[0] = wxPoint(2*b_, -2*a_) + origin_;
    top_face[1] = wxPoint(5*b_, a_) + origin_;
    top_face[2] = wxPoint(2*b_, 4*a_) + origin_;
    top_face[3] = wxPoint(-b_, a_) + origin_;

    int x_min = top_face[3].x;
    int x_max = top_face[0].x;
    int y_min = top_face[0].y;
    int y_max = top_face[3].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    // Line through points 3 and 0 is given by
    // y = A x + B
    // where A = (3.y - 0.y) / (3.x - 0.x)
    // and   B = (3.x 0.y - 3.y 0.x) / (3.x - 0.x)
    double A = (double)(top_face[3].y - top_face[0].y) / (double)(top_face[3].x - top_face[0].x);
    double B = (double)(top_face[3].x * top_face[0].y - top_face[3].y * top_face[0].x) / (double)(top_face[3].x - top_face[0].x);
    if (point.y > A * point.x + B)
        return false;
    return true;
}

bool CubeCanvas::CubeModel::left_of_left_face(const wxPoint& point)
{
    wxPoint left_face[4];
    left_face[0] = wxPoint(-b_, a_) + origin_;
    left_face[1] = wxPoint(2*b_, 4*a_) + origin_;
    left_face[2] = wxPoint(2*b_, 10*a_) + origin_;
    left_face[3] = wxPoint(-b_, 7*a_) + origin_;

    int x_min = left_face[0].x - 100;
    int x_max = left_face[0].x;
    int y_min = left_face[0].y;
    int y_max = left_face[3].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    return true;
}

bool CubeCanvas::CubeModel::right_of_right_face(const wxPoint& point)
{
    wxPoint right_face[4];
    right_face[0] = wxPoint(2*b_, 4*a_) + origin_;
    right_face[1] = wxPoint(5*b_, a_) + origin_;
    right_face[2] = wxPoint(5*b_, 7*a_) + origin_;
    right_face[3] = wxPoint(2*b_, 10*a_) + origin_;

    int x_min = right_face[1].x;
    int x_max = x_min + 100;
    int y_min = right_face[1].y;
    int y_max = right_face[2].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    return true;
}

bool CubeCanvas::CubeModel::below_left_face(const wxPoint& point)
{
    wxPoint left_face[4];
    left_face[0] = wxPoint(-b_, a_) + origin_;
    left_face[1] = wxPoint(2*b_, 4*a_) + origin_;
    left_face[2] = wxPoint(2*b_, 10*a_) + origin_;
    left_face[3] = wxPoint(-b_, 7*a_) + origin_;

    int x_min = left_face[3].x;
    int x_max = left_face[2].x;
    int y_min = left_face[3].y;
    int y_max = left_face[2].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    double A = (double)(left_face[3].y - left_face[2].y) / (double)(left_face[3].x - left_face[2].x);
    double B = (double)(left_face[3].x * left_face[2].y - left_face[3].y * left_face[2].x) / (double)(left_face[3].x - left_face[2].x);
    if (point.y < A * point.x + B)
        return false;
    return true;
}

bool CubeCanvas::CubeModel::below_right_face(const wxPoint& point)
{
    wxPoint right_face[4];
    right_face[0] = wxPoint(2*b_, 4*a_) + origin_;
    right_face[1] = wxPoint(5*b_, a_) + origin_;
    right_face[2] = wxPoint(5*b_, 7*a_) + origin_;
    right_face[3] = wxPoint(2*b_, 10*a_) + origin_;

    int x_min = right_face[3].x;
    int x_max = right_face[2].x;
    int y_min = right_face[2].y;
    int y_max = right_face[3].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    double A = (double)(right_face[3].y - right_face[2].y) / (double)(right_face[3].x - right_face[2].x);
    double B = (double)(right_face[3].x * right_face[2].y - right_face[3].y * right_face[2].x) / (double)(right_face[3].x - right_face[2].x);
    if (point.y < A * point.x + B)
        return false;
    return true;
}

bool CubeCanvas::CubeModel::inside_top_face(const wxPoint& point)
{
    wxPoint top_face[4];
    top_face[0] = wxPoint(2*b_, -2*a_) + origin_;
    top_face[1] = wxPoint(5*b_, a_) + origin_;
    top_face[2] = wxPoint(2*b_, 4*a_) + origin_;
    top_face[3] = wxPoint(-b_, a_) + origin_;

    int x_min = top_face[3].x;
    int x_max = top_face[1].x;
    int y_min = top_face[0].y;
    int y_max = top_face[2].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    // Line through points 3 and 0 is given by
    // y = A x + B
    // where A = (3.y - 0.y) / (3.x - 0.x)
    // and   B = (3.x 0.y - 3.y 0.x) / (3.x - 0.x)
    double A = (double)(top_face[3].y - top_face[0].y) / (double)(top_face[3].x - top_face[0].x);
    double B = (double)(top_face[3].x * top_face[0].y - top_face[3].y * top_face[0].x) / (double)(top_face[3].x - top_face[0].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    A = (double)(top_face[3].y - top_face[2].y) / (double)(top_face[3].x - top_face[2].x);
    B = (double)(top_face[3].x * top_face[2].y - top_face[3].y * top_face[2].x) / (double)(top_face[3].x - top_face[2].x);
    if (point.y > A * point.x + B)
        return false;

    // Line through points 0 and 1 is given by
    // y = A x + B
    // where A = (0.y - 1.y) / (0.x - 1.x)
    // and   B = (0.x 1.y - 0.y 1.x) / (0.x - 1.x)
    A = (double)(top_face[0].y - top_face[1].y) / (double)(top_face[0].x - top_face[1].x);
    B = (double)(top_face[0].x * top_face[1].y - top_face[0].y * top_face[1].x) / (double)(top_face[0].x - top_face[1].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 2 and 1 is given by
    // y = A x + B
    // where A = (2.y - 1.y) / (2.x - 1.x)
    // and   B = (2.x 1.y - 2.y 1.x) / (2.x - 1.x)
    A = (double)(top_face[2].y - top_face[1].y) / (double)(top_face[2].x - top_face[1].x);
    B = (double)(top_face[2].x * top_face[1].y - top_face[2].y * top_face[1].x) / (double)(top_face[2].x - top_face[1].x);
    if ((double)point.y > A * (double)point.x + B)
        return false;

    return true;
}

bool CubeCanvas::CubeModel::inside_left_face(const wxPoint& point)
{
    wxPoint left_face[4];
    left_face[0] = wxPoint(-b_, a_) + origin_;
    left_face[1] = wxPoint(2*b_, 4*a_) + origin_;
    left_face[2] = wxPoint(2*b_, 10*a_) + origin_;
    left_face[3] = wxPoint(-b_, 7*a_) + origin_;

    int x_min = left_face[3].x;
    int x_max = left_face[1].x;
    int y_min = left_face[0].y;
    int y_max = left_face[2].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;

    // Line through points 0 and 1 is given by
    // y = A x + B
    // where A = (0.y - 1.y) / (0.x - 1.x)
    // and   B = (0.x 1.y - 0.y 1.x) / (0.x - 1.x)
    double A = (double)(left_face[0].y - left_face[1].y) / (double)(left_face[0].x - left_face[1].x);
    double B = (double)(left_face[0].x * left_face[1].y - left_face[0].y * left_face[1].x) / (double)(left_face[0].x - left_face[1].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    A = (double)(left_face[3].y - left_face[2].y) / (double)(left_face[3].x - left_face[2].x);
    B = (double)(left_face[3].x * left_face[2].y - left_face[3].y * left_face[2].x) / (double)(left_face[3].x - left_face[2].x);
    if (point.y > A * point.x + B)
        return false;

    return true;
}

bool CubeCanvas::CubeModel::inside_right_face(const wxPoint& point)
{
    wxPoint right_face[4];
    right_face[0] = wxPoint(2*b_, 4*a_) + origin_;
    right_face[1] = wxPoint(5*b_, a_) + origin_;
    right_face[2] = wxPoint(5*b_, 7*a_) + origin_;
    right_face[3] = wxPoint(2*b_, 10*a_) + origin_;

    int x_min = right_face[3].x;
    int x_max = right_face[1].x;
    int y_min = right_face[1].y;
    int y_max = right_face[3].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;

    // Line through points 0 and 1 is given by
    // y = A x + B
    // where A = (0.y - 1.y) / (0.x - 1.x)
    // and   B = (0.x 1.y - 0.y 1.x) / (0.x - 1.x)
    double A = (double)(right_face[0].y - right_face[1].y) / (double)(right_face[0].x - right_face[1].x);
    double B = (double)(right_face[0].x * right_face[1].y - right_face[0].y * right_face[1].x) / (double)(right_face[0].x - right_face[1].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    A = (double)(right_face[3].y - right_face[2].y) / (double)(right_face[3].x - right_face[2].x);
    B = (double)(right_face[3].x * right_face[2].y - right_face[3].y * right_face[2].x) / (double)(right_face[3].x - right_face[2].x);
    if (point.y > A * point.x + B)
        return false;

    return true;
}

bool CubeCanvas::CubeModel::inside_top_cell(const wxPoint& point, const wxPoint& cell_origin)
{
    wxPoint cell[4];
    for (int i = 0; i < 4; ++i)
    {
        cell[i] = top_diamond_[i] + cell_origin;
    }

    int x_min = cell[3].x;
    int x_max = cell[1].x;
    int y_min = cell[0].y;
    int y_max = cell[2].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;
    // Line through points 3 and 0 is given by
    // y = A x + B
    // where A = (3.y - 0.y) / (3.x - 0.x)
    // and   B = (3.x 0.y - 3.y 0.x) / (3.x - 0.x)
    double A = (double)(cell[3].y - cell[0].y) / (double)(cell[3].x - cell[0].x);
    double B = (double)(cell[3].x * cell[0].y - cell[3].y * cell[0].x) / (double)(cell[3].x - cell[0].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    A = (double)(cell[3].y - cell[2].y) / (double)(cell[3].x - cell[2].x);
    B = (double)(cell[3].x * cell[2].y - cell[3].y * cell[2].x) / (double)(cell[3].x - cell[2].x);
    if (point.y > A * point.x + B)
        return false;

    // Line through points 0 and 1 is given by
    // y = A x + B
    // where A = (0.y - 1.y) / (0.x - 1.x)
    // and   B = (0.x 1.y - 0.y 1.x) / (0.x - 1.x)
    A = (double)(cell[0].y - cell[1].y) / (double)(cell[0].x - cell[1].x);
    B = (double)(cell[0].x * cell[1].y - cell[0].y * cell[1].x) / (double)(cell[0].x - cell[1].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 2 and 1 is given by
    // y = A x + B
    // where A = (2.y - 1.y) / (2.x - 1.x)
    // and   B = (2.x 1.y - 2.y 1.x) / (2.x - 1.x)
    A = (double)(cell[2].y - cell[1].y) / (double)(cell[2].x - cell[1].x);
    B = (double)(cell[2].x * cell[1].y - cell[2].y * cell[1].x) / (double)(cell[2].x - cell[1].x);
    if ((double)point.y > A * (double)point.x + B)
        return false;

    return true;
}

bool CubeCanvas::CubeModel::inside_left_cell(const wxPoint& point, const wxPoint& cell_origin)
{
    wxPoint cell[4];
    for (int i = 0; i < 4; ++i)
    {
        cell[i] = left_side_diamond_[i] + cell_origin;
    }

    int x_min = cell[3].x;
    int x_max = cell[1].x;
    int y_min = cell[0].y;
    int y_max = cell[2].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;

    // Line through points 0 and 1 is given by
    // y = A x + B
    // where A = (0.y - 1.y) / (0.x - 1.x)
    // and   B = (0.x 1.y - 0.y 1.x) / (0.x - 1.x)
    double A = (double)(cell[0].y - cell[1].y) / (double)(cell[0].x - cell[1].x);
    double B = (double)(cell[0].x * cell[1].y - cell[0].y * cell[1].x) / (double)(cell[0].x - cell[1].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    A = (double)(cell[3].y - cell[2].y) / (double)(cell[3].x - cell[2].x);
    B = (double)(cell[3].x * cell[2].y - cell[3].y * cell[2].x) / (double)(cell[3].x - cell[2].x);
    if (point.y > A * point.x + B)
        return false;

    return true;
}

bool CubeCanvas::CubeModel::inside_right_cell(const wxPoint& point, const wxPoint& cell_origin)
{
    wxPoint cell[4];
    for (int i = 0; i < 4; ++i)
    {
        cell[i] = right_side_diamond_[i] + cell_origin;
    }

    int x_min = cell[3].x;
    int x_max = cell[1].x;
    int y_min = cell[1].y;
    int y_max = cell[3].y;
    if (point.x < x_min || point.x > x_max)
        return false;
    if (point.y < y_min || point.y > y_max)
        return false;

    // Line through points 0 and 1 is given by
    // y = A x + B
    // where A = (0.y - 1.y) / (0.x - 1.x)
    // and   B = (0.x 1.y - 0.y 1.x) / (0.x - 1.x)
    double A = (double)(cell[0].y - cell[1].y) / (double)(cell[0].x - cell[1].x);
    double B = (double)(cell[0].x * cell[1].y - cell[0].y * cell[1].x) / (double)(cell[0].x - cell[1].x);
    if (point.y < A * point.x + B)
        return false;

    // Line through points 3 and 2 is given by
    // y = A x + B
    // where A = (3.y - 2.y) / (3.x - 2.x)
    // and   B = (3.x 2.y - 3.y 2.x) / (3.x - 2.x)
    A = (double)(cell[3].y - cell[2].y) / (double)(cell[3].x - cell[2].x);
    B = (double)(cell[3].x * cell[2].y - cell[3].y * cell[2].x) / (double)(cell[3].x - cell[2].x);
    if (point.y > A * point.x + B)
        return false;

    return true;
}

bool CubeCanvas::CubeModel::get_cell(const wxPoint& point, Cube::Faces& face, Cube::Cells& cell)
{
    if (inside_top_face(point))
    {
        face = Cube::TopFace;
        for (auto& item : top_face_cell_map_)
        {
            Cube::Cells c = item.first;
            wxPoint cell_origin = item.second;
            if (inside_top_cell(point, cell_origin))
            {
                cell = c;
                return true;
            }
        }
    }
    else if (inside_left_face(point))
    {
        face = Cube::LeftFrontFace;
        for (auto& item : left_face_cell_map_)
        {
            Cube::Cells c = item.first;
            wxPoint cell_origin = item.second;
            if (inside_left_cell(point, cell_origin))
            {
                cell = c;
                return true;
            }
        }
    }
    else if (inside_right_face(point))
    {
        face = Cube::RightFrontFace;
        for (auto& item : right_face_cell_map_)
        {
            Cube::Cells c = item.first;
            wxPoint cell_origin = item.second;
            if (inside_right_cell(point, cell_origin))
            {
                cell = c;
                return true;
            }
        }
    }
    return false;
}

void CubeCanvas::draw_shape3D(wxDC& dc, const Projector& projector, const Shape3D& shape, const wxPoint& origin)
{
    polygon_list_type polygon_list = shape.get_polygon_list();
    for (auto& polygon : polygon_list)
    {
        std::vector<wxPoint> projected_polygon;
        projector.project(polygon.points(), projected_polygon);
        dc.SetBrush(*brushes_[polygon.colour()]);
        dc.DrawPolygon(projected_polygon.size(), &projected_polygon[0], origin.x, origin.y);
    }
}

void CubeCanvas::draw_object3D(wxDC& dc, const Projector& projector, const Object3D& object, const wxPoint& origin)
{
    polygon_list_type polygon_list = object.get_polygon_list();
    for (auto& polygon : polygon_list)
    {
        std::vector<wxPoint> projected_polygon;
        projector.project(polygon.points(), projected_polygon);
        dc.SetBrush(*brushes_[polygon.colour()]);
        dc.DrawPolygon(projected_polygon.size(), &projected_polygon[0], origin.x, origin.y);
    }
}

void CubeCanvas::draw_cube(wxDC& dc)
{
    //cube_model_.draw_cube(dc, brushes_, parent_->cube_);
    Projector projector(screen_width_, screen_height_);

    draw_object3D(dc, projector, cube3d_model_.get_object(), cube_drag_offset_);

    Cube::RotateMove moves[] = { 
        Cube::RotateMove(Cube::TopBottom, Cube::Quarter, __LINE__),
        Cube::RotateMove(Cube::TopBottom, Cube::Half, __LINE__),
        Cube::RotateMove(Cube::TopBottom, Cube::ThreeQuarters, __LINE__),
        Cube::RotateMove(Cube::RightFrontLeftBack, Cube::Quarter, __LINE__),
        Cube::RotateMove(Cube::RightFrontLeftBack, Cube::Half, __LINE__),
        Cube::RotateMove(Cube::RightFrontLeftBack, Cube::ThreeQuarters, __LINE__)
    };
    const int move_count = sizeof(moves) / sizeof(Cube::RotateMove);
    for (int i = 0; i < move_count; ++i)
    {
        Cube::RotateMove& move = moves[i];
        cube3d_model_small_.perform_move(move);
        draw_object3D(dc, projector, cube3d_model_small_.get_object(), wxPoint(350, 250 - 90*i));
        cube3d_model_small_.perform_inverse_move(move);
    }
}

void CubeCanvas::CubeModel::draw_cube(wxDC& dc, wxBrush** brushes, const Cube& cube)
{
    // Top face
    for (auto& item : top_face_cell_map_)
    {
        dc.SetBrush(*brushes[cube.colour(Cube::TopFace, item.first)]);
        dc.DrawPolygon(WXSIZEOF(top_diamond_), top_diamond_, item.second.x, item.second.y);
    }

    // Left face
    for (auto& item : left_face_cell_map_)
    {
        dc.SetBrush(*brushes[cube.colour(Cube::LeftFrontFace, item.first)]);
        dc.DrawPolygon(WXSIZEOF(left_side_diamond_), left_side_diamond_, item.second.x, item.second.y);
    }

    // Right face
    for (auto& item : right_face_cell_map_)
    {
        dc.SetBrush(*brushes[cube.colour(Cube::RightFrontFace, item.first)]);
        dc.DrawPolygon(WXSIZEOF(right_side_diamond_), right_side_diamond_, item.second.x, item.second.y);
    }
}

void CubeCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC pdc(this);

    wxDC &dc = pdc ;

    PrepareDC(dc);

    parent_->PrepareDC(dc);

    dc.Clear();

    draw_cube(dc);
}


#define do_perform_move(axis, slice, rotation) perform_move(axis, slice, rotation, __LINE__)
#define do_perform_rotate_move(axis, rotation) perform_move(axis, rotation, __LINE__)

void CubeCanvas::move_cube(const Cube::Faces& face, const Cube::Cells& cell, directions direction)
{
    Cube::Axis axis;
    Cube::Slice slice;
    Cube::Rotation rotation;
    get_slice_move(face, cell, direction, axis, slice, rotation);
    do_perform_move(axis, slice, rotation);
}

void CubeCanvas::get_slice_move(const Cube::Faces& face, const Cube::Cells& cell, directions direction,
                                Cube::Axis& axis, Cube::Slice& slice, Cube::Rotation& rotation)
{
    // Provide safe defaults (should always be overwritten by the switch below)
    axis = Cube::TopBottom; slice = Cube::TopSlice; rotation = Cube::Quarter;
    switch (face)
    {
        case Cube::TopFace:
            switch (cell)
            {
                case Cube::TopLeftCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::TopEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::TopRightCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::LeftEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::Centre:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::RightEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::BottomLeftCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::BottomEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                    }
                    break;
                case Cube::BottomRightCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case RIGHT: axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                    }
                    break;
            }
            break;
        case Cube::LeftFrontFace:
            switch (cell)
            {
                case Cube::TopLeftCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::TopEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::TopRightCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::LeftEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::Centre:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::RightEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::BottomLeftCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::BottomEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::BottomRightCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case DOWN:  axis=Cube::RightFrontLeftBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
            }
            break;
        case Cube::RightFrontFace:
            switch (cell)
            {
                case Cube::TopLeftCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::TopEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::TopRightCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::LeftEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::Centre:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::RightEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::BottomLeftCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::TopSlice;    rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::BottomEdge:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::MiddleSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
                case Cube::BottomRightCorner:
                    switch (direction)
                    {
                        case UP:    axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                        case DOWN:  axis=Cube::LeftFrontRightBack; slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case LEFT:  axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::Quarter;       break;
                        case RIGHT: axis=Cube::TopBottom;          slice=Cube::BottomSlice; rotation=Cube::ThreeQuarters; break;
                    }
                    break;
            }
            break;
    }
}


double CubeCanvas::get_move_angle(const wxPoint& p1, const wxPoint& p2)
{
    // Calculate angle of motion from p1 to p2
    // if p2.x != p1.x
    //
    //               -1
    //    theta = tan  ((p1.y - p2.y) / (p2.x - p1.x))
    //    (with result in range 0 - 2 pi)
    //
    // else
    //
    //    theta = pi / 2 if p2.y < p1.y
    //
    //    theta = 3 pi / 2 if p2.y > p1.y
    //
    double theta = 0.0;
    if (p2.x != p1.x)
    {
        if (p2.x > p1.x && p2.y <= p1.y)
        {
            // first quadrant
            theta = std::atan((double)(p1.y - p2.y) / (double)(p2.x - p1.x));
        }
        else if (p2.x < p1.x && p2.y <= p1.y)
        {
            // second quadrant
            theta = M_PI - std::atan((double)(p1.y - p2.y) / (double)(p1.x - p2.x));
        }
        else if (p2.x < p1.x && p2.y > p1.y)
        {
            // third quadrant
            theta = M_PI + std::atan((double)(p2.y - p1.y) / (double)(p1.x - p2.x));
        }
        else if (p2.x > p1.x && p2.y > p1.y)
        {
            // fourth quadrant
            theta = 2.0 * M_PI - std::atan((double)(p2.y - p1.y) / (double)(p2.x - p1.x));
        }
    }
    else
    {
        if (p2.y < p1.y)
        {
            theta = M_PI_2;
        }
        else if (p2.y > p1.y)
        {
            theta = 3.0 * M_PI_2;
        }
    }

    return theta;
}

CubeCanvas::directions CubeCanvas::get_direction(const Cube::Faces& face, double theta)
{
    // Map angle onto direction (relative to the face we're on)
    directions direction(UP);
    if (face == Cube::LeftFrontFace)
    {
        theta += M_PI / 3.0;
    }
    else if (face == Cube::RightFrontFace)
    {
        theta += M_PI / 6.0;
    }
    if (theta > 2 * M_PI)
    {
        theta -= 2 * M_PI;
    }

    if (theta < M_PI_2)
    {
        direction = RIGHT;
    }
    else if (theta < M_PI)
    {
        direction = UP;
    }
    else if (theta < 3.0 * M_PI_2)
    {
        direction = LEFT;
    }
    else
    {
        direction = DOWN;
    }
    return direction;
}

void CubeCanvas::move_cube(const Cube::Faces& face, const Cube::Cells& cell, const wxPoint& p1, const wxPoint& p2)
{
    double theta = get_move_angle(p1, p2);
    directions direction = get_direction(face, theta);

    move_cube(face, cell, direction);
}

void CubeCanvas::OnMouseMove(wxMouseEvent &event)
{
    wxCoord xpos;
    wxCoord ypos;
    event.GetPosition(&xpos, &ypos);

    // ----- Active drag (face slice OR whole cube): handle all events here -----
    if (start_drag_point_)
    {
        if (drag_is_whole_cube_)
        {
            if (event.Dragging())
            {
                int dx = xpos - start_drag_point_->x;
                int dy = ypos - start_drag_point_->y;
                double dist = std::sqrt((double)(dx * dx + dy * dy));

                // Lock to dominant axis after threshold.
                if (!drag_axis_locked_ && dist >= DRAG_LOCK_THRESHOLD_PX)
                {
                    if (std::abs(dx) >= std::abs(dy))
                    {
                        drag_axis_ = Cube::TopBottom;
                        drag_locked_direction_ = (dx >= 0) ? RIGHT : LEFT;
                    }
                    else
                    {
                        drag_axis_ = Cube::RightFrontLeftBack;
                        drag_locked_direction_ = (dy >= 0) ? DOWN : UP;
                    }
                    drag_angle_per_pixel_ = -Cube3DModel::get_slice_angle(drag_axis_, Cube::Quarter)
                                            / DRAG_PIXELS_PER_QTR_TURN;
                    drag_axis_locked_ = true;
                }

                if (drag_axis_locked_)
                {
                    double projected = 0.0;
                    switch (drag_locked_direction_)
                    {
                        case UP:    projected = -(double)dy; break;
                        case DOWN:  projected = +(double)dy; break;
                        case LEFT:  projected = -(double)dx; break;
                        case RIGHT: projected = +(double)dx; break;
                    }
                    double new_angle = drag_angle_per_pixel_ * projected;
                    double delta = new_angle - drag_current_angle_;

                    cube3d_model_.begin_slice_drag();
                    cube3d_model_.rotate_by_angle(drag_axis_, delta);
                    cube3d_model_.end_slice_drag();
                    cube3d_model_small_.begin_slice_drag();
                    cube3d_model_small_.rotate_by_angle(drag_axis_, delta);
                    cube3d_model_small_.end_slice_drag();
                    drag_current_angle_ = new_angle;

                    parent_->Refresh();
                    parent_->Update();
                }
            }
            else if (event.ButtonUp())
            {
                if (drag_axis_locked_)
                {
                    // Snap to the nearest multiple of π/2.
                    double snapped = std::round(drag_current_angle_ / (M_PI / 2.0)) * (M_PI / 2.0);
                    double delta = snapped - drag_current_angle_;

                    cube3d_model_.begin_slice_drag();
                    cube3d_model_.rotate_by_angle(drag_axis_, delta);
                    cube3d_model_.end_slice_drag();
                    cube3d_model_small_.begin_slice_drag();
                    cube3d_model_small_.rotate_by_angle(drag_axis_, delta);
                    cube3d_model_small_.end_slice_drag();

                    if (std::abs(snapped) > DRAG_SNAP_EPSILON)
                    {
                        // Find the Cube::Rotation that matches `snapped`.
                        Cube::Rotation rotation = Cube::Quarter;
                        for (int ri = 1; ri <= 3; ++ri)
                        {
                            Cube::Rotation cand = static_cast<Cube::Rotation>(ri);
                            double diff = Cube3DModel::get_slice_angle(drag_axis_, cand) - snapped;
                            while (diff >  M_PI) diff -= 2.0 * M_PI;
                            while (diff < -M_PI) diff += 2.0 * M_PI;
                            if (std::abs(diff) < DRAG_ANGLE_MATCH_TOL)
                            {
                                rotation = cand;
                                break;
                            }
                        }
                        // Commit the logical rotate move.
                        Cube::RotateMove move(drag_axis_, rotation, __LINE__);
                        move.perform(parent_->cube_);
                        move_history_.push_back(move.clone());
                        parent_->GetStatusBar()->SetStatusText(
                            wxString::FromAscii(move.to_string().c_str()));
                    }
                }

                delete start_drag_point_;
                start_drag_point_ = 0;
                drag_axis_locked_ = false;
                parent_->Refresh();
                parent_->Update();
            }
        }
        else
        {
            // ----- Active face-slice drag -----
            if (event.Dragging())
            {
                int dx = xpos - start_drag_point_->x;
                int dy = ypos - start_drag_point_->y;
                double dist = std::sqrt((double)(dx * dx + dy * dy));

                // Lock the rotation axis/slice after the pointer has moved far enough.
                if (!drag_axis_locked_ && dist >= DRAG_LOCK_THRESHOLD_PX)
                {
                    double theta = get_move_angle(*start_drag_point_, wxPoint(xpos, ypos));
                    drag_locked_direction_ = get_direction(drag_face_, theta);
                    Cube::Rotation rotation;
                    get_slice_move(drag_face_, drag_cell_, drag_locked_direction_,
                                   drag_axis_, drag_slice_, rotation);
                    // angle_per_pixel: positive projection in lock direction → signed rotation angle
                    drag_angle_per_pixel_ = Cube3DModel::get_slice_angle(drag_axis_, rotation)
                                            / DRAG_PIXELS_PER_QTR_TURN;
                    drag_axis_locked_ = true;
                }

                if (drag_axis_locked_)
                {
                    // Project total displacement onto the locked drag direction.
                    double projected = 0.0;
                    switch (drag_locked_direction_)
                    {
                        case UP:    projected = -(double)dy; break;
                        case DOWN:  projected = +(double)dy; break;
                        case LEFT:  projected = -(double)dx; break;
                        case RIGHT: projected = +(double)dx; break;
                    }
                    double new_angle = drag_angle_per_pixel_ * projected;
                    double delta = new_angle - drag_current_angle_;

                    // Apply incremental rotation to the 3D model.
                    cube3d_model_.begin_slice_drag();
                    cube3d_model_.move_slice_by_angle(drag_axis_, drag_slice_, delta);
                    cube3d_model_.end_slice_drag();
                    drag_current_angle_ = new_angle;

                    parent_->Refresh();
                    parent_->Update();
                }
            }
            else if (event.ButtonUp())
            {
                if (drag_axis_locked_)
                {
                    // Snap to the nearest multiple of π/2 (quarter turn).
                    double snapped = std::round(drag_current_angle_ / (M_PI / 2.0)) * (M_PI / 2.0);
                    double delta = snapped - drag_current_angle_;

                    cube3d_model_.begin_slice_drag();
                    cube3d_model_.move_slice_by_angle(drag_axis_, drag_slice_, delta);
                    cube3d_model_.end_slice_drag();

                    if (std::abs(snapped) > DRAG_SNAP_EPSILON)
                    {
                        // Find the Cube::Rotation whose slice_angle matches `snapped` (mod 2π).
                        Cube::Rotation rotation = Cube::Quarter;
                        for (int ri = 1; ri <= 3; ++ri)
                        {
                            Cube::Rotation cand = static_cast<Cube::Rotation>(ri);
                            double diff = Cube3DModel::get_slice_angle(drag_axis_, cand) - snapped;
                            while (diff >  M_PI) diff -= 2.0 * M_PI;
                            while (diff < -M_PI) diff += 2.0 * M_PI;
                            if (std::abs(diff) < DRAG_ANGLE_MATCH_TOL)
                            {
                                rotation = cand;
                                break;
                            }
                        }
                        // Commit the logical move (3D model already shows the result).
                        Cube::SliceMove move(drag_axis_, drag_slice_, rotation, __LINE__);
                        move.perform(parent_->cube_);
                        cube3d_model_small_.perform_move(move);
                        move_history_.push_back(move.clone());
                        parent_->GetStatusBar()->SetStatusText(
                            wxString::FromAscii(move.to_string().c_str()));
                    }
                }

                delete start_drag_point_;
                start_drag_point_ = 0;
                drag_axis_locked_ = false;
                parent_->Refresh();
                parent_->Update();
            }
        }
        return;
    }

    // ----- No active drag: handle new interactions -----
    Cube::Faces face;
    Cube::Cells cell;
    bool on_face = cube_model_.get_cell(wxPoint(xpos, ypos), face, cell);

    if (on_face)
    {
        if (event.LeftDown())
        {
            start_drag_point_ = new wxPoint(xpos, ypos);
            drag_face_ = face;
            drag_cell_ = cell;
            drag_axis_locked_ = false;
            drag_current_angle_ = 0.0;
            drag_is_whole_cube_ = false;
        }
    }
    else
    {
        if (event.LeftDown())
        {
            start_drag_point_ = new wxPoint(xpos, ypos);
            drag_face_ = Cube::TopFace;
            drag_cell_ = Cube::TopLeftCorner;
            drag_is_whole_cube_ = true;
            drag_axis_locked_ = false;
            drag_current_angle_ = 0.0;
        }
    }
//    parent_->GetStatusBar()->SetStatusText(wxString::FromAscii(oss.str().c_str()));
}

BEGIN_EVENT_TABLE(CubeCanvas, wxPanel)
    EVT_PAINT  (CubeCanvas::OnPaint)
    EVT_MOUSE_EVENTS (CubeCanvas::OnMouseMove)
    EVT_LEFT_DCLICK (CubeCanvas::OnLeftButtonDClick)
    EVT_BUTTON (Button_Randomize, CubeCanvas::OnButtonRandomize)
    EVT_BUTTON (Button_Solve, CubeCanvas::OnButtonSolve)
    EVT_BUTTON (Button_Undo, CubeCanvas::OnButtonUndo)
    EVT_BUTTON (Button_Test, CubeCanvas::OnButtonTest)
    EVT_BUTTON (Button_Swap_Corners, CubeCanvas::OnButtonSwapCorners)
    EVT_BUTTON (Button_Flip_Edges, CubeCanvas::OnButtonFlipEdges)
    EVT_BUTTON (Button_Rotate_Three_Edges, CubeCanvas::OnButtonRotateThreeEdges)
END_EVENT_TABLE()

IMPLEMENT_APP(CubeApp)

bool CubeApp::OnInit()
{
    CubeFrame* frame = new CubeFrame(_T("Cubism"), wxPoint(50, 50), wxSize(900, 700));
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}
