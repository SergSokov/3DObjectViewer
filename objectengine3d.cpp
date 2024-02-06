#include "objectengine3d.h"
#include <QFile>
#include <QFileInfo>

ObjectEngine3D::ObjectEngine3D()
{
    m_mainObject = nullptr;
    m_mainObjectLeg = nullptr;
}

void ObjectEngine3D::loadObjectFromFile(const QString &fileName)
{
    QFile objFile(fileName);
    if(!objFile.exists())
    {
        qDebug() << "Error: File not Found";
        return;
    }

    objFile.open(QIODevice::ReadOnly);
    QTextStream input(&objFile);


    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;
    SimpleObject3D* object = 0;
    QString mtlName;
    while(!input.atEnd())
    {
        QString str = input.readLine();

        QStringList list = str.split(" ");
        if(list.isEmpty())
            continue;
        QString type = list[0];
        if(type == "#")
        {
            continue;
        }
        else if(type == "mtllib")
        {
            QFileInfo info(fileName);
            m_materialLibrary.loadMaterialFromFile(info.absolutePath() + "/" + list[1]);
            continue;
        }
        else if(type == "v")
        {
            float x,y,z;
            try {
                x = list[1].toFloat();
                y = list[2].toFloat();
                z = list[3].toFloat();
            }  catch (std::exception) {
                qDebug() << "V error";
                continue;
            }
            coords.append(QVector3D(x,y,z));
            continue;
        }
        else if(type == "vt")
        {
            float x,y;
            try {
                x = list[1].toFloat();
                y = list[2].toFloat();

            }  catch (std::exception) {
                qDebug() << "VT error";
                continue;
            }
            texCoords.append(QVector2D(x,y));
            continue;
        }
        else if(type == "vn")
        {
            float x,y,z;
            try {
                x = list[1].toFloat();
                y = list[2].toFloat();
                z = list[3].toFloat();
            }  catch (std::exception) {
                qDebug() << "VN error";
                continue;
            }
            normals.append(QVector3D(x,y,z));
            continue;
        }
        else if(type == "f")
        {
            for(int i = 1; i<= 3; ++i)
            {
                if(list.size() < 3)
                    break;
                QStringList vert = list[i].split("/");
                QVector3D p;
                QVector2D t;
                QVector3D n;
                try {
                    p = coords[vert[0].toLong() - 1];
                    t = texCoords[vert[1].toLong() - 1];
                    n = normals[vert[2].toLong() - 1];
                }  catch (std::exception) {
                    qDebug() << "F error";
                    continue;
                }
                vertexes.append(VertexData(p,t,n));
                indexes.append(indexes.size());
            }
            continue;
        }
        else if(type == "usemtl")
        {

            if(object) object->init(vertexes,indexes, m_materialLibrary.getMaterial(mtlName));
            mtlName = list[1];
            addObject(object);
            object = new SimpleObject3D;
            vertexes.clear();
            indexes.clear();
        }

    }

    if(object) object->init(vertexes,indexes, m_materialLibrary.getMaterial(mtlName));
    addObject(object);

    m_carObject = object;

    objFile.close();
}

void ObjectEngine3D::addObject(SimpleObject3D *object)
{
    if(!object) return;

    for(int i = 0 ; i < m_objects.size(); ++i)
        if(m_objects[i] == object)
            return;

    m_objects << object;
}

SimpleObject3D* ObjectEngine3D::getObject(quint32 index)
{
    if(index < (quint32)m_objects.size())
        return m_objects[index];
    else
        return 0;

}


void ObjectEngine3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    //Пока рисуем только кубик
    if(m_mainObject != 0 && m_mainObjectLeg && m_cube && m_sphere && m_elipse && m_table)
    {
        m_mainObject->draw(program, functions);
        m_mainObjectLeg->draw(program, functions);
    }
}


