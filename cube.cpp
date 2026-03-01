#include "cube.h"
#include <random>
#include <set>
#include <map>
#include <sstream>
#include <cassert>
#include <iostream>

int Cube::depth_ = 0;

void Cube::rotate(const Cube::Axis axis, const Cube::Rotation rotation)
{
    ++depth_;
    for (int i = 1; i <= rotation; ++i)
    {
        switch (axis)
        {
            case TopBottom:
            {
                Face temp(faces_[RightFrontFace]);
                faces_[RightFrontFace] = faces_[RightBackFace];
                faces_[RightBackFace] = faces_[LeftBackFace];
                faces_[LeftBackFace] = faces_[LeftFrontFace];
                faces_[LeftFrontFace] = temp;
                faces_[TopFace].rotate(Quarter);
                faces_[BottomFace].rotate(ThreeQuarters);
            }
            break;
            case RightFrontLeftBack:
            {
                Face temp(faces_[TopFace]);
                faces_[TopFace] = faces_[LeftFrontFace];
                faces_[TopFace].rotate(Quarter);//<-----
                faces_[LeftFrontFace] = faces_[BottomFace];
                faces_[LeftFrontFace].rotate(Quarter);//<----
                faces_[BottomFace] = faces_[RightBackFace];
                faces_[BottomFace].rotate(Quarter); //<----
                faces_[RightBackFace] = temp;
                faces_[RightBackFace].rotate(Quarter);//<----
                faces_[RightFrontFace].rotate(Quarter);
                faces_[LeftBackFace].rotate(ThreeQuarters);
            }
            break;
            case LeftFrontRightBack:
            {
                Face temp(faces_[TopFace]);
                faces_[TopFace] = faces_[LeftBackFace];
                faces_[TopFace].rotate(Half);//<---
                faces_[LeftBackFace] = faces_[BottomFace];
                faces_[LeftBackFace].rotate(Half);//<---
                faces_[BottomFace] = faces_[RightFrontFace];
                faces_[RightFrontFace] = temp;
                faces_[LeftFrontFace].rotate(Quarter);
                faces_[RightBackFace].rotate(ThreeQuarters);
            }
            break;
        }
    }
    --depth_;
    if (depth_ == 0)
    {
        pieces();
        update_piece_map(axis, rotation);
        if (!check_piece_map())
        {
            throw CubeException("check_piece_map() failed", __FILE__, __LINE__);
        }
    }
}

void Cube::update_piece_map(const Axis axis, const Rotation rotation)
{
    update_piece_map(axis, TopSlice, rotation);
    update_piece_map(axis, MiddleSlice, rotation);
    update_piece_map(axis, BottomSlice, rotation);
}

void Cube::move_slice(const Axis axis, const Slice slice, const Rotation rotation)
{
    ++depth_;
    switch (axis)
    {
        case TopBottom:
        {
            switch (slice)
            {
                case TopSlice:
                {
                    faces_[TopFace].rotate(rotation);
                    for (int i = 1; i <= rotation; ++i)
                    {
                        Colour temp = faces_[RightFrontFace].cell_[TopLeftCorner];
                        faces_[RightFrontFace].cell_[TopLeftCorner] = faces_[RightBackFace].cell_[TopLeftCorner];
                        faces_[RightBackFace].cell_[TopLeftCorner] = faces_[LeftBackFace].cell_[TopLeftCorner];
                        faces_[LeftBackFace].cell_[TopLeftCorner] = faces_[LeftFrontFace].cell_[TopLeftCorner];
                        faces_[LeftFrontFace].cell_[TopLeftCorner] = temp;
                        temp = faces_[RightFrontFace].cell_[TopRightCorner];
                        faces_[RightFrontFace].cell_[TopRightCorner] = faces_[RightBackFace].cell_[TopRightCorner];
                        faces_[RightBackFace].cell_[TopRightCorner] = faces_[LeftBackFace].cell_[TopRightCorner];
                        faces_[LeftBackFace].cell_[TopRightCorner] = faces_[LeftFrontFace].cell_[TopRightCorner];
                        faces_[LeftFrontFace].cell_[TopRightCorner] = temp;
                        temp = faces_[RightFrontFace].cell_[TopEdge];
                        faces_[RightFrontFace].cell_[TopEdge] = faces_[RightBackFace].cell_[TopEdge];
                        faces_[RightBackFace].cell_[TopEdge] = faces_[LeftBackFace].cell_[TopEdge];
                        faces_[LeftBackFace].cell_[TopEdge] = faces_[LeftFrontFace].cell_[TopEdge];
                        faces_[LeftFrontFace].cell_[TopEdge] = temp;
                    }
                }
                break;
                case MiddleSlice:
                {
                    for (int i = 1; i <= rotation; ++i)
                    {
                        Colour temp = faces_[RightFrontFace].cell_[LeftEdge];
                        faces_[RightFrontFace].cell_[LeftEdge] = faces_[RightBackFace].cell_[LeftEdge];
                        faces_[RightBackFace].cell_[LeftEdge] = faces_[LeftBackFace].cell_[LeftEdge];
                        faces_[LeftBackFace].cell_[LeftEdge] = faces_[LeftFrontFace].cell_[LeftEdge];
                        faces_[LeftFrontFace].cell_[LeftEdge] = temp;
                        temp = faces_[RightFrontFace].cell_[RightEdge];
                        faces_[RightFrontFace].cell_[RightEdge] = faces_[RightBackFace].cell_[RightEdge];
                        faces_[RightBackFace].cell_[RightEdge] = faces_[LeftBackFace].cell_[RightEdge];
                        faces_[LeftBackFace].cell_[RightEdge] = faces_[LeftFrontFace].cell_[RightEdge];
                        faces_[LeftFrontFace].cell_[RightEdge] = temp;
                        temp = faces_[RightFrontFace].cell_[Centre];
                        faces_[RightFrontFace].cell_[Centre] = faces_[RightBackFace].cell_[Centre];
                        faces_[RightBackFace].cell_[Centre] = faces_[LeftBackFace].cell_[Centre];
                        faces_[LeftBackFace].cell_[Centre] = faces_[LeftFrontFace].cell_[Centre];
                        faces_[LeftFrontFace].cell_[Centre] = temp;
                    }
                }
                break;
                case BottomSlice:
                {
                    faces_[BottomFace].rotate_inverse(rotation);
                    for (int i = 1; i <= rotation; ++i)
                    {
                        Colour temp = faces_[RightFrontFace].cell_[BottomLeftCorner];
                        faces_[RightFrontFace].cell_[BottomLeftCorner] = faces_[RightBackFace].cell_[BottomLeftCorner];
                        faces_[RightBackFace].cell_[BottomLeftCorner] = faces_[LeftBackFace].cell_[BottomLeftCorner];
                        faces_[LeftBackFace].cell_[BottomLeftCorner] = faces_[LeftFrontFace].cell_[BottomLeftCorner];
                        faces_[LeftFrontFace].cell_[BottomLeftCorner] = temp;
                        temp = faces_[RightFrontFace].cell_[BottomRightCorner];
                        faces_[RightFrontFace].cell_[BottomRightCorner] = faces_[RightBackFace].cell_[BottomRightCorner];
                        faces_[RightBackFace].cell_[BottomRightCorner] = faces_[LeftBackFace].cell_[BottomRightCorner];
                        faces_[LeftBackFace].cell_[BottomRightCorner] = faces_[LeftFrontFace].cell_[BottomRightCorner];
                        faces_[LeftFrontFace].cell_[BottomRightCorner] = temp;
                        temp = faces_[RightFrontFace].cell_[BottomEdge];
                        faces_[RightFrontFace].cell_[BottomEdge] = faces_[RightBackFace].cell_[BottomEdge];
                        faces_[RightBackFace].cell_[BottomEdge] = faces_[LeftBackFace].cell_[BottomEdge];
                        faces_[LeftBackFace].cell_[BottomEdge] = faces_[LeftFrontFace].cell_[BottomEdge];
                        faces_[LeftFrontFace].cell_[BottomEdge] = temp;
                    }
                }
                break;
            }
        }
        break;
        case RightFrontLeftBack:
        {
            rotate(LeftFrontRightBack, ThreeQuarters);
            move_slice(TopBottom, slice, rotation);
            rotate(LeftFrontRightBack, Quarter);
        }
        break;
        case LeftFrontRightBack:
        {
            rotate(TopBottom, ThreeQuarters);
            rotate(LeftFrontRightBack, ThreeQuarters);
            move_slice(TopBottom, slice, rotation);
            rotate(LeftFrontRightBack, Quarter);
            rotate(TopBottom, Quarter);
        }
        break;
    }
    --depth_;
    if (depth_ == 0)
    {
        pieces();
        update_piece_map(axis, slice, rotation);
        if (!check_piece_map())
        {
            throw CubeException("check_piece_map() failed", __FILE__, __LINE__);
        }
    }
}

bool Cube::check_piece_map() const
{
    Cube cube;
    for (int i = 0; i < NumberOfEdgePieces; ++i)
    {
        EdgePiece current_edge = pieces_.edges_[i]; 
        EdgePiece original_edge = cube.pieces_.edges_[piece_map_.edge_map_[i]];
        if (!((current_edge.colour_[0] == original_edge.colour_[0] && current_edge.colour_[1] == original_edge.colour_[1]) ||
             (current_edge.colour_[0] == original_edge.colour_[1] && current_edge.colour_[1] == original_edge.colour_[0])))
        {
            return false;
        }

    }
	return true;
}

