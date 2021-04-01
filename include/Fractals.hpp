#ifndef FRACTALS_INCLUDED
#define FRACTALS_INCLUDED
#include <complex>
void mandelbrot(double& x, double& y, double cx, double cy);
void burning_ship(double& x, double& y, double cx, double cy);
void feather(double& x, double& y, double cx, double cy);
void sfx(double& x, double& y, double cx, double cy);
void henon(double& x, double& y, double cx, double cy);
void duffing(double& x, double& y, double cx, double cy);
void ikeda(double& x, double& y, double cx, double cy);
void chirikov(double& x, double& y, double cx, double cy);
//Fractal abstraction definition
typedef void (*Fractal)(double&, double&, double, double);
#endif
