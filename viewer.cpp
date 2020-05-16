#include "viewer.h"
#include "ui_viewer.h"
#include "facet.h"
#include <QPainter>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <cmath>
#include <cstdio>
#include <fstream>

Viewer::Viewer(QWidget *parent): QWidget(parent), ui(new Ui::Viewer) {
    ui->setupUi(this);
    
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::white);
    this->setPalette(pal);
    this->setAutoFillBackground(true);
    
    orientation.setToIdentity();
    observe_point = QVector3D(0, 6, 0);
    cur_x = cur_y = 0;
    ortho_matrix = createOrthoMatrix();
    
    colors.push_back(Qt::red);
    colors.push_back(Qt::green);
    colors.push_back(Qt::blue);
    colors.push_back(Qt::yellow);
    colors.push_back(Qt::cyan);
    colors.push_back(Qt::magenta);
    
    mark_regions = true;
    display_normals = false;
    
    createMesh();
}

Viewer::~Viewer() {
    delete ui;
}

void Viewer::drawLine(const QVector4D & A, const QVector4D & B, QPainter & p) {
	p.drawLine(A.x() / A.w(), A.y() / A.w(), B.x() / B.w(), B.y() / B.w());
}

void Viewer::createMesh() {
    QString filename = QFileDialog::getOpenFileName(0, QString::fromUtf8("Open File"), QDir::currentPath(), "Input (*.input)");
    std::ifstream in(filename.toStdString().c_str());
    if (!in) {
        QMessageBox::information(0, QString::fromUtf8("Error"), QString::fromUtf8("No file selected."));
        exit(1);
    }
    
    min_temperature = inf;
    max_temperature = -inf;
    double min_x = inf;
    double max_x = -inf;
    double min_y = inf;
    double max_y = -inf;
    double min_z = inf;
    double max_z = -inf;
    
    int n;
    in >> n;
    files.resize(n);
    regions.resize(n);
    sizes.resize(n);
    for (int i = 0;i < n;i++) {
        std::string file, region;
        std::getline(in, file, '"');
        std::getline(in, file, '"');
        std::getline(in, region, '"');
        std::getline(in, region, '"');
        files[i] = QString::fromUtf8(file.c_str());
        regions[i] = QString::fromUtf8(region.c_str());
        in >> sizes[i];
	}
    for (int i = 0;i < n;i++) {
        for (int j = 0;j < sizes[i];j++) {
            QVector4D a, b, c, normal;
            double temperature;
            double x, y, z;
            
            in >> x >> y >> z;
            a = QVector4D(x, y, z, 1);
            in >> x >> y >> z;
            b = QVector4D(x, y, z, 1);
            in >> x >> y >> z;
            c = QVector4D(x, y, z, 1);
            
            min_x = std::min(min_x, x);
            max_x = std::max(max_x, x);
            min_y = std::min(min_y, y);
            max_y = std::max(max_y, y);
            min_z = std::min(min_z, z);
            max_z = std::max(max_z, z);
            
            in >> x >> y >> z;
            normal = QVector4D(x, y, z, 0);
            
            in >> temperature;
            mesh.push_back(Facet(a, b, c, normal, colors[i % colors.size()], temperature));
            min_temperature = std::min(min_temperature, temperature);
            max_temperature = std::max(max_temperature, temperature);
        }
    }
    
    double dx = max_x - min_x;
    double dy = max_y - min_y;
    double dz = max_z - min_z;
    
    double scale = 3.0 / std::max(dx, std::max(dy, dz));
    for (int i = 0;i < mesh.size();i++) {
        mesh[i].scale(scale);
    }
}

void Viewer::drawAxis(const QMatrix4x4 &matr, QPainter &p) {
    double length = 2.5;
    QVector4D zero(0, 0, 0, 1);
    QVector4D x1(length, 0, 0, 1);
    QVector4D y1(0, length, 0, 1);
    QVector4D z1(0, 0, length, 1);
    
    zero = matr * zero;
    x1 = matr * x1;
    y1 = matr * y1;
    z1 = matr * z1;
    
    p.setPen(QPen(Qt::black));
    drawLine(zero, x1, p);
    drawLine(zero, y1, p);
    drawLine(zero, z1, p);
    
    QFont f;
    f.setPointSize(12);
    p.setFont(f);
    p.drawText(x1.x() / x1.w(), x1.y() / x1.w(), "x");
    p.drawText(y1.x() / y1.w() - 4, y1.y() / y1.w(), "y");
    p.drawText(z1.x() / z1.w(), z1.y() / z1.w(), "z");
}

void Viewer::drawLegendRegions(QPainter &p) {
    int startx = 10;
    int starty = this->height() - 20;
    for (int i = 0;i < files.size();i++) {
        p.setPen(QPen(colors[i % colors.size()]));
        p.setBrush(QBrush(colors[i % colors.size()]));
        QPointF arr[4];
        arr[0] = QPointF(startx, starty);
        arr[1] = QPointF(startx, starty - 10);
        arr[2] = QPointF(startx + 10, starty - 10);
        arr[3] = QPointF(startx + 10, starty);
        p.drawConvexPolygon(arr, 4);
        p.setPen(QPen(Qt::black));      
        p.drawText(startx + 14, starty, files[i] + " " + regions[i]);
        starty -= 20;
    }
}

