#ifndef IIT_COMMONS_PLANNING_HPP_
#define IIT_COMMONS_PLANNING_HPP_

#include <Eigen/Dense>
#include <doglib/rbd/rbd.h>
#include <iostream>

namespace iit {
namespace planning {

static const int contactConstrCount = 3; //number of constraint for each contact point

struct Point {
    double x;
    double xd;
    double xdd;
    explicit Point(double p = 0, double v = 0, double a = 0) : x(p), xd(v), xdd(a) {}
    void setZero() { x = 0.0;
                      xd = 0.0;
                      xdd = 0.0;}

    inline Point & operator=(const Point& rhs)
    {
        x = rhs.x;
        xd = rhs.xd;
        xdd = rhs.xdd;
        return *this;
    }
};


struct Point2d {
  Eigen::Vector2d x;
  Eigen::Vector2d xd;
  Eigen::Vector2d xdd;
  explicit Point2d(Eigen::Vector2d _x = Eigen::Vector2d::Zero(),
                   Eigen::Vector2d _xd = Eigen::Vector2d::Zero(),
                   Eigen::Vector2d _xdd = Eigen::Vector2d::Zero())
      : x(_x), xd(_xd), xdd(_xdd) {}
  inline Point2d &   operator=(const Point2d& rhs)
  {
    x = rhs.x;
    xd = rhs.xd;
    xdd = rhs.xdd;
    return *this;
  }

};


struct Point3d {
  Eigen::Vector3d x;
  Eigen::Vector3d xd;
  Eigen::Vector3d xdd;
  explicit Point3d(Eigen::Vector3d _x = Eigen::Vector3d::Zero(),
                   Eigen::Vector3d _xd = Eigen::Vector3d::Zero(),
                   Eigen::Vector3d _xdd = Eigen::Vector3d::Zero())
      : x(_x), xd(_xd), xdd(_xdd) {}
  inline Point3d &   operator=(const Point3d& rhs)
  {
  	x = rhs.x;
  	xd = rhs.xd;
  	xdd = rhs.xdd;
  	return *this;
  }

};

inline std::ostream& operator<<(std::ostream& out, const Point3d& pos)
{
  out << "x=" << pos.x.transpose() << "  "
      << "xd=" << pos.xd.transpose() << "  "
      << "xdd=" << pos.xdd.transpose();
  return out;
}

struct Ori {
  Eigen::Quaterniond q;
  Eigen::Vector3d w;
  Eigen::Vector3d wd;
  explicit Ori(Eigen::Quaterniond _q = Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0),
               Eigen::Vector3d _w    = Eigen::Vector3d::Zero(),
               Eigen::Vector3d _wd    = Eigen::Vector3d::Zero())
  : q(_q), w(_w), wd(_wd) {}

};

inline std::ostream& operator<<(std::ostream& out, const Ori& ori)
{
  out << "quat=" << ori.q.w() << " " << ori.q.x() << " " << ori.q.y() << " " << ori.q.z()<< " "
      << "w=" << ori.w.transpose() << "  "
      << "wd=" << ori.wd.transpose();
  return out;
}

struct Pose {
  Point3d pos;
  Ori ori;
};

struct pointInsidePolygonInfos {
  bool isPointInside;
  Eigen::VectorXd margins;
};

inline std::ostream& operator<<(std::ostream& out, const Pose& pose)
{
  out << "\tPos: " << pose.pos << "\n"
      << "\tOri: " << pose.ori;
  return out;
}

/**
 * @brief The LineCoeff2d struct
 * p*x + q*y + r = 0
 */
struct LineCoeff2d {
  double p;
  double q;
  double r;
};

// overloading operator<< for more elegant priting of above values
inline std::ostream& operator<<(std::ostream& out, const LineCoeff2d& lc)
{
  out  << "p=" << lc.p << ", q=" << lc.q << ", r=" << lc.r;
  return out;
}

/**
 * @brief Point2isRightOfLine Checks if p2 is on the right side of line
 * from p0 to p1
 * @param p0
 * @param p1
 * @param p2
 * @return >0 for P2 right of the line from P0 to P1
 * =0 for P2 on the line
 * <0 for P2 left of the line
 */
inline double Point2isRightOfLine(const Eigen::Vector3d p0,
                                  const Eigen::Vector3d p1,
                                  const Eigen::Vector3d p2)
{
  return (p2(dls::rbd::X) - p0(dls::rbd::X)) * (p1(dls::rbd::Y) - p0(dls::rbd::Y))
          - (p1(dls::rbd::X) - p0(dls::rbd::X)) * (p2(dls::rbd::Y) - p0(dls::rbd::Y));
}

/**
 * @brief ClockwiseSort Performs a  clockwise radial sort of the input points
         starting with the bottom left point
 * @param[out] p Unsorted points or footholds.
  Uses the slow n^2 sort algorithm, shouldn't matter for sorting 4 points :)
  Fails when 3 points are on same line and one could be removed p
 */
static void ClockwiseSort(std::vector<Eigen::Vector3d>& p)
{

    // sort clockwise
    for (int i = 1; i < p.size() - 1; i++) {
        for (int j = i + 1; j < p.size(); j++) {
            //the point p2 should always be on the right to be cwise thus if it
            //is on the left <0 i swap
            if (Point2isRightOfLine(p[0], p[i], p[j])  < 0.0) {
                Eigen::Vector3d tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }
}

/**
 * @brief CounterClockwiseSort Performs a  counter clockwise radial sort of the
 * input points starting with the bottom left point
 * @param[out] p Unsorted points or footholds.
 * Uses the slow n^2 sort algorithm, shouldn't matter for sorting 4 points :)
 * Fails when 3 points are on same line and one could be removed
 */
static void CounterClockwiseSort(std::vector<Eigen::Vector3d>& p)
{

    // sort counter clockwise
    for (int i = 1; i < p.size() - 1; i++) {
        for (int j = i + 1; j < p.size(); j++) {
            //the point p2 should always be on the left of the line to be
            // ccwise thus if it is on the right  >0  swap
            if (Point2isRightOfLine(p[0], p[i], p[j])  > 0.0) {
                Eigen::Vector3d tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }
}

/**
 * @brief CounterClockwiseSort Performs a  counter clockwise radial sort of the
 * input points starting with the bottom left point, returns also  index map
 * @param[out] p Unsorted points or footholds.
 * @param[out] index_map map of indexes.
 * Uses the slow n^2 sort algorithm, shouldn't matter for sorting 4 points :)
 * Fails when 3 points are on same line and one could be removed
 */
static void CounterClockwiseSort(std::vector<Eigen::Vector3d>& p, std::vector<int> & index_map)
{

    // sort counter clockwise
    for (int i = 0; i < p.size(); i++)
            index_map[i] = i;
    for (int i = 1; i < p.size() - 1; i++) {
        for (int j = i + 1; j < p.size(); j++) {
            //the point p2 should always be on the left of the line to be
            // ccwise thus if it is on the right  >0  swap
            if (Point2isRightOfLine(p[0], p[i], p[j])  > 0.0) {
                Eigen::Vector3d tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
                index_map[i] = j;
                index_map[j] = i;
            }
        }
    }
}

/**
 * @brief LineCoeff return the coefficients p,q of the line p*x + q*y + r = 0 (in 2D) passing through pt0 and pt1
 * @param[in] pt0 first point
 * @param[in] pt1 second point
 * @param[in] normalize:  normalize the equation in order to intuitively
 * use stability margins (e.g. set the same stability margin for different lines)
 * @return the struct @sa LineCoeff2d with the computed parameters
*/
inline LineCoeff2d LineCoeff(const Eigen::Vector3d& pt0,
                             const Eigen::Vector3d& pt1,
                             bool normalize = true)
{
    //(p,q).dot(x-x0, y-y0)=0 implicit line equation
    //px + qy -rx0 -qy0 = 0 => px + qy + r = 0
    //to find (p,q) I know this is the normal to the line, so if I have another
    // point x1 a solution
    // with (p,q) perpendicular to the segment S=(x1- x0, y1-y0) is
    // p = -(y1-y0) q=x1-x0 for which (p,q).dot(S) = 0
    LineCoeff2d ret;
    ret.p = pt0(dls::rbd::Y) - pt1(dls::rbd::Y);
    ret.q = pt1(dls::rbd::X) - pt0(dls::rbd::X);
    ret.r = -ret.p * pt0(dls::rbd::X) - ret.q * pt0(dls::rbd::Y);

    // normalize the equation in order to intuitively use stability margins
    if (normalize) {
        double norm = hypot(ret.p, ret.q);
        ret.p /= norm;
        ret.q /= norm;
        ret.r /= norm;
    }

    return ret;
}

/**
 * @brief pointInTriangle checks if a given point P is inside the triagle formed by points A, B, C
 * @param A point of triangle
 * @param B point of triangle
 * @param C point of triangle
 * @param P test point
 * @return
 */
inline bool pointInTriangle(const  Eigen::Vector3d& A,
                            const  Eigen::Vector3d& B,
                            const  Eigen::Vector3d& C,
                            const  Eigen::Vector3d& P)
{
   // Barycentric Technique from
   //http://www.blackpawn.com/texts/pointinpoly/default.html
    // Compute vectors
    Eigen::Vector3d v0,v1,v2;
    //v0 = C - A
    v0= C - A;
    //v1 = B - A
    v1=  B - A;
    //v2 = P - A
    v2 = P - A;

    //TODO check coplanarity
    double dot00,	dot01,	dot02,	dot11,	dot12;
    // Compute dot products
    dot00 = v0.dot(v0);
    dot01 = v0.dot(v1);
    dot02 = v0.dot(v2);
    dot11 = v1.dot(v1);
    dot12 = v1.dot(v2);

     // Compute barycentric coordinates
     double denom = dot00 * dot11 - dot01 * dot01;

     if(denom != 0) {

       double invDenom = 1 / denom;
       double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
       double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

       // Check if point is in triangle
       return (u > 0) && (v > 0) && (u + v < 1);

     } else {
        printf("pt_in_tr: error denom == 0");
       return false;
     }
}


/**
 * @brief line_intersect_from_points Computes the intersection point from two given lines (in 3D)
 * passing through four points. The lines can be coplanar or non-coplanar,
 * in the latter casean approximation is returned and a warning is raised
 * @param point1_line1 first point of line 1
 * @param point2_line1 second point of line 1
 * @param point1_line2 first point of line 2
 * @param point2_line2 second point of line 2
 * @param intersection intersection point
 * @param s (optional) the scaling to compute the intersection point from one of the edges
 * @return false if lines are parallel
 */
inline bool  line_intersect_from_points(const Eigen::Vector3d& point1_line1,
                                        const Eigen::Vector3d& point2_line1,
                                        const Eigen::Vector3d& point1_line2,
                                        const Eigen::Vector3d& point2_line2,
                                        Eigen::Vector3d & intersection,
                                        double & signedDistance)
{
    Eigen::Vector3d a,b,c, int1, int2;
    //for 3d  see  http://mathworld.wolfram.com/Line-LineIntersection.html
    //    a=point2_line1-point1_line1;
    //    b=point2_line2-point1_line2;
    //    c=point1_line2-point1_line1;
    //    bool nonCoplanar = ((fabs(c.dot(a.cross(b))) >= 1E-3));
    //    if (!nonCoplanar)
    //    {

    //        //the distance is from the first point
    //        s=(c.cross(b).dot(a.cross(b)) )  / (a.cross(b).squaredNorm());
    //        intersection = point1_line1 + s*a;
    //        return true;
    //    } else {
    //        std::cout << "vectors are not coplanar cannot finde intersection" << std::endl << std::endl;
    //        return false;
    //    }

    //alternative approach also deals with skewed lines
    a=point1_line1-point2_line1;
    b=point1_line2-point2_line2;
    c=point2_line2-point2_line1;
    Eigen::Vector3d h = b.cross(c);
    Eigen::Vector3d k = b.cross(a);

    if ((h.norm() == 0) ||(k.norm() == 0))
    {
        std::cout<<"Lines are parallel!" << std::endl;
        return false;
    }
    signedDistance = h.norm()/k.norm();
    if (h.dot(k)>0.0)
    {
        int1 = point2_line1 + signedDistance*a;
        int2 = point2_line2 + signedDistance*b;
    } else {
        int1 = point2_line1 - signedDistance*a;
        int2 = point2_line2 - signedDistance*b;
    }
    bool nonCoplanar = ((fabs(c.dot(a.cross(b))) >= 1E-3));
    if (!nonCoplanar)
    {
        //int1 and int2 are the same
        intersection = int1;
    } else {
        std::cout << "Warning: Vectors are not co-planar outputing an approximate value between the two lines" << std::endl << std::endl;
        intersection = (int1 + int2)/2;
    }
    return true;
}

/**
 * @brief line_intersect_from_points Computes the intersection point from two given lines (in 3D)
 * passing through four points. The lines can be coplanar or non-coplanar,
 * in the latter casean approximation is returned and a warning is raised
 * @param point1_line1 first point of line 1
 * @param point2_line1 second point of line 1
 * @param point1_line2 first point of line 2
 * @param point2_line2 second point of line 2
 * @return intersection point (if lines are paraller returns Vector3d::Zero()
 */
inline Eigen::Vector3d line_intersect_from_points(const Eigen::Vector3d& point1_line1,
                                                  const Eigen::Vector3d& point2_line1,
                                                  const Eigen::Vector3d& point1_line2,
                                                  const Eigen::Vector3d& point2_line2)
{
    Eigen::Vector3d intersection =  Eigen::Vector3d::Zero();
    double s;
    line_intersect_from_points(point1_line1, point2_line1, point1_line2, point2_line2, intersection, s);
    return intersection;
}

//A x + b > 0: an inner point satisfies positive inequalities
/**
 * @brief computeHalfPlaneDescription Computes the halfplane description of the polygon
 * @param input vertices (columns, note that the input points can be unsorted!)
 * @param A half space descriprion of the polygon (assuming created with vertex sorted in CCWise order)
 * @param b half space descriprion of the polygon (assuming created with vertex sorted in CCWise order)
 * @note (considers only X,Y components of vertices)
 */
inline void  computeHalfPlaneDescription(const  Eigen::MatrixXd vertices, Eigen::MatrixXd & A, Eigen::VectorXd & b)
{
    std::vector<Eigen::Vector3d> verticesCCwiseSorted;
    planning::LineCoeff2d lineCoeff;
    int number_of_constraints = vertices.cols();
    A.resize(number_of_constraints, 3);
    b.resize(number_of_constraints);

    verticesCCwiseSorted.resize(number_of_constraints);
    for (int vertex_index = 0; vertex_index<number_of_constraints; vertex_index++)
    {
       verticesCCwiseSorted[vertex_index] = Eigen::Vector3d( vertices(dls::rbd::X, vertex_index), vertices(dls::rbd::Y, vertex_index),0.0);
    }
    //sort the positions
    planning::CounterClockwiseSort(verticesCCwiseSorted);
    //cycle along the ordered vertices to compute the line coeff p*xcp + q*ycp  +r  > + stability_margin
    for(int vertex_index = 0; vertex_index<number_of_constraints; vertex_index++)
    {
        //compute the coeffs of the line between two vertices (normal p,q pointing on the left of (P1 - P0) vector
        lineCoeff = planning::LineCoeff(verticesCCwiseSorted[vertex_index],  verticesCCwiseSorted[(vertex_index + 1) % number_of_constraints], true); //I set true to normalize and use stab margin
        if ((!std::isfinite(lineCoeff.p)) || (!std::isfinite(lineCoeff.q)))
        {
            std::cerr<<"There are two coincident vertices in the polygon, there could be NaNs in the HP description matrix" <<std::endl;
        }
        A(vertex_index,0) = lineCoeff.p;
        A(vertex_index,1) = lineCoeff.q;
        A(vertex_index,2) = 0.0; //Z component is not considered
        b(vertex_index) = lineCoeff.r;
    }

}

/**
 * @brief checkInsidePolygon Checks if a point is inside the give polygon
 * @param inner_point_to_test
 * @param A half space descriprion of he polygon (assuming created with vertex sorted in CCWise order)
 * @param b half space descriprion of he polygon (assuming created with vertex sorted in CCWise order)
 * @note (considers only X,Y components of vertices)
 * @return true if point in inside polygon
 */
inline bool checkInsidePolygon(const Eigen::Vector3d inner_point_to_test, const Eigen::MatrixXd & A, const Eigen::VectorXd & b)
{
    Eigen::VectorXd margin; margin.resize(A.rows());
    margin = A*inner_point_to_test+b;
    return (margin.array() > 0).all();
}

/**
 * @brief checkInsidePolygon Checks if a point is inside the give polygon (assumes the polygon description is in CCWISE order)s
 * @param inner_point_to_test
 * @param A half space descriprion of he polygon (assuming created with vertex sorted in CCWise order)
 * @param b half space descriprion of he polygon (assuming created with vertex sorted in CCWise order)
 * @note (considers only X,Y components of vertices)
 * @return informations about the point such as true if point in inside polygon and the margins wrt all sides of
 *          the polygon
 */
inline Eigen::VectorXd getMarginsInsidePolygon(const Eigen::Vector3d inner_point_to_test, const Eigen::MatrixXd & A, const Eigen::VectorXd & b)
{
    Eigen::VectorXd margin; margin.resize(A.rows());
    margin = A*inner_point_to_test+b;
    return margin;
}

/**
 * @brief given the index of a specific side of the support polygon, this returns the leg indices that correspond
 *          to that side.
 * @param inner_point_to_test
 * @param A half space descriprion of he polygon (assuming created with vertex sorted in CCWise order)
 * @param b half space descriprion of he polygon (assuming created with vertex sorted in CCWise order)
 * @note (considers only X,Y components of vertices)
 * @return informations about the point such as true if point in inside polygon and the margins wrt all sides of
 *          the polygon
 */
inline pointInsidePolygonInfos getFeetIndicesFromSupportPolygonIndex(const double inner_point_to_test, const Eigen::MatrixXd & A, const Eigen::VectorXd & b)
{
    pointInsidePolygonInfos pointInfos;
    pointInfos.margins = A*inner_point_to_test+b;
    pointInfos.isPointInside = (pointInfos.margins.array() > 0).all();
    return pointInfos;
}

/**
 * @brief scalePolygon Scales the polygon in input
 * @param vertices[in] vertex description of the input polygon
 * @param scaleRatio[in] percentage of shrinking (between  0 and 1)
 * @param scaled_vertices[out] vertex description of the scaled polygon
 * @note (considers only X,Y components of vertices)
 * @return
 */
inline bool scalePolygon(const  Eigen::MatrixXd vertices, const double scaleRatio, const Eigen::Vector3d scaleOrigin, Eigen::MatrixXd & scaled_vertices)
{


    int num_of_vertices = vertices.cols();
    scaled_vertices.resize(3, num_of_vertices);
    //remove centroid and scale vertices
    for (int i = 0;i < num_of_vertices; i++)
    {
        scaled_vertices.col(i) = scaleRatio*(vertices.col(i)-scaleOrigin);
        //add back centroid
        scaled_vertices.col(i)+=scaleOrigin;
    }
    return true;
}

/**
 * @brief shrinkPolygon Shrinks the polygon in input
 * @param vertices[in] vertex description of the input polygon
 * @param shrinkRatio[in] percentage of shrinking (between  0 and 1)
 * @param shrinked_vertices[out] vertex description of the shrinked polygon
 * @note (considers only X,Y components of vertices)
 * @return
 */
inline bool shrinkPolygon(const  Eigen::MatrixXd vertices, const double shrinkRatio, Eigen::MatrixXd & shrinked_vertices)
{

    if ((shrinkRatio > 1.0) || (shrinkRatio< 0.0))
    {
      std::cerr<<"shrinkRatio should be between 0 and 1" << std::endl;
      return false;
    }
    Eigen::Vector3d polygon_centroid = Eigen::Vector3d::Zero();
    int num_of_vertices = vertices.cols();
    //compute centroid
    for (int i = 0;i < num_of_vertices; i++)
    {
        polygon_centroid+= vertices.col(i);
    }
    polygon_centroid/=num_of_vertices;


    return scalePolygon(vertices, shrinkRatio, polygon_centroid, shrinked_vertices);
}

/**
 * @brief computePolygonArea Compute the area of an input polygon
 * @param vertices[in] vertex description of the input polygon
 * @note (considers only X,Y components of vertices)
 * @return area
 */
inline double computePolygonArea(const  Eigen::MatrixXd vertices)
{
    // Initialze area
    double area = 0.0;
    int num_of_vertices = vertices.cols();
    // Calculate value of shoelace formula
    int j = num_of_vertices - 1;
    for (int i = 0; i < num_of_vertices; i++)
    {
        area += (vertices.col(j)(dls::rbd::X) + vertices.col(i)(dls::rbd::X)) * (vertices.col(j)(dls::rbd::Y) - vertices.col(i)(dls::rbd::Y));
        j = i;  // j is previous vertex to i
    }

    // Return absolute value
    return fabs(area / 2.0);
}

}
}

#endif
