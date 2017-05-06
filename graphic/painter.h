#ifndef PAINTER_H
#define PAINTER_H
#include <QQuickPaintedItem>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPen>
#include <QStack>
#include <math.h>

#define PI 3.1415926

#define mymax(x, y) ((x) < (y) ? (y) : (x))
#define mymin(x, y) ((x) > (y) ? (y) : (x))

struct Complex
{
    double r;
    double c;
};

class ElementGroup
{
public:
    ElementGroup(){}
    ElementGroup(const QPen &pen, int num) : m_pen(pen), m_pfunc(num)
    {
    }
    ElementGroup(const QPen &pen, int num, int besize, int bssize, int kochsize)
        : m_pen(pen), m_pfunc(num), m_beSize(besize), m_bsSize(bssize), m_kochSize(kochsize)
    {}
    ElementGroup(const QPen &pen, int num, int ra, int rb, int angle,
                 int besize, int bssize, int kochsize)
        : m_pen(pen), m_pfunc(num), m_beSize(besize), m_bsSize(bssize), m_kochSize(kochsize)
    {
        m_era = ra;
        m_erb = rb;
        m_eangle = angle;
    }

    ElementGroup(const ElementGroup &e)
    {
        m_lines = e.m_lines;
        m_pen = e.m_pen;
        m_pfunc = e.m_pfunc;
        m_beSize = e.m_beSize;
        m_bsSize = e.m_bsSize;
        m_kochSize = e.m_kochSize;
    }

    ElementGroup & operator=(const ElementGroup &e)
    {
        if (this != &e)
        {
            m_lines = e.m_lines;
            m_pen = e.m_pen;
            m_pfunc = e.m_pfunc;
            m_bsSize = e.m_bsSize;
            m_beSize = e.m_beSize;
            m_kochSize = e.m_kochSize;
        }
        return *this;
    }

    ~ElementGroup(){}

    QVector<QLineF> m_lines;
    QPen m_pen;
    int m_pfunc;
    int m_era; // 椭圆参数
    int m_erb;
    int m_eangle;
    int m_beSize;
    int m_bsSize;
    int m_kochSize;
};

class Painter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth)
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
    Q_PROPERTY(int func READ func WRITE setFunc)
    Q_PROPERTY(int ra READ ra WRITE setRa)
    Q_PROPERTY(int rb READ rb WRITE setRb)
    Q_PROPERTY(int angle READ angle WRITE setAngle)
    Q_PROPERTY(int vthetax READ vthetax WRITE setVthetax)
    Q_PROPERTY(int lthetax READ lthetax WRITE setLthetax)
    Q_PROPERTY(int vthetay READ vthetay WRITE setVthetay)
    Q_PROPERTY(int lthetay READ lthetay WRITE setLthetay)
    Q_PROPERTY(int vthetaz READ vthetaz WRITE setVthetaz)
    Q_PROPERTY(int lthetaz READ lthetaz WRITE setLthetaz)
    Q_PROPERTY(QWidget* widget READ widget WRITE setWidget)

public:
    Painter(QQuickItem *parent = 0);
    ~Painter();

    QWidget* widget() { return m_widget; }
    void setWidget(QWidget * widget) { m_widget = widget; }

    int vthetax() { return m_vThetax; }
    void setVthetax(int vTheta) { m_vThetax = vTheta; }

    int vthetay() { return m_vThetay; }
    void setVthetay(int vTheta) { m_vThetay = vTheta; }

    int vthetaz() { return m_vThetaz; }
    void setVthetaz(int vTheta) { m_vThetaz = vTheta; }

    int lthetax() { return m_lThetax; }
    void setLthetax(int lTheta) { m_lThetax = lTheta; }

    int lthetay() { return m_lThetay; }
    void setLthetay(int lTheta) { m_lThetay = lTheta; }

    int lthetaz() { return m_lThetaz; }
    void setLthetaz(int lTheta) { m_lThetaz = lTheta; }

    bool isEnabled() const {return m_bEnabled;}
    void setEnabled(bool enabled) { m_bEnabled = enabled; }

    int penWidth() const { return m_pen.width(); }
    void setPenWidth(int width) { m_pen.setWidth(width); }

    QColor penColor() const { return m_pen.color(); }
    void setPenColor(QColor color) { m_pen.setColor(color); }

    int func() const { return m_pfunc; }
    void setFunc(int pfunc) { m_pfunc = pfunc; }

    int ra() const { return m_era; }
    void setRa(int ra) { m_era = ra; }

    int rb() const { return m_erb; }
    void setRb(int rb) { m_erb = rb; }

    int angle() const { return m_eangle; }
    void setAngle(int angle) { m_eangle = angle; }

    Q_INVOKABLE void clear();
    Q_INVOKABLE void undo();

    void paint(QPainter *painter);
    QWidget* m_widget;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void purgePaintElements();

protected:
    QPointF m_lastPoint;
    QPointF m_firstPoint;
    QVector<ElementGroup*> m_elements;
    ElementGroup *m_element;
    bool m_bEnabled;
    bool m_bPressed;
    bool m_bMoved;
    QPen m_pen;
    int m_pfunc;
    int m_era;
    int m_erb;
    int m_eangle;
    int m_beSize;
    int m_bsSize;
    int m_kochSize;
    int m_vThetax;
    int m_lThetax;
    int m_vThetay;
    int m_lThetay;
    int m_vThetaz;
    int m_lThetaz;
};

// 三维空间中的点
struct Point3D {
    double x;
    double y;
    double z;
};
struct Point3DN {
    double x;
    double y;
    double z;
    double xn;
    double yn;
    double zn;
    double px, py;
};

// 三角面
struct TriSurface {
    int no; // 所属景物序号
    Point3D p1, p2, p3;
    double xn, yn, zn; // 三角面法向量
    double I;
};
struct TriSurfaceN {
    int no; // 所属景物序号
    Point3DN p1, p2, p3;
    double xn, yn, zn; // 三角面法向量
    double I;
};



#endif // PAINTER_H