void Cube::update_piece_map(const Axis axis, const Slice slice, const Rotation rotation)
{
    for (int i = 1; i <= rotation; ++i)
    {
        switch (axis)
        {
            case TopBottom:
            {
                switch (slice)
                {
                    case TopSlice:
                    {
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[TopRightFrontEdge];
                        piece_map_.edge_map_[TopRightFrontEdge] = piece_map_.edge_map_[TopRightBackEdge];
                        piece_map_.edge_map_[TopRightBackEdge] = piece_map_.edge_map_[TopLeftBackEdge];
                        piece_map_.edge_map_[TopLeftBackEdge] = piece_map_.edge_map_[TopLeftFrontEdge];
                        piece_map_.edge_map_[TopLeftFrontEdge] = temp_edge;
                        // Corners
                        CornerPieces temp_corner = piece_map_.corner_map_[TopLeft];
                        piece_map_.corner_map_[TopLeft] = piece_map_.corner_map_[TopFront];
                        piece_map_.corner_map_[TopFront] = piece_map_.corner_map_[TopRight];
                        piece_map_.corner_map_[TopRight] = piece_map_.corner_map_[TopBack];
                        piece_map_.corner_map_[TopBack] = temp_corner;
                    }
                    break;
                    case MiddleSlice:
                    {
                        // Centres
                        Faces temp_face = piece_map_.centre_map_[RightFrontFace];
                        piece_map_.centre_map_[RightFrontFace] = piece_map_.centre_map_[RightBackFace];
                        piece_map_.centre_map_[RightBackFace] = piece_map_.centre_map_[LeftBackFace];
                        piece_map_.centre_map_[LeftBackFace] = piece_map_.centre_map_[LeftFrontFace];
                        piece_map_.centre_map_[LeftFrontFace] = temp_face;
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[MiddleLeftEdge];
                        piece_map_.edge_map_[MiddleLeftEdge] = piece_map_.edge_map_[MiddleFrontEdge];
                        piece_map_.edge_map_[MiddleFrontEdge] = piece_map_.edge_map_[MiddleRightEdge];
                        piece_map_.edge_map_[MiddleRightEdge] = piece_map_.edge_map_[MiddleBackEdge];
                        piece_map_.edge_map_[MiddleBackEdge] = temp_edge;
                    }
                    break;
                    case BottomSlice:
                    {
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[BottomRightFrontEdge];
                        piece_map_.edge_map_[BottomRightFrontEdge] = piece_map_.edge_map_[BottomRightBackEdge];
                        piece_map_.edge_map_[BottomRightBackEdge] = piece_map_.edge_map_[BottomLeftBackEdge];
                        piece_map_.edge_map_[BottomLeftBackEdge] = piece_map_.edge_map_[BottomLeftFrontEdge];
                        piece_map_.edge_map_[BottomLeftFrontEdge] = temp_edge;
                        // Corners
                        CornerPieces temp_corner = piece_map_.corner_map_[BottomLeft];
                        piece_map_.corner_map_[BottomLeft] = piece_map_.corner_map_[BottomFront];
                        piece_map_.corner_map_[BottomFront] = piece_map_.corner_map_[BottomRight];
                        piece_map_.corner_map_[BottomRight] = piece_map_.corner_map_[BottomBack];
                        piece_map_.corner_map_[BottomBack] = temp_corner;
                    }
                    break;
                }
            }
            break;
            case RightFrontLeftBack:
            {
                switch (slice)
                {
                    case TopSlice:
                    {
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[TopRightFrontEdge];
                        piece_map_.edge_map_[TopRightFrontEdge] = piece_map_.edge_map_[MiddleFrontEdge];
                        piece_map_.edge_map_[MiddleFrontEdge] = piece_map_.edge_map_[BottomRightFrontEdge];
                        piece_map_.edge_map_[BottomRightFrontEdge] = piece_map_.edge_map_[MiddleRightEdge];
                        piece_map_.edge_map_[MiddleRightEdge] = temp_edge;
                        // Corners
                        CornerPieces temp_corner = piece_map_.corner_map_[TopFront];
                        piece_map_.corner_map_[TopFront] = piece_map_.corner_map_[BottomFront];
                        piece_map_.corner_map_[BottomFront] = piece_map_.corner_map_[BottomRight];
                        piece_map_.corner_map_[BottomRight] = piece_map_.corner_map_[TopRight];
                        piece_map_.corner_map_[TopRight] = temp_corner;
                    }
                    break;
                    case MiddleSlice:
                    {
                        // Centres
                        Faces temp_face = piece_map_.centre_map_[TopFace];
                        piece_map_.centre_map_[TopFace] = piece_map_.centre_map_[LeftFrontFace];
                        piece_map_.centre_map_[LeftFrontFace] = piece_map_.centre_map_[BottomFace];
                        piece_map_.centre_map_[BottomFace] = piece_map_.centre_map_[RightBackFace];
                        piece_map_.centre_map_[RightBackFace] = temp_face;
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[TopLeftFrontEdge];
                        piece_map_.edge_map_[TopLeftFrontEdge] = piece_map_.edge_map_[BottomLeftFrontEdge];
                        piece_map_.edge_map_[BottomLeftFrontEdge] = piece_map_.edge_map_[BottomRightBackEdge];
                        piece_map_.edge_map_[BottomRightBackEdge] = piece_map_.edge_map_[TopRightBackEdge];
                        piece_map_.edge_map_[TopRightBackEdge] = temp_edge;
                    }
                    break;
                    case BottomSlice:
                    {
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[TopLeftBackEdge];
                        piece_map_.edge_map_[TopLeftBackEdge] = piece_map_.edge_map_[MiddleLeftEdge];
                        piece_map_.edge_map_[MiddleLeftEdge] = piece_map_.edge_map_[BottomLeftBackEdge];
                        piece_map_.edge_map_[BottomLeftBackEdge] = piece_map_.edge_map_[MiddleBackEdge];
                        piece_map_.edge_map_[MiddleBackEdge] = temp_edge;
                        // Corners
                        CornerPieces temp_corner = piece_map_.corner_map_[TopLeft];
                        piece_map_.corner_map_[TopLeft] = piece_map_.corner_map_[BottomLeft];
                        piece_map_.corner_map_[BottomLeft] = piece_map_.corner_map_[BottomBack];
                        piece_map_.corner_map_[BottomBack] = piece_map_.corner_map_[TopBack];
                        piece_map_.corner_map_[TopBack] = temp_corner;
                    }
                    break;
                }
            }
            break;
            case LeftFrontRightBack:
            {
                switch (slice)
                {
                    case TopSlice:
                    {
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[TopLeftFrontEdge];
                        piece_map_.edge_map_[TopLeftFrontEdge] = piece_map_.edge_map_[MiddleLeftEdge];
                        piece_map_.edge_map_[MiddleLeftEdge] = piece_map_.edge_map_[BottomLeftFrontEdge];
                        piece_map_.edge_map_[BottomLeftFrontEdge] = piece_map_.edge_map_[MiddleFrontEdge];
                        piece_map_.edge_map_[MiddleFrontEdge] = temp_edge;
                        // Corners
                        CornerPieces temp_corner = piece_map_.corner_map_[TopFront];
                        piece_map_.corner_map_[TopFront] = piece_map_.corner_map_[TopLeft];
                        piece_map_.corner_map_[TopLeft] = piece_map_.corner_map_[BottomLeft];
                        piece_map_.corner_map_[BottomLeft] = piece_map_.corner_map_[BottomFront];
                        piece_map_.corner_map_[BottomFront] = temp_corner;
                    }
                    break;
                    case MiddleSlice:
                    {
                        // Centres
                        Faces temp_face = piece_map_.centre_map_[TopFace];
                        piece_map_.centre_map_[TopFace] = piece_map_.centre_map_[LeftBackFace];
                        piece_map_.centre_map_[LeftBackFace] = piece_map_.centre_map_[BottomFace];
                        piece_map_.centre_map_[BottomFace] = piece_map_.centre_map_[RightFrontFace];
                        piece_map_.centre_map_[RightFrontFace] = temp_face;
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[TopLeftBackEdge];
                        piece_map_.edge_map_[TopLeftBackEdge] = piece_map_.edge_map_[BottomLeftBackEdge];
                        piece_map_.edge_map_[BottomLeftBackEdge] = piece_map_.edge_map_[BottomRightFrontEdge];
                        piece_map_.edge_map_[BottomRightFrontEdge] = piece_map_.edge_map_[TopRightFrontEdge];
                        piece_map_.edge_map_[TopRightFrontEdge] = temp_edge;
                    }
                    break;
                    case BottomSlice:
                    {
                        // Edges
                        EdgePieces temp_edge = piece_map_.edge_map_[TopRightBackEdge];
                        piece_map_.edge_map_[TopRightBackEdge] = piece_map_.edge_map_[MiddleBackEdge];
                        piece_map_.edge_map_[MiddleBackEdge] = piece_map_.edge_map_[BottomRightBackEdge];
                        piece_map_.edge_map_[BottomRightBackEdge] = piece_map_.edge_map_[MiddleRightEdge];
                        piece_map_.edge_map_[MiddleRightEdge] = temp_edge;
                        // Corners
                        CornerPieces temp_corner = piece_map_.corner_map_[TopRight];
                        piece_map_.corner_map_[TopRight] = piece_map_.corner_map_[TopBack];
                        piece_map_.corner_map_[TopBack] = piece_map_.corner_map_[BottomBack];
                        piece_map_.corner_map_[BottomBack] = piece_map_.corner_map_[BottomRight];
                        piece_map_.corner_map_[BottomRight] = temp_corner;
                    }
                    break;
                }
            }
            break;
        }
    }
}

void Cube::Face::rotate(const Rotation rotation)
{
    for (int i = 1; i <= rotation; ++i)
    {
        Cube::Colour temp = cell_[TopLeftCorner];
        cell_[TopLeftCorner] = cell_[BottomLeftCorner];
        cell_[BottomLeftCorner] = cell_[BottomRightCorner];
        cell_[BottomRightCorner] = cell_[TopRightCorner];
        cell_[TopRightCorner] = temp;
        temp = cell_[TopEdge];
        cell_[TopEdge] = cell_[LeftEdge];
        cell_[LeftEdge] = cell_[BottomEdge];
        cell_[BottomEdge] = cell_[RightEdge];
        cell_[RightEdge] = temp;
    }
}

void Cube::Face::rotate_inverse(const Rotation rotation)
{
    Rotation r = rotation;
    if (r == Quarter)
        r = ThreeQuarters;
    else if (r == ThreeQuarters)
        r = Quarter;
    rotate(r);
}

int Cube::Face::count_corners(const Colour& colour) const
{
    int count = 0;
    if (cell_[TopLeftCorner] == colour) ++count;
    if (cell_[TopRightCorner] == colour) ++count;
    if (cell_[BottomLeftCorner] == colour) ++count;
    if (cell_[BottomRightCorner] == colour) ++count;
    return count;
}

#define newSliceMove(axis, slice, rotation) new SliceMove(axis, slice, rotation, __LINE__)
#define newRotateMove(axis, rotation) new RotateMove(axis, rotation, __LINE__)

void Cube::randomize(move_sequence_type& moves)
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> axis_dist(0, NumberOfAxes - 1);
    std::uniform_int_distribution<int> slice_dist(0, NumberOfSlices - 1);
    std::uniform_int_distribution<int> rotation_dist(1, NumberOfRotations);
    std::uniform_int_distribution<int> count_dist(20, 119);
    int count = count_dist(rng);
    for (int i = 0; i < count; ++i)
    {
        Axis axis = static_cast<Axis>(axis_dist(rng));
        Slice slice = static_cast<Slice>(slice_dist(rng));
        Rotation rotation = static_cast<Rotation>(rotation_dist(rng));
        Move* move = newSliceMove(axis, slice, rotation);
        moves.push_back(move);
    }
}

void Cube::randomize()
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> axis_dist(0, NumberOfAxes - 1);
    std::uniform_int_distribution<int> slice_dist(0, NumberOfSlices - 1);
    std::uniform_int_distribution<int> rotation_dist(1, NumberOfRotations);
    std::uniform_int_distribution<int> count_dist(20, 119);
    int count = count_dist(rng);
    for (int i = 0; i < count; ++i)
    {
        Axis axis = static_cast<Axis>(axis_dist(rng));
        Slice slice = static_cast<Slice>(slice_dist(rng));
        Rotation rotation = static_cast<Rotation>(rotation_dist(rng));
        move_slice(axis, slice, rotation);
    }
}

