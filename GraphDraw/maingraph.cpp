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


#include "maingraph.h"

static double fipart(double x)
{
    if(x < 0)
    {
        return ceil(x);
    }
    else
    {
        return floor(x);
    }
}

MainGraph::MainGraph(Informations *info) : GraphDraw(info)
{   
    connect(info, SIGNAL(updateOccured()), this, SLOT(updateGraph()));
    connect(info, SIGNAL(animationUpdate()), this, SLOT(updateParEq()));  
    connect(info, SIGNAL(drawStateUpdateOccured()), this, SLOT(reactivateSmoothing()));

    exprCalculator = new ExprCalculator(false, info->getFuncsList());

    selectedCurve.isSomethingSelected = false;
    cancelUpdateSignal = false;
    resaveTangent = animationUpdate = false;

    mouseNotOnHWidget.setInterval(1000);
    mouseNotOnHWidget.setSingleShot(true);

    connect(&mouseNotOnHWidget, SIGNAL(timeout()), &hWidgetHideTransition, SLOT(start()));

    mouseNotOnVWidget.setInterval(1000);
    mouseNotOnVWidget.setSingleShot(true);

    connect(&mouseNotOnVWidget, SIGNAL(timeout()), &vWidgetHideTransition, SLOT(start()));

    hWidgetHideTransition.setInterval(30);
    vWidgetHideTransition.setInterval(30);

    hWidgetShowTransition.setInterval(30);
    vWidgetShowTransition.setInterval(30);

    connect(&hWidgetHideTransition, SIGNAL(timeout()), this, SLOT(hideHorWidget()));
    connect(&vWidgetHideTransition, SIGNAL(timeout()), this, SLOT(hideVerWidget()));
    connect(&hWidgetShowTransition, SIGNAL(timeout()), this, SLOT(showHorWidget()));
    connect(&vWidgetShowTransition, SIGNAL(timeout()), this, SLOT(showVerWidget()));

    timeWaitForXYWidgets.setInterval(1000);
    timeWaitForXYWidgets.setSingleShot(true);

    connect(&timeWaitForXYWidgets, SIGNAL(timeout()), &xyWidgetsHideTransition, SLOT(start()));

    xyWidgetsShowTransition.setInterval(30);
    xyWidgetsHideTransition.setInterval(30);

    connect(&xyWidgetsShowTransition, SIGNAL(timeout()), this, SLOT(showXYWidgets()));
    connect(&xyWidgetsHideTransition, SIGNAL(timeout()), this, SLOT(hideXYWidgets()));


    repaintTimer.setInterval(100);
    repaintTimer.setSingleShot(true);
    connect(&repaintTimer, SIGNAL(timeout()), this, SLOT(reactivateSmoothing()));

    timerX.setInterval(35);
    connect(&timerX, SIGNAL(timeout()), this, SLOT(zoomX()));

    timerY.setInterval(35);
    connect(&timerY, SIGNAL(timeout()), this, SLOT(zoomY()));

    dispPoint = false;
    boutonPresse = false;

    sourisSurUneCourbe = dispRectangle = recalculate = false;
    hHideStarted = vHideStarted = xyWidgetsState = mouseState.hovering = false;   
    moving = false;

    kLabel.setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #FFFFFF, stop: 0.3 #D0D0D0 , stop: 0.75 #FFFFFF, stop: 1 #FFFFFF); border-width: 1px; border-color: #D0D0D0; border-style: solid; border-radius: 10;");

    customFunctions << "f(x) =" << "g(x) =" << "h(x) =" << "p(x) =" << "r(x) =" << "m(x) =";
    customSequences << "u<sub>n</sub> =" << "v<sub>n</sub> =" << "l<sub>n</sub> =" << "w<sub>n</sub> =" << "q<sub>n</sub> =" << "t<sub>n</sub> =";

    kLabel.setParent(this);
    kLabel.hide();

    savedGraph = NULL;

    setMouseTracking(true);
    typeCurseur = NORMAL;

    addOtherWidgets();
}

void MainGraph::reactivateSmoothing()
{
    moving = recalculate = false;
    resaveGraph = true;
    update();
}

void MainGraph::updateParEq()
{
    animationUpdate = true;
    repaint();
}

void MainGraph::updateGraph()
{    
    if(!cancelUpdateSignal)
    {        
        resaveGraph = true;
        recalculate = true;
        update();
    }
    cancelUpdateSignal = false;

}

