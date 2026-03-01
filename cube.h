#ifndef CUBE_H
#define CUBE_H
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>

class CubeException
{
    public:
        CubeException(const std::string& message, const std::string& file, int line_number)
            : message_(message), file_(file), line_number_(line_number) {}
        ~CubeException() {}
        std::string what() const
        {
            std::ostringstream oss;
            oss << "CubeException : " << message_ << " : " << file_ << " (" << line_number_ << ")";

            return oss.str();
        }

    private:
        std::string message_;
        std::string file_;
        int line_number_;
};

class Cube
{
    public:
        enum Colour { Red = 0, Blue, Green, Black, White, Cyan, NumberOfColours };
        enum Faces { TopFace = 0, BottomFace, RightFrontFace, RightBackFace, LeftFrontFace, LeftBackFace, NumberOfFaces };
        enum Cells { TopLeftCorner = 0, TopEdge,    TopRightCorner,
                     LeftEdge,          Centre,     RightEdge,
                     BottomLeftCorner,  BottomEdge, BottomRightCorner, NumberOfCells
                   };
        enum EdgePieces { TopRightFrontEdge = 0, TopRightBackEdge,    TopLeftFrontEdge,    TopLeftBackEdge,
                          MiddleLeftEdge,        MiddleBackEdge,      MiddleRightEdge,     MiddleFrontEdge,
                          BottomRightFrontEdge,  BottomRightBackEdge, BottomLeftFrontEdge, BottomLeftBackEdge, NumberOfEdgePieces
                        };
        enum CornerPieces { TopLeft = 0, TopBack, TopRight, TopFront,
                            BottomLeft, BottomBack, BottomRight, BottomFront, NumberOfCornerPieces
                          };

        static std::string Faces2string(const Faces& face)
        {
            switch (face)
            {
                case TopFace:
                    return "Top";
                case BottomFace:
                    return "Bottom";
                case RightFrontFace:
                    return "RightFront";
                case RightBackFace:
                    return "RightBack";
                case LeftFrontFace:
                    return "LeftFront";
                case LeftBackFace:
                    return "LeftBack";
                default:
                    return "";
            }
        }
        static std::string Cells2string(const Cells& cell)
        {
            switch (cell)
            {
                case TopLeftCorner:
                    return "TopLeftCorner";
                case TopEdge:
                    return "TopEdge";
                case TopRightCorner:
                    return "TopRightCorner";
                case LeftEdge:
                    return "LeftEdge";
                case Centre:
                    return "Centre";
                case RightEdge:
                    return "RightEdge";
                case BottomLeftCorner:
                    return "BottomLeftCorner";
                case BottomEdge:
                    return "BottomEdge";
                case BottomRightCorner:
                    return "BottomRightCorner";
                default:
                    return "";
            }
        }

        Cube()
        {
            for (int i = 0; i < NumberOfFaces; ++i)
            {
                faces_[i] = Face(static_cast<Cube::Colour>(i));
                piece_map_.centre_map_[i] = static_cast<Cube::Faces>(i);
            }
            for (int i = 0; i < NumberOfEdgePieces; ++i)
            {
                piece_map_.edge_map_[i] = static_cast<Cube::EdgePieces>(i);
            }
            for (int i = 0; i < NumberOfCornerPieces; ++i)
            {
                piece_map_.corner_map_[i] = static_cast<Cube::CornerPieces>(i);
            }
            pieces();
        }

