#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "polynomial.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

const num_t EPSILON = 0.0001;
const num_t EPSILON_2 = EPSILON * EPSILON;
const num_t EPSILON_3 = EPSILON_2 * EPSILON;
const num_t _2_EPSILON = 2 * EPSILON; // NOLINT(*-reserved-identifier)
const num_t _3_EPSILON = 3 * EPSILON; // NOLINT(*-reserved-identifier)
const num_t OVER_EPSILON = 1 / EPSILON;

typedef num_t (*MathFn)(num_t x);

char powerSymbols[10][4] = {"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹"};

uint factorial(uint n) {
    uint product = 1;
    for (uint i = 2; i <= n; i++) {
        product *= i;
    }
    return product;
}

uint oneOverFactorial(uint n) {
    uint product = 1;
    for (uint i = 2; i <= n; i++) {
        product /= i;
    }
    return product;
}

uint permutation(uint a, uint b) {
    if (a < b) {
        fprintf(stderr, "Invalid permutation argument.");
        exit(EXIT_FAILURE);
    }
    if (b == 0) return 1;
    if (a == b) return factorial(a);
    uint product = 1;
    // 6, 2 -> 5 * 6
    for (uint i = a - b + 1; i <= a; i++) {
        product *= i;
    }
    return product;
}

uint combination(uint a, uint b) {
    if (a < b) {
        fprintf(stderr, "Invalid combination argument.");
        exit(EXIT_FAILURE);
    }
    if (a == b || b == 0) return 1;
    if (b > a / 2) b = a - b;
    if (b == 1) return a;
    return permutation(a, b) / factorial(b);
}

num_t evaluatePolynomial(Polynomial polynomial, num_t at) {
    num_t result = 0;
    num_t c_at = 1;
    uint i = 0;
    while (true) {
        result += polynomial.terms[i] * c_at;
        i++;
        if (i == polynomial.size) return result;
        c_at *= at;
    }
}

Polynomial emptyPolynomial(num_t val) {
    num_t *terms = malloc(sizeof(num_t));
    terms[0] = val;
    return (Polynomial) {terms, 1};
}

void addAPolynomialTo(Polynomial *target, Polynomial polynomial) {
    unsigned target_size = target->size;
    unsigned poly_size = polynomial.size;
    if (target_size == 0 || poly_size == 0) return;
    unsigned min_size = (target_size > poly_size) ? poly_size : target_size;
    unsigned size_diff = poly_size - target_size;
    if (size_diff > 0) {
        target->size = polynomial.size;
        num_t *newPoly = (num_t *) realloc(target->terms, poly_size * sizeof(num_t));
        if (newPoly == NULL) {
            fprintf(stderr, "Memory reallocation failed. Method: addPolynomialTo()\n");
            free(target->terms);
            exit(EXIT_FAILURE);
        }
        for (unsigned i = target_size; i < poly_size; i++) {
            newPoly[i] = polynomial.terms[i];
        }
        target->terms = newPoly;
    }
    for (int i = 0; i < min_size; i++) {
        target->terms[i] += polynomial.terms[i];
    }
}

Polynomial addTwoPolynomials(Polynomial a, Polynomial b) {
    uint max_size = (a.size > b.size) ? a.size : b.size;
    if (max_size == 0) return emptyPolynomial(0);
    num_t *terms = (num_t *) malloc(max_size * sizeof(num_t));
    for (uint i = 0; i < max_size; i++) {
        terms[i] = (a.size > i ? a.terms[i] : 0) + (b.size > i ? b.terms[i] : 0);
    }
    return (Polynomial) {terms, max_size};
}

Polynomial addPolynomials(Polynomial *polynomials, uint size) {
    if (size == 0) return emptyPolynomial(0);

    uint max_size = 0;
    for (uint i = 0; i < size; i++) {
        uint s = polynomials[i].size;
        if (s > max_size) max_size = s;
    }

    if (max_size == 0) return emptyPolynomial(0);

    num_t *terms = (num_t *) malloc(max_size * sizeof(num_t));
    for (uint i = 0; i < max_size; i++) {
        terms[i] = 0;
        for (uint j = 0; j < size; j++) {
            Polynomial p = polynomials[j];
            if (p.size > i) terms[i] += p.terms[i];
        }
    }

    return (Polynomial) {terms, max_size};
}

void multiplyPolynomialByInt(Polynomial *a, num_t num) {
    for (uint i = 0; i < a->size; i++) {
        a->terms[i] *= num;
    }
}

