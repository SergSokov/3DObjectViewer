#include "material.h"

Material::Material()
{

}

void Material::setName(const QString &mtlName)
{
    m_mtlName = mtlName;
}

const QString &Material::mtlName() const
{
    return m_mtlName;
}

void Material::setDiffuseColor(const QVector3D &diffuseColor)
{
    m_diffuseColor = diffuseColor;
}

const QVector3D &Material::diffuseColor() const
{
    return m_diffuseColor;
}

void Material::setAmbienceColor(const QVector3D &ambienceColor)
{
    m_ambienceColor = ambienceColor;
}

const QVector3D &Material::ambienceColor() const
{
    return m_ambienceColor;
}

void Material::setSpecularColor(const QVector3D &specularColor)
{
    m_specularColor = specularColor;
}

const QVector3D &Material::specularColor() const
{
    return  m_specularColor;
}

void Material::setShinnes(const qreal &shinnes)
{
    m_shinnes = shinnes;
}

const qreal &Material::shinnes() const
{
    return m_shinnes;
}

void Material::setDiffuseMap(const QString &path)
{
    m_diffuseMap = QImage(path);
}

const QImage &Material::diffuseMap() const
{
    return  m_diffuseMap;
}
