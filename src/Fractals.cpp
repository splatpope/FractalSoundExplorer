#include <cmath>
#include "Fractals.hpp"

void mandelbrot(gam::Complex<double>& p, gam::Complex<double> c) {
  p = p*p + c;
}
void burning_ship(gam::Complex<double>& p, gam::Complex<double> c) {
  gam::Complex<double> p_abs{gam::abs(p)};
  p = p*p + c;
}
void feather(gam::Complex<double>& p, gam::Complex<double> c) {
  gam::Complex<double> p2{pow(p.r, 2), pow(p.i, 2)};
  gam::Complex<double> one{1.0, 0.0};
  p = p*p*p/(one + p2) + c;
}
void sfx(gam::Complex<double>& p, gam::Complex<double> c) {
  gam::Complex<double> c2{pow(c.r, 2), pow(c.i, 2)};
  p = p * p.normSqr() - (p * c2);
}
void henon(gam::Complex<double>& p, gam::Complex<double> c) {
  double nx {1.0 - c.r * p.r * p.r + p.i};
  double ny {c.i * p.r};
  p.r = nx;
  p.i = ny;
}
void duffing(gam::Complex<double>& p, gam::Complex<double> c) {
  double nx {p.i};
  double ny {-c.i * p.r + c.r * p.i - pow(p.i, 3)};
  p.r = nx;
  p.i = ny;
}
void ikeda(gam::Complex<double>& p, gam::Complex<double> c) { // apparently this is only one in a family of parametrized functions
  double t {0.4 - 6.0 / (1.0 + pow(p.r, 2) + pow(p.i, 2))};
  double st {std::sin(t)};
  double ct {std::cos(t)};
  double nx {1.0 + c.r * (p.r * ct - p.i * st)};
  double ny {c.i * (p.r * st + p.i * ct)};
  p.r = nx;
  p.i = ny;
}
void chirikov(gam::Complex<double>& p, gam::Complex<double> c) {
  p.i += c.i *std::sin(p.r);
  p.r += c.r * p.i;
}