void Cube::solve(Cube::move_sequence_type& moves)
{
    try 
    {
        Cube cube(*this);
        cube.sort_out_corners(moves);
        cube.sort_out_opposite_faces(moves);
        cube.sort_out_centre_slice(moves);
    }
    catch (const CubeException& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Cube::add_move_to_solution(Cube::Move* move, Cube::move_sequence_type& moves)
{
    moves.push_back(move);
    move->perform(*this);
}

void Cube::find_edge_cells(const Colour& colour, std::vector<Cube::EdgePieces>& edge_cells) const
{
    edge_cells.clear();
    for (int e = TopRightFrontEdge; e < NumberOfEdgePieces; ++e)
    {
        if (pieces_.edges_[e].colour_[0] == colour || pieces_.edges_[e].colour_[1] == colour)
        {
			edge_cells.push_back(static_cast<Cube::EdgePieces>(e));
        }
    }
}

void Cube::find_corner_cells(const Colour& colour1, const Colour& colour2,
                             std::set<std::pair<Faces, Cells> >& corner_cells,
                             std::map<Faces, int>& face_counts) const
{
    // Find where all our corner cells are
    static Cells corners[] = { TopLeftCorner, TopRightCorner, BottomLeftCorner, BottomRightCorner };
    static size_t number_of_corners = sizeof(corners) / sizeof(Cells);
    for (int f = TopFace; f < NumberOfFaces; ++f)
    {
        face_counts[static_cast<Faces>(f)] = 0;
        for (size_t c = 0; c < number_of_corners; ++c)
        {
            if (faces_[f].cell_[corners[c]] == colour1 ||
                    faces_[f].cell_[corners[c]] == colour2)
            {
                corner_cells.insert(std::make_pair(static_cast<Faces>(f), corners[c]));
                face_counts[static_cast<Faces>(f)]++;
            }
        }
    }
}

void Cube::swap_two_adjacent_corners(const Faces& face, const Cells& left_corner, Cube::move_sequence_type& moves)
{
    if (face == BottomFace) return; // For the moment
    // move face and corners into position
    switch (face)
    {
        case TopFace:
        {
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                    add_move_to_solution(newRotateMove(TopBottom, Quarter), moves);
                }
                break;
                case TopRightCorner:
                {
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(TopBottom, Half), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(TopBottom, ThreeQuarters), moves);
                }
                break;
            }
            add_move_to_solution(newRotateMove(RightFrontLeftBack, ThreeQuarters), moves);
        }
        break;
        case RightFrontFace:
        {
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, ThreeQuarters), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Quarter), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Half), moves);
                }
                break;
            }
            add_move_to_solution(newRotateMove(TopBottom, Quarter), moves);
        }
        break;
        case LeftFrontFace:
        {
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, ThreeQuarters), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Quarter), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Half), moves);
                }
                break;
            }
        }
        break;
        case LeftBackFace:
        {
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Quarter), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, ThreeQuarters), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Half), moves);
                }
                break;
            }
            add_move_to_solution(newRotateMove(TopBottom, ThreeQuarters), moves);
        }
        break;
        case RightBackFace:
        {
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Quarter), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, ThreeQuarters), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Half), moves);
                }
                break;
            }
            add_move_to_solution(newRotateMove(TopBottom, Half), moves);
        }
        break;
        case BottomFace:
        {
            // ??? NOT YET IMPLEMENTED ???
        }
        break;
    }

    // swap top left and top right corners of left front face
    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, ThreeQuarters), moves);
    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);

    // reverse original move
    switch (face)
    {
        case TopFace:
        {
            add_move_to_solution(newRotateMove(RightFrontLeftBack, Quarter), moves);
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                    add_move_to_solution(newRotateMove(TopBottom, ThreeQuarters), moves);
                }
                break;
                case TopRightCorner:
                {
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(TopBottom, Half), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(TopBottom, Quarter), moves);
                }
                break;
            }
        }
        break;
        case RightFrontFace:
        {
            add_move_to_solution(newRotateMove(TopBottom, ThreeQuarters), moves);
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Quarter), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, ThreeQuarters), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Half), moves);
                }
                break;
            }
        }
        break;
        case LeftFrontFace:
        {
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Quarter), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, ThreeQuarters), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Half), moves);
                }
                break;
            }
        }
        break;
        case LeftBackFace:
        {
            add_move_to_solution(newRotateMove(TopBottom, Quarter), moves);
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, ThreeQuarters), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Quarter), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(RightFrontLeftBack, Half), moves);
                }
                break;
            }
        }
        break;
        case RightBackFace:
        {
            add_move_to_solution(newRotateMove(TopBottom, Half), moves);
            switch (left_corner)
            {
                case TopLeftCorner:
                {
                }
                break;
                case TopRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, ThreeQuarters), moves);
                }
                break;
                case BottomLeftCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Quarter), moves);
                }
                break;
                case BottomRightCorner:
                {
                    add_move_to_solution(newRotateMove(LeftFrontRightBack, Half), moves);
                }
                break;
            }
        }
        break;
        case BottomFace:
        {
            // ??? NOT YET IMPLEMENTED ???
        }
        break;
    }
}

void Cube::three_on_each_side(const Cube::Colour& rff_colour,
                              const Cube::Colour& lbf_colour,
                              Cube::move_sequence_type& moves)
{
    while (faces_[RightFrontFace].cell_[TopLeftCorner] == rff_colour ||
            faces_[RightFrontFace].cell_[TopLeftCorner] == lbf_colour)
    {
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
    }
    while (faces_[LeftBackFace].cell_[TopRightCorner] == rff_colour ||
            faces_[LeftBackFace].cell_[TopRightCorner] == lbf_colour)
    {
        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
    }
    if (faces_[LeftFrontFace].cell_[TopRightCorner] != rff_colour &&
            faces_[LeftFrontFace].cell_[TopRightCorner] != lbf_colour)
    {
        add_move_to_solution(newRotateMove(LeftFrontRightBack, Half), moves);
        add_move_to_solution(newRotateMove(RightFrontLeftBack, Quarter), moves);
    }
    swap_two_adjacent_corners(LeftFrontFace, TopLeftCorner, moves);
    if (faces_[RightFrontFace].cell_[Centre] != rff_colour)
    {
        add_move_to_solution(newRotateMove(LeftFrontRightBack, Half), moves);
    }
}

