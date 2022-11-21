#ifndef _QUADRATURE_LEGENDREGAUSS_
#define _QUADRATURE_LEGENDREGAUSS_

#include <iostream>
#include "quadrature.h"
#include "quadrature_legendrepolynomials.h"

class mathio::QuadratureLegendreGauss : public Quadrature
{
public:
    void Initialize(int order,
                    int max_iterations = 1000,
                    double tolerance = 1.0E-10,
                    bool verbose = false);
};

void mathio::QuadratureLegendreGauss::Initialize(int order, int max_iterations, double tolerance, bool verbose)
{
    if(verbose)
    {
        std::cout << "Initializing Guass-Legendre Quadratures with " << order << "quadrature points" << "\n";
    }

    double dx = 2.0/2000;
    double gx_old = -1.0;
    double gx_new;
    double func_old = LegendrePolynomials::Legendre(order, gx_old);
    double func_new;

    for(int i = 0; i < 2000; ++i)
    {
        gx_new = gx_old + dx;
        func_new = LegendrePolynomials::Legendre(order, gx_new);

        if((func_new * func_old) < 0.0)
        {
            abscissas.push_back(gx_new);
            weights.push_back(1.0);
        }

        gx_old = gx_new;
        func_old = func_new;
    }

    for(unsigned k = 0; k < abscissas.size(); k++)
    {
        int i = 0;
        double x_old, x_new;
        double a, b, c;
        double residual;

        while(i < max_iterations)
        {
            x_old = abscissas[k];
            a = LegendrePolynomials::Legendre(order, x_old);
            b = LegendrePolynomials::LegendreDX(order, x_old);
            c = 0;

            for(unsigned j = 0; j < k; ++j)
            {
                c += 1.0 / (x_old - abscissas[j]);
            }

            x_new = x_old - (a / (b - a * c));

            residual = fabs(x_new - x_old);
            abscissas[k] = x_new;

            if(residual < tolerance){ break;}

            i++;
        }

        weights[k] = 2.0 *
                     (1.0 - abscissas[k] * abscissas[k]) /
                     (order + 1) /
                     (order + 1) /
                     LegendrePolynomials::Legendre(order + 1, abscissas[k]) /
                     LegendrePolynomials::Legendre(order + 1, abscissas[k]);

        if(verbose)
        {
            std::cout << "root = " << abscissas[k] << ", weight = " << weights[k] << "\n";
        }
    }



}


#endif