void MainGraph::addOtherWidgets()
{
    QLabel *zoom1 = new QLabel();
    zoom1->setFixedSize(25,25);
    zoom1->setScaledContents(true);
    zoom1->setPixmap(QPixmap(":/icons/zoom-in-icon.png"));

    QLabel *zoom2 = new QLabel();
    zoom2->setFixedSize(25,25);
    zoom2->setScaledContents(true);
    zoom2->setPixmap(QPixmap(":/icons/zoom-in-icon.png"));

    QLabel *unZoom1 = new QLabel();
    unZoom1->setFixedSize(25,25);
    unZoom1->setScaledContents(true);
    unZoom1->setPixmap(QPixmap(":/icons/zoom-out-icon.png"));

    QLabel *unZoom2 = new QLabel();
    unZoom2->setFixedSize(25,25);
    unZoom2->setScaledContents(true);
    unZoom2->setPixmap(QPixmap(":/icons/zoom-out-icon.png"));

    xTextLabel = new QLabel();
    xTextLabel->setFixedHeight(25);
    xTextLabel->setMaximumWidth(35);
    xTextLabel->setMinimumWidth(25);

    yTextLabel = new QLabel();
    yTextLabel->setFixedHeight(25);
    yTextLabel->setMaximumWidth(35);
    yTextLabel->setMinimumWidth(25);

    hSlider = new QSlider(Qt::Horizontal);
    hSlider->setFixedSize(250,20);
    hSlider->setMinimum(-50);
    hSlider->setMaximum(50);
    hSlider->setTickPosition(QSlider::TicksBothSides);
    hSlider->setFocusPolicy(Qt::NoFocus);
    hSlider->setValue(0);

    connect(hSlider, SIGNAL(sliderPressed()), &timerX, SLOT(start()));
    connect(hSlider, SIGNAL(sliderReleased()), this, SLOT(stop_X_zoom()));

    vSlider = new QSlider(Qt::Vertical);
    vSlider->setFixedSize(20,250);
    vSlider->setMinimum(-50);
    vSlider->setMaximum(50);
    vSlider->setTickPosition(QSlider::TicksBothSides);
    vSlider->setFocusPolicy(Qt::NoFocus);
    vSlider->setValue(0);

    connect(vSlider, SIGNAL(sliderPressed()), &timerY, SLOT(start()));
    connect(vSlider, SIGNAL(sliderReleased()), this, SLOT(stop_Y_Zoom()));

    lineX = new QLineEdit();
    lineX->setFixedSize(150,25);
    connect(lineX, SIGNAL(returnPressed()), this, SLOT(lineXReturnPressed()));

    lineY = new QLineEdit();   
    lineY->setReadOnly(true);
    lineY->setFixedSize(150,25);

    QVBoxLayout *verLayout = new QVBoxLayout();
    verLayout->addWidget(zoom1);
    verLayout->addWidget(vSlider);
    verLayout->addWidget(unZoom1);

    vWidget = new QWidget(this);
    vWidget->setLayout(verLayout);

    QHBoxLayout *horLayout = new QHBoxLayout();
    horLayout->addWidget(zoom2);
    horLayout->addWidget(hSlider);
    horLayout->addWidget(unZoom2);

    hWidget = new QWidget(this);
    hWidget->setLayout(horLayout);

    xWidget = new QWidget(this);
    QHBoxLayout *hbox1 = new QHBoxLayout();
    hbox1->addWidget(xTextLabel);
    hbox1->addWidget(lineX);
    xWidget->setLayout(hbox1);


    yWidget = new QWidget(this);
    QHBoxLayout *hbox2 = new QHBoxLayout();
    hbox2->addWidget(yTextLabel);
    hbox2->addWidget(lineY);
    yWidget->setLayout(hbox2);

    vWidget->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #FFFFFF, stop: 0.3 #D0D0D0 , stop: 0.75 #FFFFFF, stop: 1 #FFFFFF); border-width: 1px; border-color: #D0D0D0; border-style: solid; border-radius: 10;");

    hWidget->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 0.3 #D0D0D0 , stop: 1 #FFFFFF); border-width: 1px; border-color: #D0D0D0; border-style: solid; border-radius: 10;");
    hSlider->setStyleSheet("background-color: none; border: none;");
    zoom2->setStyleSheet("background-color: none; border: none;");
    unZoom2->setStyleSheet("background-color: none; border: none;");
    vSlider->setStyleSheet("background-color: none; border: none;");
    zoom1->setStyleSheet("background-color: none; border: none;");
    unZoom1->setStyleSheet("background-color: none; border: none;");
}

void MainGraph::hideHorWidget()
{   
    if(hWidget->y() >= hBottom)
    {
        hWidgetHideTransition.stop();
        hWidget->hide();
        hWidgetState = false;
        hHideStarted = false;
    }
    else hWidget->move(hWidget->x(), hWidget->y()+6);
}

void MainGraph::hideVerWidget()
{  
    if(vWidget->x() >= vBottom)
    {
        vWidgetHideTransition.stop();
        vWidget->hide();
        vWidgetState = false;
        vHideStarted = false;
    }
    else vWidget->move(vWidget->x()+6, vWidget->y());
}

void MainGraph::hideXYWidgets()
{
    xWidget->move(xWidget->x(), xWidget->y()+6);
    yWidget->move(yWidget->x(), yWidget->y()+6);
    if(xWidget->y() >= xyBottom)
    {
        xyWidgetsState = false;
        xyWidgetsHideTransition.stop();
        xWidget->hide();
        yWidget->hide();
    }
}

void MainGraph::showHorWidget()
{    
    hWidget->show();

    if(hWidget->y() <= hTopLeft.y())
    {
        hWidgetState = true;
        hWidgetShowTransition.stop();

    }
    else hWidget->move(hWidget->x(), hWidget->y()-6);
}

void MainGraph::showVerWidget()
{
    vWidget->show();
    if(vWidget->x() <= vTopLeft.x())
    {
        vWidgetState = true;
        vWidgetShowTransition.stop();        
    }
    else vWidget->move(vWidget->x()-6, vWidget->y());
}

void MainGraph::showXYWidgets()
{
    xWidget->show();
    yWidget->show();
    xWidget->move(xWidget->x(), xWidget->y()-6);
    yWidget->move(yWidget->x(), yWidget->y()-6);
    if(xWidget->y() <= xTopLeft.y())
    {
        xyWidgetsState = true;
        xyWidgetsShowTransition.stop();
    }
}

void MainGraph::lineXReturnPressed()
{
    bool ok = true;
    double val = exprCalculator->calculateExpression(lineX->text(), ok);

    if(ok)
        afficherPtX(val);
}