void Cube::sort_out_corners(Cube::move_sequence_type& moves)
{
    // Pick two opposite faces and count how many corner cells are correct or opposite colour

    Colour rff_colour = faces_[RightFrontFace].cell_[Centre];
    Colour lbf_colour = faces_[LeftBackFace].cell_[Centre];

    // Get 4 corners on right front face correct or opposite colour
    int rff_corners = faces_[RightFrontFace].count_corners(rff_colour) + faces_[RightFrontFace].count_corners(lbf_colour);
    int case_count[4];
    for (int i = 0; i < 4; i++) case_count[i] = 0;

    while (rff_corners < 4)
    {
        std::set<std::pair<Faces, Cells> > corner_cells;
        std::map<Faces, int> face_counts;
        find_corner_cells(rff_colour, lbf_colour, corner_cells, face_counts);

        case_count[rff_corners]++;
        if (case_count[rff_corners] > 6)
        {
            // stuck in a loop, so escape
            return;
        }
        switch (rff_corners)
        {
            case 0:
            {
                if (face_counts[TopFace] > 0)
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end() ||
                            corner_cells.find(std::make_pair<Faces, Cells>(TopFace, BottomRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end() ||
                             corner_cells.find(std::make_pair<Faces, Cells>(TopFace, BottomLeftCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Quarter), moves);
                    }
                }
                else if (face_counts[LeftFrontFace] > 0)
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, TopLeftCorner)) != corner_cells.end() ||
                            corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, TopRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomLeftCorner)) != corner_cells.end() ||
                             corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(TopBottom, BottomSlice, ThreeQuarters), moves);
                    }
                }
            }
            break;

            case 1:
            {
                // make sure the 1 correct colour is in the top left corner
                if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, BottomLeftCorner)) != corner_cells.end())
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                    break;
                }
                if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, BottomRightCorner)) != corner_cells.end())
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Half), moves);
                    break;
                }
                if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopRightCorner)) != corner_cells.end())
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, ThreeQuarters), moves);
                    break;
                }

                if (face_counts[TopFace] > 0)
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end() ||
                            corner_cells.find(std::make_pair<Faces, Cells>(TopFace, BottomRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, BottomSlice, ThreeQuarters), moves);
                    }
                }
                else if (face_counts[LeftFrontFace] > 0)
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomLeftCorner)) != corner_cells.end() ||
                            corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(TopBottom, BottomSlice, ThreeQuarters), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, TopLeftCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
                    }
                }
                else if (face_counts[LeftBackFace] > 0)
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomLeftCorner)) != corner_cells.end() ||
                            corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() ||
                             corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                    }
                }
                else if (face_counts[RightBackFace] > 0)
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, BottomLeftCorner)) != corner_cells.end() ||
                            corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, BottomRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Quarter), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopLeftCorner)) != corner_cells.end() ||
                             corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Quarter), moves);
                    }
                }

            }
            break;

            case 2:
            {
                // make sure the 2 correct colours are in the top left corner and either bottom left or bottom right
                if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopRightCorner)) != corner_cells.end())
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopLeftCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, ThreeQuarters), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, BottomLeftCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, ThreeQuarters), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, BottomRightCorner)) != corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                    }
                    break;
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopLeftCorner)) == corner_cells.end())
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                    break;
                }

                // find another corner
                if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, BottomLeftCorner)) != corner_cells.end())
                {
                    // x . .
                    // . . .
                    // x . .
                    if (face_counts[TopFace] > 0)
                    {
                        if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end() ||
                                corner_cells.find(std::make_pair<Faces, Cells>(TopFace, BottomRightCorner)) != corner_cells.end())
                        {
                            add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
                        }
                        else
                        {
                            add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Half), moves);
                            add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Quarter), moves);
                        }
                    }
                    else if (face_counts[LeftFrontFace] > 0)
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
                    }
                    else if (face_counts[LeftBackFace] > 0)
                    {
                        if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() ||
                                corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomLeftCorner)) != corner_cells.end())
                        {
                            add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                        }
                        else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end() ||
                                 corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomRightCorner)) != corner_cells.end())
                        {
                            add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                            add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                        }
                    }
                    else if (face_counts[RightBackFace] > 0)
                    {
                        if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopLeftCorner)) != corner_cells.end() ||
                                corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, BottomLeftCorner)) != corner_cells.end())
                        {
                            add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                        }
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
                    }
                }
                else
                {
                    // x . .
                    // . . .
                    // . . x
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                }
            }
            break;

            case 3:
            {
                // make sure the 1 incorrect colour is in the top right corner
                if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopRightCorner)) != corner_cells.end())
                {
                    if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopLeftCorner)) == corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, BottomLeftCorner)) == corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Half), moves);
                    }
                    else if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, BottomRightCorner)) == corner_cells.end())
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, ThreeQuarters), moves);
                    }
                    break;
                }

                // x . .
                // . . .
                // x . x
                if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, BottomRightCorner)) != corner_cells.end())
                {
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, ThreeQuarters), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopLeftCorner)) != corner_cells.end())
                {
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, ThreeQuarters), moves);
                }
            }
            break;

        }
        rff_corners = faces_[RightFrontFace].count_corners(rff_colour) + faces_[RightFrontFace].count_corners(lbf_colour);
    }

    // Now get 4 correct colours on left back face
    int lbf_corners = faces_[LeftBackFace].count_corners(rff_colour) + faces_[LeftBackFace].count_corners(lbf_colour);
    for (int i = 0; i < 4; i++) case_count[i] = 0;
    while (lbf_corners < 4)
    {
        std::set<std::pair<Faces, Cells> > corner_cells;
        std::map<Faces, int> face_counts;
        find_corner_cells(rff_colour, lbf_colour, corner_cells, face_counts);

        case_count[lbf_corners]++;
        if (case_count[lbf_corners] > 6)
        {
            // stuck in a loop, so escape
            return;
        }
        switch (lbf_corners)
        {
            case 0:
            {
                if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end() &&
                        corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end() &&
                        corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomLeftCorner)) != corner_cells.end() &&
                        corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, BottomRightCorner)) != corner_cells.end())
                {
                    // Case 1
                    /*
                      v   v
                      . . .
                      . . .
                     >. . .<
                     */
                    // => Case 2
                    swap_two_adjacent_corners(TopFace, TopLeftCorner, moves);

                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, BottomRightCorner)) != corner_cells.end())
                {
                    // Case 2
                    /*
                     >. . .<
                      . . .
                     >. . .<
                     */
                    swap_two_adjacent_corners(LeftBackFace, TopLeftCorner, moves); // => 3 correct on each side
                    swap_two_adjacent_corners(RightBackFace, BottomRightCorner, moves); // => 4 correct on each side
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopRightCorner)) != corner_cells.end())
                {
                    // Case 3
                    /*
                     >. . .<
                      . . .
                      . . .
                      ^   ^
                     */
                    // => Case 1
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                }
                else
                {
                    // Case 4
                    /*
                      v   v
                      . . .
                      . . .
                      . . .
                      ^   ^
                     */
                    // => Case 2
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                break;
            }
            break;
            case 1:
            {
                if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomLeftCorner)) != corner_cells.end() &&
                        corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end())
                {
                    // Case 1
                    /*
                      v
                      . . .<
                      . . .
                     >. . .
                     */
                    // DEBUGGING
                    swap_two_adjacent_corners(LeftBackFace, TopLeftCorner, moves);
                    // should be 3 correct colours on each side
                    three_on_each_side(rff_colour, lbf_colour, moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end())
                {
                    // Case 2
                    /*
                      v
                      . . .<
                      . . .
                      . . .
                          ^
                     */
                    // => Case 1
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomLeftCorner)) != corner_cells.end())
                {
                    // Case 3
                    /*

                      . . .<
                      . . .
                    > . . .
                          ^
                     */
                    // => Case 1
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end())
                {
                    // Case 4
                    /*
                      v
                      . . .
                      . . .
                    > . . .
                          ^
                     */
                    // => Case 1
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end())
                {
                    // Case 5
                    /*
                          v
                      . . .
                      . . .
                      . . .<
                      ^
                     */
                    swap_two_adjacent_corners(LeftBackFace, BottomRightCorner, moves);
                    // should be 3 correct colours on each side
                    three_on_each_side(rff_colour, lbf_colour, moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopLeftCorner)) != corner_cells.end())
                {
                    // Case 6
                    /*
                     >. . .
                      . . .
                      . . .<
                      ^
                     */
                    // => Case 5
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end())
                {
                    // Case 7
                    /*
                          v
                     >. . .
                      . . .
                      . . .
                      ^
                     */
                    // => Case 5
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end())
                {
                    // Case 8
                    /*
                          v
                     >. . .
                      . . .
                      . . .<
                     */
                    // => Case 5
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
            }
            break;
            case 2:
            {
                if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end() &&
                        corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end() &&
                        corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomRightCorner)) != corner_cells.end())
                {
                    // Case 1
                    /*
                          v
                      . . .
                      . . .
                      . . .
                          ^
                     */
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    three_on_each_side(rff_colour, lbf_colour, moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, BottomRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomLeftCorner)) != corner_cells.end())
                {
                    // Case 2
                    /*

                      . . .
                      . . .
                     >. . .<
                     */
                    // => Case 1
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomLeftCorner)) != corner_cells.end())
                {
                    // Case 3
                    /*
                      v
                      . . .
                      . . .
                      . . .
                      ^
                     */
                    // => Case 1
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, TopLeftCorner)) != corner_cells.end())
                {
                    // Case 4
                    /*
                     >. . .<
                      . . .
                      . . .
                     */
                    // => Case 1
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, BottomRightCorner)) != corner_cells.end())
                {
                    // Case 5
                    /*
                      . . .<
                      . . .
                      . . .<
                     */
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    three_on_each_side(rff_colour, lbf_colour, moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftFrontFace, BottomLeftCorner)) != corner_cells.end())
                {
                    // Case 6
                    /*
                     >. . .
                      . . .
                     >. . .
                     */
                    // => Case 5
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end())
                {
                    // Case 7
                    /*
                      v   v
                      . . .
                      . . .
                      . . .
                     */
                    // => Case 5
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end())
                {
                    // Case 8
                    /*
                      . . .
                      . . .
                      . . .
                      ^   ^
                     */
                    // => Case 5
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(RightBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomLeftCorner)) != corner_cells.end())
                {
                    // Case 9
                    /*
                      . . .<
                      . . .
                      . . .
                      ^
                     */
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    three_on_each_side(rff_colour, lbf_colour, moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomRightCorner)) != corner_cells.end())
                {
                    // Case 10
                    /*
                      v
                      . . .
                      . . .
                      . . .<
                     */
                    // => Case 9
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(TopFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopRightCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomLeftCorner)) != corner_cells.end())
                {
                    // Case 11
                    /*
                          v
                      . . .
                      . . .
                     >. . .
                     */
                    // => Case 9
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
                }
                else if (corner_cells.find(std::make_pair<Faces, Cells>(RightFrontFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, TopLeftCorner)) != corner_cells.end() &&
                         corner_cells.find(std::make_pair<Faces, Cells>(LeftBackFace, BottomRightCorner)) != corner_cells.end())
                {
                    // Case 12
                    /*
                     >. . .
                      . . .
                      . . .
                          ^
                     */
                    // => Case 9
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                }
            }
            break;
            case 3:
            {
                // Impossible!
                return;
            }
            break;
        }
        lbf_corners = faces_[LeftBackFace].count_corners(rff_colour) + faces_[LeftBackFace].count_corners(lbf_colour);
    }

    // Now the RightFrontFace and the LeftBackFace have corners which are one of the two correct colours
    // We need to get the correct colours on the correct faces
    rff_corners = faces_[RightFrontFace].count_corners(rff_colour);
    for (int i = 0; i < 4; i++) case_count[i] = 0;
    while (rff_corners < 4)
    {
        std::set<std::pair<Faces, Cells> > corner_cells;
        std::map<Faces, int> face_counts;
        find_corner_cells(rff_colour, lbf_colour, corner_cells, face_counts);

        case_count[rff_corners]++;
        if (case_count[rff_corners] > 6)
        {
            // stuck in a loop, so escape
            return;
        }
        switch (rff_corners)
        {
            case 0:
            {
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
            }
            break;

            case 1:
            {
                while (faces_[RightFrontFace].cell_[TopLeftCorner] != rff_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                while (faces_[LeftBackFace].cell_[TopLeftCorner] != lbf_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                /*
                 x . y   y . x
                 . x .   . y .
                 y . y   x . x
                 */
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                /*
                 x . x   y . x
                 . x .   . y .
                 y . y   y . x
                 */
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                /*
                 x . x   y . y
                 . x .   . y .
                 y . y   x . x
                 */
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
            }
            break;

            case 2:
            {

                while (faces_[LeftBackFace].cell_[TopLeftCorner] != lbf_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                if (faces_[LeftBackFace].cell_[BottomLeftCorner] == lbf_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, ThreeQuarters), moves);
                }
                while (faces_[RightFrontFace].cell_[TopLeftCorner] != rff_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                if (faces_[RightFrontFace].cell_[BottomLeftCorner] == rff_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }

                if (faces_[LeftBackFace].cell_[TopRightCorner] == lbf_colour &&
                        faces_[RightFrontFace].cell_[TopRightCorner] == rff_colour)
                {
                    // case a:
                    /*
                     x . x   y . y
                     . x .   . y .
                     y . y   x . x
                     */
                    // !!!
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                }
                else if (faces_[LeftBackFace].cell_[TopRightCorner] == lbf_colour &&
                         faces_[RightFrontFace].cell_[BottomRightCorner] == rff_colour)
                {
                    // case b:
                    /*
                     x . x   y . x
                     . x .   . y .
                     y . y   x . y
                     */
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                    /*
                     x . x   y . x
                     . x .   . y .
                     x . y   y . y
                     */
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, ThreeQuarters), moves);
                    /*
                     x . x   x . y
                     . x .   . y .
                     x . y   y . y
                     */
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                    /*
                     x . x   x . y
                     . x .   . y .
                     y . y   x . y
                     */
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, ThreeQuarters), moves);
                    /*
                     x . x   y . y
                     . x .   . y .
                     y . y   x . x
                     */
                    // !!!
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                }
                else if (faces_[LeftBackFace].cell_[BottomRightCorner] == lbf_colour &&
                         faces_[RightFrontFace].cell_[BottomRightCorner] == rff_colour)
                {
                    // case c:
                    /*
                     x . y   y . x
                     . x .   . y .
                     y . x   x . y
                     */
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                    /*
                     x . y   y . x
                     . x .   . y .
                     x . y   y . x
                     */
                    // !!!!
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                }
                else if (faces_[LeftBackFace].cell_[TopRightCorner] == rff_colour &&
                         faces_[RightFrontFace].cell_[BottomRightCorner] == lbf_colour)
                {
                    // case d:
                    /*
                     x . y   y . y
                     . x .   . y .
                     y . x   x . x
                     */
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                    /*
                     x . y   y . y
                     . x .   . y .
                     x . x   y . x
                     */
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                    /*
                     x . y   y . y
                     . x .   . y .
                     x . x   x . y
                     */
                    add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                    /*
                     x . y   y . y
                     . x .   . y .
                     x . y   x . x
                     */
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                    /*
                     x . y   x . y
                     . x .   . y .
                     x . y   x . y
                     */
                    add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                }
            }
            break;

            case 3:
            {
                while (faces_[LeftBackFace].cell_[TopLeftCorner] == lbf_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                while (faces_[RightFrontFace].cell_[TopLeftCorner] == rff_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                /*
                 y . x   x . y
                 . x .   . y .
                 x . x   y . y
                 */
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                /*
                 y . x   x . x
                 . x .   . y .
                 y . x   y . y
                 */
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                /*
                 y . x   y . x
                 . x .   . y .
                 y . x   y . x
                 */
                // !!!!
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
            }
            break;
        }

        rff_corners = faces_[RightFrontFace].count_corners(rff_colour);
    }

    // Now all the corners are the correct colour on the RightFrontFace and the LeftBackFace, but not
    // necessarily in the correct order
    int rff_corner_pairs = 0;
    if (faces_[TopFace].cell_[BottomLeftCorner] == faces_[TopFace].cell_[BottomRightCorner])
    {
        rff_corner_pairs++;
    }
    if (faces_[LeftFrontFace].cell_[BottomRightCorner] == faces_[LeftFrontFace].cell_[TopRightCorner])
    {
        rff_corner_pairs++;
    }
    if (faces_[RightBackFace].cell_[BottomLeftCorner] == faces_[RightBackFace].cell_[TopLeftCorner])
    {
        rff_corner_pairs++;
    }
    if (faces_[BottomFace].cell_[TopLeftCorner] == faces_[BottomFace].cell_[TopRightCorner])
    {
        rff_corner_pairs++;
    }
    int lbf_corner_pairs = 0;
    if (faces_[TopFace].cell_[TopLeftCorner] == faces_[TopFace].cell_[TopRightCorner])
    {
        lbf_corner_pairs++;
    }
    if (faces_[LeftFrontFace].cell_[BottomLeftCorner] == faces_[LeftFrontFace].cell_[TopLeftCorner])
    {
        lbf_corner_pairs++;
    }
    if (faces_[RightBackFace].cell_[BottomRightCorner] == faces_[RightBackFace].cell_[TopRightCorner])
    {
        lbf_corner_pairs++;
    }
    if (faces_[BottomFace].cell_[BottomLeftCorner] == faces_[BottomFace].cell_[BottomRightCorner])
    {
        lbf_corner_pairs++;
    }
    if (rff_corner_pairs == 0 && lbf_corner_pairs == 0)
    {
        swap_two_adjacent_corners(TopFace, BottomRightCorner, moves);
        while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[BottomRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        swap_two_adjacent_corners(TopFace, BottomRightCorner, moves);
        while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[BottomRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Half), moves);
        swap_two_adjacent_corners(TopFace, BottomRightCorner, moves);
        //swap_two_adjacent_corners(LeftFrontFace, TopRightCorner, moves);
        //swap_two_adjacent_corners(RightBackFace, BottomLeftCorner, moves);
    }
    else if (rff_corner_pairs == 0 && lbf_corner_pairs == 1)
    {
        // Need to swap two corners on the LeftBackFace
        while (faces_[TopFace].cell_[TopLeftCorner] != faces_[TopFace].cell_[TopRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
        swap_two_adjacent_corners(TopFace, TopLeftCorner, moves);
    }
    else if (rff_corner_pairs == 1 && lbf_corner_pairs == 0)
    {
        // Need to swap two corners on the RightFrontFace
        while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[BottomRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Half), moves);
        swap_two_adjacent_corners(TopFace, BottomRightCorner, moves);
    }
    else if (rff_corner_pairs == 1 && lbf_corner_pairs == 1)
    {
        // Need to swap one pair of corners on both LeftBackFace and RightFrontFace
        while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[BottomRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        }
        swap_two_adjacent_corners(TopFace, BottomRightCorner, moves);
        while (faces_[TopFace].cell_[TopLeftCorner] != faces_[TopFace].cell_[TopRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
        swap_two_adjacent_corners(TopFace, TopLeftCorner, moves);
    }
    else if (rff_corner_pairs == 4 && lbf_corner_pairs == 0)
    {
        // Need to swap both pairs of corners on LeftBackFace
        swap_two_adjacent_corners(LeftFrontFace, BottomLeftCorner, moves);
        swap_two_adjacent_corners(RightBackFace, TopRightCorner, moves);
    }
    else if (rff_corner_pairs == 0 && lbf_corner_pairs == 4)
    {
        // Need to swap both pairs of corners on RightFrontFace
        swap_two_adjacent_corners(LeftFrontFace, TopRightCorner, moves);
        swap_two_adjacent_corners(RightBackFace, BottomLeftCorner, moves);
    }
    else if (rff_corner_pairs == 4 && lbf_corner_pairs == 1)
    {
        // Need to permute three corners on LeftBackFace
        while (faces_[TopFace].cell_[TopLeftCorner] != faces_[TopFace].cell_[TopRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
        swap_two_adjacent_corners(TopFace, TopLeftCorner, moves);
        while (faces_[TopFace].cell_[TopLeftCorner] != faces_[TopFace].cell_[TopRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Half), moves);
        swap_two_adjacent_corners(TopFace, TopLeftCorner, moves);
    }
    else if (rff_corner_pairs == 1 && lbf_corner_pairs == 4)
    {
        // Need to permute three corners on RightFrontFace
        while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[BottomRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        swap_two_adjacent_corners(TopFace, BottomRightCorner, moves);
        while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[BottomRightCorner])
        {
            add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
        }
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Half), moves);
        swap_two_adjacent_corners(TopFace, BottomRightCorner, moves);
    }

    // Line up corners correctly
    while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[Centre])
    {
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
    }
    while (faces_[TopFace].cell_[TopLeftCorner] != faces_[TopFace].cell_[Centre])
    {
        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
    }

    if (!check_corners())
    {
        throw CubeException("check_corners() failed", __FILE__, __LINE__);
    }
}

bool Cube::check_corners()
{
    for (int f = TopFace; f < NumberOfFaces; ++f)
    {
        Colour c = faces_[f].cell_[Centre];
        if (c != faces_[f].cell_[TopLeftCorner]) return false;
        if (c != faces_[f].cell_[TopRightCorner]) return false;
        if (c != faces_[f].cell_[BottomLeftCorner]) return false;
        if (c != faces_[f].cell_[BottomRightCorner]) return false;
    }
    return true;
}

bool Cube::check_edge(const EdgePieces& edge) const
{
    switch (edge)
    {
        case TopRightFrontEdge:
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[TopFace].cell_[BottomRightCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[RightFrontFace].cell_[TopLeftCorner]) return true;
        }
        break;
        case TopRightBackEdge:
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[TopFace].cell_[BottomRightCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[RightBackFace].cell_[TopLeftCorner]) return true;
        }
        break;
        case TopLeftFrontEdge:
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[TopFace].cell_[TopLeftCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[LeftFrontFace].cell_[TopLeftCorner]) return true;
        }
        break;
        case TopLeftBackEdge:
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[TopFace].cell_[TopLeftCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[LeftBackFace].cell_[TopLeftCorner]) return true;
        }
        break;
        case MiddleLeftEdge:        
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[LeftBackFace].cell_[TopRightCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[LeftFrontFace].cell_[TopLeftCorner]) return true;
        }
        break;
        case MiddleBackEdge:      
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[LeftBackFace].cell_[TopLeftCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[RightBackFace].cell_[TopRightCorner]) return true;
        }
        break;
        case MiddleRightEdge:     
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[RightFrontFace].cell_[TopRightCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[RightBackFace].cell_[TopLeftCorner]) return true;
        }
        break;
        case MiddleFrontEdge:
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[RightFrontFace].cell_[TopLeftCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[LeftFrontFace].cell_[TopRightCorner]) return true;
        }
        break;
        case BottomRightFrontEdge:  
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[BottomFace].cell_[TopLeftCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[RightFrontFace].cell_[BottomRightCorner]) return true;
        }
        break;
        case BottomRightBackEdge: 
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[BottomFace].cell_[TopRightCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[RightBackFace].cell_[BottomLeftCorner]) return true;
        }
        break;
        case BottomLeftFrontEdge: 
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[BottomFace].cell_[TopLeftCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[LeftFrontFace].cell_[BottomRightCorner]) return true;
        }
        break;
        case BottomLeftBackEdge: 
        {
            if (pieces_.edges_[edge].colour_[0] == faces_[BottomFace].cell_[BottomLeftCorner] &&
                pieces_.edges_[edge].colour_[1] == faces_[LeftBackFace].cell_[BottomRightCorner]) return true;
        }
        break;
    }
    return false;
}

