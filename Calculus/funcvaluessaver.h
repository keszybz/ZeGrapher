/****************************************************************************
**  Copyright (c) 2013, Adel Kara Slimane, the ZeGrapher project <contact@zegrapher.com>
**
**  This file is part of the ZeGrapher project, version 2.0.
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

#ifndef FUNCVALUESSAVER_H
#define FUNCVALUESSAVER_H

#include "informations.h"

class FuncValuesSaver
{
public:
    FuncValuesSaver(Informations *info);

    void calculateAll(double new_xUnit, double new_yUnit);
    void move(double pixels);

    double getStartAbsicssaUnit();
    double getStartAbscissaPixel();

    QList< QList< QList<double> > >* getFuncValsListPointer();

protected slots:
    void recalculateFuncColors(int id);

protected:
    void calculateAllFuncColors();

    Informations *informations;
    GraphRange graphRange;
    QList<FuncCalculator*> funcs;

    double xUnit, yUnit, pixelStep, unitStep;
    double startAbscissa_pixel, startAbscissa_unit, endAbscissa_pixel, endAbscissa_unit;
    double deplacement;

    QList< QList< QList<double> > > funcVals;
    QList< QList<QColor> > funcColors;
};

#endif // FUNCVALUESSAVER_H