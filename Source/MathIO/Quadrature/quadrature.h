#ifndef _MATHIO_QUADRATURE_
#define _MATHIO_QUADRATURE_

#include <vector>

#include "../mathio.h"

struct QuadPoint2D
{
    double point_x;
    double point_y;
};

struct QuadPoint3D
{
    double point_x;
    double point_y;
    double point_z;
};

class mathio::Quadrature
{
public:
    std::vector<double> abscissas;
    std::vector<double> weights;
    std::vector<double> quadrature_point;

};

#endif // QUADRATURE_H