bool Cube::check_edges(const Faces& face) const
{
    switch (face)
    {
        case TopFace:
        {
            return (check_edge(TopRightFrontEdge) && check_edge(TopRightBackEdge) && check_edge(TopLeftBackEdge) && check_edge(TopLeftFrontEdge));
        }
        break;
        case RightFrontFace:
        {
            return (check_edge(TopRightFrontEdge) && check_edge(BottomRightFrontEdge) && check_edge(MiddleFrontEdge) && check_edge(MiddleRightEdge));
        }
        break;
        case LeftFrontFace:
        {
            return (check_edge(TopLeftFrontEdge) && check_edge(BottomLeftFrontEdge) && check_edge(MiddleFrontEdge) && check_edge(MiddleLeftEdge));
        }
        break;
        case RightBackFace:
        {
            return (check_edge(TopRightBackEdge) && check_edge(BottomRightBackEdge) && check_edge(MiddleRightEdge) && check_edge(MiddleBackEdge));
        }
        break;
        case LeftBackFace:
        {
            return (check_edge(TopLeftBackEdge) && check_edge(BottomLeftBackEdge) && check_edge(MiddleLeftEdge) && check_edge(MiddleBackEdge));
        }
        break;
        case BottomFace:
        {
            return (check_edge(BottomRightFrontEdge) && check_edge(BottomRightBackEdge) && check_edge(BottomLeftBackEdge) && check_edge(BottomLeftFrontEdge));
        }
        break;
    }
    return true;
}

std::pair<Cube::Colour, Cube::Colour> Cube::get_edge_colours(const EdgePieces& edge) const
{
    return std::make_pair(pieces_.edges_[edge].colour_[0], pieces_.edges_[edge].colour_[1]);
}

Cube::EdgePieces Cube::find_candidate_edge_cell(const Cube::Faces& face) const
{
    Colour colour = faces_[face].cell_[Centre];
    std::vector<Cube::EdgePieces> edge_cells;
    find_edge_cells(colour, edge_cells);
    for (auto& edge: edge_cells)
    {
        if (!check_edge(edge))
        {
            return edge;
        }
    }
    switch (face)
    {
        case TopFace: return TopRightFrontEdge;
        case BottomFace: return BottomRightFrontEdge;
        case RightFrontFace: return TopRightFrontEdge;
        case RightBackFace: return TopRightBackEdge;
        case LeftFrontFace: return TopLeftFrontEdge;
        case LeftBackFace: return TopLeftBackEdge;
        default:
            throw CubeException("Error: Cube::find_candidate_edge_cell : invalid face", __FILE__, __LINE__);
    }
}