        void pieces()
        {
            // Centre pieces
            for (int i = 0; i < NumberOfFaces; ++i)
            {
                pieces_.centres_[i].colour_ = faces_[i].cell_[Centre];
            }
            // Edge pieces
            // Top Slice
            // RightFront
            pieces_.edges_[TopRightFrontEdge].colour_[0] = faces_[TopFace].cell_[BottomEdge];
            pieces_.edges_[TopRightFrontEdge].colour_[1] = faces_[RightFrontFace].cell_[TopEdge];
            // RightBack
            pieces_.edges_[TopRightBackEdge].colour_[0] = faces_[TopFace].cell_[RightEdge];
            pieces_.edges_[TopRightBackEdge].colour_[1] = faces_[RightBackFace].cell_[TopEdge];
            // LeftFront
            pieces_.edges_[TopLeftFrontEdge].colour_[0] = faces_[TopFace].cell_[LeftEdge];
            pieces_.edges_[TopLeftFrontEdge].colour_[1] = faces_[LeftFrontFace].cell_[TopEdge];
            // LeftBack
            pieces_.edges_[TopLeftBackEdge].colour_[0] = faces_[TopFace].cell_[TopEdge];
            pieces_.edges_[TopLeftBackEdge].colour_[1] = faces_[LeftBackFace].cell_[TopEdge];
            // Middle Slice
            // Left
            pieces_.edges_[MiddleLeftEdge].colour_[0] = faces_[LeftBackFace].cell_[RightEdge];
            pieces_.edges_[MiddleLeftEdge].colour_[1] = faces_[LeftFrontFace].cell_[LeftEdge];
            // Back
            pieces_.edges_[MiddleBackEdge].colour_[0] = faces_[LeftBackFace].cell_[LeftEdge];
            pieces_.edges_[MiddleBackEdge].colour_[1] = faces_[RightBackFace].cell_[RightEdge];
            // Right
            pieces_.edges_[MiddleRightEdge].colour_[0] = faces_[RightFrontFace].cell_[RightEdge];
            pieces_.edges_[MiddleRightEdge].colour_[1] = faces_[RightBackFace].cell_[LeftEdge];
            // Front
            pieces_.edges_[MiddleFrontEdge].colour_[0] = faces_[RightFrontFace].cell_[LeftEdge];
            pieces_.edges_[MiddleFrontEdge].colour_[1] = faces_[LeftFrontFace].cell_[RightEdge];
            // Bottom Slice
            // RightFront
            pieces_.edges_[BottomRightFrontEdge].colour_[0] = faces_[BottomFace].cell_[TopEdge];
            pieces_.edges_[BottomRightFrontEdge].colour_[1] = faces_[RightFrontFace].cell_[BottomEdge];
            // RightBack
            pieces_.edges_[BottomRightBackEdge].colour_[0] = faces_[BottomFace].cell_[RightEdge];
            pieces_.edges_[BottomRightBackEdge].colour_[1] = faces_[RightBackFace].cell_[BottomEdge];
            // LeftFront
            pieces_.edges_[BottomLeftFrontEdge].colour_[0] = faces_[BottomFace].cell_[LeftEdge];
            pieces_.edges_[BottomLeftFrontEdge].colour_[1] = faces_[LeftFrontFace].cell_[BottomEdge];
            // LeftBack
            pieces_.edges_[BottomLeftBackEdge].colour_[0] = faces_[BottomFace].cell_[BottomEdge];
            pieces_.edges_[BottomLeftBackEdge].colour_[1] = faces_[LeftBackFace].cell_[BottomEdge];

            // Corner pieces
            // Top Slice
            // Left
            pieces_.corners_[TopLeft].colour_[0] = faces_[TopFace].cell_[TopLeftCorner];
            pieces_.corners_[TopLeft].colour_[1] = faces_[LeftFrontFace].cell_[TopLeftCorner];
            pieces_.corners_[TopLeft].colour_[2] = faces_[LeftBackFace].cell_[TopRightCorner];
            // Back
            pieces_.corners_[TopBack].colour_[0] = faces_[TopFace].cell_[TopRightCorner];
            pieces_.corners_[TopBack].colour_[1] = faces_[LeftBackFace].cell_[TopLeftCorner];
            pieces_.corners_[TopBack].colour_[2] = faces_[RightBackFace].cell_[TopRightCorner];
            // Right
            pieces_.corners_[TopRight].colour_[0] = faces_[TopFace].cell_[BottomRightCorner];
            pieces_.corners_[TopRight].colour_[1] = faces_[RightBackFace].cell_[TopLeftCorner];
            pieces_.corners_[TopRight].colour_[2] = faces_[RightFrontFace].cell_[TopRightCorner];
            // Front
            pieces_.corners_[TopFront].colour_[0] = faces_[TopFace].cell_[BottomLeftCorner];
            pieces_.corners_[TopFront].colour_[1] = faces_[RightFrontFace].cell_[TopLeftCorner];
            pieces_.corners_[TopFront].colour_[2] = faces_[LeftFrontFace].cell_[TopRightCorner];
            // Bottom Slice
            // Left
            pieces_.corners_[BottomLeft].colour_[0] = faces_[BottomFace].cell_[BottomLeftCorner];
            pieces_.corners_[BottomLeft].colour_[1] = faces_[LeftBackFace].cell_[BottomRightCorner];
            pieces_.corners_[BottomLeft].colour_[2] = faces_[LeftFrontFace].cell_[BottomLeftCorner];
            // Back
            pieces_.corners_[BottomBack].colour_[0] = faces_[BottomFace].cell_[BottomRightCorner];
            pieces_.corners_[BottomBack].colour_[1] = faces_[RightBackFace].cell_[BottomRightCorner];
            pieces_.corners_[BottomBack].colour_[2] = faces_[LeftBackFace].cell_[BottomLeftCorner];
            // Right
            pieces_.corners_[BottomRight].colour_[0] = faces_[BottomFace].cell_[TopRightCorner];
            pieces_.corners_[BottomRight].colour_[1] = faces_[RightFrontFace].cell_[BottomRightCorner];
            pieces_.corners_[BottomRight].colour_[2] = faces_[RightBackFace].cell_[BottomLeftCorner];
            // Front
            pieces_.corners_[BottomFront].colour_[0] = faces_[BottomFace].cell_[TopLeftCorner];
            pieces_.corners_[BottomFront].colour_[1] = faces_[LeftFrontFace].cell_[BottomRightCorner];
            pieces_.corners_[BottomFront].colour_[2] = faces_[RightFrontFace].cell_[BottomLeftCorner];
        }

