/****************************************************************************
**  Copyright (c) 2015, Adel Kara Slimane <adel.ks@zegrapher.com>
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



#ifndef FENETREBORNES_H
#define FENETREBORNES_H

#include "Structures.h"
#include "informations.h"
#include "Calculus/exprcalculator.h"


namespace Ui {
    class FenetreBornes;
}

class FenetreBornes : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreBornes(Informations *info);
    ~FenetreBornes();

public slots:
    void resetToStandardView();
    void updateWidgets();


private slots:
    void standardView();
    void orthonormal(bool state);
    void appliquer();

signals:
    void nouvelleFenetre(GraphRange fenetre);
    void orthonormalBasis(bool state);

private:

    Informations *informations;
    Ui::FenetreBornes *ui;
    QMessageBox *messageBox;    
    ExprCalculator *calculator;
};

#endif // FENETREBORNES_H