std::pair<Cube::Faces, Cube::Faces> Cube::get_faces(const Cube::EdgePieces& edge) const
{
    switch (edge)
    {
        case TopRightFrontEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(TopFace, RightFrontFace);
        }
        break;
        case TopRightBackEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(TopFace, RightBackFace);
        }
        break;
        case TopLeftFrontEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(TopFace, LeftFrontFace);
        }
        break;
        case TopLeftBackEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(TopFace, LeftBackFace);
        }
        break;
        case MiddleLeftEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(LeftFrontFace, LeftBackFace);
        }
        break;
        case MiddleBackEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(LeftBackFace, RightBackFace);
        }
        break;
        case MiddleRightEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(RightBackFace, RightFrontFace);
        }
        break;
        case MiddleFrontEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(RightFrontFace, LeftFrontFace);
        }
        break;
        case BottomRightFrontEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(BottomFace, RightFrontFace);
        }
        break;
        case BottomRightBackEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(BottomFace, RightBackFace);
        }
        break;
        case BottomLeftFrontEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(BottomFace, LeftFrontFace);
        }
        break;
        case BottomLeftBackEdge:
        {
            return std::make_pair<Cube::Faces, Cube::Faces>(BottomFace, LeftBackFace);
        }
        break;
        default:
        {
            throw CubeException("Error: Cube::get_faces : invalid edge", __FILE__, __LINE__);
        }
    }
}

Cube::Faces Cube::get_other_face(const Cube::Faces& face, const Cube::EdgePieces& edge) const
{
    switch (edge)
    {
        case TopRightFrontEdge:
        {
            return (face == TopFace ? RightFrontFace : face);
        }
        break;
        case TopRightBackEdge:
        {
            return (face == TopFace ? RightBackFace : face);
        }
        break;
        case TopLeftFrontEdge:
        {
            return (face == TopFace ? LeftFrontFace : face);
        }
        break;
        case TopLeftBackEdge:
        {
            return (face == TopFace ? LeftBackFace : face);
        }
        break;
        case MiddleLeftEdge:
        {
            return (face == LeftFrontFace ? LeftBackFace : face);
        }
        break;
        case MiddleBackEdge:
        {
            return (face == LeftBackFace ? RightBackFace : face);
        }
        break;
        case MiddleRightEdge:
        {
            return (face == RightBackFace ? RightFrontFace : face);
        }
        break;
        case MiddleFrontEdge:
        {
            return (face == RightFrontFace ? LeftFrontFace : face);
        }
        break;
        case BottomRightFrontEdge:
        {
            return (face == BottomFace ? RightFrontFace : face);
        }
        break;
        case BottomRightBackEdge:
        {
            return (face == BottomFace ? RightBackFace : face);
        }
        break;
        case BottomLeftFrontEdge:
        {
            return (face == BottomFace ? LeftFrontFace : face);
        }
        break;
        case BottomLeftBackEdge:
        {
            return (face == BottomFace ? LeftBackFace : face);
        }
        break;
        default:
        {
            throw CubeException("Error: Cube::get_other_face : invalid edge", __FILE__, __LINE__);
        }
    }
}

bool Cube::is_middle_slice_edge(const EdgePieces& edge)
{
    if (edge == TopLeftFrontEdge ||
        edge == TopRightBackEdge ||
        edge == BottomLeftFrontEdge ||
        edge == BottomRightBackEdge) return true;
    return false;
}

bool Cube::is_right_front_face_edge(const EdgePieces& edge)
{
    if (edge == TopRightFrontEdge ||
        edge == BottomRightFrontEdge ||
        edge == MiddleFrontEdge ||
        edge == MiddleRightEdge) return true;
    return false;
}

bool Cube::is_left_back_face_edge(const EdgePieces& edge)
{
    if (edge == TopLeftBackEdge ||
        edge == BottomLeftBackEdge ||
        edge == MiddleBackEdge ||
        edge == MiddleLeftEdge) return true;
    return false;
}