void MainGraph::wheelEvent(QWheelEvent *event)
{
    repaintTimer.stop();

    double x = event->pos().x();
    double y = event->pos().y();

    x = (x-centre.x)/uniteX;
    y = -(y-centre.y)/uniteY;  

    double valeur = (double)event->delta() / 1200;

    if(isinf(valeur))
        return;

    if((graphRange.Xmax - graphRange.Xmin > MIN_RANGE && graphRange.Ymax - graphRange.Ymin > MIN_RANGE) || valeur < 0)
    {
        graphRange.Xmax -= (graphRange.Xmax - x)*valeur;
        graphRange.Xmin -= (graphRange.Xmin - x)*valeur;
        graphRange.Ymax -= (graphRange.Ymax - y)*valeur;
        graphRange.Ymin -= (graphRange.Ymin - y)*valeur;

        moving = true;

        if(parametres.lissage)
            repaintTimer.start();

        informations->setRange(graphRange);
    }

    event->accept();

    //the graph will update from the "informations" class updateoccured() signal
}

void MainGraph::afficherPtX(double x)
{
    if(selectedCurve.isSomethingSelected && !selectedCurve.tangentSelection)
    {
        if(selectedCurve.funcType == FUNC_HOVER)
        {
            double k = 0;

            if(selectedCurve.isParametric)
            {
                Range info = funcs[selectedCurve.id]->getParametricRange();
                k = info.start + (double)(selectedCurve.kPos) * info.step;
            }

            pointUnit.x = x;
            pointUnit.y = funcs[selectedCurve.id]->getFuncValue(x, k);

            dispPoint = true;          
        }
        else if(selectedCurve.funcType == SEQ_HOVER)
        {
            if(fipart(x) >= seqs[selectedCurve.id]->get_nMin())
            {
                bool ok = true;
                x = fipart(x);
                pointUnit.x = x;
                pointUnit.y = seqs[selectedCurve.id]->getSeqValue(x, ok, selectedCurve.kPos);               
                dispPoint = true;
            }
        }

        update();

    }
}

void MainGraph::newWindowSize()
{
    emit sizeChanged(graphWidth, graphHeight);
    windowSize = size();

    if(hWidget != NULL)
    {
        hTopLeft.setX((width()-hWidget->width())/2);
        hTopLeft.setY(height()-hWidget->height());

        hBottom = hTopLeft.y()+hWidget->height()+5;

        xTopLeft.setX(hTopLeft.x()-xWidget->width());
        xTopLeft.setY(hTopLeft.y());

        yTopLeft.setX(hTopLeft.x()+hWidget->width());
        yTopLeft.setY(hTopLeft.y());

        vTopLeft.setX(width()-vWidget->width());
        vTopLeft.setY((height()-vWidget->height())/2);

        xyBottom = yTopLeft.y()+yWidget->height()+5;

        vBottom = vTopLeft.x()+vWidget->width()+5;

        yWidget->move(yTopLeft);
        xWidget->move(xTopLeft);

        if(!xyWidgetsState && !selectedCurve.isSomethingSelected)
            timeWaitForXYWidgets.start();

        vWidget->move(vTopLeft);
        hWidget->move(hTopLeft);

        hWidgetRect.setTopLeft(hTopLeft);
        hWidgetRect.setHeight(hWidget->height());
        hWidgetRect.setWidth(hWidget->width());

        vWidgetRect.setTopLeft(vTopLeft);
        vWidgetRect.setHeight(vWidget->height());
        vWidgetRect.setWidth(vWidget->width());

        hWidgetState = true;
        vWidgetState = true;

    }
}

void MainGraph::paintEvent(QPaintEvent *event)
{
    graphWidth = width();
    graphHeight = height();

    parametres = informations->getOptions();
    graphRange = informations->getRange();

    if(windowSize != size())
    {
       newWindowSize();
       resaveGraph = true;       
       recalculate = true;
    }

    if(!moving && (typeCurseur == NORMAL || hWidgetHideTransition.isActive() || vWidgetHideTransition.isActive() ||
            hWidgetShowTransition.isActive() || vWidgetShowTransition.isActive() || hWidgetState || vWidgetState ||
                   animationUpdate))
        indirectPaint();
    else directPaint();    

    event->accept();
}

void MainGraph::indirectPaint()
{
    if(resaveTangent)
        addTangentToBuffer();
    if(resaveGraph)
        resaveImageBuffer();

    painter.begin(this);

    painter.drawImage(QPoint(0,0), *savedGraph);
    painter.translate(QPointF(centre.x, centre.y));

    drawAnimatedParEq();
    animationUpdate = false;

    if(dispPoint)
        afficherPoint();

    if(selectedCurve.isSomethingSelected && selectedCurve.tangentSelection)
        drawOneTangent(selectedCurve.id);

    if(mouseState.hovering)
        drawHoveringConsequence();

    painter.end();

}

void MainGraph::directPaint()
{
    resaveGraph = true;

    painter.begin(this);
    //trace du background

    font.setPixelSize(parametres.numSize);
    painter.setFont(font);

    painter.setBrush(QBrush(parametres.couleurDuFond));
    painter.drawRect(-1, -1, graphWidth+1, graphHeight+1);

    determinerCentreEtUnites();
    drawAxes();
    placerGraduations();

    if(dispRectangle)
    {
        painter.setBrush(Qt::NoBrush);
        pen.setWidth(1);
        pen.setColor(parametres.couleurDesAxes);
        painter.setPen(pen);
        painter.drawRect(rectReel);
    }

    if(recalculate)
        funcValuesSaver->calculateAll(uniteX, uniteY);

    painter.translate(QPointF(centre.x, centre.y));

    drawFunctions();
    drawSequences();
    drawStraightLines();
    drawTangents();
    drawAllParEq();

    if(dispPoint)
        afficherPoint();    

    painter.end();
}