void Viewer::drawLegendTemperature(QPainter &p) {
    int width = 10, length = 256;
    int startx = this->width() / 2 - length / 2;
    int starty = this->height() - 30 - width;
    for (int i = 0;i < length;i++) {
        p.setPen(QPen(QColor(i, 50, 255 - i)));
        p.drawLine(startx + i, starty, startx + i, starty + width);
    }
    
    QFont f;
    f.setPointSize(10);
    p.setFont(f);
    p.setPen(QPen(Qt::black));
    QString str;
    p.drawText(startx, starty - 2, QString::fromUtf8("Temperature:"));
    p.drawText(startx - 5, starty + width + 11, str.sprintf("%6.2lf", min_temperature));
    p.drawText(startx + length - 40, starty + width + 11, str.sprintf("%6.2lf", max_temperature));
}

void Viewer::drawObject(const QVector < Facet > &facets, QPainter &p) {    
    for (int i = 0;i < facets.size();i++) {
        const Facet& f = facets[i];
        
        if (f.isNormal()) {
            p.setPen(QPen(Qt::black));
            f.draw(p);
            continue;
        }
        
        QColor color;
        if (mark_regions) {
            color = f.getColor();
        } else {
            double temperature = f.getTemperature();
            int red = static_cast<int>((temperature - min_temperature) / (max_temperature - min_temperature + eps) * 255.0 + eps);
            color = QColor(red, 50, 255 - red);
        }
        p.setPen(QPen(Qt::black));
        p.setBrush(QBrush(color));
        f.draw(p);
    }
}

QMatrix4x4 Viewer::createOrthoMatrix() {
    QVector3D newz = observe_point.normalized();
    QVector3D top = QVector3D(0, 0, 1);
    QVector3D newx = QVector3D::crossProduct(newz, top).normalized();
    QVector3D newy = QVector3D::crossProduct(newz, newx).normalized();
    
    QMatrix4x4 result = QMatrix4x4(newx.x(), newy.x(), newz.x(), 0,
                                   newx.y(), newy.y(), newz.y(), 0,
                                   newx.z(), newy.z(), newz.z(), 0,
                                   0, 0, 0, 1);
    
    return result.inverted();
}

void Viewer::paintEvent(QPaintEvent *) {
    QPainter p(this);
    
    double scale = 0.2 * std::min(this->width(), this->height());
    QMatrix4x4 axis_matrix, mesh_scene_matrix, mesh_viewport_matrix;
    
    axis_matrix = QMatrix4x4(scale, 0, 0, this->width() - 0.8 * scale,
                             0, scale, 0, this->height() - 0.8 * scale,
                             0, 0, 0, 0,
                             0, 0, 0, 1) * ortho_matrix * orientation;
    mesh_viewport_matrix = QMatrix4x4(scale, 0, 0, this->width() / 2,
                                      0, scale, 0, this->height() / 2,
                                      0, 0, 0, 0,
                                      0, 0, 0, 1);
    mesh_scene_matrix = ortho_matrix * orientation;
    
    QVector < Facet > to_draw = mesh;
    if (display_normals) {
        for (int i = 0;i < mesh.size();i++) {
            to_draw.push_back(mesh[i].getNormal());
        }
    }
    
    for (int i = 0;i < to_draw.size();i++) {
        to_draw[i].transform(mesh_scene_matrix);
    }
    qSort(to_draw.begin(), to_draw.end());
    for (int i = 0;i < to_draw.size();i++) {
        to_draw[i].transform(mesh_viewport_matrix);
    }
    
    drawObject(to_draw, p);
    
    drawAxis(mesh_viewport_matrix * mesh_scene_matrix, p);
    
    if (mark_regions) {
        drawLegendRegions(p);
    } else {
        drawLegendTemperature(p);
    }
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
    double dx = e->x() - cur_x;
    double dy = e->y() - cur_y;
    cur_x = e->x();
    cur_y = e->y();
    
    QMatrix4x4 matr_dy = QMatrix4x4(1, 0, 0, 0,
                                    0, cos(dy * 0.01), -sin(dy * 0.01), 0,
                                    0, sin(dy * 0.01), cos(dy * 0.01), 0,
                                    0, 0, 0, 1);
    QMatrix4x4 matr_dx = QMatrix4x4(cos(dx * 0.01), -sin(dx * 0.01), 0, 0,
                                    sin(dx * 0.01), cos(dx * 0.01), 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1);
    orientation = matr_dx * matr_dy * orientation;
    
    update();
}

void Viewer::mousePressEvent(QMouseEvent * e) {
    cur_x = e->x();
    cur_y = e->y();
}

void Viewer::setMarkRegions(bool value) {
    mark_regions = value;
    update();
}

void Viewer::setDisplayNormals(bool value) {
    display_normals = value;
    update();
}
