/****************************************************************************
**  Copyright (c) 2016, Adel Kara Slimane <adel.ks@zegrapher.com>
**
**  This file is part of ZeGrapher's source code.
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


#include "numberlineedit.h"

NumberLineEdit::NumberLineEdit(bool allowK, QList<FuncCalculator*> otherFuncs) : calculator(allowK, otherFuncs)
{
    QColor color;
    color.setNamedColor(VALID_COLOR);
    validPalette.setColor(QPalette::Base, color);
    validPalette.setColor(QPalette::Text, Qt::black);

    color.setNamedColor(INVALID_COLOR);
    invalidPalette.setColor(QPalette::Base, color);
    invalidPalette.setColor(QPalette::Text, Qt::black);

    valid = false;
    value = NAN;

    connect(this, SIGNAL(returnPressed()), this, SLOT(checkVal()));
    connect(this, SIGNAL(editingFinished()), this, SLOT(checkVal()));
}

void NumberLineEdit::clear()
{
    setPalette(neutralPalette);
    QLineEdit::clear();
}

bool NumberLineEdit::isValid()
{
    return valid;
}

double NumberLineEdit::getValue()
{
    return value;
}

void NumberLineEdit::setNumber(double num)
{
    setText(QString::number(num));
    value = num;
    setPalette(validPalette);
    valid = true;
}

void NumberLineEdit::checkVal()
{
    valid = false;
    if(text().isEmpty())
    {
        setPalette(neutralPalette);
        return;
    }

    bool ok = true;
    double num = calculator.calculateExpression(text(), ok);

    if(ok)
    {
        setNumber(num);
        value = num;
        valid = true;
        emit newVal(num);
    }
    else setPalette(invalidPalette);    
}