void MainGraph::drawHoveringConsequence()
{
    if(mouseState.funcType == FUNC_HOVER)
    {
        drawOneFunction(mouseState.id, parametres.epaisseurDesCourbes + 1, mouseState.kPos);
    }
    else if(mouseState.funcType == SEQ_HOVER)
    {
        drawOneSequence(mouseState.id, parametres.epaisseurDesCourbes + 4);
    }
    else if(mouseState.funcType == TANGENT_MOVE_HOVER || mouseState.funcType == TANGENT_RESIZE_HOVER)
    {
        TangentWidget *tangent = tangents->at(mouseState.id);
        TangentPoints points = tangent->getCaracteristicPoints();

        pen.setWidth(parametres.epaisseurDesCourbes + 4);
        pen.setColor(tangent->getColor());
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing);

        if(mouseState.funcType == TANGENT_MOVE_HOVER)
        {
            QPointF pt(points.center.x * uniteX, - points.center.y * uniteY);
            painter.drawPoint(pt);
        }
        else
        {
            QPointF pt1(points.left.x * uniteX, - points.left.y * uniteY);
            painter.drawPoint(pt1);

            QPointF pt2(points.right.x * uniteX, - points.right.y * uniteY);
            painter.drawPoint(pt2);
        }
    }
}

void MainGraph::addTangentToBuffer()
{
    resaveTangent = false;

    painter.begin(savedGraph);

    painter.translate(QPointF(centre.x, centre.y));
    painter.setRenderHint(QPainter::Antialiasing);
    drawOneTangent(tangentDrawException);
    tangentDrawException = -1;

    cancelUpdateSignal = true;
    informations->emitUpdateSignal();

    painter.end();
}

void MainGraph::resaveImageBuffer()
{
    resaveGraph = false;

    delete savedGraph;
    savedGraph = new QImage(size(), QImage::Format_RGB32);

    painter.begin(savedGraph);
    //trace du background

    font.setPixelSize(parametres.numSize);
    painter.setFont(font);

    painter.setBrush(QBrush(parametres.couleurDuFond));
    painter.drawRect(-1, -1, graphWidth+1, graphHeight+1);

    determinerCentreEtUnites();
    drawAxes();
    placerGraduations();

    if(recalculate)
        funcValuesSaver->calculateAll(uniteX, uniteY);

    painter.translate(QPointF(centre.x, centre.y));

    drawFunctions();
    drawSequences();
    drawStraightLines();
    drawTangents();
    drawStaticParEq();

    painter.end();
}

void MainGraph::determinerCentreEtUnites()
{
    uniteY = graphHeight / (graphRange.Ymax - graphRange.Ymin);
    uniteX = graphWidth / (graphRange.Xmax - graphRange.Xmin);

    bool orthonormal = informations->isOrthonormal();

    double rapport =  uniteY / uniteX;
    if(orthonormal && !(0.9999 < rapport && rapport < 1.0001))
    {
        graphRange.Ymin *= rapport;
        graphRange.Ymax *= rapport;
        uniteY = uniteX;

        cancelUpdateSignal = true;
        informations->setRange(graphRange);
        recalculate = true;
    }

    centre.x = - graphRange.Xmin * uniteX;
    centre.y =  graphRange.Ymax * uniteY;

    bool scaleChanged = false;

    QFontMetrics fontMeter(font);
    double last = fipart(graphRange.Xmax / graphRange.Xscale) * graphRange.Xscale;
    double numberSize = fontMeter.width(QString::number(last, 'g', NUM_PREC)) + 5;

    if(uniteX * graphRange.Xscale < numberSize + 10)
    {
        while(uniteX * graphRange.Xscale < numberSize + 10)
            graphRange.Xscale *= 2;
        if(orthonormal)
             graphRange.Yscale = graphRange.Xscale;
        scaleChanged = true;
    }
    else if(uniteX * graphRange.Xscale > 300)
    {
        while(uniteX * graphRange.Xscale > 300)
            graphRange.Xscale /= 2;
        if(orthonormal)
             graphRange.Yscale = graphRange.Xscale;
        scaleChanged = true;
    }
    if(!orthonormal)
    {
        if(uniteY * graphRange.Yscale < 25)
        {
            while(uniteY * graphRange.Yscale < 25)
                graphRange.Yscale *= 2;
            scaleChanged = true;
        }
        else if(uniteY * graphRange.Yscale > 200)
        {
            while(uniteY * graphRange.Yscale > 200)
                graphRange.Yscale /= 2;
            scaleChanged = true;
        }
    }

    if(scaleChanged)
    {
       cancelUpdateSignal = true;
       informations->setRange(graphRange);
       recalculate = true;
    }

}

void MainGraph::drawAllParEq()
{
    drawAnimatedParEq();
    drawStaticParEq();
}

void MainGraph::drawAnimatedParEq()
{
    painter.setRenderHint(QPainter::Antialiasing, parametres.lissage && !moving);

    QList< QList<Point> > *list;
    QPolygonF polygon;
    ParEqWidget *parWidget;
    ColorSaver *colorSaver;
    Point point;    

    pen.setWidth(parametres.epaisseurDesCourbes);
    painter.setPen(pen);

    int listEnd;

    for(int i = 0; i < parEqs->size(); i++)
    {
        parWidget = parEqs->at(i);
        colorSaver = parWidget->getColorSaver();

        if(parWidget->isAnimated())
        {
            if(parWidget->is_t_Animated())
                list = parWidget->getPointsList();
            else list = parWidget->getCurrentPolygon();

            for(int curve = 0; curve < list->size(); curve++)
            {
                if(!parWidget->is_t_Animated())// equals is_k_animated
                    pen.setColor(colorSaver->getColor(parWidget->getCurrentKPos()));
                else pen.setColor(colorSaver->getColor(curve));

                painter.setPen(pen);

                polygon.clear();

                if(parWidget->is_t_Animated())
                    listEnd = parWidget->getCurrentTPos();
                else listEnd = list->at(curve).size();

                for(int pos = 0 ; pos < listEnd; pos ++)
                {
                    point = list->at(curve).at(pos);
                    polygon << QPointF(point.x * uniteX, - point.y * uniteY);
                }

                if(parWidget->keepTracks() || !parWidget->is_t_Animated())
                    painter.drawPolyline(polygon);

                if(parWidget->is_t_Animated())
                {
                    pen.setWidth(parametres.epaisseurDesCourbes + 3);
                    painter.setPen(pen);

                    painter.drawPoint(polygon.last());

                    pen.setWidth(parametres.epaisseurDesCourbes);
                    painter.setPen(pen);
                }
            }
        }
    }
}

