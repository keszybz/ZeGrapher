/****************************************************************************
**  Copyright (c) 2015, Adel Kara Slimane, the ZeGrapher project <contact@zegrapher.com>
**
**  This file is part of the ZeGrapher project, version 2.1.
**
**  ZeGrapher is free software: you may copy, redistribute and/or modify it
**  under the terms of the GNU General Public License as published by the
**  Free Software Foundation, either version 3 of the License, or (at your
**  option) any later version.
**
**  This file is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <QList>
#include "Structures.h"

class Polynomial
{
public:
    Polynomial(const Polynomial &pol);    
    Polynomial(int monicMonomialDegree);
    Polynomial();

    double eval(double x);
    double getCoef(int degree);
    int degree();
    Polynomial product(const Polynomial &otherPol);
    Polynomial sum(const Polynomial &otherPol);
    Polynomial antiderivative();

    void setAffine(Point a, Point b);

    QList<double> getCoefsList();

    void operator+=(const Polynomial &P);
    void operator*=(double scal);
    void operator-=(const Polynomial &P);

    ~Polynomial();

protected:
    QList<double> coefficients;
};

Polynomial operator+(const Polynomial &P, const Polynomial &Q);
Polynomial operator-(const Polynomial &P, const Polynomial &Q);
Polynomial operator*(const Polynomial &P, const Polynomial &Q);
Polynomial operator*(double scal, const Polynomial &Q);

double continuousScalarProduct(const Polynomial &P, const Polynomial &Q, double a, double b);
double discreteScalarProduct(const Polynomial &P, const Polynomial &Q, const QList<Point> &data);


#endif // POLYNOMIAL_H