void ObjectEngine3D::transform(float set_Radius)
{
    QVector<VertexData> newVertexes;
    QVector<VertexData> newLegVertexes;
    QVector<VertexData> firstObject;
    QVector<VertexData> secondObject;

    Material* firstMat;
    Material* secondMat;

    QVector<VertexData> firstLegObj;
    QVector<VertexData> secondLegObj;
    Material* firstLegMat;
    Material* secondLegMat;

    bool legsFlag{false};
    if(set_Radius < 100)
    {
        firstObject = m_cube->getVertData();
        secondObject = m_sphere->getVertData();
        firstMat = m_cube->getMaterial();
        secondMat = m_sphere->getMaterial();

        firstLegObj = m_cubeLeg->getVertData();
        secondLegObj = m_sphereLeg->getVertData();

        firstLegMat = m_cubeLeg->getMaterial();
        secondLegMat = m_sphereLeg->getMaterial();
    }
    else if(set_Radius <= 200)
    {
        firstObject = m_sphere->getVertData();
        secondObject = m_elipse->getVertData();
        firstMat = m_sphere->getMaterial();
        secondMat = m_elipse->getMaterial();

        firstLegObj = m_sphereLeg->getVertData();
        secondLegObj = m_elipseLeg->getVertData();

        firstLegMat = m_sphereLeg->getMaterial();
        secondLegMat = m_elipseLeg->getMaterial();

        set_Radius -= 100;
    }
    else if(set_Radius <= 300)
    {
        firstObject = m_elipse->getVertData();
        secondObject = m_table->getVertData();
        firstMat = m_elipse->getMaterial();
        secondMat = m_table->getMaterial();

        firstLegObj = m_elipseLeg->getVertData();
        secondLegObj = m_tableLeg->getVertData();

        firstLegMat = m_elipseLeg->getMaterial();
        secondLegMat = m_tableLeg->getMaterial();
        set_Radius -= 200;

    }

    for(int i = 0; i < firstObject.size(); i++)
    {
        QVector3D newVert;
        QVector3D cubeVertPosition;
        QVector3D sphereVertPosition;
        try {
            cubeVertPosition = firstObject[i].position;
        }  catch (std::exception e) {
            qDebug() << e.what();
            continue;
        }

        try {
            sphereVertPosition = secondObject[i].position;
        }  catch (std::exception e) {
            qDebug() << e.what();
            continue;
        }

        QVector3D oldVert;
        oldVert = cubeVertPosition;

        float dist = sqrt(oldVert.x() * oldVert.x() +  oldVert.y() * oldVert.y() + oldVert.z() * oldVert.z());

        newVert = oldVert + (sphereVertPosition - cubeVertPosition) * set_Radius / 100;

        if(set_Radius * m_maxRadius / 100 >= m_maxRadius)
        {
            try {
                newVert = secondObject[i].position;
            }  catch (std::exception e) {
                qDebug() << i;
                qDebug() << e.what();
            }
        }

        QVector3D newNormal(oldVert.x() / dist, oldVert.y() / dist, oldVert.z() / dist);
        VertexData newData;
        try {
            newData = firstObject[i];
        }  catch (std::exception e) {
            qDebug() << i;
            qDebug() << e.what();
        }
        newData.changePosition(newVert);
        newVertexes.append(newData);
    }

    for(int i = 0; i < newVertexes.size(); i+=4)
    {

            QVector3D zeroOne = newVertexes[i+1].position - newVertexes[i].position;
            QVector3D zeroTwo = newVertexes[i+2].position - newVertexes[i].position;

            QVector3D y(0,1,0);
            QVector3D x(1,0,0);
            QVector3D z = QVector3D::normal(zeroOne,zeroTwo);

            QVector3D normal = QVector3D::normal(zeroOne,zeroTwo);
            newVertexes[i].changeNormal(normal);
            newVertexes[i + 1].changeNormal(normal);
            newVertexes[i + 2].changeNormal(normal);

            QVector3D threeTwo = newVertexes[i + 2].position - newVertexes[i + 3].position;
            QVector3D threeOne = newVertexes[i + 1].position - newVertexes[i + 3].position;
            QVector3D normal2 = QVector3D::normal(threeTwo, threeOne);
            newVertexes[i + 2].changeNormal(normal2);
            newVertexes[i + 1].changeNormal(normal2);
            newVertexes[i + 3].changeNormal(normal2);
    }

    if(m_mainObject != nullptr)
        delete  m_mainObject;

    if(m_mainObjectLeg != nullptr)
        delete  m_mainObjectLeg;

    for(int i = 0; i < firstLegObj.size(); i++)
    {
        QVector3D newVert;
        QVector3D cubeVertPosition;
        QVector3D sphereVertPosition;
        try {
            cubeVertPosition = firstLegObj[i].position;
        }  catch (std::exception e) {
            qDebug() << e.what();
            continue;
        }

        try {
            sphereVertPosition = secondLegObj[i].position;
        }  catch (std::exception e) {
            qDebug() << e.what();
            continue;
        }

        QVector3D oldVert;
        oldVert = cubeVertPosition;

        float dist = sqrt(oldVert.x() * oldVert.x() +  oldVert.y() * oldVert.y() + oldVert.z() * oldVert.z());

        newVert = oldVert + (sphereVertPosition - cubeVertPosition) * set_Radius / 100;

        if(set_Radius * m_maxRadius / 100 >= m_maxRadius)
        {
            try {
                newVert = secondLegObj[i].position;
            }  catch (std::exception e) {
                qDebug() << i;
                qDebug() << e.what();
            }
        }

        QVector3D newNormal(oldVert.x() / dist, oldVert.y() / dist, oldVert.z() / dist);
        VertexData newData;
        try {
            newData = firstLegObj[i];
        }  catch (std::exception e) {
            qDebug() << i;
            qDebug() << e.what();
        }
        newData.changePosition(newVert);
        newLegVertexes.append(newData);
    }

    for(int i = 0; i < newLegVertexes.size(); i+=4)
    {
        //QVector3D first = newVertexes[i].position;

            QVector3D zeroOne = newLegVertexes[i+1].position - newLegVertexes[i].position;
            QVector3D zeroTwo = newLegVertexes[i+2].position - newLegVertexes[i].position;

            QVector3D y(0,1,0);
            QVector3D x(1,0,0);
            QVector3D z = QVector3D::normal(zeroOne,zeroTwo);

            QVector3D normal = QVector3D::normal(zeroOne,zeroTwo);
            newLegVertexes[i].changeNormal(normal);
            newLegVertexes[i + 1].changeNormal(normal);
            newLegVertexes[i + 2].changeNormal(normal);

            QVector3D threeTwo = newLegVertexes[i + 2].position - newLegVertexes[i + 3].position;
            QVector3D threeOne = newLegVertexes[i + 1].position - newLegVertexes[i + 3].position;
            QVector3D normal2 = QVector3D::normal(threeTwo, threeOne);
            newLegVertexes[i + 2].changeNormal(normal2);
            newLegVertexes[i + 1].changeNormal(normal2);
            newLegVertexes[i + 3].changeNormal(normal2);
    }


    m_mainObject = new SimpleObject3D(newVertexes, m_cube->getIndexBuffer(), firstMat, secondMat, set_Radius);
    m_mainObjectLeg = new SimpleObject3D(newLegVertexes, m_cubeLeg->getIndexBuffer(), firstLegMat, secondLegMat, set_Radius);
}

void ObjectEngine3D::translate(const QVector3D &translateVector)
{
    for(int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->translate(translateVector);
}

void ObjectEngine3D::scale(const QVector3D &scaleVector)
{
    /*for(int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->scale(scaleVector);*/
}
