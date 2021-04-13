#ifndef COMPLEX_INCLUDED
#define COMPLEX_INCLUDED
#include <complex>
#include "Gamma/Types.h"
#include "SFML/System.hpp"

namespace FSE {

template <typename T=float>
class Complex : public std::complex<T> {

public:
    // from real and imag values
    Complex(const T& r = T(), const T& v = T()) : std::complex<T>(r, v)
    { 
    }
    // from same spec std::complex
    Complex(const std::complex<T>& other) : std::complex<T>(other)
    {
    }
    // from diff spec std::complex
    template <class X>
    Complex(const std::complex<X> other) : std::complex<T>(other)
    {
    }
    // from same spec gamma complex
    Complex(const gam::Complex<T>& gamma_complex) : std::complex<T>(gamma_complex.real(), gamma_complex.imag())
    {    
    }
    // from diff spec gamma complex
    template <class X>
    Complex(const gam::Complex<X>& gamma_complex) : std::complex<T>(static_cast<T>(gamma_complex.real()), static_cast<T>(gamma_complex.imag()))
    {
    }
    // from same spec SFML vector2
    Complex(const sf::Vector2<T>& sf_vec2) : std::complex<T>(sf_vec2.x, sf_vec2.y)
    {
    }
    // from diff spec SFML vector2
    template <class X>
    Complex(const sf::Vector2<X>& sf_vec2) : std::complex<T>(static_cast<T>(sf_vec2.x), static_cast<T>(sf_vec2.y))
    {
    }

};
}
#endif
