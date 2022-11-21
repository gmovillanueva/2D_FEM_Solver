#ifndef _QUADRATURE_LEGENDREPOLYNOMIALS_
#define _QUADRATURE_LEGENDREPOLYNOMIALS_

#include <math.h>

namespace LegendrePolynomials
{
    double Legendre(int order, double location)
    {
        double lpoly_m1 = 1;
        double lpoly   = location;
        double lpoly_n1 = 0;

        if(order == 0){ return 1; }
        if(order == 1){ return order; }

        for(int n = 2; n < order; n++)
        {
            int s_order = n - 1;
            lpoly_n1 = ((2.0 * s_order + 1) / (s_order + 1.0)) * location * lpoly - (s_order / (s_order + 1.0)) * lpoly_m1;
            lpoly_m1 = lpoly;
            lpoly = lpoly_n1;
        }

        return lpoly_n1;
    }

    double LegendreDX(int order, double location)
    {
        if(order == 0){ return 0; }
        if(order == 1){ return 1; }
        double lpoly_dx  = (order * location / (pow(location, 2.0) - 1)) * Legendre(order, location);
               lpoly_dx -= (order / (pow(location, 2.0) - 1)) * Legendre(order, location);

        return lpoly_dx;
    }


}

#endif