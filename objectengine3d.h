#ifndef OBJECTENGINE3D_H
#define OBJECTENGINE3D_H

#include <cmath>
#include <QVector>
#include "simpleobject3d.h"
#include "materiallibrary.h"

class ObjectEngine3D
{
public:
    ObjectEngine3D();
    void loadObjectFromFile(const QString& fileName);
    void addObject(SimpleObject3D* object);
    SimpleObject3D* getObject(quint32 index);

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions);

    void translate(const QVector3D& translateVector);
    void scale(const QVector3D& translateVector);
    QVector3D getScale();
    void transform(float set_Radius);

    void setCubeMaxRadius(const float maxRadius){ m_maxRadius = maxRadius; }

    void setCube(SimpleObject3D* cube){ m_cube = cube;}
    void setSphere(SimpleObject3D* sphere){ m_sphere = sphere;}
    void setElipse(SimpleObject3D* elipse){ m_elipse = elipse;}
    void setTable(SimpleObject3D* table){ m_table = table;}
    void setDuga(SimpleObject3D* duga){ m_duga = duga;}
    void setCar(SimpleObject3D* car){ m_carObject = car;}

    void setCubeLeg(SimpleObject3D* cubeLeg){ m_cubeLeg = cubeLeg;}
    void setSphereLeg(SimpleObject3D* sphereLeg){ m_sphereLeg = sphereLeg;}
    void setElipseLeg(SimpleObject3D* elipseLeg){ m_elipseLeg = elipseLeg;}
    void setTableLeg(SimpleObject3D* tableLeg){ m_tableLeg = tableLeg;}

    SimpleObject3D* getSphere(){return m_sphere;}
    SimpleObject3D* getMainObj(){return m_mainObject;}
    SimpleObject3D* getMainObjLeg(){return m_mainObjectLeg;}

    int setBaseBertexCount(int count) {return baseVertexCount = count;}
    void setWoodMaterial(Material* material){m_wood_material = material;}
private:

    SimpleObject3D* m_cube=0;
    SimpleObject3D* m_cubeLeg=0;
    SimpleObject3D* m_sphere=0;
    SimpleObject3D* m_sphereLeg=0;
    SimpleObject3D* m_elipse=0;
    SimpleObject3D* m_elipseLeg=0;
    SimpleObject3D* m_table=0;
    SimpleObject3D* m_tableLeg=0;
    SimpleObject3D* m_mainObject=0;
    SimpleObject3D* m_mainObjectLeg=0;

    SimpleObject3D* m_duga=0;
    SimpleObject3D* m_carObject=0;

    Material* m_wood_material=0;
    int baseVertexCount = 0;
    QVector<SimpleObject3D*> m_objects;
    MaterialLibrary m_materialLibrary;

    float m_maxRadius;
};

#endif // OBJECTENGINE3D_H