void MainGraph::afficherPoint()
{
    if(selectedCurve.funcType == FUNC_HOVER)
    {
        xTextLabel->setText("x = ");
        yTextLabel->setText(customFunctions[selectedCurve.id]);
    }
    else
    {
        xTextLabel->setText("n = ");
        yTextLabel->setText(customSequences[selectedCurve.id]);
    }

    xTextLabel->setStyleSheet("color: " + parametres.couleurDesAxes.name());
    yTextLabel->setStyleSheet("color: " + parametres.couleurDesAxes.name());



    if(selectedCurve.isParametric)
    {
        Range kRange;
        if(selectedCurve.funcType == FUNC_HOVER)
            kRange = funcs[selectedCurve.id]->getParametricRange();
        else kRange = seqs[selectedCurve.id]->getKRange();

        kLabel.move(5, 5);
        double result = kRange.start + selectedCurve.kPos * kRange.step;
        kLabel.setText("k = " + QString::number(result));
        kLabel.adjustSize();
        kLabel.show();
    }

    if(selectedCurve.funcType == FUNC_HOVER)
        pen.setColor(funcs[selectedCurve.id]->getColorSaver()->getColor(selectedCurve.kPos));
    else  pen.setColor(seqs[selectedCurve.id]->getColorSaver()->getColor(selectedCurve.kPos));

    int extraWidth = 3;
    if(selectedCurve.funcType == SEQ_HOVER)
        extraWidth += 2;

    pen.setWidth(parametres.epaisseurDesCourbes + extraWidth);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);

    pointPx.x = pointUnit.x * uniteX;
    pointPx.y = -pointUnit.y * uniteY;

    painter.drawPoint(pointPx.x, pointPx.y);

    lineX->setText(QString::number(pointUnit.x, 'g', NUM_PREC));
    lineY->setText(QString::number(pointUnit.y, 'g', NUM_PREC));

    dispPoint = false;
}

void MainGraph::mousePressEvent(QMouseEvent *event)
{
    boutonPresse = true;
    lastPosSouris.x = event->x();
    lastPosSouris.y = event->y();

    if(event->buttons() & Qt::LeftButton)
    {
        typeCurseur = NORMAL;

        if(mouseState.hovering && mouseState.tangentHovering)
        {
            selectedCurve.tangentSelection = true;
            selectedCurve.isSomethingSelected = true;
            selectedCurve.tangentPtSelection = mouseState.tangentPtSelection;
            selectedCurve.id = mouseState.id;
            selectedCurve.funcType = mouseState.funcType;
            tangentDrawException = mouseState.id;
            resaveGraph = true;
        }
    }
    else if(event->buttons() & Qt::RightButton)
    {
        typeCurseur = ZOOMBOX;
        dispRectangle = true;
        rectReel.setTopLeft(QPoint(event->x(), event->y()));
    }
}

void MainGraph::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    boutonPresse = false;

    if(typeCurseur == NORMAL)
    {
        if(mouseState.hovering && !mouseState.tangentHovering)
        {
            selectedCurve.tangentSelection = mouseState.tangentHovering;
            selectedCurve.isSomethingSelected = true;
            selectedCurve.isParametric = mouseState.isParametric;
            selectedCurve.kPos = mouseState.kPos;
            selectedCurve.id = mouseState.id;
            selectedCurve.funcType = mouseState.funcType;
            selectedCurve.tangentPtSelection = mouseState.tangentPtSelection;
            moving = false;

            if(selectedCurve.tangentSelection)
            {
                selectedCurve.isSomethingSelected = false;
                tangentDrawException = selectedCurve.id;
                resaveGraph = true;
            }
            else if(!xyWidgetsState)
                xyWidgetsShowTransition.start();
        }
        else
        {
            if(selectedCurve.isSomethingSelected && !moving)
            {
                if(selectedCurve.isSomethingSelected && selectedCurve.tangentSelection)
                    resaveTangent = true;

                selectedCurve.isSomethingSelected = false;
                xyWidgetsHideTransition.start();
                kLabel.hide();

            }
            else moving = false;

            update();
        }
    }   
    else if(typeCurseur == ZOOMBOX)
    {
        dispRectangle = false;
        if(rectReel.height() > 10 && rectReel.width() > 10)
        {
            GraphRange fen = graphRange;
            fen.Xmax = (rectReel.right() - centre.x) / uniteX;
            fen.Xmin = (rectReel.left() - centre.x) / uniteX;
            fen.Ymax = (- rectReel.top() + centre.y) / uniteY;
            fen.Ymin = (- rectReel.bottom() + centre.y) / uniteY;

            if(fen.Xmax - fen.Xmin > MIN_RANGE && fen.Ymax - fen.Ymin > MIN_RANGE)
                informations->setRange(fen);

            typeCurseur = NORMAL;
        }
    }  
}

