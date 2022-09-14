#include "kcubewidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QtMath>

#define PROGRAM_VERTEX_ATTRIBUTE   0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

KCubeWidget::KCubeWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_clearColor(Qt::black)
    , m_program()
{

}

KCubeWidget::~KCubeWidget()
{
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    doneCurrent();
}

void KCubeWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
     rectangleInfo.xRot += xAngle;
     rectangleInfo.yRot += yAngle;
     rectangleInfo.zRot += zAngle;
     update();
     emit dataChanged();
}

void KCubeWidget::setScale(GLfloat scale)
{
    rectangleInfo.scale += scale;
    update();
}

void KCubeWidget::setRoteX(GLfloat xRot)
{
    rectangleInfo.xRot = xRot;
    update();
}

void KCubeWidget::setRoteY(GLfloat yRot)
{
    rectangleInfo.yRot = yRot;
    update();
}

void KCubeWidget::setRoteZ(GLfloat zRot)
{
    rectangleInfo.zRot = zRot;
    update();
}

void KCubeWidget::setLength(GLfloat length)
{
    rectangleInfo.length = length;
    update();
}

void KCubeWidget::setWidth(GLfloat width)
{
    rectangleInfo.width = width;
    update();
}

void KCubeWidget::setHeight(GLfloat height)
{
    rectangleInfo.height = height;
    update();
}

void KCubeWidget::setClearColor(const QColor &color)
{
    m_clearColor = color;
    update();
}

const rectangleInfo_stru &KCubeWidget::getInfo() const
{
    return rectangleInfo;
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

    m_program = new QOpenGLShaderProgram;
    m_program->addShader(vshader);
    m_program->addShader(fshader);
    m_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    m_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    m_program->link();
    m_program->bind();
    m_program->setUniformValue("texture", 0);
}

void KCubeWidget::paintGL()
{
    //设置刷新显示的时候的背景颜色
    glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), m_clearColor.alphaF());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;   //定义一个矩阵，由于未初始化，所以构造函数会初始化未一个单位矩阵；
    m.perspective(M_PI / 2.0f, 1.3f, 3.0f, 200.0f);
    qDebug() << "m111===>" << m;
    m.lookAt(QVector3D(0, 0, 100), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    qDebug() << "m111311111===>" << m;
    //平移变换
    m.translate(0.0f, 0.0f, rectangleInfo.scale);
    qDebug() << "m222===>" << m;
//    //沿着X轴旋转
    m.rotate(rectangleInfo.xRot, 1.0f, 0.0f, 0.0f);
    //沿着Y轴旋转
    m.rotate(rectangleInfo.yRot, 0.0f, 1.0f, 0.0f);
    qDebug() << "m333===>" << m;
//    //缩放
    m.scale(rectangleInfo.length, rectangleInfo.width, rectangleInfo.height);
    qDebug() << "m444===>" << m;

    //注意:上面关于m的操作在程序运行过程中，是先将顶点旋转，然后平移，最后通过ortho()定义视景体
    //将当前上下文中名为matrix的变量赋值为矩阵
    m_program->setUniformValue("matrix", m);

    //启用此着色器程序中位于PROGRAM_VERTEX_ATTRIBUTE位置的顶点数组
    m_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    //启用此着色器程序中位于enableAttributeArray位置的顶点数组
    m_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

    //QOpenGLShaderProgram::setAttriBuffer 在此着色器程序中属性的位置设置一个顶点值数组，从当前绑定的顶点缓冲区中的特定偏移量开始。
    //步幅表示顶点之间的字节数。默认的步幅值为零，表示顶点密集地排列在值数组中。该类型表示顶点值数组中元素的类型，通常是gl_float、gl_unsigned_byte等
    m_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof (GLfloat));
    m_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof (GLfloat));
    m_textures->bind();

//    for(int i=0;i<6;i++)
//    {
//    glDrawArrays(GL_TRIANGLE_FAN,i*4,4);
//    }
//    textures->release();

    for(int i=0;i<6;i++)
    {
        glLineWidth(2.0f);
//        glDrawArrays(GL_LINE_LOOP,i*4,4);
        glDrawArrays(GL_TRIANGLE_FAN,i*4,4);
    }
}

void KCubeWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);
}

void KCubeWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void KCubeWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(dy, dx, 0);
    }

    m_lastPos = event->pos();
}

void KCubeWidget::wheelEvent(QWheelEvent *event)
{
    double val = event->delta() / 120.0;
    rectangleInfo.scale += (GLfloat)val;
    update();
    emit dataChanged();
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

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_data.constData(), m_data.size() * sizeof(CubePoint));

    m_textures = new QOpenGLTexture(QImage(":/2.jpg").mirrored());

}

