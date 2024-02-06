#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class Material;

struct VertexData
{
    VertexData()
    {

    }
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), textCoord(t), normal(n)
    {

    }
    void changePosition(QVector3D p)
    {
        position = p;
    }
    void changeNormal(QVector3D n)
    {
        normal = n;
    }
    QVector3D position;
    QVector2D textCoord;
    QVector3D normal;
};

class SimpleObject3D : protected QOpenGLFunctions
{
public:
    SimpleObject3D();
    SimpleObject3D(const QVector<VertexData> &vertData,
                   const QVector<GLuint> &indexes,
                   Material* material, Material* secondMaterial, int blend_percent);

    virtual ~SimpleObject3D();
    void init(const QVector<VertexData> &vertData,
              const QVector<GLuint> &indexes,
              Material* material, Material* secondMaterial = 0,int blend_percent = 0);

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions);

    void translate(const QVector3D& translateVector);
    void scale(const QVector3D& translateVector);

    void setModelLegVertexes(QVector<VertexData> legVert){modelLegVertexes = legVert;}
    void setLegMaterial(Material* legMat){legMaterial = legMat;}

    QVector3D getScale();

    void setCubeTransformRadius(float Radius){ m_radius = Radius;}
    QVector<VertexData> getVertData(){
        if(!m_vertData.isEmpty())
            return m_vertData;
        else
            return QVector<VertexData>();
    }
    QVector<GLuint>  getIndexBuffer() { return m_indexes;}
    Material* getMaterial(){return m_material;}
private:

    float m_radius;
    int blend_percent_;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_modelMatrix;
    QOpenGLTexture* m_texture;
    QOpenGLTexture* m_texture2;

    QVector<VertexData> m_vertData;
    QVector<GLuint> m_indexes;
    Material* m_material;

    QVector<VertexData> modelLegVertexes;
    Material* legMaterial;

};

#endif // SIMPLEOBJECT3D_H
