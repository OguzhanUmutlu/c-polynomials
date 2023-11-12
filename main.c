#include <stdio.h>
#include "polynomial.h"

num_t my_func(num_t x) {
    return x * x;
}

int main() {
    printPolynomial(
            // (x + 1)^2
            powPolynomialInt(CreatePolynomial(2, 1, 1), 10)
    );
    // expected output:

    // x¹⁰ + 10.000000x⁹ +
    // 45.000000x⁸ + 120.000000x⁷ +
    // 210.000000x⁶ + 252.000000x⁵ +
    // 210.000000x⁴ + 120.000000x³ +
    // 45.000000x² + 10.000000x + 1.000000

    Polynomial solving = CreatePolynomial(2, 1, 1); // x + 1
    solving = multiplyTwoPolynomials(solving, CreatePolynomial(2, 2, 1)); // x + 2
    solving = multiplyTwoPolynomials(solving, CreatePolynomial(2, 3, 1)); // x + 3
    solving = multiplyTwoPolynomials(solving, CreatePolynomial(2, 4, 1)); // x + 4
    solving = multiplyTwoPolynomials(solving, CreatePolynomial(2, 5, 1)); // x + 5
    solving = multiplyTwoPolynomials(solving, CreatePolynomial(2, 6, 1)); // x + 6

    printf("The solution to the polynomial: %Lf", solvePolynomial(solving, 20, 5));
    return 0;
}