#ifndef KCUBEWIDGET_H
#define KCUBEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QVector3D>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)


class KCubeWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    struct CubePoint {
        QVector3D posCoord; //空间左边
        QVector2D textureCoord; //纹理坐标

        CubePoint(const QVector3D& position = QVector3D(),
                  const QVector2D& texture = QVector2D())
            : textureCoord(texture),posCoord(position) {}
    };

    explicit KCubeWidget(QWidget *parent = nullptr);
    ~KCubeWidget();
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setScale(GLfloat scale);
    void setClearColor(const QColor& color);
signals:
    void clicked();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);

private:
    void makeObject(); //创建三维空间中的顶点数据
    QColor clearColor;  //opengl 窗口背景颜色
    QPoint lastPos; //鼠标上一次位置
    int xRot;       //绕x旋转
    int yRot;       //绕y旋转
    int zRot;       //绕z旋转
    GLfloat scale;  //缩放

    QVector<CubePoint> m_data;
    QOpenGLTexture *textures;
    QOpenGLShaderProgram *program;  //定义一个着色器程序类变量,用来执行opengl的着色器语言代码
    QOpenGLBuffer vbo;  //在Opengl中创建的缓冲区对象,复制该变量时，可作为底层opengl缓冲区对象的引用进行复制.
};
#endif // KCUBEWIDGET_H
