#ifndef C_POLYNOMIALS_POLYNOMIAL_H
#define C_POLYNOMIALS_POLYNOMIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#define CreatePolynomial(size, ...) (Polynomial){ (num_t[size]){__VA_ARGS__}, size }
#define num_t long double
#define uint unsigned

typedef num_t (*MathFn)(num_t x);

typedef struct {
    num_t *terms;
    uint size;
} Polynomial;

uint factorial(uint n);

uint oneOverFactorial(uint n);

uint permutation(uint a, uint b);

uint combination(uint a, uint b);

num_t evaluatePolynomial(Polynomial polynomial, num_t at);

Polynomial emptyPolynomial(num_t val);

void addAPolynomialTo(Polynomial *target, Polynomial polynomial);

Polynomial addTwoPolynomials(Polynomial a, Polynomial b);

Polynomial addPolynomials(Polynomial *polynomials, uint size);

void multiplyPolynomialByInt(Polynomial *a, num_t num);

Polynomial multiplyPolynomialInt(Polynomial a, num_t num);

Polynomial multiplyTwoPolynomials(Polynomial a, Polynomial b);

Polynomial multiplyPolynomials(Polynomial *polynomials, uint size);

Polynomial powPolynomialInt(Polynomial polynomial, uint power);

Polynomial derivePolynomial(Polynomial polynomial);

Polynomial nthDerivativePolynomial(Polynomial polynomial, uint nth);

num_t approximate1stDerivativeFn(MathFn fn, num_t at);

num_t approximate2ndDerivativeFn(MathFn fn, num_t at);

num_t approximate3rdDerivativeFn(MathFn fn, num_t at);

num_t approximateNthDerivativeFn(MathFn fn, uint n, num_t at);

num_t approximateNthDerivativeFnTaylor(MathFn fn, uint n, num_t at);

Polynomial integratePolynomial(Polynomial polynomial);

Polynomial approximatePolynomial(MathFn fn, int a, uint iterations);

Polynomial approximatePolynomialAZero(MathFn fn, uint iterations);

num_t solvePolynomial(Polynomial polynomial, uint iterations, num_t x0);

void printAsPower(uint number);

void printPolynomial(Polynomial polynomial);

void printFastPolynomial(Polynomial polynomial);

#pragma clang diagnostic pop

#endif //C_POLYNOMIALS_POLYNOMIAL_H