#include <cmath>
#include "Fractals.hpp"

void mandelbrot(FSE::Complex<>& p, FSE::Complex<> c) {
	p = p*p + c;
}
void burning_ship(FSE::Complex<>& p, FSE::Complex<> c) {
	double o_r {p.real()};
	double o_i {p.imag()};
	p.real(o_r*o_r - o_i*o_i);
	p.imag(2 * std::abs(o_r * o_i));
	p += c;
}
void feather(FSE::Complex<>& p, FSE::Complex<> c) {
	FSE::Complex<> p2{pow(p.real(), 2), pow(p.imag(), 2)};
	FSE::Complex<> one{1.0, 0.0};
	p = p*p*p/(one + p2) + c;
}
void sfx(FSE::Complex<>& p, FSE::Complex<> c) {
	FSE::Complex<> c2{pow(c.real(), 2), pow(c.imag(), 2)};
	p = p * std::norm(p) - (p * c2);
}
void henon(FSE::Complex<>& p, FSE::Complex<> c) {
	double n_r {1.0 - c.real() * p.real() * p.real() + p.imag()};
	double n_i {c.imag() * p.real()};
	p.real(n_r);
	p.imag(n_i);
}
void duffing(FSE::Complex<>& p, FSE::Complex<> c) {
	double n_r {p.imag()};
	double n_i {-c.imag() * p.real() + c.real() * p.imag() - pow(p.imag(), 3)};
	p.real(n_r);
	p.imag(n_i);
}
void ikeda(FSE::Complex<>& p, FSE::Complex<> c) { // apparently this is only one in a family of parametrized functions
	double t {0.4 - 6.0 / (1.0 + pow(p.real(), 2) + pow(p.imag(), 2))};
	double st {std::sin(t)};
	double ct {std::cos(t)};
	double n_r {1.0 + c.real() * (p.real() * ct - p.imag() * st)};
	double n_i {c.imag() * (p.real() * st + p.imag() * ct)};
	p.real(n_r);
	p.imag(n_i);
}
void chirikov(FSE::Complex<>& p, FSE::Complex<> c) {
	p.imag( p.imag() + c.imag() *std::sin(p.real()));
	p.real( p.real() + c.real() * p.imag());
}