void MainGraph::mouseMoveEvent(QMouseEvent *event)
{    
    bool refresh = false;

    mouseX = event->x();
    mouseY = event->y();

    if(hWidgetRect.contains(mouseX, mouseY))
    {
        hWidgetHideTransition.stop();
        hHideStarted = false;
        hWidgetShowTransition.start();
    }
    else if(hWidgetState && !hHideStarted)
    {
        mouseNotOnHWidget.start();
        hHideStarted = true;
    }

    if(vWidgetRect.contains(mouseX, mouseY))
    {        
        vWidgetHideTransition.stop();
        vHideStarted = false;
        vWidgetShowTransition.start();
    }
    else if(vWidgetState && !vHideStarted)
    {
        mouseNotOnVWidget.start();
        vHideStarted = true;
    }

    if(typeCurseur == NORMAL)
    {
        double x =  (mouseX - centre.x) / uniteX;
        double y = - (mouseY - centre.y) / uniteY;

        if(selectedCurve.isSomethingSelected)
        {
            mouseMoveWithActiveSelection(x, y);
            refresh = true;
        }

        mouseState.hovering = false;

        mouseTangentHoverTest(x, y);

        if(!mouseState.hovering)
        {
            mouseSeqHoverTest(x, y);
            refresh = refresh || mouseState.hovering;
        }

        if(!mouseState.hovering)
        {
            mouseFuncHoverTest(x, y);
            refresh = refresh || mouseState.hovering;
        }

        if(!mouseState.hovering && mouseState.id != -1)
        {
            mouseState.id = -1;
            refresh = true;
        }

        if(boutonPresse && !(selectedCurve.isSomethingSelected && selectedCurve.tangentSelection))
        {
            double dx = mouseX - lastPosSouris.x;
            double dy = mouseY - lastPosSouris.y;

            centre.x += dx;
            centre.y += dy;

            graphRange.Xmax -= dx / uniteX;
            graphRange.Xmin -= dx / uniteX;

            graphRange.Ymax += dy / uniteY;
            graphRange.Ymin += dy / uniteY;

            cancelUpdateSignal = true;
            informations->setRange(graphRange);

            determinerCentreEtUnites();

            if(dx != 0)
                funcValuesSaver->move(dx);

            moving = true;
            refresh = true;
        }
    }  
    else if(typeCurseur == ZOOMBOX)
    {
        rectReel.setBottomRight(QPoint(event->x(), event->y()));
        refresh = true;
    }   

    lastPosSouris.x = event->x();
    lastPosSouris.y = event->y();

    if(refresh)
        update();


}

void MainGraph::mouseMoveWithActiveSelection(double x, double y)
{
    Q_UNUSED(y);

    short k_pos = 0;
    double k = 0;

    if(selectedCurve.isParametric)
    {
        if(selectedCurve.funcType == FUNC_HOVER)
        {
            Range info = funcs[selectedCurve.id]->getParametricRange();
            k = info.start + (double)selectedCurve.kPos * info.step;
        }
        else k_pos = selectedCurve.kPos;
    }
    if(selectedCurve.funcType == FUNC_HOVER)
    {
        pointUnit.x = x;
        pointUnit.y = funcs[selectedCurve.id]->getFuncValue(x, k);

        dispPoint = true;
        recalculate = false;
    }
    else if(selectedCurve.funcType == SEQ_HOVER && x >= seqs[0]->get_nMin()-0.3)
    {
        double start, step = 1;
        bool ok = true;

        if(graphRange.Xmin > seqs[0]->get_nMin())
            start = fipart(graphRange.Xmin);
        else start = seqs[0]->get_nMin();

        if(uniteX < 1)
            step = 5*fipart(1/uniteX);

        if(fabs(fipart((x-start)/step) - (x-start)/step) < 0.5*step)
            pointUnit.x = fipart((x-start)/step) * step + start;
        else pointUnit.x = fipart((x-start)/step) * step + start + step;

        pointUnit.y = seqs[selectedCurve.id]->getSeqValue(pointUnit.x, ok, k_pos);

        if(!ok)
        {
            selectedCurve.isSomethingSelected = false;
            return;
        }

        dispPoint = true;
        recalculate = false;
    }
    else if(selectedCurve.funcType == TANGENT_RESIZE_HOVER)
    {       
         double dx = (mouseX - lastPosSouris.x)/uniteX;
         tangents->at(selectedCurve.id)->resizeTangent(dx, selectedCurve.tangentPtSelection);
    }
    else if(selectedCurve.funcType == TANGENT_MOVE_HOVER)
    {
         tangents->at(selectedCurve.id)->move(x);
    }
}

void MainGraph::mouseFuncHoverTest(double x, double y)
{
    double calcY = 0;   
    double k = 0;
    int draw;
    Range kRange;

    for(short i = 0; i < funcs.size(); i++)
    {
        if(!funcs[i]->isFuncValid())
            continue;

        kRange = funcs[i]->getParametricRange();
        k = kRange.start;
        draw = 0;

        while(k <= kRange.end)
        {
            calcY = funcs[i]->getFuncValue(x, k);

            if(!(isnan(calcY) || isinf(calcY)))
            {
                if(!(selectedCurve.isSomethingSelected && selectedCurve.funcType == FUNCTION && draw == selectedCurve.kPos && i == selectedCurve.id))
                {
                    if((fabs(calcY - y) * uniteY) < parametres.epaisseurDesCourbes + 1)
                    {
                        mouseState.hovering = true;
                        mouseState.tangentHovering = false;
                        mouseState.funcType = FUNC_HOVER;
                        mouseState.isParametric = funcs[i]->isFuncParametric();
                        mouseState.kPos = draw;
                        mouseState.id = i;
                        recalculate = false;

                        return;
                    }
                }
            }
            k += kRange.step;
            draw++;
        }
    }
}

