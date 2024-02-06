#include "objviewer.h"
#include "objectengine3d.h"
#include "material.h"

#define PI 3.141592653
#define PI_4 0.78539816325

#define D 150.0

QVector3D defaultXrot(1.0f,0.0f,0.0f);
float defaultXAnagle = 15.f;
QVector3D defaultYrot(0.0f,1.0f,0.0f);
float defaultYAnagle = 30.f;

objViewer::objViewer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = -5.0f;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

objViewer::~objViewer()
{
    makeCurrent();
    for(int i = 0; i < m_objects.size(); i++)
    {
        delete m_objects[i];
    }
    doneCurrent();
}

void objViewer::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  initShaders();
  initCubeV6(2.5 , 60);
  changeObjectForm(1.0f);

  glEnable(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB);
}

void objViewer::resizeGL(int w, int h)
{
    float aspect = w /(h ? (float)h : 1);

    m_projectMatrix.setToIdentity();
    m_projectMatrix.perspective(60,aspect, 0.01f,D);
}

void objViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();

    viewMatrix.translate(m_x,m_y,m_z);
    viewMatrix.rotate(m_rotate);
    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectMatrix);
    m_program.setUniformValue("u_viewMatrix", viewMatrix);

    m_program.setUniformValue("u_lightPosition", viewMatrix);
    m_program.setUniformValue("u_lightPower", 1.0f);

    for(int i = 0; i < m_objects.size(); ++i)
    {
        m_objects[i]->draw(&m_program, context()->functions());    
    }



}

QVector3D objViewer::calcNewPosition(){

    float new_x_pos = m_x + x_direction;
    float new_z_pos = m_z + z_direction;

    m_x = new_x_pos;
    m_z = new_z_pos;

    return QVector3D(round(new_x_pos), m_y, round(new_z_pos));
}

void objViewer::initShaders()
{
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.vsh"))
        close();

    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.fsh"))
        close();

    if(!m_program.link())
        close();
}

QVector<VertexData> objViewer::mashroomVertexex(float width)
{

    QVector<VertexData> vertexes;

    float H = width * 0.75;

    int angleCount = 60;

    int hCount = 60;

    float hCoeff = H / hCount;

    float rCoeff = width * 0.1;

    float hH = 4.0 / hCount;
    float bitmp_coeff = 1.0/hCount;
    float bitmap_coeff2 = 1.0/angleCount;

    float alpha = PI * 2 / angleCount;

    for(int j = 0; j < angleCount; j++)
    {
        float h0;
        float h1;

        float R0;
        float R1;

        float X0;
        float Y0;
        float Z0;

        float X1;
        float Y1;
        float Z1;

        float X2;
        float Y2;
        float Z2;

        float X3;
        float Y3;
        float Z3;


        for(int i = 0; i < hCount; i++)
        {
            h0 = i * hCoeff;
            h1 = (i + 1) * hCoeff;

            R0 = rCoeff * (2.0 + sin( PI_4 *( i * hH - 1) ));
            R1 = rCoeff * (2.0 + sin( PI_4 *( (i + 1) * hH - 1) ));

            X0 = R0* sin(alpha *j);
            Y0 = -h0;
            Z0 = R0* cos(alpha *j);

            X1 = R1* sin(alpha *j);
            Y1 = -h1;
            Z1 = R1* cos(alpha *j);

            X2 = R0* sin(alpha *(j + 1));
            Y2 = -h0;
            Z2 = R0* cos(alpha *(j + 1));

            X3 = R1* sin(alpha *(j + 1));
            Y3 = -h1;
            Z3 = R1* cos(alpha *(j + 1));

            vertexes.append(VertexData(QVector3D(X0,Y0,Z0), QVector2D(j*bitmap_coeff2,1.0-i*bitmp_coeff), QVector3D(0.0, 0.0, 1.0)));
            vertexes.append(VertexData(QVector3D(X1,Y1,Z1), QVector2D(j*bitmap_coeff2,1.0-(i+1)*bitmp_coeff), QVector3D(0.0, 0.0, 1.0)));
            vertexes.append(VertexData(QVector3D(X2,Y2,Z2), QVector2D((j+1)*bitmap_coeff2,1.0-i*bitmp_coeff), QVector3D(0.0, 0.0, 1.0)));
            vertexes.append(VertexData(QVector3D(X3,Y3,Z3), QVector2D((j+1)*bitmap_coeff2,1.0-(i+1)*bitmp_coeff), QVector3D(0.0, 0.0, 1.0)));
        }

        vertexes.append(VertexData(QVector3D(X1,-h1,Z1), QVector2D(0,1.0), QVector3D(0.0, -1.0, 0.0)));
        vertexes.append(VertexData(QVector3D(0,-h1,0), QVector2D(0.0,0.0), QVector3D(0.0, -1.0, 0.0)));
        vertexes.append(VertexData(QVector3D(X1,-h1,Z1), QVector2D(1.0,1.0), QVector3D(0.0, -1.0, 0.0)));
        vertexes.append(VertexData(QVector3D(X3,-h1,Z3), QVector2D(1.0,0.0), QVector3D(0.0, -1.0, 0.0)));
    }
    return vertexes;
}

