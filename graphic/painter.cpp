/*
  计算机图形学算法实现：使用C++实现非界面的业务逻辑和复杂运算
  包括直线，椭圆，区域填充
  贝塞尔曲线，B-样条
  真实感图形生成：球体
*/


#include "painter.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <math.h>
#include <QTime>

Painter::Painter(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_element(0)
    , m_bEnabled(true)
    , m_bPressed(false)
    , m_bMoved(false)
    , m_pen(Qt::black)
    , m_pfunc(0), m_era(40), m_erb(20), m_eangle(0)
    , m_beSize(0), m_bsSize(0)
    , m_kochSize(-1)
{
    m_lThetax = -1;
    m_lThetay = -1;
    m_lThetaz = 0;
    m_vThetax = 0;
    m_vThetay = 0;
    m_vThetaz = 1;
    m_widget = NULL;
    setAcceptedMouseButtons(Qt::LeftButton);
}

Painter::~Painter()
{
    purgePaintElements();
}

void Painter::clear()
{
    purgePaintElements();
    m_beSize = 0;
    m_bsSize = 0;
    m_kochSize = -1;
    update();
}

void Painter::undo()
{
    if (m_elements.size())
    {
        delete m_elements.takeLast();
        update();
    }
}

// Bresenham画直线算法：
void drawLine(QPainter *painter, QLineF line)
{
    int x1, y1, x2, y2;
    x1 = line.x1(); y1 = line.y1();
    x2 = line.x2(); y2 = line.y2();
    int x = x1, y = y1;
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    painter->drawPoint(x, y);
    painter->drawPoint(x2, y2);
    if (dx == 0 && y < y2)
    {
        while (y < y2)
            painter->drawPoint(x, y++);
        return;
    }
    if (dx == 0 && y > y2)
    {
        while (y > y2)
            painter->drawPoint(x, y--);
        return;
    }
    if (dy == 0 && x < x2)
    {
        while (x < x2)
            painter->drawPoint(x++, y);
        return;
    }
    if (dy == 0 && x > x2)
    {
        while (x > x2)
            painter->drawPoint(x--, y);
        return;
    }
    if (dx == dy && y < y2)
    {
        while (x < x2)
            painter->drawPoint(x++, y++);
        while (x > x2)
            painter->drawPoint(x--, y++);
        return;
    }
    if (dx == dy && y > y2)
    {
        while (x < x2)
            painter->drawPoint(x++, y--);
        while (x > x2)
            painter->drawPoint(x--, y--);
        return;
    }

    if (dy < dx)
    {
        int p = 2 * dy - dx;
        int dy2 = 2 * dy, dymdx2 = 2 * (dy - dx);
        if (x < x2)
        while (x < x2)
        {
            x++;
            if (p < 0)
            {
                p += dy2;
            }
            else
            {
                if ((y2 - y1) < 0)
                    y--;
                else
                    y++;
                p += dymdx2;
            }
            painter->drawPoint(x, y);
        }
        else
        while (x > x2)
        {
            x--;
            if (p < 0)
            {
                p += dy2;
            }
            else
            {
                if ((y2 - y1) < 0)
                    y--;
                else
                    y++;
                p += dymdx2;
            }
            painter->drawPoint(x, y);
        }
        return;
    }
    else
    {
        int p = 2 * dx - dy;
        int dy2 = 2 * dx, dymdx2 = 2 * (dx - dy);
        if (y < y2)
            while (y < y2)
            {
                y++;
                if (p < 0)
                {
                    p += dy2;
                }
                else
                {
                    if ((x2 - x1) < 0)
                        x--;
                    else
                        x++;
                    p += dymdx2;
                }
                painter->drawPoint(x, y);
        }
        else
        {
            while (y > y2)
            {
                y--;
                if (p < 0)
                {
                    p += dy2;
                }
                else
                {
                    if ((x2 - x1) < 0)
                        x--;
                    else
                        x++;
                    p += dymdx2;
                }
                painter->drawPoint(x, y);
            }
        }
    }
}


QPointF rotate(int x, int y, float cosa, float sina)
{
    int xt = x;
    QPointF p;
    p.setX(x * cosa + y * sina);
    p.setY(y * cosa - xt * sina);
    return p;
}

// 中点画椭圆算法
void drawEllipse(QPainter *painter, QLineF line, int rx, int ry, int ang)
{
    float cosa = cos(ang * PI / 180.0);
    float sina = sin(ang * PI / 180.0);
    int xc = line.x2(), yc = line.y2();
    sina = (abs(sina) < 1e-3) ? 0 : sina;
    cosa = (abs(cosa) < 1e-3) ? 0 : cosa;
    int x = 0, y = ry;
    QPointF p = rotate(x, y, cosa, sina);
    int xx = p.x(), yy = p.y();
    painter->drawPoint(xc + xx, yc + yy);
    p = rotate(x, -y, cosa, sina);
    xx = p.x(), yy = p.y();
    painter->drawPoint(xc + xx, yc + yy);
    p = rotate(-x, y, cosa, sina);
    xx = p.x(), yy = p.y();
    painter->drawPoint(xc + xx, yc + yy);
    p = rotate(-x, -y, cosa, sina);
    xx = p.x(), yy = p.y();
    painter->drawPoint(xc + xx, yc + yy);

    int rx2 = rx * rx, ry2 = ry * ry;
    int tworx2 = 2 * rx2, twory2 = 2 * ry2;
    int p1 = ry * ry - rx * rx * ry - rx * rx / 4;
    int px = 0, py = tworx2 * y;

    while (px < py)
    {
        x++;
        px += twory2;
        if (p1 < 0)
        {
            p1 += px + ry2;
        }
        else
        {
            y--;
            py -= tworx2;
            p1 += px - py + ry2;
        }

        p = rotate(x, y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
        p = rotate(x, -y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
        p = rotate(-x, y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
        p = rotate(-x, -y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
    }

    int p2 = round(ry2 * (x + 1/2) * (x + 1/2) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while (y > 0)
    {
        y--;
        py -= tworx2;
        if (p2 > 0)
        {
            p2 += rx2 - py;
        }
        else
        {
            x++;
            px += twory2;
            p2 += px - py + rx2;
        }

        p = rotate(x, y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
        p = rotate(x, -y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
        p = rotate(-x, y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
        p = rotate(-x, -y, cosa, sina);
        xx = p.x(), yy = p.y();
        painter->drawPoint(xc + xx, yc + yy);
    }
}

// 椭圆填充

void fill_ellipse(QPainter *painter, int x1, int y1, int xc, int yc, float ra, float rb, float ang, bool** mask)
{
    float cosa = cos(ang * PI / 180.0);
    float sina = sin(ang * PI / 180.0);
    sina = (abs(sina) < 1e-3) ? 0 : sina;
    cosa = (abs(cosa) < 1e-3) ? 0 : cosa;

    QStack<QPointF> s;
    s.append(QPointF(x1, y1));
    QPointF p, ptmp;
    int x0, y0, xl, x, y, xtmp, ytmp;
    while(!s.empty())
    {
        p = s.last(); s.pop_back();
        qDebug() << p.x() << p.y();
        x = p.x(); y = p.y();
        painter->drawPoint(p);
        mask[y][x] = 1;
        x0 = x + 1;
        y0 = y;
        ptmp = rotate(x0 - xc, y0 - yc, cosa, -sina);
        xtmp = ptmp.x(); ytmp = ptmp.y();
        qDebug() << x0 << y0 << xtmp << ytmp << ra << rb;
        while (x0 < 1040 && !mask[y0][x0] && (xtmp * xtmp / (ra * ra) + ytmp * ytmp / (rb * rb)) < 1)
        {
            painter->drawPoint(QPointF(x0, y0));
            mask[y0][x0] = 1;
            x0++;
            ptmp = rotate(x0 - xc, y0 - yc, cosa, -sina);
            xtmp = ptmp.x(); ytmp = ptmp.y();
        }

        x0 = x - 1;
        ptmp = rotate(x0 - xc, y0 - yc, cosa, -sina);
        xtmp = ptmp.x(); ytmp = ptmp.y();
        while (x0 >= 0 && !mask[y0][x0] && (xtmp * xtmp / (ra * ra) + ytmp * ytmp / (rb * rb)) < 1)
        {
            painter->drawPoint(QPointF(x0, y0));
            mask[y0][x0] = 1;
            x0--;
            ptmp = rotate(x0 - xc, y0 - yc, cosa, -sina);
            xtmp = ptmp.x(); ytmp = ptmp.y();
        }
        xl = x0 + 1;

        x = xl;
        ptmp = rotate(x - xc, y0 + 1 - yc, cosa, -sina);
        xtmp = ptmp.x(); ytmp = ptmp.y();

        {
            while (x < 1040 && ((xtmp * xtmp / (ra * ra) + ytmp * ytmp / (rb * rb)) >= 1 || mask[y0 + 1][x]))
            {
                x++;
                ptmp = rotate(x - xc, y0 + 1 - yc, cosa, -sina);
                xtmp = ptmp.x(); ytmp = ptmp.y();
            }
            if (y0 + 1 <= 665 && !mask[y0 + 1][x] && (xtmp * xtmp / (ra * ra) + ytmp * ytmp / (rb * rb)) < 1)
                s.push_back(QPointF(x, y0 + 1));
        }
        x = xl;
        ptmp = rotate(x - xc, y0 - 1 - yc, cosa, -sina);
        xtmp = ptmp.x(); ytmp = ptmp.y();

       {
            while (x < 1040 && ((xtmp * xtmp / (ra * ra) + ytmp * ytmp / (rb * rb)) >= 1 || mask[y0 - 1][x] ))
            {
                x++;
                ptmp = rotate(x - xc, y0 - 1 - yc, cosa, -sina);
                xtmp = ptmp.x(); ytmp = ptmp.y();
            }
            if (y0 - 1 >= 0 && !mask[y0 - 1][x] && (xtmp * xtmp / (ra * ra) + ytmp * ytmp / (rb * rb)) < 1)
                s.push_back(QPointF(x, y0 - 1));
        }
    }
}

bool in4region(int x1, int y1, int x2, int y2)
{
    if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1)
        return true;
    else
        return false;
}
// bezier曲线生成
void bezier(QPainter *painter, QVector<QPointF> *points)
{
    int size = points->size();
    painter->drawPoint(points->at(0));
    painter->drawPoint(points->at(size - 1));
    bool flag = true;
    for (int i = 0; i < size - 1; i++)
    {
        int x1 = points->at(i).x(), y1 = points->at(i).y();
        int x2 = points->at(i + 1).x(), y2 = points->at(i + 1).y();
        if (!in4region(x1, y1, x2, y2))
        {
            flag = false;
            break;
        }
    }
    if (flag)
    {
        for (int i = 1; i < size - 1; i++)
            painter->drawPoint(points->at(i));
        return;
    }

    QVector<QPointF> tmp1;
    QVector<QPointF> tmp2;
    QVector<QPointF> points1;
    QVector<QPointF> points2;
    points1.append(points->at(0));
    points2.append(points->at(size - 1));

    for (int i = 0; i < size; i++)
    {
        tmp2.append(points->at(i));
        if ((i + 1) % 3 == 0 && (i + 1) < size)
            tmp2.append((points->at(i) + points->at(i + 1)) / 2);
    }
    size = tmp2.size();
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < tmp2.size() - 1; j++)
        {
            QPointF tmpP0 = tmp2.at(j);
            QPointF tmpP1 = tmp2.at(j + 1);
            tmp1.append((tmpP0 + tmpP1) / 2);
        }
        tmp2.clear();
        for (int j = 0; j < tmp1.size(); j++)
        {
            tmp2.append(tmp1.at(j));
        }
        tmp1.clear();
        points1.append(tmp2.at(0));
        points2.append(tmp2.at(tmp2.size() - 1));

    }
    bezier(painter, &points1);
    bezier(painter, &points2);
}

//B-样条
void bspline(QPainter *painter, QVector<QPointF> *points)
{
    int n = points->size() - 1;
    int d = 4;
    int* t = new int[n + d + 2];
    for (int i = 0; i < n + d + 2; i++)
        t[i] = i;

    int xt, yt;
    float step = 0.1 / sqrt((points->at(n).x() - points->at(0).x()) * (points->at(n).x() - points->at(0).x()) +
                   (points->at(n).y() - points->at(0).y()) * (points->at(n).y() - points->at(0).y()));
    for (int k = 0; k < n - 2; k += 1)
    {
        xt = 0; yt = 0;
        for (float i = 0; i <= 1; i += step)
        {
            int x1 = points->at(k).x(), y1 = points->at(k).y();
            int x2 = points->at(k + 1).x(), y2 = points->at(k + 1).y();
            int x3 = points->at(k + 2).x(), y3 = points->at(k + 2).y();
            int x4 = points->at(k + 3).x(), y4 = points->at(k + 3).y();
            xt = ((-x1 + 3 * x2 - 3 * x3 + x4) * i * i * i +
                    (x1 * 3 - 6 * x2 + 3 * x3) * i * i +
                    (-x1 * 3 + 3 * x3) * i +
                    (x1 + 4 * x2 + x3)) / 6;
            yt = ((-y1 + 3 * y2 - 3 * y3 + y4) * i * i * i +
                    (y1 * 3 - 6 * y2 + 3 * y3) * i * i +
                    (-y1 * 3 + 3 * y3) * i +
                    (y1 + 4 * y2 + y3)) / 6;
            painter->drawPoint(xt, yt);
        }
    }

    delete[] t;
    t = NULL;
}
//koch曲线
void Koch(QPainter *painter, QLineF line, int n)
{
    int x1 = line.x1(), y1 = line.y1();
    int x2 = line.x2(), y2 = line.y2();
    if (n == 0)
    {
        drawLine(painter, line);
        return;
    }
    else
    {
        int dx = (x2 - x1) / 3, dy = (y2 - y1) / 3;
        int x11 = x1 + dx, y11 = y1 + dy;
        int x12 = x2 - dx, y12 = y2 - dy;
        int x13 = x11 + (dx - sqrt(3) * dy) / 2;
        int y13 = y11 + (dx * sqrt(3) + dy) / 3;

        QLineF tmp;
        QPoint tmpp1, tmpp2;
        tmpp1.setX(x11); tmpp1.setY(y11);
        tmp.setP1(line.p1()); tmp.setP2(tmpp1);
        Koch(painter, tmp, n - 1);
        tmpp2.setX(x13); tmpp2.setY(y13);
        tmp.setP1(tmpp1); tmp.setP2(tmpp2);
        Koch(painter, tmp, n - 1);
        tmpp1.setX(x12); tmpp1.setY(y12);
        tmp.setP1(tmpp2); tmp.setP2(tmpp1);
        Koch(painter, tmp, n - 1);
        tmp.setP1(tmpp1); tmp.setP2(line.p2());
        Koch(painter, tmp, n - 1);
    }
}

//蕨类植物
void ferns(QPainter *painter, QLineF line)
{
    int x1 = line.x1(), y1 = line.y1();

    painter->setPen("green");
    float x0 = 0, y0 = 0;
    for (int t = 0; t < 15550; t++)
    {
        float prob = rand() / (RAND_MAX - 0.1);

        float x = x0, y = y0;
        if (prob <= 0.01)
        {
            x0 = 0;
            y0 = 0.16 * y;
        }
        else if (prob <= 0.08)
        {
            x0 = (-0.15 * x + 0.28 * y);
            y0 = (0.26 * x + 0.24 * y + 0.44);
        }
        else if (prob <= 0.15)
        {
            x0 =  (0.2 * x - 0.26 * y);
            y0 =  (0.23 * x + 0.22 * y + 1.6);
        }
        else
        {
            x0 = (0.85 * x + 0.04 * y);
            y0 = (-0.04 * x + 0.85 * y + 1.6);
        }
        painter->drawPoint(x1 + x0 * 30, y1 + y0 * 30);
    }
}


double getVectorAngle(double xn1, double yn1, double zn1, double xn2,
                      double yn2, double zn2)
{
    return (xn1 * xn2 + yn1 * yn2 + zn1 * zn2) / (
                sqrt(xn1 * xn1 + yn1 * yn1 + zn1 * zn1) *
                sqrt(xn2 * xn2 + yn2 * yn2 + zn2 * zn2));
}

void drawLine(QPainter* painter, bool** mask, QLineF line)
{
    int x1, y1, x2, y2;
    x1 = line.x1(); y1 = line.y1();
    x2 = line.x2(); y2 = line.y2();
    int x = x1, y = y1;
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    mask[y][x] = 1;
    mask[y2][x2] = 1;
    if (dx == 0 && y < y2)
    {
        while (y < y2)
        {
            mask[y][x] = 1;
            y++;
        }
        return;
    }
    if (dx == 0 && y > y2)
    {
        while (y > y2)
        {
            mask[y][x] = 1;
            y--;
        }
        return;
    }
    if (dy == 0 && x < x2)
    {
        while (x < x2)
        {
            mask[y][x] = 1;
            x++;
        }
        return;
    }
    if (dy == 0 && x > x2)
    {
        while (x > x2)
        {
            mask[y][x] = 1;
            x--;
        }
        return;
    }
    if (dx == dy && y < y2)
    {
        while (x < x2)
        {
            mask[y][x] = 1;
            x++; y++;
        }
        while (x > x2)
        {
            mask[y][x] = 1;
            x--; y++;
        }
        return;
    }
    if (dx == dy && y > y2)
    {
        while (x < x2)
        {
            mask[y][x] = 1;
            x++; y--;
        }
        while (x > x2)
        {
            mask[y][x] = 1;
            x--; y--;
        }
        return;
    }

    if (dy < dx)
    {
        int p = 2 * dy - dx;
        int dy2 = 2 * dy, dymdx2 = 2 * (dy - dx);
        if (x < x2)
        while (x < x2)
        {
            x++;
            if (p < 0)
            {
                p += dy2;
            }
            else
            {
                if ((y2 - y1) < 0)
                    y--;
                else
                    y++;
                p += dymdx2;
            }
            mask[y][x] = 1;

        }
        else
        while (x > x2)
        {
            x--;
            if (p < 0)
            {
                p += dy2;
            }
            else
            {
                if ((y2 - y1) < 0)
                    y--;
                else
                    y++;
                p += dymdx2;
            }
            mask[y][x] = 1;
        }
        return;
    }
    else
    {
        int p = 2 * dx - dy;
        int dy2 = 2 * dx, dymdx2 = 2 * (dx - dy);
        if (y < y2)
            while (y < y2)
            {
                y++;
                if (p < 0)
                {
                    p += dy2;
                }
                else
                {
                    if ((x2 - x1) < 0)
                        x--;
                    else
                        x++;
                    p += dymdx2;
                }
                mask[y][x] = 1;
        }
        else
        {
            while (y > y2)
            {
                y--;
                if (p < 0)
                {
                    p += dy2;
                }
                else
                {
                    if ((x2 - x1) < 0)
                        x--;
                    else
                        x++;
                    p += dymdx2;
                }
                mask[y][x] = 1;
            }
        }
    }
}
//三角形填充
void fill_triangle(QPainter *painter, QPointF p1, QPointF p2, QPointF p3, bool** mask,
                     int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        memset(mask[i], 0, sizeof(bool) * width);
    }

    double x1 = p1.x(), y1 = p1.y();
    double x2 = p2.x(), y2 = p2.y();
    double x3 = p3.x(), y3 = p3.y();

    drawLine(painter, mask, QLineF(p1, p2));
    drawLine(painter, mask, QLineF(p2, p3));
    drawLine(painter, mask, QLineF(p3, p1));


    double xmax = mymax(mymax(x1, x2), x3), xmin = mymin(mymin(x1, x2), x3);
    double ymax = mymax(mymax(y1, y2), y3), ymin = mymin(mymin(y1, y2), y3);

    for (int i = ymin - 1; i <= ymax + 1; i++)
    {
        int xb = xmin, xe = xmax;
        int mn0 = 0;
        for (int j = xmin - 1; j <= xmax + 1; j++)
        {
            if (mask[i][j] && mn0 == 0)
            {
                mn0++;
                xb = j;
            }
            else if (mask[i][j])
            {
                mn0++;
                xe = j;
            }
        }
        if (mn0 >= 2)
        {
            for (int j = xb - 1; j <= xe + 1; j++)
                painter->drawPoint(j, i);
        }
    }

}



Point3D getN(double vx, double vy, double vz, double nx, double ny, double nz)
{
    double tmp = sqrt((vy * nz - vz * ny) * (vy * nz - vz * ny) +
                      (nx * vz - vx * nz) * (vz * nx - vx * nz) +
                      (vx * ny - vy * nx) * (vx * ny - nx * vy));
    Point3D p;
    p.x = (vy * nz - vz * ny) / tmp;
    p.y = (nx * vz - vx * nz) / tmp;
    p.z = (vx * ny - vy * nx) / tmp;
    return p;
}

QPointF projection_v2(Point3D p, double nx, double ny, double nz, int bx, int by)
{
    double ux = 0, uy = 1.0, uz = 0;
    if (ux == nx && ny == uy && nz == uz)
        ux++;
    double vx, vy, vz;
    Point3D up, vp;
    if (fabs(ux * nx + uy * ny + uz * nz) != 0)
    {
        up = getN(ux, uy, uz, nx, ny, nz);
        ux = up.x; uy = up.y; uz = up.z;
    }
    vp = getN(ux, uy, uz, nx, ny, nz);
    vx = vp.x; vy = vp.y; vz = vp.z;
    QPointF rp;
    rp.setX(ux * (p.x) + uy * (p.y) + uz * (p.z) + bx);
    rp.setY(vx * (p.x)+ vy * (p.y) + vz * (p.z) + by);
    return rp;
}


Point3DN yminp(Point3DN p1, Point3DN p2, Point3DN p3)
{
    double ymin = mymin(mymin(p1.py, p2.py), p3.py);
    if (p1.py == ymin)
        return p1;
    else if (p2.py == ymin)
        return p2;
    else
        return p3;
}
Point3DN ymaxp(Point3DN p1, Point3DN p2, Point3DN p3)
{
    double ymax = mymax(mymax(p1.py, p2.py), p3.py);
    if (p1.py == ymax)
        return p1;
    else if (p2.py == ymax)
        return p2;
    else
        return p3;
}

Point3DN ymidp(Point3DN p1, Point3DN p2, Point3DN p3)
{
    double ymin = mymin(mymin(p1.py, p2.py), p3.py);
    double ymax = mymax(mymax(p1.py, p2.py), p3.py);
    if (p1.py != ymin && p1.py != ymax)
        return p1;
    else if (p2.py != ymin && p2.py != ymax)
        return p2;
    else
        return p3;
}

//真实感图形球体生成
void sphere(QPainter *painter, QLineF line, QRgb rgb,
            float lx, float ly, float lz,
            float vx, float vy, float vz)
{
    QPaintDevice* qpd =  painter->device();
    int height = qpd->height(), width = qpd->width();
    QVector<TriSurfaceN> surfaceList;
    Point3DN p3d[101][101];
    int countx, county;
    double x1 = line.x1(), y1 = line.y1(), bz = 0;
    double bx = 0, by = 0;
    int x2 = line.x2(), y2 = line.y2();
    double radius = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    radius = (radius > height / 4) ? height /4 : radius;
    float Ia = 228, I0 = 228;
    double ka = 0.5, kd = 0.8, ks = 0.3;
    int n = 2;

    qDebug() << bx << by << radius;

    int i = 0, j;
    float Imax = 0, Imin = 999999;
    for (double u = -PI / 2; u < PI * 3 / 2; u += PI / 32)
    {
        j = 0;
        for (double v = 0; v < 2 * PI; v += PI / 32)
        {
            p3d[i][j].x = bx + radius * cos(u) * cos(v);
            p3d[i][j].y = by + radius * cos(u) * sin(v);
            p3d[i][j].z = bz + radius * sin(u);
            p3d[i][j].xn = p3d[i][j].x / radius;
            p3d[i][j].yn = p3d[i][j].y / radius;
            p3d[i][j].zn = p3d[i][j].z / radius;
            float cosfi = getVectorAngle(p3d[i][j].xn, p3d[i][j].yn, p3d[i][j].zn, -lx, -ly, -lz);
            float cosnh = getVectorAngle((-lx + vx)/2, (-ly + vy)/2, (-lz + vz)/2, p3d[i][j].xn,
                                         p3d[i][j].yn, p3d[i][j].zn);
            float I = Ia * ka + kd * I0 * cosfi + ks * I0 * pow(cosnh, n);
            if (I < Imin)   Imin = I;
            if (I > Imax)   Imax = I;
            j++;
        }
        i++;
    }
    countx = i;
    county = j;

    double ymax, ymin, ymid;
    double xmin, xmax;
    double nax, nay, naz, nbx, nby, nbz;
    double dnpx, dnpy, dnpz;
    double xa, xb;
    double xn, yn, zn;

    int nnn0 = 0;
    for (i = 0; i < countx; i++)
    {
        for (j = 0; j < county; j++)
        {
            TriSurfaceN surface1, surface2;
            surface1.p1 = p3d[i][j];
            surface1.p2 = p3d[(i + 1) % countx][(j + 1) % county];
            surface1.p3 = p3d[(i + 1) % countx][j];

            surface2.p1 = p3d[i][j];
            surface2.p2 = p3d[i][(j + 1) % county];
            surface2.p3 = p3d[(i + 1) % countx][(j + 1) % county];

            surfaceList.append(surface1);
            surfaceList.append(surface2);
        }
    }
    qDebug() << surfaceList.size() << nnn0 << Imin << Imax;

    Imin = (Imin < 1e-3) ? 0 : Imin;
    QPointF ptmp[3];
    bool** mask = new bool*[height];
    for (i = 0; i < height; i++)
        mask[i] = new bool[width];
    for (i = 0; i < surfaceList.size(); i++)
    {
        TriSurfaceN surface = surfaceList.at(i);
        Point3D p10, p20, p30;
        p10.x = surface.p1.x; p10.y = surface.p1.y; p10.z = surface.p1.z;
        p20.x = surface.p2.x; p20.y = surface.p2.y; p20.z = surface.p2.z;
        p30.x = surface.p3.x; p30.y = surface.p3.y; p30.z = surface.p3.z;
        ptmp[0] = projection_v2(p10, vx, vy, vz, x1, y1);
        ptmp[1] = projection_v2(p20, vx, vy, vz, x1, y1);
        ptmp[2] = projection_v2(p30, vx, vy, vz, x1, y1);
        Point3DN p100, p200, p300, p1, p2, p3;
        p100 = surface.p1; p200 = surface.p2; p300 = surface.p3;
        p100.px = ptmp[0].x(); p100.py = ptmp[0].y();
        p200.px = ptmp[1].x(); p200.py = ptmp[1].y();
        p300.px = ptmp[2].x(); p300.py = ptmp[2].y();
        p3 = yminp(p100, p200, p300);
        p2 = ymidp(p100, p200, p300);
        p1 = ymaxp(p100, p200, p300);

        ymax = (p1.py);
        ymid = (p2.py);
        ymin = (p3.py);

        for (j = 0; j < height; j++)
            memset(mask[j], 0, sizeof(bool) * width);
        drawLine(painter, mask, QLineF(ptmp[0], ptmp[1]));
        drawLine(painter, mask, QLineF(ptmp[1], ptmp[2]));
        drawLine(painter, mask, QLineF(ptmp[2], ptmp[0]));

        xmin = mymin(mymin(ptmp[0].x(), ptmp[1].x()), ptmp[2].x());
        xmax = mymax(mymax(ptmp[0].x(), ptmp[1].x()), ptmp[2].x());

        double p23 = sqrt((p2.px - p3.px) * (p2.px - p3.px) + (p2.py - p3.py) * (p2.py - p3.py));
        double p31 = sqrt((p3.px - p1.px) * (p3.px - p1.px) + (p3.py - p1.py) * (p3.py - p1.py));
        double p12 = sqrt((p2.px - p1.px) * (p2.px - p1.px) + (p2.py - p1.py) * (p2.py - p1.py));

        for (int y = ymin; y <= ymax; y++)
        {
            int mn0 = 0;
            xa = xmin; xb = xmax;
            for (int j = xmin; j <= xmax; j++)
            {
                if (mask[y][j] && mn0 == 0)
                {
                    xa = j;
                    mn0++;
                }
                else if (mask[y][j])
                {
                    xb = j;
                    mn0++;
                }
            }

            if (mn0 >= 2)
            {
                if (xa != xb)
                {
                    if (y < ymid)
                    {
                        double p2a, p3a, p3b, p1b;
                        p2a = sqrt((y - p2.py) * (y - p2.py) + (xa - p2.px) * (xa - p2.px));
                        p3a = sqrt((y - p3.py) * (y - p3.py) + (xa - p3.px) * (xa - p3.px));
                        p1b = sqrt((y - p1.py) * (y - p1.py) + (xb - p1.px) * (xb - p1.px));
                        p3b = sqrt((y - p3.py) * (y - p3.py) + (xb - p3.px) * (xb - p3.px));
                        if (p23 != 0)
                        {
                            nax = (p2a * p3.xn + p3a * p2.xn) / p23;
                            nay = (p2a * p3.yn + p3a * p2.yn) / p23;
                            naz = (p2a * p3.zn + p3a * p2.zn) / p23;
                        }
                        else
                        {
                            nax = p2.xn; nay = p2.yn; naz = p2.zn;
                        }
                        if (p31 != 0)
                        {
                            nbx = (p3b * p1.xn + p1b * p3.xn) / p31;
                            nby = (p3b * p1.yn + p1b * p3.yn) / p31;
                            nbz = (p3b * p1.zn + p1b * p3.zn) / p31;
                        }
                        else
                        {
                            nbx = p1.xn; nby = p1.yn; nbz = p1.zn;
                        }
                    }
                    else
                    {
                        double p2a, p1a, p3b, p1b;
                        p2a = sqrt((y - p2.py) * (y - p2.py) + (xa - p2.px) * (xa - p2.px));
                        p1a = sqrt((y - p1.py) * (y - p1.py) + (xa - p1.px) * (xa - p1.px));
                        p1b = sqrt((y - p1.py) * (y - p1.py) + (xb - p1.px) * (xb - p1.px));
                        p3b = sqrt((y - p3.py) * (y - p3.py) + (xb - p3.px) * (xb - p3.px));
                        if (p12 != 0)
                        {
                            nax = (p2a * p1.xn + p1a * p2.xn) / p12;
                            nay = (p2a * p1.yn + p1a * p2.yn) / p12;
                            naz = (p2a * p1.zn + p1a * p2.zn) / p12;
                        }
                        else
                        {
                            nax = p2.xn; nay = p2.yn; naz = p2.zn;
                        }
                        if (p31 != 0)
                        {
                            nbx = (p3b * p1.xn + p1b * p3.xn) / p31;
                            nby = (p3b * p1.yn + p1b * p3.yn) / p31;
                            nbz = (p3b * p1.zn + p1b * p3.zn) / p31;
                        }
                        else
                        {
                            nbx = p1.xn; nby = p1.yn; nbz = p1.zn;
                        }
                    }
                    double len = sqrt(nax * nax + nay * nay + naz * naz);
                    nax /= len; nay /= len; naz /= len;
                    len = sqrt(nbx * nbx + nby * nby + nbz * nbz);
                    nbx /= len; nby /= len; nbz /= len;
                    dnpx = (nbx - nax) / (xb - xa);
                    dnpy = (nby - nay) / (xb - xa);
                    dnpz = (nbz - naz) / (xb - xa);
                }
                else
                {
                    nax = p3.xn; nay = p3.yn; naz = p3.zn;
                    dnpx = dnpy = dnpz = 0;
                }
                xn = nax; yn = nay; zn = naz;
                for (double j = xa; j <= xb; j++)
                {
                    if (getVectorAngle(xn, yn, zn, vx, vy, vz) < 0)
                        continue;
                    float cosfi = getVectorAngle(xn, yn, zn, -lx, -ly, -lz);
                    float cosnh = getVectorAngle((-lx + vx)/2, (-ly + vy)/2, (-lz + vz)/2, xn, yn, zn);
                    float I = Ia * ka + kd * I0 * cosfi + ks * I0 * pow(cosnh, n);
                    I = (I - Imin) / (Imax - Imin);

                    double rtmp = (qRed(rgb)) * I;
                    rtmp = rtmp > 255 ? 255 : rtmp;
                    rtmp = rtmp < 0 ? 0 : rtmp;
                    double gtmp = (qGreen(rgb)) * I;
                    gtmp = gtmp > 255 ? 255 : gtmp;
                    gtmp = gtmp < 0 ? 0 : gtmp;
                    double btmp = (qBlue(rgb)) * I;
                    btmp = btmp > 255 ? 255 : btmp;
                    btmp = btmp < 0 ? 0 : btmp;
                    uchar r = rtmp;
                    uchar g = gtmp;
                    uchar b = btmp;
                    painter->setPen(QColor(r, g, b));
                    painter->drawPoint(j, y);
                    xn += dnpx;
                    yn += dnpy;
                    zn += dnpz;
                }
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        delete[] mask[i];
        mask[i] = NULL;
    }
    delete[] mask;
    mask = NULL;
}

//纹理映射
void sphere_texture(QPainter *painter, QLineF line, QRgb rgb,
            float lx, float ly, float lz,
            float vx, float vy, float vz)
{
    QPaintDevice* qpd =  painter->device();
    int height = qpd->height(), width = qpd->width();
    QVector<TriSurfaceN> surfaceList;
    Point3DN p3d[101][101];
    int countx, county;
    double x1 = line.x1(), y1 = line.y1(), bz = 0;
    double bx = 0, by = 0;
    int x2 = line.x2(), y2 = line.y2();
    double radius = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    radius = (radius > height / 4) ? height /4 : radius;
    float Ia = 228, I0 = 228;
    double ka = 0.5, kd = 0.8, ks = 0.3;
    int n = 2;

    qDebug() << bx << by << radius;
    int i = 0, j;
    float Imax = 0, Imin = 999999;
    for (double u = -PI / 2; u < PI * 3 / 2; u += PI / 32)
    {
        j = 0;
        for (double v = 0; v < 2 * PI; v += PI / 32)
        {
            p3d[i][j].x = bx + radius * cos(u) * cos(v);
            p3d[i][j].y = by + radius * cos(u) * sin(v);
            p3d[i][j].z = bz + radius * sin(u);
            p3d[i][j].xn = p3d[i][j].x / radius;
            p3d[i][j].yn = p3d[i][j].y / radius;
            p3d[i][j].zn = p3d[i][j].z / radius;
            float cosfi = getVectorAngle(p3d[i][j].xn, p3d[i][j].yn, p3d[i][j].zn, -lx, -ly, -lz);
            float cosnh = getVectorAngle((-lx + vx)/2, (-ly + vy)/2, (-lz + vz)/2, p3d[i][j].xn,
                                         p3d[i][j].yn, p3d[i][j].zn);
            float I = Ia * ka + kd * I0 * cosfi + ks * I0 * pow(cosnh, n);
            if (I < Imin)   Imin = I;
            if (I > Imax)   Imax = I;
            j++;
        }
        i++;
    }
    countx = i;
    county = j;

    double ymax, ymin, ymid;
    double xmin, xmax;
    double nax, nay, naz, nbx, nby, nbz;
    double dnpx, dnpy, dnpz;
    double xa, xb;
    double xn, yn, zn;

    int nnn0 = 0;
    for (i = 0; i < countx; i++)
    {
        for (j = 0; j < county; j++)
        {
            TriSurfaceN surface1, surface2;
            surface1.p1 = p3d[i][j];
            surface1.p2 = p3d[(i + 1) % countx][(j + 1) % county];
            surface1.p3 = p3d[(i + 1) % countx][j];

            surface2.p1 = p3d[i][j];
            surface2.p2 = p3d[i][(j + 1) % county];
            surface2.p3 = p3d[(i + 1) % countx][(j + 1) % county];

            surfaceList.append(surface1);
            surfaceList.append(surface2);
        }
    }
    qDebug() << surfaceList.size() << nnn0 << Imin << Imax;

    Imin = (Imin < 1e-3) ? 0 : Imin;
    QPointF ptmp[3];
    bool** mask = new bool*[height];
    for (i = 0; i < height; i++)
        mask[i] = new bool[width];
    for (i = 0; i < surfaceList.size(); i++)
    {
        TriSurfaceN surface = surfaceList.at(i);
        Point3D p10, p20, p30;
        p10.x = surface.p1.x; p10.y = surface.p1.y; p10.z = surface.p1.z;
        p20.x = surface.p2.x; p20.y = surface.p2.y; p20.z = surface.p2.z;
        p30.x = surface.p3.x; p30.y = surface.p3.y; p30.z = surface.p3.z;
        ptmp[0] = projection_v2(p10, vx, vy, vz, x1, y1);
        ptmp[1] = projection_v2(p20, vx, vy, vz, x1, y1);
        ptmp[2] = projection_v2(p30, vx, vy, vz, x1, y1);
        Point3DN p100, p200, p300, p1, p2, p3;
        p100 = surface.p1; p200 = surface.p2; p300 = surface.p3;
        p100.px = ptmp[0].x(); p100.py = ptmp[0].y();
        p200.px = ptmp[1].x(); p200.py = ptmp[1].y();
        p300.px = ptmp[2].x(); p300.py = ptmp[2].y();

        p3 = yminp(p100, p200, p300);
        p2 = ymidp(p100, p200, p300);
        p1 = ymaxp(p100, p200, p300);

        ymax = (p1.py + 1);
        ymid = (p2.py);
        ymin = (p3.py - 1);

        for (j = 0; j < height; j++)
            memset(mask[j], 0, sizeof(bool) * width);
        drawLine(painter, mask, QLineF(ptmp[0], ptmp[1]));
        drawLine(painter, mask, QLineF(ptmp[1], ptmp[2]));
        drawLine(painter, mask, QLineF(ptmp[2], ptmp[0]));

        xmin = mymin(mymin(ptmp[0].x(), ptmp[1].x()), ptmp[2].x());
        xmax = mymax(mymax(ptmp[0].x(), ptmp[1].x()), ptmp[2].x());

        double p23 = sqrt((p2.px - p3.px) * (p2.px - p3.px) + (p2.py - p3.py) * (p2.py - p3.py));
        double p31 = sqrt((p3.px - p1.px) * (p3.px - p1.px) + (p3.py - p1.py) * (p3.py - p1.py));
        double p12 = sqrt((p2.px - p1.px) * (p2.px - p1.px) + (p2.py - p1.py) * (p2.py - p1.py));

        for (int y = ymin; y <= ymax; y++)
        {
            int mn0 = 0;
            xa = xmin; xb = xmax;
            for (int j = xmin; j <= xmax; j++)
            {
                if (mask[y][j] && mn0 == 0)
                {
                    xa = j;
                    mn0++;
                }
                else if (mask[y][j])
                {
                    xb = j;
                    mn0++;
                }
            }

            if (mn0 >= 2)
            {
                if (xa != xb)
                {
                    if (y < ymid)
                    {
                        double p2a, p3a, p3b, p1b;
                        p2a = sqrt((y - p2.py) * (y - p2.py) + (xa - p2.px) * (xa - p2.px));
                        p3a = sqrt((y - p3.py) * (y - p3.py) + (xa - p3.px) * (xa - p3.px));
                        p1b = sqrt((y - p1.py) * (y - p1.py) + (xb - p1.px) * (xb - p1.px));
                        p3b = sqrt((y - p3.py) * (y - p3.py) + (xb - p3.px) * (xb - p3.px));
                        if (p23 != 0)
                        {
                            nax = (p2a * p3.xn + p3a * p2.xn) / p23;
                            nay = (p2a * p3.yn + p3a * p2.yn) / p23;
                            naz = (p2a * p3.zn + p3a * p2.zn) / p23;
                        }
                        else
                        {
                            nax = p2.xn; nay = p2.yn; naz = p2.zn;
                        }
                        if (p31 != 0)
                        {
                            nbx = (p3b * p1.xn + p1b * p3.xn) / p31;
                            nby = (p3b * p1.yn + p1b * p3.yn) / p31;
                            nbz = (p3b * p1.zn + p1b * p3.zn) / p31;
                        }
                        else
                        {
                            nbx = p1.xn; nby = p1.yn; nbz = p1.zn;
                        }
                    }
                    else
                    {
                        double p2a, p1a, p3b, p1b;
                        p2a = sqrt((y - p2.py) * (y - p2.py) + (xa - p2.px) * (xa - p2.px));
                        p1a = sqrt((y - p1.py) * (y - p1.py) + (xa - p1.px) * (xa - p1.px));
                        p1b = sqrt((y - p1.py) * (y - p1.py) + (xb - p1.px) * (xb - p1.px));
                        p3b = sqrt((y - p3.py) * (y - p3.py) + (xb - p3.px) * (xb - p3.px));
                        if (p12 != 0)
                        {
                            nax = (p2a * p1.xn + p1a * p2.xn) / p12;
                            nay = (p2a * p1.yn + p1a * p2.yn) / p12;
                            naz = (p2a * p1.zn + p1a * p2.zn) / p12;
                        }
                        else
                        {
                            nax = p2.xn; nay = p2.yn; naz = p2.zn;
                        }
                        if (p31 != 0)
                        {
                            nbx = (p3b * p1.xn + p1b * p3.xn) / p31;
                            nby = (p3b * p1.yn + p1b * p3.yn) / p31;
                            nbz = (p3b * p1.zn + p1b * p3.zn) / p31;
                        }
                        else
                        {
                            nbx = p1.xn; nby = p1.yn; nbz = p1.zn;
                        }
                    }
                    double len = sqrt(nax * nax + nay * nay + naz * naz);
                    nax /= len; nay /= len; naz /= len;
                    len = sqrt(nbx * nbx + nby * nby + nbz * nbz);
                    nbx /= len; nby /= len; nbz /= len;
                    dnpx = (nbx - nax) / (xb - xa);
                    dnpy = (nby - nay) / (xb - xa);
                    dnpz = (nbz - naz) / (xb - xa);
                }
                else
                {
                    nax = p3.xn; nay = p3.yn; naz = p3.zn;
                    dnpx = dnpy = dnpz = 0;
                }
                xn = nax; yn = nay; zn = naz;
                for (double j = xa - 1; j <= xb + 1; j++)
                {
                    if (getVectorAngle(xn, yn, zn, vx, vy, vz) < 0)
                        continue;

                    float cosfi = getVectorAngle(xn, yn, zn, -lx, -ly, -lz);
                    float cosnh = getVectorAngle((-lx + vx)/2, (-ly + vy)/2, (-lz + vz)/2, xn, yn, zn);
                    float I = Ia * ka + kd * I0 * cosfi + ks * I0 * pow(cosnh, n);
                    I = (I - Imin) / (Imax - Imin);

                    double rtmp, gtmp, btmp;
                    double u, v;

                    u = atan(xn / yn);
                    v = atan(sqrt(xn * xn + yn * yn) / zn);
                    int t = u * 8 + v * 8;
                    if (t % 2 == 1)
                    {
                        rtmp = 0; gtmp = 0; btmp = 0;
                    }
                    else
                    {
                        rtmp = qRed(rgb); gtmp = qGreen(rgb); btmp = qBlue(rgb);
                    }

                    rtmp = rtmp * I;
                    rtmp = rtmp > 255 ? 255 : rtmp;
                    rtmp = rtmp < 0 ? 0 : rtmp;
                    gtmp = gtmp * I;
                    gtmp = gtmp > 255 ? 255 : gtmp;
                    gtmp = gtmp < 0 ? 0 : gtmp;
                    btmp = btmp * I;
                    btmp = btmp > 255 ? 255 : btmp;
                    btmp = btmp < 0 ? 0 : btmp;
                    uchar r = rtmp;
                    uchar g = gtmp;
                    uchar b = btmp;
                    painter->setPen(QColor(r, g, b));
                    painter->drawPoint(j, y);
                    xn += dnpx;
                    yn += dnpy;
                    zn += dnpz;
                }
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        delete[] mask[i];
        mask[i] = NULL;
    }
    delete[] mask;
    mask = NULL;
}



void Painter::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    int size = m_elements.size();
    ElementGroup *element;
    QVector<QPointF> BezierP;
    QVector<QPointF> BsplineP;
    for (int i = 0; i < size; i++)
    {
        element = m_elements.at(i);
        int size1 = element->m_lines.size();
        painter->setPen(element->m_pen);
        switch (element->m_pfunc)
        {
        case 1:
        {
            drawLine(painter, element->m_lines.at(size1 - 1));
            break;
        }

        case 2:
        {
            drawEllipse(painter, element->m_lines.at(size1 - 1), element->m_era,
                        element->m_erb, element->m_eangle);
            break;
        }
        case 3:
        {
            QPaintDevice* qpd =  painter->device();
            int height = qpd->height(), width = qpd->width();
            bool** tmp = new bool*[height];
            for (int k = 0; k < height; k++)
            {
                tmp[k] = new bool[width];
                memset(tmp[k], 0, sizeof(bool) * width);
            }

           if (i > 0 && m_elements.at(i - 1)->m_pfunc == 2)
           {
               int x1 = element->m_lines.at(size1 - 1).x1();
               int y1 = element->m_lines.at(size1 - 1).y1();
               int size2 =  m_elements.at(i - 1)->m_lines.size();
               int xc = m_elements.at(i - 1)->m_lines.at(size2 - 1).x2();
               int yc = m_elements.at(i - 1)->m_lines.at(size2 - 1).y2();
               float ra = m_elements.at(i - 1)->m_era;
               float rb = m_elements.at(i - 1)->m_erb;
               float ang = m_elements.at(i - 1)->m_eangle;
               fill_ellipse(painter, x1, y1, xc, yc, ra, rb, ang, tmp);
           }
           else if (i > 0 && m_elements.at(i - 1)->m_pfunc == 1)
           {
               int size2 =  m_elements.at(i - 1)->m_lines.size();
               QPointF p3 = m_elements.at(i - 1)->m_lines.at(size2 - 1).p2();
               QPointF p2 = m_elements.at(i - 1)->m_lines.at(size2 - 1).p1();
               QPointF p1;
               for (int j = i - 2; j > 0 && m_elements.at(j)->m_pfunc == 1; j++)
               {
                   int sizej = m_elements.at(j)->m_lines.size();
                   p1 = m_elements.at(j)->m_lines.at(sizej - 1).p1();
                   fill_triangle(painter, p1, p2, p3, tmp, height, width);
               }
           }
            for (int k = 0; k < height; k++)
            {
                delete[] tmp[k];
                tmp[k] = NULL;
            }
            tmp = NULL;
            break;
        }
        case 4:
        {
            drawLine(painter, element->m_lines.at(size1 - 1));
            if (BezierP.size() == 0)
                BezierP.append(element->m_lines.at(size1 - 1).p1());
            if (BezierP.size()== element->m_beSize)
            {
                BezierP.append(element->m_lines.at(size1 - 1).p2());
                if ((i + 1) == size || BezierP.size() != m_elements.at(i + 1)->m_beSize)
                {
                    bezier(painter, &BezierP);
                    BezierP.clear();
                }
            }
            break;
        }
        case 5:
        {
            drawLine(painter, element->m_lines.at(size1 - 1));
            if (BsplineP.size() == 0)
                BsplineP.append(element->m_lines.at(size1 - 1).p1());
            if (BsplineP.size() == element->m_bsSize)
            {
                BsplineP.append(element->m_lines.at(size1 - 1).p2());
                if (((i + 1) == size || BsplineP.size() != m_elements.at(i + 1)->m_bsSize))
                {
                    bspline(painter, &BsplineP);
                    BsplineP.clear();
                }
            }
            break;
        }
        case 6:
        {
                Koch(painter, element->m_lines.at(size1 - 1), element->m_kochSize);
                QPointF tmp1 = element->m_lines.at(size1 - 1).p1();
                QPointF tmp2 = element->m_lines.at(size1 - 1).p2();
                QPointF tmp3;
                int dx = tmp2.x() - tmp1.x();
                int dy = tmp2.y() - tmp1.y();
                tmp3.setX(tmp1.x() + (dx + dy * sqrt(3)) / 2);
                tmp3.setY(tmp1.y() + (dy - sqrt(3) * dx) / 2);
                QLineF tmp;
                tmp.setP1(tmp2);    tmp.setP2(tmp3);
                Koch(painter, tmp, element->m_kochSize);
                tmp.setP1(tmp3);    tmp.setP2(tmp1);
                Koch(painter, tmp, element->m_kochSize);
            break;
        }

        case 7:
        {
            ferns(painter, element->m_lines.at(size1 - 1));
            break;
        }
        case 8:
        {
            float tmpl = sqrt(m_lThetax * m_lThetax + m_lThetay * m_lThetay + m_lThetaz * m_lThetaz);
            float tmpv = sqrt(m_vThetax * m_vThetax + m_vThetay * m_vThetay + m_vThetaz * m_vThetaz);
            sphere(painter, element->m_lines.at(size1 - 1), element->m_pen.color().rgb(),
                   m_lThetax / tmpl, m_lThetay / tmpl, m_lThetaz / tmpl,
                   m_vThetax / tmpv, m_vThetay / tmpv, m_vThetaz / tmpv);
            break;
        }
        case 9:
        {
            float tmpl = sqrt(m_lThetax * m_lThetax + m_lThetay * m_lThetay + m_lThetaz * m_lThetaz);
            float tmpv = sqrt(m_vThetax * m_vThetax + m_vThetay * m_vThetay + m_vThetaz * m_vThetaz);
            sphere_texture(painter, element->m_lines.at(size1 - 1), element->m_pen.color().rgb(),
                       m_lThetax / tmpl, m_lThetay / tmpl, m_lThetaz / tmpl,
                       m_vThetax / tmpv, m_vThetay / tmpv, m_vThetaz / tmpv);
            break;
         }
        default:
            qDebug() << "朋友，请按规范操作";
        }
    }
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    m_bMoved = false;
    if (!m_bEnabled || !(event->button() & acceptedMouseButtons()))
    {
        QQuickPaintedItem::mousePressEvent(event);
    }
    else
    {
        m_bPressed = true;

        if (m_pfunc == 4)
        {
            m_beSize++;
            qDebug() << m_beSize;
        }
        if (m_pfunc == 5)
        {
            m_bsSize++;
        }
        if (m_pfunc == 6)
        {
            m_kochSize++;
            if (m_kochSize > 0)
            {
                m_elements.at(m_elements.size() - 1)->m_kochSize++;
                return;
            }
        }

        if (m_pfunc != 2)
            m_element = new ElementGroup(m_pen, m_pfunc, m_beSize, m_bsSize, m_kochSize);
        else
            m_element = new ElementGroup(m_pen, m_pfunc, m_era, m_erb, m_eangle
                                         , m_beSize, m_bsSize, m_kochSize);

        m_elements.append(m_element);
        m_lastPoint = event->localPos();
        m_firstPoint = m_lastPoint;
        event->setAccepted(true);
    }
}

void Painter::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_bEnabled || !m_bPressed || !m_element)
    {
        QQuickPaintedItem::mouseMoveEvent(event);
    }
    else
    {
        m_element->m_lines.append(QLineF(m_firstPoint, event->localPos()));
        m_lastPoint = event->localPos();
        update();
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_element || !m_bEnabled || !(event->button() & acceptedMouseButtons()))
    {
        QQuickPaintedItem::mousePressEvent(event);
    }
    else
    {
        m_bPressed = false;
        m_bMoved = false;
        m_element->m_lines.append(QLineF(m_firstPoint, event->localPos()));
        update();
    }
}

void Painter::purgePaintElements()
{
    int size = m_elements.size();
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            delete m_elements.at(i);
        }
        m_elements.clear();
    }
    m_element = 0;
}