void Cube::sort_out_opposite_faces(Cube::move_sequence_type& moves)
{
    Colour rff_colour = faces_[RightFrontFace].cell_[Centre];
    Colour lbf_colour = faces_[LeftBackFace].cell_[Centre];
    int loop_count = 0;
    while (!check_edges(RightFrontFace) || !check_edges(LeftBackFace))
    {
        // Find two edge pieces, preferably not in correct position, with one colour which is rff_colour or lbf_colour
        EdgePieces edge1 = find_candidate_edge_cell(RightFrontFace);
        EdgePieces original_edge1 = piece_map_.edge_map_[edge1];
        EdgePieces edge2 = find_candidate_edge_cell(LeftBackFace);
        EdgePieces original_edge2 = piece_map_.edge_map_[edge2];
        if (is_right_front_face_edge(edge1) && is_right_front_face_edge(edge2))
        {
            // Case (i): both edges are on RightFrontFace
            while (piece_map_.edge_map_[TopRightFrontEdge] != original_edge1)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
            }
            while (check_edge(TopLeftBackEdge))
            {
                // this loop must terminate because at least one of the edges on the LeftBackFace must 
                // be out of position
                add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
            }
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
            add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
            // edge1 is now at BottomLeftFrontEdge
            while (piece_map_.edge_map_[TopRightFrontEdge] != original_edge2)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
            }
            if (pieces_.edges_[TopRightFrontEdge].colour_[0] == lbf_colour)
            {
                if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                {
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                }
                else
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                }
            }
            else // colour_[1] == lbf_colour
            {
                if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                }
                else
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                }
            }
        }
        else if (is_left_back_face_edge(edge1) && is_left_back_face_edge(edge2))
        {
            // Case (ii): both edges are on LeftBackFace
            while (piece_map_.edge_map_[TopLeftBackEdge] != original_edge1)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
            }
            while (check_edge(TopRightFrontEdge))
            {
                // this loop must terminate because at least one of the edges on the RightFrontFace must 
                // be out of position
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
            }
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
            add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
            // edge1 is now at BottomLeftFrontEdge
            while (piece_map_.edge_map_[TopLeftBackEdge] != original_edge2)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
            }
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
            if (pieces_.edges_[TopRightFrontEdge].colour_[0] == lbf_colour)
            {
                if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                {
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                }
                else
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                }
            }
            else // colour_[1] == lbf_colour
            {
                if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                }
                else
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                }
            }
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves); 
        }
        else if ((is_right_front_face_edge(edge1) && is_left_back_face_edge(edge2)) ||
                 (is_left_back_face_edge(edge1) && is_right_front_face_edge(edge2)))
        {
            // Case (iii): one edge is on RightFrontFace and one edge is on LeftBackFace
            if (is_right_front_face_edge(edge1))
            {
                while (piece_map_.edge_map_[TopRightFrontEdge] != original_edge1)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                while (piece_map_.edge_map_[TopLeftBackEdge] != original_edge2)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                if (pieces_.edges_[TopRightFrontEdge].colour_[0] == rff_colour)
                {
                    if (pieces_.edges_[TopLeftBackEdge].colour_[0] == lbf_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    }
                    // edge1 should now be at BottomLeftFrontEdge
                    // edge2 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                    }
                }
                else
                {
                    if (pieces_.edges_[TopLeftBackEdge].colour_[0] == lbf_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    }
                    // edge2 should now be at BottomLeftFrontEdge
                    // edge1 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                    }
                }
            }
            else
            {
                while (piece_map_.edge_map_[TopRightFrontEdge] != original_edge2)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                while (piece_map_.edge_map_[TopLeftBackEdge] != original_edge1)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                if (pieces_.edges_[TopRightFrontEdge].colour_[0] == lbf_colour)
                {
                    if (pieces_.edges_[TopLeftBackEdge].colour_[0] == rff_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    }
                    // edge2 should now be at BottomLeftFrontEdge
                    // edge1 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                    }
                }
                else
                {
                    if (pieces_.edges_[TopLeftBackEdge].colour_[0] == rff_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    }
                    // edge1 should now be at BottomLeftFrontEdge
                    // edge2 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                    }
                }
            }
        }
        else if ((is_right_front_face_edge(edge1) && is_middle_slice_edge(edge2)) ||
                 (is_right_front_face_edge(edge2) && is_middle_slice_edge(edge1)))
        {
            // Case (iv): one edge is on RightFrontFace and one edge is on middle slice
            if (is_right_front_face_edge(edge1))
            {
                while (piece_map_.edge_map_[TopRightFrontEdge] != original_edge1)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                while (piece_map_.edge_map_[BottomLeftFrontEdge] != original_edge2)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                }
                while (check_edge(TopLeftBackEdge))
                {
                    // this loop must terminate because at least one of the edges on the LeftBackFace must 
                    // be out of position
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                if (pieces_.edges_[TopRightFrontEdge].colour_[0] == rff_colour)
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    // edge1 should now be at BottomLeftFrontEdge
                    // edge2 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                    }
                }
                else
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour)
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    }
                    // edge1 should now be at BottomRightBackEdge
                    // edge2 should now be at BottomLeftFrontEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                    }
                }
            }
            else
            {
                while (piece_map_.edge_map_[TopRightFrontEdge] != original_edge2)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                while (piece_map_.edge_map_[BottomLeftFrontEdge] != original_edge1)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                }
                while (check_edge(TopLeftBackEdge))
                {
                    // this loop must terminate because at least one of the edges on the LeftBackFace must 
                    // be out of position
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                if (pieces_.edges_[TopRightFrontEdge].colour_[0] == lbf_colour)
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    // edge1 should now be at BottomRightBackEdge
                    // edge2 should now be at BottomLeftFrontEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                    }
                }
                else
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    }
                    // edge1 should now be at BottomLeftFrontEdge
                    // edge2 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                    }
                }
            }
        }
        else if ((is_left_back_face_edge(edge1) && is_middle_slice_edge(edge2)) ||
                 (is_left_back_face_edge(edge2) && is_middle_slice_edge(edge1)))
        {
            // Case (v): one edge is on LeftBackFace and one edge is on middle slice
            if (is_left_back_face_edge(edge1))
            {
                while (piece_map_.edge_map_[TopLeftBackEdge] != original_edge1)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                while (piece_map_.edge_map_[BottomLeftFrontEdge] != original_edge2)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                }
                while (check_edge(TopRightBackEdge))
                {
                    // this loop must terminate because at least one of the edges on the RightFrontFace must 
                    // be out of position
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                if (pieces_.edges_[TopLeftBackEdge].colour_[0] == rff_colour)
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    // edge1 should now be at BottomLeftFrontEdge
                    // edge2 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                    }
                }
                else
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour)
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    }
                    // edge1 should now be at BottomRightBackEdge
                    // edge2 should now be at BottomLeftFrontEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                    }
                }
            }
            else
            {
                while (piece_map_.edge_map_[TopLeftBackEdge] != original_edge2)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
                }
                while (piece_map_.edge_map_[BottomLeftFrontEdge] != original_edge1)
                {
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                }
                while (check_edge(TopRightFrontEdge))
                {
                    // this loop must terminate because at least one of the edges on the LeftBackFace must 
                    // be out of position
                    add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
                }
                if (pieces_.edges_[TopLeftBackEdge].colour_[0] == lbf_colour)
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                    {
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    // edge1 should now be at BottomRightBackEdge
                    // edge2 should now be at BottomLeftFrontEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                    }
                }
                else
                {
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour)
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                    }
                    else
                    {
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                    }
                    // edge1 should now be at BottomLeftFrontEdge
                    // edge2 should now be at BottomRightBackEdge
                    // with rff_colour opposite lbf_colour
                    // check
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                    {
                        throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                    }
                    if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                    {
                        throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                    }
                }
            }
        }
        else if (is_middle_slice_edge(edge1) && is_middle_slice_edge(edge2))
        {
            // Case (vi): both edges are on middle slice
            // move RightFrontFace and LeftBackFace so that gaps are at top
            while (check_edge(TopRightFrontEdge))
            {
                // this loop must terminate because at least one of the edges on the LeftBackFace must 
                // be out of position
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
            }
            while (check_edge(TopLeftBackEdge))
            {
                // this loop must terminate because at least one of the edges on the LeftBackFace must 
                // be out of position
                add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
            }
            // move middle slice until one is at BottomLeftFrontEdge and the other is either at BottomRightBackEdge or TopRightBackEdge
            while (piece_map_.edge_map_[BottomLeftFrontEdge] != original_edge1 &&
                   piece_map_.edge_map_[BottomLeftFrontEdge] != original_edge2)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
            }
            if (piece_map_.edge_map_[TopLeftFrontEdge] == original_edge1 ||
                piece_map_.edge_map_[TopLeftFrontEdge] == original_edge2)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
            }
            if (piece_map_.edge_map_[BottomLeftFrontEdge] == original_edge1)
            {
                if (piece_map_.edge_map_[BottomRightBackEdge] == original_edge2)
                {
                    //
                    // |_ _|
                    //  1 2
                    //
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour &&
                        pieces_.edges_[BottomRightBackEdge].colour_[0] == lbf_colour)
                    {
                        // _ _
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        // edge1 should now be at BottomRightBackEdge
                        // edge2 should now be at BottomLeftFrontEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour &&
                             pieces_.edges_[BottomRightBackEdge].colour_[1] == lbf_colour)
                    {
                        // _  |
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomRightBackEdge
                        // edge2 should now be at BottomLeftFrontEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] == rff_colour &&
                             pieces_.edges_[BottomRightBackEdge].colour_[0] == lbf_colour)
                    {
                        // |  _ 
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomRightBackEdge
                        // edge2 should now be at BottomLeftFrontEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                        }
                    }
                    else
                    {
                        // |  |
                        // do nothing
                    }
                }
                else
                {
                    //
                    //    _
                    //     |
                    // 
                    // |_ 
                    //  1 2
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour &&
                        pieces_.edges_[TopRightBackEdge].colour_[0] == lbf_colour)
                    {
                        // 
                        //    _
                        //    
                        //
                        // _
                        //
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomRightBackEdge
                        // edge2 should now be at BottomLeftFrontEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == rff_colour &&
                             pieces_.edges_[TopRightBackEdge].colour_[1] == lbf_colour)
                    {
                        // 
                        //     | 
                        //
                        // _
                        //
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        // edge1 should now be at BottomRightBackEdge
                        // edge2 should now be at BottomLeftFrontEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] == rff_colour &&
                             pieces_.edges_[TopRightBackEdge].colour_[0] == lbf_colour)
                    {
                        // 
                        //    _
                        //    
                        //
                        // | 
                        //
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        // edge1 should now be at BottomLeftFrontEdge
                        // edge2 should now be at BottomRightBackEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                        }
                    }
                    else
                    {
                        // 
                        //     |
                        //
                        // | 
                        //
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomRightBackEdge
                        // edge2 should now be at BottomLeftFrontEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                        }
                    }
                }
            }
            else // BottomLeftFrontEdge is original edge2
            {
                if (piece_map_.edge_map_[BottomRightBackEdge] == original_edge1)
                {
                    //
                    // |_ _|
                    //  2 1
                    //
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour &&
                        pieces_.edges_[BottomRightBackEdge].colour_[0] == rff_colour)
                    {
                        // _ _
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        // edge1 should now be at BottomLeftFrontEdge
                        // edge2 should now be at BottomRightBackEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour &&
                             pieces_.edges_[BottomRightBackEdge].colour_[1] == rff_colour)
                    {
                        // _  |
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomLeftFrontEdge
                        // edge2 should now be at BottomRightBackEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] == lbf_colour &&
                             pieces_.edges_[BottomRightBackEdge].colour_[0] == rff_colour)
                    {
                        // |  _ 
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomLeftFrontEdge
                        // edge2 should now be at BottomRightBackEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                        }
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                {
                    //
                    //    _
                    //     |
                    // 
                    // |_ 
                    //  2 1
                    if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour &&
                        pieces_.edges_[TopRightBackEdge].colour_[0] == rff_colour)
                    {
                        // 
                        //    _
                        //    
                        //
                        // _
                        //
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomLeftFrontEdge
                        // edge2 should now be at BottomRightBackEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[0] == lbf_colour &&
                             pieces_.edges_[TopRightBackEdge].colour_[1] == rff_colour)
                    {
                        // 
                        //     | 
                        //
                        // _
                        //
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        // edge1 should now be at BottomLeftFrontEdge
                        // edge2 should now be at BottomRightBackEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                        }
                    }
                    else if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] == lbf_colour &&
                             pieces_.edges_[TopRightBackEdge].colour_[0] == rff_colour)
                    {
                        // 
                        //    _
                        //    
                        //
                        // | 
                        //
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        // edge1 should now be at BottomRightBackEdge
                        // edge2 should now be at BottomLeftFrontEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be lbf_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be rff_colour", __FILE__, __LINE__);
                        }
                    }
                    else
                    {
                        // 
                        //     |
                        //
                        // | 
                        //
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                        add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                        // edge1 should now be at BottomLeftFrontEdge
                        // edge2 should now be at BottomRightBackEdge
                        // with rff_colour opposite lbf_colour
                        // check
                        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] != rff_colour)
                        {
                            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should be rff_colour", __FILE__, __LINE__);
                        }
                        if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
                        {
                            throw CubeException("Error: BottomRightBackEdge on RightBackFace should be lbf_colour", __FILE__, __LINE__);
                        }
                    }
                }
            }
        }

        if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] == rff_colour)
        {
            if (pieces_.edges_[BottomRightBackEdge].colour_[1] != lbf_colour)
            {
                throw CubeException("Error: BottomRightBackEdge on RightBackFace should have lbf_colour", __FILE__, __LINE__);
            }
            Colour colour1 = pieces_.edges_[BottomLeftFrontEdge].colour_[0];
            Colour colour2 = pieces_.edges_[BottomRightBackEdge].colour_[0];
            while (faces_[TopFace].cell_[BottomRightCorner] != colour1)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
            }
            while (faces_[TopFace].cell_[TopRightCorner] != colour2)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
            }
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
            add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
        }
        else if (pieces_.edges_[BottomLeftFrontEdge].colour_[1] == lbf_colour)
        {
            if (pieces_.edges_[BottomRightBackEdge].colour_[1] != rff_colour)
            {
                throw CubeException("Error: BottomRightBackEdge on RightBackFace should have rff_colour", __FILE__, __LINE__);
            }
            Colour colour1 = pieces_.edges_[BottomLeftFrontEdge].colour_[0];
            Colour colour2 = pieces_.edges_[BottomRightBackEdge].colour_[0];
            while (faces_[TopFace].cell_[TopRightCorner] != colour1)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
            }
            while (faces_[TopFace].cell_[BottomRightCorner] != colour2)
            {
                add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
            }
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, Quarter), moves);
            add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
            add_move_to_solution(newSliceMove(TopBottom, TopSlice, ThreeQuarters), moves);
        }
        else
        {
            throw CubeException("Error: BottomLeftFrontEdge on LeftFrontFace should have lbf_colour or rff_colour", __FILE__, __LINE__);
        }
        ++loop_count;
        if (loop_count > 20)
        {
            break;
        }
    }

    // Line up corners correctly
    while (faces_[TopFace].cell_[BottomLeftCorner] != faces_[TopFace].cell_[Centre])
    {
        add_move_to_solution(newSliceMove(RightFrontLeftBack, TopSlice, Quarter), moves);
    }
    while (faces_[TopFace].cell_[TopLeftCorner] != faces_[TopFace].cell_[Centre])
    {
        add_move_to_solution(newSliceMove(RightFrontLeftBack, BottomSlice, Quarter), moves);
    }
}

void Cube::flip_pair_of_edges(Cube::move_sequence_type& moves)
{
    // Flip the TopLeftFrontEdge and TopRightBackEdge pieces
    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
    add_move_to_solution(newSliceMove(TopBottom, MiddleSlice, Half), moves);
    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
    add_move_to_solution(newSliceMove(TopBottom, MiddleSlice, Quarter), moves);
    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Quarter), moves);
    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, ThreeQuarters), moves);
    add_move_to_solution(newSliceMove(TopBottom, MiddleSlice, ThreeQuarters), moves);
    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
    add_move_to_solution(newSliceMove(TopBottom, MiddleSlice, Half), moves);
    add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, ThreeQuarters), moves);
    add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
}