void objViewer::initCubeV6(float width, int divider)
{

    float width_div_2 = width / 2;
    if(!divider)
        return;

    float bitmapDiv = 1.0 / divider;
    float width_div_5 = width / divider;

    QVector<VertexData> vertexes;

    for(int i=0;i<divider;i++)
        for(int j=0;j<divider;j++)
        {
            vertexes.append(VertexData(QVector3D(-width_div_2 + j*width_div_5, width_div_2 - i*width_div_5, width_div_2), QVector2D(j * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, 0.0, 1.0)));
            vertexes.append(VertexData(QVector3D(-width_div_2 + j*width_div_5, width_div_2 - (i+1)*width_div_5, width_div_2), QVector2D(j * bitmapDiv, 1.0 - (1 + i) * bitmapDiv), QVector3D(0.0, 0.0, 1.0)));
            vertexes.append(VertexData(QVector3D(-width_div_2 + (j+1)*width_div_5, width_div_2 - i*width_div_5, width_div_2), QVector2D((j + 1) * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, 0.0, 1.0)));
            vertexes.append(VertexData(QVector3D(-width_div_2 + (j+1)*width_div_5, width_div_2 - (i+1)*width_div_5, width_div_2), QVector2D((j + 1) * bitmapDiv, 1.0 - (i + 1) * bitmapDiv), QVector3D(0.0, 0.0, 1.0)));
        }
    for(int i=0;i<divider;i++)
        for(int j=0;j<divider;j++)
        {
            vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2 + j*width_div_5, width_div_2 - i*width_div_5 ), QVector2D(j * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(1.0, 0.0, 0.0)));
            vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2 + j*width_div_5, width_div_2 - (i+1)*width_div_5), QVector2D(j * bitmapDiv, 1.0 - (1 + i) * bitmapDiv), QVector3D(1.0, 0.0, 0.0)));
            vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2 + (j+1)*width_div_5, width_div_2 - i*width_div_5), QVector2D((j + 1) * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(1.0, 0.0, 0.0)));
            vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2 + (j+1)*width_div_5, width_div_2 - (i+1)*width_div_5), QVector2D((j + 1) * bitmapDiv, 1.0 - (i + 1) * bitmapDiv), QVector3D(1.0, 0.0, 0.0)));
        }

    for(int i=0;i<divider;i++)
        for(int j=0;j<divider;j++)
        {
            vertexes.append(VertexData(QVector3D( width_div_2 - j*width_div_5, width_div_2, width_div_2 - i*width_div_5 ), QVector2D(j * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, 1.0, 0.0)));
            vertexes.append(VertexData(QVector3D(width_div_2 - j*width_div_5, width_div_2, width_div_2 - (i+1)*width_div_5), QVector2D(j * bitmapDiv, 1.0 - (1 + i) * bitmapDiv), QVector3D(0.0, 1.0, 0.0)));
            vertexes.append(VertexData(QVector3D(width_div_2 - (j+1)*width_div_5, width_div_2, width_div_2 - i*width_div_5), QVector2D((j + 1) * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, 1.0, 0.0)));
            vertexes.append(VertexData(QVector3D(width_div_2 - (j+1)*width_div_5, width_div_2, width_div_2 - (i+1)*width_div_5),QVector2D((j + 1) * bitmapDiv, 1.0 - (i + 1) * bitmapDiv), QVector3D(0.0, 1.0, 0.0)));
        }

    for(int i=0;i<divider;i++)
        for(int j=0;j<divider;j++)
        {
            vertexes.append(VertexData(QVector3D(-(-width_div_2 + j*width_div_5), width_div_2 - i*width_div_5, -width_div_2), QVector2D(j * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, 0.0, -1.0)));
            vertexes.append(VertexData(QVector3D(-(-width_div_2 + j*width_div_5), width_div_2 - (i+1)*width_div_5, -width_div_2), QVector2D(j * bitmapDiv, 1.0 - (1 + i) * bitmapDiv), QVector3D(0.0, 0.0, -1.0)));
            vertexes.append(VertexData(QVector3D(-(-width_div_2 + (j+1)*width_div_5), width_div_2 - i*width_div_5, -width_div_2), QVector2D((j + 1) * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, 0.0, -1.0)));
            vertexes.append(VertexData(QVector3D(-(-width_div_2 + (j+1)*width_div_5), width_div_2 - (i+1)*width_div_5, -width_div_2), QVector2D((j + 1) * bitmapDiv, 1.0 - (i + 1) * bitmapDiv), QVector3D(0.0, 0.0, -1.0)));
        }


    for(int i=0;i<divider;i++)
        for(int j=0;j<divider;j++)
        {
            vertexes.append(VertexData(QVector3D(-width_div_2, -(-width_div_2 + j*width_div_5), width_div_2 - i*width_div_5 ), QVector2D(j * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(-1.0, 0.0, 0.0)));
            vertexes.append(VertexData(QVector3D(-width_div_2, -(-width_div_2 + j*width_div_5), width_div_2 - (i+1)*width_div_5), QVector2D(j * bitmapDiv, 1.0 - (1 + i) * bitmapDiv), QVector3D(-1.0, 0.0, 0.0)));
            vertexes.append(VertexData(QVector3D(-width_div_2, -(-width_div_2 + (j+1)*width_div_5), width_div_2 - i*width_div_5), QVector2D((j + 1) * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(-1.0, 0.0, 0.0)));
            vertexes.append(VertexData(QVector3D(-width_div_2, -(-width_div_2 + (j+1)*width_div_5), width_div_2 - (i+1)*width_div_5), QVector2D((j + 1) * bitmapDiv, 1.0 - (i + 1) * bitmapDiv), QVector3D(-1.0, 0.0, 0.0)));
        }

    for(int i=0;i<divider;i++)
        for(int j=0;j<divider;j++)
        {
            vertexes.append(VertexData(QVector3D(-( width_div_2 - j*width_div_5), -width_div_2, width_div_2 - i*width_div_5 ), QVector2D(j * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, -1.0, 0.0)));
            vertexes.append(VertexData(QVector3D(-(width_div_2 - j*width_div_5), -width_div_2, width_div_2 - (i+1)*width_div_5), QVector2D(j * bitmapDiv, 1.0 - (1 + i) * bitmapDiv), QVector3D(0.0, -1.0, 0.0)));
            vertexes.append(VertexData(QVector3D(-(width_div_2 - (j+1)*width_div_5), -width_div_2, width_div_2 - i*width_div_5), QVector2D((j + 1) * bitmapDiv, 1.0 - i * bitmapDiv), QVector3D(0.0, -1.0, 0.0)));
            vertexes.append(VertexData(QVector3D(-(width_div_2 - (j+1)*width_div_5), -width_div_2, width_div_2 - (i+1)*width_div_5), QVector2D((j + 1) * bitmapDiv, 1.0 - (i + 1) * bitmapDiv), QVector3D(0.0, -1.0, 0.0)));
        }

    QVector<VertexData> mushroomLeg = mashroomVertexex(width);

    QVector<VertexData> tableLeg;
    for(VertexData point : mushroomLeg)
    {
        QVector3D pos = point.position;
        float x,y,z;
        x = pos.x();
        y = pos.y();
        z = pos.z();

        float a,b;
        a = width * 0.05;
        b = 0.05;

        float Rleg;
        float Rmushroom = sqrt(x * x + z * z);
        if(Rmushroom <= 0.01)
        {
            Rleg = 0;
        }
        else
            Rleg = (a + b * y *y * y * y) / Rmushroom;

        float newX = x * Rleg;
        float newZ = z * Rleg;
        VertexData newPoint = point;
        newPoint.changePosition(QVector3D(newX,y,newZ));
        tableLeg.append(newPoint);
    }

    QVector<VertexData> retractedMushroomLeg = mushroomLeg;
    for(int i = 0; i < retractedMushroomLeg.size(); i++)
    {
       retractedMushroomLeg[i].changePosition(retractedMushroomLeg[i].position * 0.01);
    }

    QVector<GLuint> indexes;
    for(int i = 0; i < (divider * divider * 4 * 6); i += 4)
    {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    QVector<GLuint> legIndexes;
    for(int i = 0; i < mushroomLeg.size(); i += 4)
    {
        legIndexes.append(i + 0);
        legIndexes.append(i + 1);
        legIndexes.append(i + 2);
        legIndexes.append(i + 2);
        legIndexes.append(i + 1);
        legIndexes.append(i + 3);
    }

    QVector<VertexData> legVertexes;

    Material* material = new Material;
    material->setDiffuseMap(":/textures/Doge.png");
    material->setShinnes(96);
    material->setDiffuseColor(QVector3D(1.0,1.0,1.0));
    material->setAmbienceColor(QVector3D(1.0,1.0,1.0));
    material->setSpecularColor(QVector3D(1.0,1.0,1.0));

    Material* WoodMaterial = new Material;
    WoodMaterial->setDiffuseMap(":/textures/Wood.jpg");
    WoodMaterial->setShinnes(96);
    WoodMaterial->setDiffuseColor(QVector3D(1.0,1.0,1.0));
    WoodMaterial->setAmbienceColor(QVector3D(1.0,1.0,1.0));
    WoodMaterial->setSpecularColor(QVector3D(1.0,1.0,1.0));

    Material* MushMaterial = new Material;
    MushMaterial->setDiffuseMap(":/textures/mushroom.png");
    MushMaterial->setShinnes(96);
    MushMaterial->setDiffuseColor(QVector3D(1.0,1.0,1.0));
    MushMaterial->setAmbienceColor(QVector3D(1.0,1.0,1.0));
    MushMaterial->setSpecularColor(QVector3D(1.0,1.0,1.0));

    Material* MushLegMaterial = new Material;
    MushLegMaterial->setDiffuseMap(":/textures/mushroomLeg.jpg");
    MushLegMaterial->setShinnes(96);
    MushLegMaterial->setDiffuseColor(QVector3D(1.0,1.0,1.0));
    MushLegMaterial->setAmbienceColor(QVector3D(1.0,1.0,1.0));
    MushLegMaterial->setSpecularColor(QVector3D(1.0,1.0,1.0));

    Material* SteelMaterial = new Material;
    SteelMaterial->setDiffuseMap(":/textures/al.jpeg");
    SteelMaterial->setShinnes(96);
    SteelMaterial->setDiffuseColor(QVector3D(1.0,1.0,1.0));
    SteelMaterial->setAmbienceColor(QVector3D(1.0,1.0,1.0));
    SteelMaterial->setSpecularColor(QVector3D(1.0,1.0,1.0));

    //Сфера vertex -> sphere
    QVector<VertexData> sphereVertexes;

    float con = 0.62035049089940001666800681204778;

    float R = con * width;

    for(auto vert : vertexes)
    {
        QVector3D newVert;
        QVector3D oldPosition = vert.position;
        float dist = sqrt(oldPosition.x() * oldPosition.x() +  oldPosition.y() * oldPosition.y() + oldPosition.z() * oldPosition.z());
        float coeff = R / dist;
        newVert.setX(vert.position.x() * coeff);
        newVert.setY(vert.position.y() * coeff);
        newVert.setZ(vert.position.z() * coeff);

        QVector3D newNormal(oldPosition.x() / dist, oldPosition.y() / dist, oldPosition.z() / dist);

        VertexData newData = vert;
        newData.changePosition(newVert);
        newData.changeNormal(newNormal);
        sphereVertexes.append(newData);
    }

    QVector<VertexData> elipseVertexes;
    for(auto vert : sphereVertexes)
    {
        QVector3D newVert;
        float coeff = 0.3;
        float coeff2 = 0.01;
        float yPos = vert.position.y();

        if(yPos > 0)
            yPos *= coeff;
        else
            yPos *= coeff2;

        newVert.setX(vert.position.x());
        newVert.setY(yPos);
        newVert.setZ(vert.position.z());
        VertexData newData = vert;
        newData.changePosition(newVert);
        elipseVertexes.append(newData);
    }

    QVector<VertexData> tableVertexes;
    for(auto vert : sphereVertexes)
    {
        QVector3D newVert;
        float coeff = 0.1;
        float coeff2 = 0.025 * width;
        float yPos = vert.position.y() * coeff;

        float multyCoeff = 1.5;

        if(yPos > coeff2)
            yPos = coeff2;
        else if(yPos < -coeff2)
            yPos = -coeff2;

        newVert.setX(vert.position.x() * multyCoeff);
        newVert.setY(yPos);
        newVert.setZ(vert.position.z() * multyCoeff);
        VertexData newData = vert;
        newData.changePosition(newVert);
        tableVertexes.append(newData);
    }

    ObjectEngine3D *newObj = new ObjectEngine3D;

    newObj->setBaseBertexCount(vertexes.count());
    newObj->setCube(new SimpleObject3D(vertexes, indexes, material,material,0));
    newObj->setCubeLeg(new SimpleObject3D(retractedMushroomLeg, legIndexes, MushLegMaterial,MushLegMaterial,0));

    newObj->setCubeMaxRadius(R);
    newObj->setSphere(new SimpleObject3D(sphereVertexes, indexes, material,material,0));
    newObj->setSphereLeg(new SimpleObject3D(retractedMushroomLeg, legIndexes, MushLegMaterial,MushLegMaterial,0));

    newObj->setElipse(new SimpleObject3D(elipseVertexes, indexes, MushMaterial, MushMaterial, 0));
    newObj->setElipseLeg(new SimpleObject3D(mushroomLeg, legIndexes, MushLegMaterial, MushLegMaterial, 0));

    newObj->setTable(new SimpleObject3D(tableVertexes, indexes, WoodMaterial, WoodMaterial, 0));
    newObj->setTableLeg(new SimpleObject3D(tableLeg, legIndexes, SteelMaterial, SteelMaterial, 0));

    m_objects.append(newObj);
}

void objViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton ||
            event->buttons() == Qt::RightButton)
    {
        mouse_position = QVector2D(event->localPos());
    }
    else if(event->button() == Qt::LeftButton ||
            event->buttons() == Qt::LeftButton)
    {
        mouse_position = QVector2D(event->localPos());

    }
    else if(event->button() == Qt::MidButton ||
            event->buttons() == Qt::MiddleButton)
    {
        mouse_position = QVector2D(0.0,0.0);
        m_x = 0.0;
        m_y = 0.0;
        m_z = 0.0;

        QQuaternion x = QQuaternion::fromAxisAndAngle(defaultXrot,defaultXAnagle);
        QQuaternion y = QQuaternion::fromAxisAndAngle(defaultYrot,defaultYAnagle);
        m_rotate = x * y;

        update();
    }
    event->accept();
}

