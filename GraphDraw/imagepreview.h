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


#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include "graphdraw.h"


class ImagePreview : public GraphDraw
{
    Q_OBJECT
public:
    explicit ImagePreview(Informations *info);
    QImage* drawImage();

public slots:    
    void setlegendFontSize(int size);
    void setLegendState(bool show);

    void setXaxisLegend(QString legend);
    void setYaxisLegend(QString legend);

    void setBold(bool state);
    void setUnderline(bool state);
    void setItalic(bool state);
    void setNumPrec(int prec);

protected:   

    void paintEvent(QPaintEvent *event);
    void determinerCentreEtUnites();
    void placerGraduations();
    void drawAxes();
    void paint();
    void assignGraphSize();
    void writeLegends();

    int leftMargin, rightMargin, topMargin, bottomMargin, legendFontSize, additionalMargin, numPrec;
    QString xLegend, yLegend;
    bool legendState, bold, italic, underline;
    QTimer updateTimer;

};

#endif // ImagePreview_H