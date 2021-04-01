#include "Fractals.hpp"

void mandelbrot(double& x, double& y, double cx, double cy) {
  double nx = x*x - y*y + cx;
  double ny = 2.0*x*y + cy;
  x = nx;
  y = ny;
}
void burning_ship(double& x, double& y, double cx, double cy) {
  double nx = x*x - y*y + cx;
  double ny = 2.0*std::abs(x*y) + cy;
  x = nx;
  y = ny;
}
void feather(double& x, double& y, double cx, double cy) {
  std::complex<double> z(x, y);
  std::complex<double> z2(x*x, y*y);
  std::complex<double> c(cx, cy);
  std::complex<double> one(1.0, 0.0);
  z = z*z*z/(one + z2) + c;
  x = z.real();
  y = z.imag();
}
void sfx(double& x, double& y, double cx, double cy) {
  std::complex<double> z(x, y);
  std::complex<double> c2(cx*cx, cy*cy);
  z = z * (x*x + y*y) - (z * c2);
  x = z.real();
  y = z.imag();
}
void henon(double& x, double& y, double cx, double cy) {
  double nx = 1.0 - cx*x*x + y;
  double ny = cy*x;
  x = nx;
  y = ny;
}
void duffing(double& x, double& y, double cx, double cy) {
  double nx = y;
  double ny = -cy*x + cx*y - y*y*y;
  x = nx;
  y = ny;
}
void ikeda(double& x, double& y, double cx, double cy) {
  double t = 0.4 - 6.0 / (1.0 + x*x + y*y);
  double st = std::sin(t);
  double ct = std::cos(t);
  double nx = 1.0 + cx*(x*ct - y*st);
  double ny = cy*(x*st + y*ct);
  x = nx;
  y = ny;
}
void chirikov(double& x, double& y, double cx, double cy) {
  y += cy*std::sin(x);
  x += cx*y;
}
