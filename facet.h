#ifndef FACET_H
#define FACET_H

#include <QColor>
#include <QVector>
#include <QVector4D>
#include <QMatrix4x4>
#include <QPainter>

class Facet
{
private:
    bool is_normal;
    QVector4D a, b, c, normal;
    QColor color;
    double temperature;
public:
    Facet();
    Facet(const QVector4D &a, const QVector4D &b, const QVector4D &c, const QVector4D &normal, const QColor &color, const double &temperature);
    Facet(const QVector4D &a, const QVector4D &b);
    void transform(const QMatrix4x4 &matr);
    QColor getColor() const;
    double getTemperature() const;
    QVector4D getCentroid() const;
    Facet getNormal() const;
    bool isNormal() const;
    void draw(QPainter &p) const;
    void scale(double value);

    bool operator<(const Facet &t) const;
};

#endif // FACET_H
