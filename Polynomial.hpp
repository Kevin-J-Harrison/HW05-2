/*
 * Polynomial.hpp
 *
 *      Author: "Hans Dulimarta <dulimarh@cis.gvsu.edu>"
 *
 *      Completed by:Harrison, Kevin
 *                   Vanderhoef, Trevor
 *                   Vansteel, Alexander
 */

#ifndef POLYNOM_H_
#define POLYNOM_H_

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <climits>
#include <math.h>

#include <unordered_map>   // we use a HASH MAP

using namespace std;

template<typename T>
class Polynomial {
private:
    /* coeffient and exponent of the polynom are stored as a pair<> */
    vector<pair<T,int>> poly;

public:
    /* each term is a pair of coefficient and exponent, the type of
     * coefficient is determined by <T>, but exponent is always an integer */

    Polynomial() {
    }

    Polynomial(const string& input) {
        load(input);
    }

    Polynomial(const Polynomial& other) {
        poly = other.poly;
    }

    ~Polynomial()
    {
        poly.clear();
    }

    Polynomial& operator= (const Polynomial& other) {
        poly = other.poly;
        return *this;
    }

    Polynomial operator* (const Polynomial & other) const {
        Polynomial result;
        Polynomial temp;
        if(this->poly.size() != 0 && other.poly.size() != 0) {
            for (auto x: this->poly) {
                for (auto i: other.poly) {
                    temp.poly.push_back(make_pair(x.first * i.first, x.second + i.second));
                }
            }

            sort(temp.poly.begin(), temp.poly.end(), exponent_comparator());

            int expo = temp.maxDegree();
            float coef = 0.0;
            for (auto a: temp.poly) {
                if (a.second == expo) {
                    coef += a.first;
                }
                else {
                    result.poly.push_back(make_pair(coef, expo));
                    expo = a.second;
                    coef = a.first;
                }
            }
            result.poly.push_back(make_pair(coef, expo));
        }
        else {
            result.poly.push_back(make_pair(0, 0));
        }

        return result;

    }

    Polynomial operator% (const Polynomial& other) const {
        Polynomial result;
        map<int, T> polys;

        if(this->poly.size() != 0 && other.poly.size() != 0) {
            for (auto x: this->poly) {
                for (auto i: other.poly) {
                    T coe = x.first * i.first;
                    int exp = x.second + i.second;
                    polys[exp] += coe;
                }
            }

            for (auto x: polys) {
                result.poly.push_back(make_pair(x.second, x.first));
            }

            sort(result.poly.begin(), result.poly.end(), exponent_comparator());

        }
        else {
            result.poly.push_back(make_pair(0, 0));
        }

        return result;
    }

    /* Return the highest degree in the polynomial */
    int maxDegree() const {
        return poly[0].second;
    }

    /* return the k-th exponent or zero when the polynom has no terms */
    int operator% (int k) {
        if(k < poly.size() && poly.size() != 0)
            return poly[k].second;
        return 0;
    }

    /* return the k-th coefficient, or zero when the polynom is empty */
    T operator[] (int k) const {
        if(k < poly.size() && poly.size() != 0)
            return poly[k].first;
        return 0;
    }

    T operator() (T arg) const {
        double result = 0.0;
        for(auto x: poly){
            result += pow(arg, x.second) * x.first;
        }
        return result;
    }

    /* The following function "object" is needed for sorting
     * the polynomial terms in descending order of the exponent */
    struct exponent_comparator {
        bool operator () (const pair<T,int>& a, const pair<T,int>& b) {
            return a.second > b.second;
        }
    };

private:
    /* The load function reads in a string representation of a polynomial
     * and creates a vector of "polynomial terms".
     * The input string has the following format:
     *
     * [coeff int] [coeff int] .....
     *
     * For instance,   3x^5 - 7x^2 + 11 can be represented as one of 
     * the following string (whitespaces do not matter)
     *
     * [3 5] [-7 2] [11 0]
     * [3 5]   [-7   2]  [11 0]
     */
    void load(const string& polystring) {
        /* use a string input stream */
        stringstream input (polystring);
        const int L = polystring.length();
        T coeff;
        int expo = INT_MIN, last_expo;
        bool sortNeeded = false;

        /* skip the input, upto and including the '[' */
        input.ignore(L, '[');
        last_expo = expo;

        /* our string input stream is like a file, so we can check for
         * "end-of-file".... */
        while (!input.eof()) {
            input >> coeff >> expo;
            input.ignore(L, ']');

            if (fabs(coeff) > 1e-6)  /* include only non-zero coeffs */
            {
                poly.push_back(make_pair(coeff,expo));
                if (expo > last_expo)
                    sortNeeded = true;
                last_expo = expo;
            }
            input.ignore(L, '[');
        }

        /* sort the terms in increasing order of exponents */
        if (sortNeeded)
            sort(poly.begin(), poly.end(), exponent_comparator());
    }
};

#endif /* POLYNOM_H_ */