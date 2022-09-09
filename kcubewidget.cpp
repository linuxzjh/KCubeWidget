#include "kcubewidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#define PROGRAM_VERTEX_ATTRIBUTE   0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

KCubeWidget::KCubeWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , clearColor(Qt::black)
    , xRot(0)
    , yRot(0)
    , zRot(0)
    , scale(0.5)
    , program()
{

}

KCubeWidget::~KCubeWidget()
{
    makeCurrent();
    vbo.destroy();
    delete program;
    doneCurrent();
}

void KCubeWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;

    update();
}

void KCubeWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void KCubeWidget::initializeGL()
{
    //为当前环境初始化OpenGL功能
    initializeOpenGLFunctions();
    //构建立方体每个顶点坐标
    makeObject();
    //阴影平滑设置
    glShadeModel(GL_SMOOTH);
    //开启OpenGL的一些功能
    //如果启用，请进行深度比较并更新深度缓冲区。请注意，
    //即使深度缓冲区存在，并且深度掩模非零,如果禁用了深度测试，深度缓冲区也不会更新
    glEnable(GL_DEPTH_TEST);
    //在可编程管线中，需要纯手写顶点和片段着色器，用到opengl的着色器语言GLSL
    //GLSL是在GPU上执行的，使渲染管线中不同层次具有可编程性，比如视图转换，投影转换等
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char* vsrc =
            "attribute highp vec4 vertex; \n"
            "attribute mediump vec4 texCoord; \n"
            "varying mediump vec4 texc; \n"
            "uniform mediump mat4 matrix; \n"
            "void main(void)\n"
            "{\n"
            "   gl_Position = matrix * vertex; \n"
            "   texc = texCoord; \n"
            "}\n"
            ;
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char* fsrc =
            "uniform sampler2D texture; \n"
            "varying mediump vec4 texc; \n"
            "void main(void)\n"
            "{\n"
            "   gl_FragColor = texture2D(texture, texc.st); \n"
            "}\n"
            ;
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("texture", 0);
}

void KCubeWidget::paintGL()
{
    //设置刷新显示的时候的背景颜色
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    //定义一个长方体，长方体正面与camera的光轴垂直，4.0f表示长方体正面与camera的距离,15.0f表示长方体背面
    m.ortho(-1.0f, +1.0f, +1.0f, -1.0f, 4.0f, 15.0f);
    //平移
    m.translate(0.0f, 0.0f, -10.0f);
//    //沿着X轴旋转
//    m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    //沿着Y轴旋转
    m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
//    //缩放
//    m.scale(scale / 10.0f, scale / 10.0f, scale / 10.0f);

    //注意:上面关于m的操作在程序运行过程中，是先将顶点旋转，然后平移，最后通过ortho()定义视景体
    //将当前上下文中名为matrix的变量赋值为矩阵
    program->setUniformValue("matrix", m);

    //启用此着色器程序中位于PROGRAM_VERTEX_ATTRIBUTE位置的顶点数组
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    //启用此着色器程序中位于enableAttributeArray位置的顶点数组
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

    //QOpenGLShaderProgram::setAttriBuffer 在此着色器程序中属性的位置设置一个顶点值数组，从当前绑定的顶点缓冲区中的特定偏移量开始。
    //步幅表示顶点之间的字节数。默认的步幅值为零，表示顶点密集地排列在值数组中。该类型表示顶点值数组中元素的类型，通常是gl_float、gl_unsigned_byte等
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof (GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof (GLfloat));
    textures->bind();

//    for(int i=0;i<6;i++)
//    {
//    glDrawArrays(GL_TRIANGLE_FAN,i*4,4);
//    }
//    textures->release();

    for(int i=0;i<6;i++)
    {
        glLineWidth(2.0f);
        glDrawArrays(GL_LINE_LOOP,i*4,4);
//        glDrawArrays(GL_TRIANGLE_FAN,i*4,4);
    }
}

void KCubeWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);
}

void KCubeWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void KCubeWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 *dx, 0);
    }

    lastPos = event->pos();
}

void KCubeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked();
}

void KCubeWidget::wheelEvent(QWheelEvent *event)
{
    double val = event->delta() / 120.0;
    scale += (GLfloat)val;
    update();
}

void KCubeWidget::makeObject()
{
    m_data = {
        //Front face
        {{-0.5, -0.5, 0.5}, {0, 0}},  {{0.5, -0.5, 0.5}, {1, 0}},
        {{0.5, 0.5, 0.5}, {1, 1}},  {{-0.5, 0.5, 0.5}, {0, 1}},

        //Top face
        {{-0.5, 0.5, 0.5}, {0, 0}},  {{0.5, 0.5, 0.5}, {1, 0}},
        {{0.5, 0.5, -0.5}, {1, 1}},  {{-0.5, 0.5, -0.5}, {0, 1}},

        //Back face
        //{{-0.5, 0.5, -0.5}, {0, 0}},  {{0.5, 0.5, -0.5}, {1, 0}},
        //{{0.5, -0.5, -0.5}, {1, 1}},  {{0.5, -0.5, -0.5}, {0, 1}},
        {{-0.5, -0.5, -0.5}, {0, 0}},  {{0.5, -0.5, -0.5}, {1, 0}},
        {{0.5, 0.5, -0.5}, {1, 1}},  {{-0.5, 0.5, -0.5}, {0, 1}},

        //Bottom face
        {{-0.5, -0.5, -0.5}, {0, 0}},  {{0.5, -0.5, -0.5}, {1, 0}},
        {{0.5, -0.5, 0.5}, {1, 1}},  {{-0.5, -0.5, 0.5}, {0, 1}},

        //Left face
        {{-0.5, -0.5, -0.5}, {0, 0}},  {{-0.5, -0.5, 0.5}, {1, 0}},
        {{-0.5, 0.5, 0.5}, {1, 1}},  {{-0.5, 0.5, -0.5}, {0, 1}},

        //Right face
        {{0.5, -0.5, 0.5}, {0, 0}},  {{0.5, -0.5, -0.5}, {1, 0}},
        {{0.5, 0.5, -0.5}, {1, 1}},  {{0.5, 0.5, 0.5}, {0, 1}},
    };

    vbo.create();
    vbo.bind();
    vbo.allocate(m_data.constData(), m_data.size() * sizeof(CubePoint));

    textures = new QOpenGLTexture(QImage(":/2.jpg").mirrored());

}

