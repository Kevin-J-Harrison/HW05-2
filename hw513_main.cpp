#include "gvsu_cis.h"

#include "Polynomial.hpp"
#include <sstream>

TEST_CASE ("Getter")
{
    /* 8x^100 - 3x^5 + 9 */
    Polynomial<float> one("[8 100] [-3 5] [9 0]");

    SECTION ("Coefficient getters") {
        REQUIRE (one[0] == Approx(8.0));
        REQUIRE (one[1] == Approx(-3.0));
        REQUIRE (one[2] == Approx(9.0));
    }
    SECTION ("Exponent getters") {
        REQUIRE (one % 0 == 100);
        REQUIRE (one % 1 == 5);
        REQUIRE (one % 2 == 0);
    }

    SECTION ("Highest Order") {
        REQUIRE (one.maxDegree() == 100);
    }
}

TEST_CASE ("Evaluate Constant Polynom")
{
    Polynomial<float> one("[2.5 0]");

    for (int k = -10; k < 10; k++)
        REQUIRE (one(k) == Approx(2.5));
}

TEST_CASE ("Evaluate Polynom")
{
    /* 4x^8 - 6x^3 - 9 */
    Polynomial<float> one("[4 8] [-6 3] [-9 0]");

    REQUIRE (one(-1) == Approx (1.0));
    REQUIRE (one(0) == Approx (-9.0));
    REQUIRE (one(1) == Approx (-11.0));
    REQUIRE (one(2) == Approx (967));
}

TEST_CASE ("Multiplication")
{
    /* 2x^3 - 5x + 7 */
    Polynomial<float> one("[2 3] [-5 1] [7 0]");

    /* 6x^5 + x^2 */
    Polynomial<float> two("[6 5] [1 2]");

    Polynomial<float> result;

    SECTION ("Multiplication by zero") {
        Polynomial<float> zero("[0 0]");
        result = one * zero;
        REQUIRE (result.maxDegree() == 0);
        REQUIRE (result[0] == Approx(0.0));
        REQUIRE (result%0 == 0);
    }

    SECTION ("Multiplication by constant") {
        Polynomial<float> constant ("[4 0]");
        /* 2x^3 - 5x + 7 */
        result = one * constant;
        REQUIRE (result.maxDegree() == 3);
        REQUIRE (result[0] == Approx(8.0));
        REQUIRE (result[1] == Approx(-20.0));
        REQUIRE (result[2] == Approx(28.0));
        REQUIRE (result%0 == 3);
        REQUIRE (result%1 == 1);
        REQUIRE (result%2 == 0);
    }

    SECTION ("Self multiply") {
        result = one * one;
        //Polynomial<float> one("[2 3] [-5 1] [7 0]");
        //[4 6][-20 4][28 3][25 2][-70 1][49 0]
        REQUIRE (result.maxDegree() == 6);
        REQUIRE (result[0] == Approx(4.00));
        REQUIRE (result % 0 == 6);
        REQUIRE (result[1] == Approx(-20.00));
        REQUIRE (result % 1 == 4);
        REQUIRE (result[2] == Approx(28.00));
        REQUIRE (result % 2 == 3);
        REQUIRE (result[3] == Approx(25.00));
        REQUIRE (result % 3 == 2);
        REQUIRE (result[4] == Approx(-70.00));
        REQUIRE (result % 4 == 1);
        REQUIRE (result[5] == Approx(49.00));
        REQUIRE (result % 5 == 0);
    }

    SECTION("Multiplication of two polynoms") {
        result = one * two;
        REQUIRE (result.maxDegree() == 8);
        /* 2x^3 - 5x + 7 */
        /* 6x^5 + x^2 */
        REQUIRE (result[0] == Approx(12.0));
        REQUIRE (result % 0 == 8);
        REQUIRE (result[1] == Approx(-30.0));
        REQUIRE (result % 1 == 6);
        REQUIRE (result[2] == Approx(44.0));
        REQUIRE (result % 2 == 5);
        REQUIRE (result[3] == Approx(-5.0));
        REQUIRE (result % 3 == 3);
        REQUIRE (result[4] == Approx(7.0));
        REQUIRE (result % 4 == 2);
    }

    SECTION ("Multiplication is commutative") {
        Polynomial<float> r1, r2;
        r1 = one * two;
        r2 = two * one;
        for (int k = 0; k < 5; k++)
            REQUIRE (r1[k] == Approx (r2[k]));
    }

    SECTION ("Compare two techniques") {
        Polynomial<float> r1, r2;
        r1 = one * two;
        r2 = one % two;
        for (int k = 0; k < 5; k++) {
            REQUIRE (r1[k] == Approx (r2[k]));
            REQUIRE (r1 % k == r2 % k);
        }
    }
}

TEST_CASE ("Large Polynomials") {
    const int N1 = 5000, N2 = 2000;
    ostringstream t1, t2;
    for (int k = N1; k >= 0; k--)
        t1 << "[1 " << k << "]";
    for (int k = N2; k >= 0; k--)
        t2 << "[1 " << k << "]";
    Polynomial<float> p1(t1.str());
    Polynomial<float> p2(t2.str());
    Polynomial<float> prod1, prod2;
    prod1 = p1 * p2;
    for (int k = 0; k < N2; k++) {
        REQUIRE (prod1[k] == k + 1);
    }
    for (int k = N2; k < N1; k++) {
        REQUIRE (prod1[k] == Approx(N2+1));
    }
}