        Colour colour(Faces face, Cells cell) const
        {
            return faces_[face].cell_[cell];
        }

        void randomize();

        struct Move
        {
            Move(int line_number) : line_number_(line_number) {}
            virtual ~Move() {}
            virtual void perform(Cube& cube) const = 0;
            virtual void perform_inverse(Cube& cube) const = 0;
            virtual std::string to_string() const = 0;
            virtual std::string inverse_to_string() const = 0;
            virtual Move* clone() const = 0;
            virtual std::string type() const = 0;
            int line_number_;
        };

        typedef std::vector<Move*> move_sequence_type;
        void solve(move_sequence_type& moves);
        void randomize(move_sequence_type& moves);

        /*
         There are 6 faces:
         Top / Bottom / RightFront / RightBack / LeftFront / LeftBack
         Each face has 9 cells: 1 centre, 4 edges and 4 corners.
         Position of cube is represented by the colour of each cell.
         There are 3 axes of rotation:
         top->bottom
         rightfront->leftback
         leftfront->rightback
         Each axis has 3 slices:
         Top / Middle / Bottom
         Each slice can have moves (clockwise, relative to Top slice):
         Quarter, Half, Threequarters
         Giving a total of 27 moves.

         */
        enum Axis { TopBottom = 0, RightFrontLeftBack, LeftFrontRightBack, NumberOfAxes };
        enum Slice { TopSlice = 0, MiddleSlice, BottomSlice, NumberOfSlices };
        enum Rotation { Quarter = 1, Half = 2, ThreeQuarters = 3, NumberOfRotations = 3 };

        static Rotation invert(const Rotation& rotation)
        {
            switch (rotation)
            {
                case Quarter:
                    return ThreeQuarters;
                case ThreeQuarters:
                    return Quarter;
                case Half:
                    return Half;
		default:
		    throw CubeException("Error: Cube::invert : invalid rotation", __FILE__, __LINE__);
            }
        }

        static std::string Axis2string(const Axis& axis)
        {
            switch (axis)
            {
                case TopBottom:
                    return "TopBottom";
                case RightFrontLeftBack:
                    return "RightFrontLeftBack";
                case LeftFrontRightBack:
                    return "LeftFrontRightBack";
                default:
                    return "";
            }
        }
        static std::string Slice2string(const Slice& slice)
        {
            switch (slice)
            {
                case TopSlice:
                    return "TopSlice";
                case MiddleSlice:
                    return "MiddleSlice";
                case BottomSlice:
                    return "BottomSlice";
                default:
                    return "";
            }
        }
        static std::string Rotation2string(const Rotation& rotation)
        {
            switch (rotation)
            {
                case Quarter:
                    return "Quarter";
                case Half:
                    return "Half";
                case ThreeQuarters:
                    return "ThreeQuarters";
                default:
                    return "";
            }
        }

