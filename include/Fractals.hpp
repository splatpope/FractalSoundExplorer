#ifndef FRACTALS_INCLUDED
#define FRACTALS_INCLUDED
#include "Complex.hpp"
void mandelbrot(FSE::Complex<>& p, FSE::Complex<> c);
void burning_ship(FSE::Complex<>& p, FSE::Complex<> c);
void feather(FSE::Complex<>& p, FSE::Complex<> c);
void sfx(FSE::Complex<>& p, FSE::Complex<> c);
void henon(FSE::Complex<>& p, FSE::Complex<> c);
void duffing(FSE::Complex<>& p, FSE::Complex<> c);
void ikeda(FSE::Complex<>& p, FSE::Complex<> c);
void chirikov(FSE::Complex<>& p, FSE::Complex<> c);
//Fractal abstraction definition
typedef void (*Fractal)(FSE::Complex<>&, FSE::Complex<>);
#endif