Polynomial multiplyPolynomialInt(Polynomial a, num_t num) {
    num_t *terms = malloc(a.size * sizeof(num_t));
    for (uint i = 0; i < a.size; i++) {
        terms[i] *= num;
    }
    return (Polynomial) {terms, a.size};
}

Polynomial multiplyTwoPolynomials(Polynomial a, Polynomial b) {
    uint max_size = a.size * b.size;
    if (max_size == 0) return emptyPolynomial(0);
    num_t *terms = (num_t *) malloc(max_size * sizeof(num_t));
    for (uint i = 0; i < max_size; i++) {
        terms[i] = 0;
    }
    for (uint i = 0; i < a.size; i++) {
        for (uint j = 0; j < b.size; j++) {
            terms[i + j] += a.terms[i] * b.terms[j];
        }
    }
    return (Polynomial) {terms, max_size};
}

Polynomial multiplyPolynomials(Polynomial *polynomials, uint size) {
    if (size == 0) return emptyPolynomial(0);
    if (size == 1) return polynomials[0];
    if (size == 2) return multiplyTwoPolynomials(polynomials[0], polynomials[1]);

    Polynomial p = polynomials[0];
    for (uint i = 1; i < size; i++) {
        p = multiplyTwoPolynomials(p, polynomials[i]);
    }

    return p;
}

Polynomial powPolynomialInt(Polynomial polynomial, uint power) {
    if (power == 0) return emptyPolynomial(1);
    if (power == 1) return polynomial;
    if (power == 2) return multiplyTwoPolynomials(polynomial, polynomial);

    Polynomial p = polynomial;
    for (uint i = 1; i < power; i++) {
        p = multiplyTwoPolynomials(p, polynomial);
    }

    return p;
}

Polynomial derivePolynomial(Polynomial polynomial) {
    if (polynomial.size == 0) return emptyPolynomial(0);
    num_t *terms = (num_t *) malloc((polynomial.size - 1) * sizeof(num_t));

    for (int i = 0; i < polynomial.size - 1; i++) {
        terms[i] = polynomial.terms[i + 1] * (i + 1);
    }

    return (Polynomial) {terms, polynomial.size - 1};
}

Polynomial nthDerivativePolynomial(Polynomial polynomial, uint nth) {
    if (nth == 0) return polynomial;
    if (nth == 1) return derivePolynomial(polynomial);

    Polynomial p = polynomial;
    for (uint i = 0; i < nth; i++) {
        p = derivePolynomial(p);
    }

    return p;
}

num_t approximate1stDerivativeFn(MathFn fn, num_t at) {
    return (fn(at + EPSILON) - fn(at)) / EPSILON;
}

num_t approximate2ndDerivativeFn(MathFn fn, num_t at) {
    return (fn(at + _2_EPSILON) - 2 * fn(at + EPSILON) + fn(at)) / EPSILON_2;
}

num_t approximate3rdDerivativeFn(MathFn fn, num_t at) {
    return (fn(at + _3_EPSILON) - 3 * fn(at + _2_EPSILON) + 3 * fn(at + EPSILON) - fn(at)) / EPSILON_3;
}

num_t approximateNthDerivativeFn(MathFn fn, uint n, num_t at) {
    if (n == 0) return fn(at);
    if (n == 1) return approximate1stDerivativeFn(fn, at);
    if (n == 2) return approximate2ndDerivativeFn(fn, at);
    if (n == 3) return approximate3rdDerivativeFn(fn, at);
    // changing the formula I found so that it is suitable for computing:
    // sum_{i=0}^n (1/ε)^n (-1)^i C(n, i) f(x + (n - i) ε)
    // sum_{i=0}^n (1/ε)^n (-1)^(n + i) C(n, i) f(x + iε)

    // sum_{i=0}^n (-1/ε)^n (-1)^i C(n, i) f(x + iε)

    num_t sum = 0;
    num_t constant = ((n % 2) * 2 - 1) * powl(OVER_EPSILON, n);
    for (uint i = 0; i <= n; i++) {
        // (-1)^i -> (i % 2) * 2 - 1
        sum += ((i % 2) * 2 - 1) * combination(n, i) * fn(at + i * EPSILON);
    }
    return constant * sum;
}

