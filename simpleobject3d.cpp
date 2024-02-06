#include "simpleobject3d.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "material.h"

SimpleObject3D::SimpleObject3D():
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0), m_texture2(0)
{

}

SimpleObject3D::SimpleObject3D(const QVector<VertexData> &vertData,
                               const QVector<GLuint> &indexes,
                               Material* material, Material* secondMaterial, int blend_percent):
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0), m_texture2(0)
{
    initializeOpenGLFunctions();
    init(vertData, indexes, material, secondMaterial, blend_percent);
}

SimpleObject3D::~SimpleObject3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();

    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != 0)
        if(m_texture->isCreated())
            m_texture->destroy();
    if(m_texture2 != 0)
        if(m_texture2->isCreated())
            m_texture2->destroy();
}


void SimpleObject3D::init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, Material* material, Material* secondMaterial, int blend_percent)
{
    m_vertData = vertData;
    m_indexes = indexes;

    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();

    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != 0)
    {
        if(m_texture->isCreated())
        {
           delete m_texture;
            m_texture = 0;
        }
    }

    if(m_texture2 != 0)
    {
        if(m_texture2->isCreated())
        {
           delete m_texture2;
            m_texture2 = 0;
        }
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();

    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();


    m_texture = new QOpenGLTexture(material->diffuseMap().mirrored());

    m_material = material;

    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

    m_texture->generateMipMaps();

    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    m_texture->setWrapMode(QOpenGLTexture::Repeat);

    m_texture2 = new QOpenGLTexture(secondMaterial->diffuseMap().mirrored());

    m_texture2->setMinificationFilter(QOpenGLTexture::Nearest);

    m_texture2->generateMipMaps();

    m_texture2->setMagnificationFilter(QOpenGLTexture::Linear);

    m_texture2->setWrapMode(QOpenGLTexture::Repeat);

    blend_percent_ = blend_percent;


    m_modelMatrix.setToIdentity();

}

void SimpleObject3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated())
        return;

    m_texture->bind(0);
    m_texture2->bind(1);
    program->setUniformValue("u_texture",0);
    program->setUniformValue("u_texture2",1);
    program->setUniformValue(" w", float(blend_percent_));
    program->setUniformValue("u_modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;
    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLoc);
    program->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normLoc);
    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT,0);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
    m_texture2->release();

}

void SimpleObject3D::translate(const QVector3D &translateVector)
{
    m_modelMatrix.translate(translateVector);
}

void SimpleObject3D::scale(const QVector3D &scaleVector)
{
    m_modelMatrix.scale(scaleVector);
}


