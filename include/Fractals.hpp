#ifndef FRACTALS_INCLUDED
#define FRACTALS_INCLUDED
#include "Gamma/Types.h"
void mandelbrot(gam::Complex<double>& p, gam::Complex<double> c);
void burning_ship(gam::Complex<double>& p, gam::Complex<double> c);
void feather(gam::Complex<double>& p, gam::Complex<double> c);
void sfx(gam::Complex<double>& p, gam::Complex<double> c);
void henon(gam::Complex<double>& p, gam::Complex<double> c);
void duffing(gam::Complex<double>& p, gam::Complex<double> c);
void ikeda(gam::Complex<double>& p, gam::Complex<double> c);
void chirikov(gam::Complex<double>& p, gam::Complex<double> c);
//Fractal abstraction definition
typedef void (*Fractal)(gam::Complex<double>&, gam::Complex<double>);
#endif