num_t approximateNthDerivativeFnTaylor(MathFn fn, uint n, num_t at) {
    // the result won't be the nth derivative.
    // C(n, i) = n! / ( i! (n - i)! )
    // in taylor series there is already an n! at the bottom
    // so they will cancel out and the thing stays will be:
    // 1 / ( i! (n - i)! )
    // which is just: oneOverFactorial(i) * oneOverFactorial(n - i)
    if (n == 0) return fn(at);
    if (n == 1) return approximate1stDerivativeFn(fn, at);
    if (n == 2) return approximate2ndDerivativeFn(fn, at);
    if (n == 3) return approximate3rdDerivativeFn(fn, at);

    num_t sum = 0;
    num_t constant = ((n % 2) * 2 - 1) * OVER_EPSILON;
    for (uint i = 0; i <= n; i++) {
        sum += ((i % 2) * 2 - 1) * fn(at + i * EPSILON) * oneOverFactorial(i) * oneOverFactorial(n - i);
    }
    return constant * sum;
}

Polynomial integratePolynomial(Polynomial polynomial) {
    if (polynomial.size == 0) return emptyPolynomial(0);
    num_t *terms = (num_t *) malloc((polynomial.size + 1) * sizeof(num_t));
    terms[0] = 0;

    for (int i = 1; i < polynomial.size + 1; i++) {
        terms[i] = polynomial.terms[i - 1] / i;
    }

    return (Polynomial) {terms, polynomial.size + 1};
}

Polynomial approximatePolynomial(MathFn fn, int a, uint iterations) {
    // sum_{n=0}^inf f^(n) (x-a)^n / n!
    Polynomial polynomial = emptyPolynomial(0);
    for (uint n = 0; n < iterations; n++) {
        num_t *addingPolyTerms = malloc(2 * sizeof(num_t));
        addingPolyTerms[0] = a;
        addingPolyTerms[1] = 1;
        Polynomial addingPoly = (Polynomial) {addingPolyTerms, 2};
        Polynomial adding = powPolynomialInt(addingPoly, n);
        multiplyPolynomialByInt(&adding, approximateNthDerivativeFnTaylor(fn, n, a));
        polynomial = addTwoPolynomials(polynomial, adding);
        free(addingPolyTerms);
        free(adding.terms);
    }
    return polynomial;
}

Polynomial approximatePolynomialAZero(MathFn fn, uint iterations) {
    // sum_{n=0}^inf f^(n) (x-a)^n / n!
    Polynomial polynomial = emptyPolynomial(0);
    for (uint n = 0; n < iterations; n++) {
        num_t *addingTerms = malloc((n + 1) * sizeof(num_t));
        for (uint i = 0; i < n; i++) {
            addingTerms[i] = 0;
        }
        addingTerms[n] = 1;
        Polynomial adding = {addingTerms, n};
        multiplyPolynomialByInt(&adding, approximateNthDerivativeFnTaylor(fn, n, 0));
        polynomial = addTwoPolynomials(polynomial, adding);
        free(addingTerms);
    }
    return polynomial;
}

num_t solvePolynomial(Polynomial polynomial, uint iterations, num_t x0) {
    // finds the real root which is closest to given "x0" value
    num_t x = x0;
    Polynomial derivative = derivePolynomial(polynomial);
    for (uint i = 0; i < iterations; i++) {
        x = x - evaluatePolynomial(polynomial, x) / evaluatePolynomial(derivative, x);
    }
    free(derivative.terms);
    return x;
}

void printAsPower(uint number) {
    char numberString[20];
    sprintf(numberString, "%d", number);
    uint long long length = strlen(numberString);
    for (int i = 0; i < length; ++i) {
        int digit = numberString[i] - '0';
        printf("%s", powerSymbols[digit]);
    }
}

void printPolynomial(Polynomial polynomial) {
    if (polynomial.size == 0) {
        printf("0\n");
        return;
    }

    uint max_index = polynomial.size - 1;
    bool has_first = false;

    for (uint i = max_index;; i--) {
        num_t term = polynomial.terms[i];
        if (term == 0) {
            if (i == 0) break;
            continue;
        }
        if (has_first) {
            if (term > 0) {
                printf(" + ");
            } else {
                printf(" - ");
            }
        }
        has_first = true;
        if (term != 1 || i == 0) {
            printf("%Lf", term > 0 ? term : -term);
        }
        if (i > 1) {
            printf("x");
            printAsPower(i);
        } else if (i == 1) {
            printf("x");
        }
        if (i == 0) break;
    }

    if (!has_first) {
        printf("0");
    }

    printf("\n");
}

void printFastPolynomial(Polynomial polynomial) {
    if (polynomial.size == 0) {
        printf("0");
        return;
    }
    for (uint i = polynomial.size - 1;; i--) {
        num_t term = polynomial.terms[i];
        if (term > 0) printf("+ "); else printf("- ");
        printf("%Lfx^%u ", term, i);
        if (i == 0) break;
    }

    printf("\n");
}

#pragma clang diagnostic pop