        struct SliceMove : public Move
        {
            Axis axis_;
            Slice slice_;
            Rotation rotation_;
            SliceMove(const Axis axis, const Slice slice, const Rotation rotation, int line_number)
                : Move(line_number), axis_(axis), slice_(slice), rotation_(rotation) {}
            void perform(Cube& cube) const
            {
                cube.move(*this);
            }
            void perform_inverse(Cube& cube) const;
            std::string to_string() const;
            std::string inverse_to_string() const;
            std::string type() const { return "SliceMove"; }
            Move* clone() const;
        };
        struct RotateMove : public Move
        {
            Axis axis_;
            Rotation rotation_;
            RotateMove(const Axis axis, const Rotation rotation, int line_number)
                : Move(line_number), axis_(axis), rotation_(rotation) {}
            void perform(Cube& cube) const
            {
                cube.rotate(*this);
            }
            void perform_inverse(Cube& cube) const;
            std::string to_string() const;
            std::string inverse_to_string() const;
            std::string type() const { return "RotateMove"; }
            Move* clone() const;
        };

        static Move* make_move(const std::string& move_string)
        {
            // moves are defined by Axis, Slice and Rotation
            // Axis:
            //  T : TopBottom   
            //  R : RightFrontLeftBack
            //  L : LeftFrontRightBack
            // Slice:
            //  T : TopSlice
            //  M : MiddleSlice
            //  B : BottomSlice
            // Rotation:
            //  1 : Quarter
            //  2 : Half
            //  3 : ThreeQuarters
            //
            //  e.g TT2 is TopBottom axis, TopSlice slice and Half turn

            // move_string must be 3 characters
            if (move_string.size() != 3)
            {
                std::ostringstream oss;
                oss << "Invalid move_string [" << move_string << "]";
                throw CubeException(oss.str(), __FILE__, __LINE__);
            }
            char axis_char = move_string[0];
            char slice_char = move_string[1];
            char rotation_char = move_string[2];
            enum Axis axis(TopBottom);
            enum Slice slice(TopSlice);
            enum Rotation rotation(Quarter);
            switch (axis_char)
            {
                case 'T': axis = TopBottom; break;
                case 'R': axis = RightFrontLeftBack; break;
                case 'L': axis = LeftFrontRightBack; break;
                default:
                {
                    std::ostringstream oss;
                    oss << "Invalid move_string [" << move_string << "]";
                    throw CubeException(oss.str(), __FILE__, __LINE__);
                }
            }
            switch (slice_char)
            {
                case 'T': slice = TopSlice; break;
                case 'M': slice = MiddleSlice; break;
                case 'B': slice = BottomSlice; break;
                default:
                {
                    std::ostringstream oss;
                    oss << "Invalid move_string [" << move_string << "]";
                    throw CubeException(oss.str(), __FILE__, __LINE__);
                }
            }
            switch (rotation_char)
            {
                case '1': rotation = Quarter; break;
                case '2': rotation = Half; break;
                case '3': rotation = ThreeQuarters; break;
                default:
                {
                    std::ostringstream oss;
                    oss << "Invalid move_string [" << move_string << "]";
                    throw CubeException(oss.str(), __FILE__, __LINE__);
                }
            }
            return new SliceMove(axis, slice, rotation, __LINE__);
        }

        static int depth_;
        void move_slice(const Axis axis, const Slice slice, const Rotation rotation);
        void move(const SliceMove& move)
        {
            move_slice(move.axis_, move.slice_, move.rotation_);
        }
        void rotate(const Axis axis, const Rotation rotation);
        void rotate(const RotateMove& move)
        {
            rotate(move.axis_, move.rotation_);
        }
        void display() const;
        int identify_piece(int position) const
        {
            if (position >= Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces &&
                position < Cube::NumberOfCornerPieces+Cube::NumberOfEdgePieces+Cube::NumberOfFaces)
            {
                // Centre piece
                int face = position - (Cube::NumberOfCornerPieces + Cube::NumberOfEdgePieces);
                return piece_map_.centre_map_[face] + Cube::NumberOfCornerPieces + Cube::NumberOfEdgePieces;

            }
            else if (position >= Cube::NumberOfCornerPieces)
            {
                // Edge piece
                int edge = position - Cube::NumberOfCornerPieces;
                return piece_map_.edge_map_[edge] + Cube::NumberOfCornerPieces;
            }
            else if (position >= 0)
            {
                // CornerPiece
                return piece_map_.corner_map_[position];
            }
            else
            {
		        throw CubeException("Error: Cube::identify_piece : invalid position", __FILE__, __LINE__);
            }
        }

