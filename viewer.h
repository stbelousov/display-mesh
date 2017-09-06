#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QVector3D>
#include <QMatrix4x4>
#include "facet.h"

namespace Ui {
    class Viewer;
}

class Viewer : public QWidget
{
    Q_OBJECT

    static const double eps = 1e-8;
    static const double inf = 1e18;

public:
    bool mark_regions, display_normals;
    
    QMatrix4x4 orientation, ortho_matrix;
    double cur_x, cur_y;
    QVector3D observe_point;
    
    QVector < Facet > mesh;
    double min_temperature, max_temperature;
    QVector < QColor > colors;
    QVector < QString > files, regions;
    QVector < int > sizes;
    
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();
    
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    
    void setMarkRegions(bool value);
    void setDisplayNormals(bool value);
    
private:
    Ui::Viewer *ui;
    
    QMatrix4x4 createOrthoMatrix();
    void createMesh();
    
    void drawAxis(const QMatrix4x4 &matr, QPainter &p);
    void drawLegendRegions(QPainter &p);
    void drawLegendTemperature(QPainter &p);
    void drawObject(const QVector < Facet > &facets, QPainter &p);
    void drawLine(const QVector4D &A, const QVector4D &B, QPainter &p);
    
};

#endif // VIEWER_H