void MainGraph::mouseSeqHoverTest(double x, double y)
{
    double calcY = 0;
    short drawsNum = 1;
    bool ok;
    double nMin = seqs[0]->get_nMin();

    if(x > nMin)
    {
        double intAbscissa = NAN;
        double start, step = 1;

        if(graphRange.Xmin > nMin)
            start = fipart(graphRange.Xmin);
        else start = nMin;

        if(uniteX < 1)
             step = 5*fipart(1/uniteX);

        if(fabs((fipart((x-start)/step) - (x-start)/step) * uniteX) < (double)(parametres.epaisseurDesCourbes) + 2)
            intAbscissa = fipart((x-start)/step) * step + start;

        if(isnan(intAbscissa))
            return;

        for(short i = 0; i < 6; i++)
        {
            if(!seqs[i]->isSeqValid())
                continue;

            drawsNum = seqs[i]->getDrawsNum();
            ok = true;

            for(short draw = 0 ; draw < drawsNum ; draw++)
            {
                calcY = seqs[i]->getSeqValue(intAbscissa, ok, draw);

                if(!ok)
                    break;

                if(!(selectedCurve.isSomethingSelected && selectedCurve.funcType == SEQUENCE && draw == selectedCurve.kPos && i == selectedCurve.id))
                {
                    if((fabs(calcY - y) * uniteY) < parametres.epaisseurDesCourbes + 3)
                    {
                        mouseState.hovering = true;
                        mouseState.tangentHovering = false;
                        mouseState.funcType = SEQ_HOVER;
                        mouseState.isParametric = seqs[i]->isSeqParametric();
                        mouseState.kPos = draw;
                        mouseState.id = i;
                        recalculate = false;
                        return;
                    }
                }
            }
        }
    }




}

void MainGraph::mouseTangentHoverTest(double x, double y)
{
    TangentWidget *tangent;
    TangentPoints tangentPoints;

    bool found = false;

    for(int i = 0 ; i < tangents->size() && !found; i++)
    {
        tangent = tangents->at(i);

        if(!tangent->isTangentValid())
            continue;

        tangentPoints = tangent->getCaracteristicPoints();

        if(fabs(tangentPoints.left.x - x) < 4/uniteX && fabs(tangentPoints.left.y - y) < 4/uniteY)
        {
            mouseState.hovering = true;
            mouseState.tangentHovering = true;
            mouseState.tangentPtSelection = -1; //-1 for left point, so when we add dx to the tangent's lenght, we multiply it by -1
            mouseState.funcType = TANGENT_RESIZE_HOVER;
            mouseState.id = i;
            found = true;
            //recalculate = false;
        }
        else if(fabs(tangentPoints.right.x - x) < 4/uniteX && fabs(tangentPoints.right.y - y) < 4/uniteY)
        {
            mouseState.hovering = true;
            mouseState.tangentHovering = true;
            mouseState.tangentPtSelection = 1;
            mouseState.funcType = TANGENT_RESIZE_HOVER;
            mouseState.id = i;
            found = true;
            //recalculate = false;
        }
        else if(fabs(tangentPoints.center.x - x) < 5/uniteX && fabs(tangentPoints.center.y - y) < 5/uniteY)
        {
            mouseState.hovering = true;
            mouseState.tangentHovering = true;
            mouseState.funcType = TANGENT_MOVE_HOVER;
            mouseState.id = i;
            found = true;
            //recalculate = false;
        }
    }

    if(found)
        update();
}

void MainGraph::placerGraduations()
{    

    pen.setColor(parametres.couleurDesAxes);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, false);

    double start, end, Ypos, posTxt;

    //trace sur l'axe des X
    if(centre.y < 20)
    {       
        Ypos = 20;
        posTxt = Ypos + parametres.numSize + 3;
    }
    else if(graphHeight - centre.y < 20)
    {       
        Ypos = graphHeight - 20;
        posTxt = Ypos - 7;
    }
    else
    {
        Ypos = centre.y;
        posTxt = Ypos + parametres.numSize + 3;
    }

    double Xreal = fipart(graphRange.Xmin / graphRange.Xscale) * graphRange.Xscale;
    double Xpos = Xreal * uniteX + centre.x;
    double pos;

    double pas = graphRange.Xscale * uniteX;

    double bas = height();
    double haut = 0;

    QString num = QString::number(Xreal, 'g', NUM_PREC);

    start = 5;
    end = graphWidth - 5;

    if(centre.x < 10)
        start = 10 + painter.fontMetrics().width(num)/2 + 5;
    else if(centre.x > graphWidth - 10)
        end = graphWidth - 10 - painter.fontMetrics().width(num)/2 - 5;

    while(Xpos <= end)
    {       
        if(start <= Xpos && fabs(Xpos - centre.x) > 1)
        {
            if(start <= Xpos && informations->getGridState() && Xpos <= end)
            {
                pen.setColor(parametres.couleurQuadrillage);
                pen.setWidthF(0.5);
                painter.setPen(pen);
                painter.drawLine(QPointF(Xpos, bas), QPointF(Xpos, haut));

                pen.setColor(parametres.couleurDesAxes);
                pen.setWidth(1);
                painter.setPen(pen);
            }

            painter.drawLine(QPointF(Xpos, Ypos -3), QPointF(Xpos, Ypos));
            num = QString::number(Xreal, 'g', NUM_PREC);
            pos = Xpos - painter.fontMetrics().width(num)/2;
            painter.drawText(QPointF(pos, posTxt), num);
        }

        Xpos += pas;
        Xreal += graphRange.Xscale;
    }