void objViewer::rotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;

    update();
}

void objViewer::rotateY(const QQuaternion &r)
{
    m_rotateY = r * m_rotateY;
    m_rotate = m_rotateX * m_rotateY;

    update();
}

void objViewer::changeObjectForm(float scale)
{
    for(auto object : m_objects)
    {
        if(object)
        {
            object->transform(scale);
            object->translate(calcNewPosition());
            update();
        }
    }
}

void objViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton)
    {
        QVector2D diff = QVector2D(event->localPos()) - mouse_position;
        mouse_position = QVector2D(event->localPos());


        float angleX = diff.y() / 2.0f;
        float angleY = diff.x() / 2.0f;

        rotateX(QQuaternion::fromAxisAndAngle(1.0f,0.0f,0.0f,angleX));
        rotateY(QQuaternion::fromAxisAndAngle(0.0f,1.0f,0.0f,angleY));
        update();
    }
    else if(event->buttons() == Qt::LeftButton)
    {
        QVector2D diff = QVector2D(event->localPos()) - mouse_position;
        mouse_position = QVector2D(event->localPos());

        float dx = diff.x() / width();
        float dy = diff.y() / height();

        m_x += dx * 2;
        m_y += dy * 2;
        update();
    }
    event->accept();


}

void objViewer::moveObject(moveDirection direction)
{
    qDebug() << direction;
    switch (direction) {
        case moveDirection::left :{
            if(!moving_left)
            {
                qDebug() << "moving left";
                x_direction = -0.005;
                moving_left = true;
            }
            break;
        }
        case moveDirection::right :{
            if(!moving_right){
                x_direction = 0.005;
                moving_right = true;
            }
            break;
        }
        case moveDirection::forward :{
            if(!moving_forward){
                qDebug() << "moving forward";
                z_direction = 0.005;
                moving_forward = true;
            }
            break;
        }
        case moveDirection::backward :{
            if(!moving_backward){
                qDebug() << "moving backward";
                z_direction = -0.005;
                moving_backward = true;
            }
            break;
        }

    }

    update();
}

void objViewer::mouseReleaseEvent(QMouseEvent * event)
{
    //event->accept();
}

void objViewer::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0 && m_z <= -3)
    {
        m_z += 0.25f;
    }
    else if(event->angleDelta().y() < 0 && m_z >= -15)
    {
        m_z -= 0.25f;
    }

    update();
}