    private:
        void update_piece_map(const Axis axis, const Slice slice, const Rotation rotation);
        void update_piece_map(const Axis axis, const Rotation rotation);
        bool check_piece_map() const;
        bool check_corners();
        void sort_out_corners(move_sequence_type& moves);
        void sort_out_opposite_faces(move_sequence_type& moves);
        void flip_pair_of_edges(move_sequence_type& moves);
        void sort_out_centre_slice(move_sequence_type& moves);
        void add_move_to_solution(Cube::Move* move, Cube::move_sequence_type& moves);
        bool check_edge(const EdgePieces& edge) const;
        bool check_edges(const Faces& face) const;
        static bool is_middle_slice_edge(const EdgePieces& edge);
        static bool is_right_front_face_edge(const EdgePieces& edge);
        static bool is_left_back_face_edge(const EdgePieces& edge);
        Cube::Faces get_other_face(const Cube::Faces& face, const Cube::EdgePieces& edge) const;
        std::pair<Cube::Faces, Cube::Faces> get_faces(const Cube::EdgePieces& edge) const;
        std::pair<Cube::Colour, Cube::Colour> get_edge_colours(const EdgePieces& edge) const;
        Cube::EdgePieces find_candidate_edge_cell(const Cube::Faces& face) const;
        void find_edge_cells(const Colour& colour, std::vector<Cube::EdgePieces>& edge_cells) const;
        void find_corner_cells(const Colour& colour1, const Colour& colour2,
                               std::set<std::pair<Faces, Cells> >& corner_cells,
                               std::map<Faces, int>& face_counts) const;
        void swap_two_adjacent_corners(const Faces& face, const Cells& left_corner, Cube::move_sequence_type& moves);
        void three_on_each_side(const Cube::Colour& rbf_colour,
                                const Cube::Colour& lff_colour, Cube::move_sequence_type& moves);

        struct CentrePiece
        {
            Colour colour_;
        };
        struct EdgePiece
        {
            Colour colour_[2]; // 0 is always the top, bottom, right front or left back face
        };
        struct CornerPiece
        {
            Colour colour_[3]; // clockwise direction, 0 is always on the top or bottom face
        };
        struct CubePieces
        {
            // Centre pieces are stored in order:
            // Top, Bottom, RightFront, RightBack, LeftFront, LeftBack
            CentrePiece centres_[NumberOfFaces];
            // Edge pieces are stored in order:
            // Top slice: RightFront, RightBack, LeftFront, LeftBack
            // Middle slice: Left, Back, Right, Front
            // Bottom slice: RightFront, RightBack, LeftFront, LeftBack
            EdgePiece edges_[NumberOfEdgePieces];
            // Corner pieces are stored in order:
            // Top slice: Left, Back, Right, Front
            // Bottom slice: Left, Back, Right, Front
            CornerPiece corners_[NumberOfCornerPieces];
        };
        struct CubePiecesMap
        {
            Faces centre_map_[NumberOfFaces];
            EdgePieces edge_map_[NumberOfEdgePieces];
            CornerPieces corner_map_[NumberOfCornerPieces];
        };
        struct Face
        {
            Face(const Cube::Colour initial_colour = Red)
            {
                for (int i = 0; i < NumberOfCells; i++)
                {
                    cell_[i] = initial_colour;
                }
            }
            void rotate(const Rotation);
            void rotate_inverse(const Rotation);

            Colour cell_[NumberOfCells];
            int count_corners(const Colour& colour) const;
        };
        // faces_ gives the colours of each cell on each face,
        // the positions stay fixed, but the colours change as
        // moves are applied to the cube
        Face faces_[NumberOfFaces];
        // pieces_ gives the colours of each piece,
        // the positions of the pieces stay fixed, but the colours
        // change as moves are applied to the cube
        CubePieces pieces_;
        // piece_map_ gives the current position of each piece
        // relative to the fixed positions used by pieces_
        // It is a map from the fixed position to the original position
        // of the piece, the original position of the piece acting as
        // a proxy for the actual piece.
        CubePiecesMap piece_map_;
};
#endif
