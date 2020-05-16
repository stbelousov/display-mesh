#include "facet.h"
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <cmath>

QPointF getPoint(const QVector4D &a) {
    return QPointF(a.x() / a.w(), a.y() / a.w());
}

Facet::Facet():
    is_normal(false), temperature(0.0)
{}

Facet::Facet(const QVector4D &a, const QVector4D &b, const QVector4D &c, const QVector4D &normal, const QColor &color, const double &temperature):
    is_normal(false), a(a), b(b), c(c), normal(normal), color(color), temperature(temperature)
{}

Facet::Facet(const QVector4D &a, const QVector4D &b):
    is_normal(true), a(a), b(b), c((a + b) / 2.0), temperature(0.0)
{}

void Facet::transform(const QMatrix4x4 &matr)
{
    a = matr * a;
    b = matr * b;
    c = matr * c;
    normal = matr * normal;
}

QColor Facet::getColor() const
{
    return color;
}

double Facet::getTemperature() const
{
    return temperature;
}

QVector4D Facet::getCentroid() const
{
    QVector3D result(0, 0, 0);
    result += QVector3D(a / a.w());
    result += QVector3D(b / b.w());
    result += QVector3D(c / c.w());
    result /= 3.0;
    return QVector4D(result, 1);
}

Facet Facet::getNormal() const
{
    double length = 0.03;
    QVector4D from = this->getCentroid() + normal.normalized() * 0.001;
    QVector4D to = from + normal.normalized() * length;
    return Facet(from, to);
}

bool Facet::isNormal() const
{
    return is_normal;
}

void Facet::draw(QPainter &p) const
{
    QPointF arr[3];
    arr[0] = getPoint(a);
    arr[1] = getPoint(b);
    arr[2] = getPoint(c);
    p.drawConvexPolygon(arr, 3);
}

void Facet::scale(double value)
{
    QMatrix4x4 matr = QMatrix4x4(value, 0, 0, 0,
                                 0, value, 0, 0,
                                 0, 0, value, 0,
                                 0, 0, 0, 1);

    transform(matr);
}

bool Facet::operator <(const Facet &t) const {
    double d1 = getCentroid().z();
    double d2 = t.getCentroid().z();
    return d1 > d2;
}