//trace sur l'axe des Y

    bool drawOnRight = false;

    if(centre.x < 10)
    {
        Xpos = 10;
        posTxt = Xpos + 4;
    }
    else if(graphWidth - centre.x < 10)
    {
        Xpos = graphWidth - 10;
        posTxt = Xpos - 8;
        drawOnRight = true;
    }
    else
    {
        Xpos = centre.x;
        posTxt = Xpos + 5;
    }

    double Yreal = fipart(graphRange.Ymax / graphRange.Yscale) * graphRange.Yscale;
    Ypos = -Yreal * uniteY + centre.y;
    pas = graphRange.Yscale * uniteY;

    bas =  0;
    haut =  graphWidth;

    start = 5;
    end = graphHeight - 5;


    if(graphHeight - centre.y < 10)
        end = graphHeight - 50;
    else if(centre.y < 10)
        start = 50;


    double txtCorr = + painter.fontMetrics().ascent()/2 - 2;

    while(Ypos <= end)
    {        
        if(start <= Ypos && fabs(Ypos - centre.y) > 1)
        {
            if(informations->getGridState())
            {
                pen.setColor(parametres.couleurQuadrillage);
                pen.setWidthF(0.5);
                painter.setPen(pen);
                painter.drawLine(QPointF(bas, Ypos), QPointF(haut, Ypos));

                pen.setColor(parametres.couleurDesAxes);
                pen.setWidth(1);
                painter.setPen(pen);
            }

            painter.drawLine(QPointF(Xpos  -3, Ypos), QPointF(Xpos, Ypos));
            num = QString::number(Yreal, 'g', NUM_PREC);
            if(drawOnRight)
                painter.drawText(QPointF(posTxt - painter.fontMetrics().width(num), Ypos + txtCorr), num);
            else painter.drawText(QPointF(posTxt, Ypos + txtCorr), num);
        }

        Yreal -= graphRange.Yscale;
        Ypos += pas;
    }
}

void MainGraph::drawAxes()
{
    // *********** remarque: les y sont positifs en dessous de l'axe x, pas au dessus !! ************//
    pen.setWidth(1);
    pen.setColor(parametres.couleurDesAxes);
    painter.setPen(pen);    
    painter.setRenderHint(QPainter::Antialiasing, false);

    axesIntersec.y = centre.y;
    axesIntersec.x = centre.x;

    if(graphRange.Ymin > -20/uniteY)
        axesIntersec.y = graphHeight - 20;
    else if(graphRange.Ymax < 20/uniteY)
        axesIntersec.y = 20;

    if(graphRange.Xmin > -10/uniteX)
        axesIntersec.x = 10;
    else if(graphRange.Xmax < 10/uniteX)
        axesIntersec.x = graphWidth - 10;


    //ordinates axis
    painter.drawLine(QPointF(axesIntersec.x, 0), QPointF(axesIntersec.x, graphHeight));

    //abscissa axis
    painter.drawLine(QPointF(0, axesIntersec.y), QPointF(graphWidth, axesIntersec.y));


    if(graphRange.Ymin > -20/uniteY)
    {
        painter.drawLine(QPointF(axesIntersec.x-3, axesIntersec.y-6), QPointF(axesIntersec.x+3, axesIntersec.y-4));
        painter.drawLine(QPointF(axesIntersec.x-3, axesIntersec.y-9), QPointF(axesIntersec.x+3, axesIntersec.y-7));

        pen.setColor(parametres.couleurDuFond);
        painter.setPen(pen);

        painter.drawLine(QPointF(axesIntersec.x, axesIntersec.y-6), QPointF(axesIntersec.x, axesIntersec.y-7));
    }
    else if(graphRange.Ymax < 20/uniteY)
    {
        painter.drawLine(QPointF(axesIntersec.x-3, axesIntersec.y+6), QPointF(axesIntersec.x+3, axesIntersec.y+4));
        painter.drawLine(QPointF(axesIntersec.x-3, axesIntersec.y+9), QPointF(axesIntersec.x+3, axesIntersec.y+7));

        pen.setColor(parametres.couleurDuFond);
        painter.setPen(pen);

        painter.drawLine(QPointF(axesIntersec.x, axesIntersec.y+6), QPointF(axesIntersec.x, axesIntersec.y+7));
    }
}

void MainGraph::zoomX()
{
    repaintTimer.stop();

    double valeur = (graphRange.Xmax- graphRange.Xmin) * (double)(hSlider->value()) * 0.0016;

    graphRange.Xmin -= valeur;
    graphRange.Xmax += valeur;
    moving = true;
    informations->setRange(graphRange);

    if(parametres.lissage)
        repaintTimer.start();
}

void MainGraph::stop_X_zoom()
{
    timerX.stop();
    hSlider->setValue(0);
}

void MainGraph::zoomY()
{
    repaintTimer.stop();

    double valeur = (graphRange.Ymax - graphRange.Ymin) * (double)(vSlider->value()) * 0.0016;

    if(!informations->isOrthonormal())
    {        
        graphRange.Ymin -= valeur;
        graphRange.Ymax += valeur;
        recalculate = false;
    }
    else
    {
        graphRange.Xmin -= valeur;
        graphRange.Xmax += valeur;
        recalculate = true;
    }

    moving = true;
    if(parametres.lissage)
        repaintTimer.start();

    informations->setRange(graphRange);
}

void MainGraph::stop_Y_Zoom()
{
    timerY.stop();
    vSlider->setValue(0);
}

MainGraph::~MainGraph()
{
    delete savedGraph;
    delete exprCalculator;
}