void Cube::sort_out_centre_slice(Cube::move_sequence_type& moves)
{
    // The 4 remaining edge pieces are TopLeftFrontEdge (A), TopRightBackEdge (B), BottomRightBackEdge (C) and BottomLeftFrontEdge (D)
    //
    //                Top 
    //                 |
    //                 v
    //
    //               A . B
    // LeftFront->   .   .   <-RightBack
    //               D . C
    //
    //                 ^
    //                 |
    //              Bottom 
    //

    // The edges in these positions are given by the values of piece_map_.edge_map_ for each of these edges positions.
    // The required edges for these positions can be derived from the values of piece_map_.centre_map_, e.g. the required
    // edge at position A is the edge with colours of TopFace and LeftFrontFace
    enum CentreEdgePiecesIndex { A = 0, B, C, D, NumberOfCentreEdgePieces };
    static EdgePieces centre_edge_pieces[NumberOfCentreEdgePieces] = { TopLeftFrontEdge, TopRightBackEdge, BottomRightBackEdge, BottomLeftFrontEdge };
    EdgePieces current_centre_edge_pieces[NumberOfCentreEdgePieces];
    for (int i = A; i < NumberOfCentreEdgePieces; ++i)
    {
        current_centre_edge_pieces[i] = piece_map_.edge_map_[centre_edge_pieces[i]];
    }
    Colour centre_colours[NumberOfCentreEdgePieces];
    centre_colours[0] = pieces_.centres_[LeftFrontFace].colour_;
    centre_colours[1] = pieces_.centres_[TopFace].colour_;
    centre_colours[2] = pieces_.centres_[RightBackFace].colour_;
    centre_colours[3] = pieces_.centres_[BottomFace].colour_;

    EdgePieces permutation[NumberOfCentreEdgePieces];
    for (int j = A; j < NumberOfCentreEdgePieces; ++j)
    {
        EdgePiece required_edge_piece;
        required_edge_piece.colour_[0] = centre_colours[j];
        required_edge_piece.colour_[1] = centre_colours[(j+1)%4];
        for (int i = A; i < NumberOfCentreEdgePieces; ++i)
        {
            Colour colour1 = pieces_.edges_[centre_edge_pieces[i]].colour_[0];
            Colour colour2 = pieces_.edges_[centre_edge_pieces[i]].colour_[1];
            if ((colour1 == required_edge_piece.colour_[0] && colour2 == required_edge_piece.colour_[1]) ||
                (colour1 == required_edge_piece.colour_[1] && colour2 == required_edge_piece.colour_[0]))
            {
                permutation[i] = current_centre_edge_pieces[j];
                break;
            }
        }
    }

    // Out of the possible 24 permutations of 4 pieces, not all are possible, e.g. where 2 edges have swapped positions.
    // First get them into the correct position
    if (permutation[A] == current_centre_edge_pieces[A])
    {
        if (permutation[B] == current_centre_edge_pieces[B])
        {
            if (permutation[C] == current_centre_edge_pieces[C])
            {
                // A B C D      Identity
                // Do nothing
            }
            else
            {
                // A B D C      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[C])
        {
            if (permutation[C] == current_centre_edge_pieces[B])
            {
                // A C B D      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
            else
            {
                //
                // A C D B      (B C D)
                //
                // A . C      A . B
                // .   .  ->  .   .
                // B . D      D . C
                add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[D])
        {
            if (permutation[C] == current_centre_edge_pieces[B])
            {
                // A D B C      (B D C)
                //
                // A . D      A . B
                // .   .  ->  .   .
                // C . B      D . C
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
            }
            else
            {
                // A D C B      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
        }
        else
        {
            throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
        }
    }
    else if (permutation[A] == current_centre_edge_pieces[B])
    {
        if (permutation[B] == current_centre_edge_pieces[A])
        {
            if (permutation[C] == current_centre_edge_pieces[C])
            {
                // B A C D      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
            else
            {
                // B A D C      (A B) (C D) = 
                //
                // B . A      A . B
                // .   .  ->  .   .
                // C . D      D . C
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[C])
        {
            if (permutation[C] == current_centre_edge_pieces[A])
            {
                // B C A D      
                //
                // B . C      A . B
                // .   .  ->  .   .
                // D . A      D . C
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
            }
            else
            {
                // B C D A      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[D])
        {
            if (permutation[C] == current_centre_edge_pieces[A])
            {
                // B D A C      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
            else
            {
                // B D C A
                //
                // B . D      A . B
                // .   .  ->  .   .
                // A . C      D . C
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
            }
        }
        else
        {
            throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
        }
    }
    else if (permutation[A] == current_centre_edge_pieces[C])
    {
        if (permutation[B] == current_centre_edge_pieces[A])
        {
            if (permutation[C] == current_centre_edge_pieces[B])
            {
                // C A B D
                //
                // C . A      A . B
                // .   .  ->  .   .
                // D . B      D . C
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
            }
            else
            {
                // C A D B      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[B])
        {
            if (permutation[C] == current_centre_edge_pieces[A])
            {
                // C B A D      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
            else
            {
                // C B D A
                //
                // C . B      A . B
                // .   .  ->  .   .
                // A . D      D . C
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[D])
        {
            if (permutation[C] == current_centre_edge_pieces[A])
            {
                // C D A B
                //
                // C . D      A . B
                // .   .  ->  .   .
                // B . A      D . C
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                add_move_to_solution(newSliceMove(TopBottom, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
            }
            else
            {
                // C D B A      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
        }
        else
        {
            throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
        }
    }
    else if (permutation[A] == current_centre_edge_pieces[D])
    {
        if (permutation[B] == current_centre_edge_pieces[A])
        {
            if (permutation[C] == current_centre_edge_pieces[B])
            {
                // D A B C      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
            else
            {
                // D A C B
                //
                // D . A      A . B
                // .   .  ->  .   .
                // B . C      D . C
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[B])
        {
            if (permutation[C] == current_centre_edge_pieces[A])
            {
                // D B A C
                //
                // D . B      A . B
                // .   .  ->  .   .
                // C . A      D . C
                add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                add_move_to_solution(newSliceMove(TopBottom, BottomSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
            }
            else
            {
                // D B C A      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
        }
        else if (permutation[B] == current_centre_edge_pieces[C])
        {
            if (permutation[C] == current_centre_edge_pieces[A])
            {
                // D C A B      X
                throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
            }
            else
            {
                // D C B A
                //
                // D . C      A . B
                // .   .  ->  .   .
                // A . B      D . C
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
            }
        }
        else
        {
            throw CubeException("Error: Cube::sort_out_centre_slice : invalid permutation", __FILE__, __LINE__);
        }
    }

    // Check that edges are all in the correct positions
    for (int i = A; i < NumberOfCentreEdgePieces; ++i)
    {
        current_centre_edge_pieces[i] = piece_map_.edge_map_[centre_edge_pieces[i]];
    }
    EdgePieces check_permutation[NumberOfCentreEdgePieces];
    for (int j = A; j < NumberOfCentreEdgePieces; ++j)
    {
        EdgePiece required_edge_piece;
        required_edge_piece.colour_[0] = centre_colours[j];
        required_edge_piece.colour_[1] = centre_colours[(j+1)%4];
        for (int i = A; i < NumberOfCentreEdgePieces; ++i)
        {
            Colour colour1 = pieces_.edges_[centre_edge_pieces[i]].colour_[0];
            Colour colour2 = pieces_.edges_[centre_edge_pieces[i]].colour_[1];
            if ((colour1 == required_edge_piece.colour_[0] && colour2 == required_edge_piece.colour_[1]) ||
                (colour1 == required_edge_piece.colour_[1] && colour2 == required_edge_piece.colour_[0]))
            {
                check_permutation[i] = current_centre_edge_pieces[j];
                break;
            }
        }
    }
    for (int i = A; i < NumberOfCentreEdgePieces; ++i)
    {
        if (check_permutation[i] != current_centre_edge_pieces[i])
        {
            throw CubeException("Error: Cube::sort_out_centre_slice : gone wrong!", __FILE__, __LINE__);
        }
    }

    // Now all edge pieces are in the correct position, but possibly in the wrong orientation
    // Possibilities are (. => correct orientation, X => incorrect orientation):
    bool orientation[NumberOfCentreEdgePieces];
    int number_of_flipped_edges = 0;
    for (int i = A; i < NumberOfCentreEdgePieces; ++i)
    {
        orientation[i] = check_edge(centre_edge_pieces[i]);
        if (!orientation[i])
        {
            ++number_of_flipped_edges;
        }
    }

    if (number_of_flipped_edges % 2)
    {
        throw CubeException("Error: Cube::sort_out_centre_slice : odd number of flipped edges", __FILE__, __LINE__);
    }

    switch (number_of_flipped_edges)
    {
        case 0:
        {
            // A B C D
            // . . . .
            // do nothing, cube is solved!
        }
        break;
        case 2:
        {
            // flip one pair of edges
            if (!orientation[A] && !orientation[B])
            {
                // A B C D
                // X X . .
                flip_pair_of_edges(moves);
            }
            else if (!orientation[A] && !orientation[C])
            {
                // A B C D
                // X . X .
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
                flip_pair_of_edges(moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, BottomSlice, Half), moves);
            }
            else if (!orientation[A] && !orientation[D])
            {
                // A B C D
                // X . . X
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
                flip_pair_of_edges(moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
            }
            else if (!orientation[B] && !orientation[C])
            {
                // A B C D
                // . X X .
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, ThreeQuarters), moves);
                flip_pair_of_edges(moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Quarter), moves);
            }
            else if (!orientation[B] && !orientation[D])
            {
                // A B C D
                // . X . X
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
                flip_pair_of_edges(moves);
                add_move_to_solution(newSliceMove(LeftFrontRightBack, TopSlice, Half), moves);
            }
            else if (!orientation[C] && !orientation[D])
            {
                // A B C D
                // . . X X
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
                flip_pair_of_edges(moves);
                add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
            }
        }
        break;
        case 4:
        {
            // flip two pairs of edges
            // A B C D
            // X X X X
            flip_pair_of_edges(moves);
            add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
            flip_pair_of_edges(moves);
            add_move_to_solution(newSliceMove(RightFrontLeftBack, MiddleSlice, Half), moves);
        }
        break;
        default:
        {
            throw CubeException("Error: Cube::sort_out_centre_slice : invalid number of flipped edges", __FILE__, __LINE__);
        }
        break;
    }
}

std::string Cube::SliceMove::to_string() const
{
    std::ostringstream oss;
    oss << "Move " << Slice2string(slice_) << " through " << Rotation2string(rotation_) << " about " << Axis2string(axis_) << " axis (" << line_number_ << ")";
    return oss.str();
}

std::string Cube::SliceMove::inverse_to_string() const
{
    std::ostringstream oss;
    oss << "Move " << Slice2string(slice_) << " through " << Rotation2string(invert(rotation_)) << " about " << Axis2string(axis_) << " axis (" << line_number_ << ")";
    return oss.str();
}

std::string Cube::RotateMove::to_string() const
{
    std::ostringstream oss;
    oss << "Rotate through " << Rotation2string(rotation_) << " about " << Axis2string(axis_) << " axis (" << line_number_ << ")";
    return oss.str();
}

std::string Cube::RotateMove::inverse_to_string() const
{
    std::ostringstream oss;
    oss << "Rotate through " << Rotation2string(invert(rotation_)) << " about " << Axis2string(axis_) << " axis (" << line_number_ << ")";
    return oss.str();
}

void Cube::SliceMove::perform_inverse(Cube& cube) const
{
    SliceMove inverse_move(*this);
    inverse_move.rotation_ = invert(inverse_move.rotation_);
    cube.move(inverse_move);
}

void Cube::RotateMove::perform_inverse(Cube& cube) const
{
    RotateMove inverse_move(*this);
    inverse_move.rotation_ = invert(inverse_move.rotation_);
    cube.rotate(inverse_move);
}

Cube::Move* Cube::SliceMove::clone() const
{
    return new Cube::SliceMove(*this);
}

Cube::Move* Cube::RotateMove::clone() const
{
    return new Cube::RotateMove(*this);
}
