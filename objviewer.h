#ifndef OBJVIEWER_H
#define OBJVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QObject>
#include <QFile>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLContext>
#include <QVector3D>
#include <QMap>
#include "objectengine3d.h"
#include <QMessageBox>

#include <QLabel>
class SimpleObject3D;


class objViewer : public QOpenGLWidget , protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit objViewer(QWidget* parent = 0);
    ~objViewer();

    enum moveDirection
    {
        forward,
        backward,
        left,
        right
    };

    void moveObject(moveDirection direction);
    void slowObject(moveDirection direction);
private:
    QMatrix4x4 m_projectMatrix;
    QOpenGLShaderProgram m_program;
    QVector2D mouse_position;
    QQuaternion m_rotation;

    QVector<ObjectEngine3D*> m_objects;

    float m_x,m_y,m_z;

    QQuaternion m_rotate;
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;

    float max_z = 10;
    float min_z =  1;

    float x_direction;
    float z_direction;

    bool moving_forward{false};
    bool moving_left{false};
    bool moving_right{false};
    bool moving_backward{false};

    void rotateX(const QQuaternion &r);
    void rotateY(const QQuaternion &r);
    QVector<VertexData> extendedLegVertexes(float width);
    QVector<VertexData> retractedLegVertexes(float width);
    QVector<VertexData> mashroomVertexex(float width);
public slots:
    void changeObjectForm(float scale);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initCubeV6(float width, int divider);

    void loadObj(const QString& path);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    QVector3D calcNewPosition();
};

#endif // OBJVIEWER_H
