#ifndef KCUBEWIDGET_H
#define KCUBEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QVector3D>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

typedef  struct  _rectangleInfo_stru {
    GLfloat length;    //长, 取值范围0.01-10.00
    GLfloat width;     //宽, 取值范围0.01-10.00
    GLfloat height;    //高, 取值范围0.01-10.00

    GLfloat xRot;      //绕x旋转，取值范围0.00-180.00.支持微调
    GLfloat yRot;      //绕y旋转，取值范围0.00-180.00.支持微调
    GLfloat zRot;      //绕z旋转，取值范围0.00-180.00.支持微调
    GLfloat scale;     //缩放, 默认为3.00,取值范围0.01-200.00;
    _rectangleInfo_stru() {
        length = 3;
        width = 3;
        height = 3;

        xRot = 0;
        yRot = 0;
        zRot = 0;
        scale = 3;
    }
} rectangleInfo_stru;

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
    void setClearColor(const QColor& color);

    void setScale(GLfloat scale);
    void setRoteX(GLfloat xRot);
    void setRoteY(GLfloat yRot);
    void setRoteZ(GLfloat zRot);

    void setLength(GLfloat length);
    void setWidth(GLfloat width);
    void setHeight(GLfloat height);
    const rectangleInfo_stru& getInfo() const;
signals:
    void dataChanged();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);

private:
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void makeObject();                  //创建三维空间中的顶点数据
private:
    QColor m_clearColor;                //opengl 窗口背景颜色
    QPoint m_lastPos;                   //鼠标上一次位置

    rectangleInfo_stru rectangleInfo;   //矩形相关信息

    QVector<CubePoint> m_data;
    QOpenGLTexture *m_textures;
    QOpenGLShaderProgram *m_program;    //定义一个着色器程序类变量,用来执行opengl的着色器语言代码
    QOpenGLBuffer m_vbo;                //在Opengl中创建的缓冲区对象,复制该变量时，可作为底层opengl缓冲区对象的引用进行复制.
};
#endif // KCUBEWIDGET_H
