#include "materiallibrary.h"
#include "material.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
MaterialLibrary::MaterialLibrary()
{

}

void MaterialLibrary::addMaterial(Material *material)
{
    if(!material)
        return;

    for(int i = 0; i < m_materials.size(); ++i)
        if(m_materials[i] == material)
            return;
    m_materials << material;
}

Material *MaterialLibrary::getMaterial(quint32 index)
{
    if(index < (quint32)m_materials.size())
        return m_materials[index];
    else
        return 0;
}

Material *MaterialLibrary::getMaterial(const QString &mtlName)
{
    for(auto material : m_materials)
        if(material->mtlName() == mtlName)
            return material;

    return 0;
}

quint32 MaterialLibrary::count()
{
    return m_materials.size();
}

void MaterialLibrary::loadMaterialFromFile(const QString &path)
{

    QFile m_mtlFile(path);
    if(!m_mtlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cant open mtl file " + path;
        return;
    }

    for(int i = 0; i < m_materials.size(); ++i)
    {
        delete m_materials[i];
    }
    m_materials.clear();

    QTextStream m_inputStream(&m_mtlFile);
    QFileInfo fileInfo(path);

    Material* newMtl = 0;
    while(!m_inputStream.atEnd())
    {
        QString str = m_inputStream.readLine();

        QStringList list = str.split(" ");
        if(list.isEmpty())
            continue;
        QString type = list[0];
        if(type == "#")
        {
            continue;
        }
        else if(type == "newmtl")
        {
            addMaterial(newMtl);
            newMtl = new Material;
            newMtl->setName(list[1]);
        }
        else if(type == "Ns")
        {
            newMtl->setShinnes(list[1].toFloat());
            continue;
        }
        else if(type == "Ka")
            newMtl->setAmbienceColor(QVector3D(list[1].toFloat(),
                                     list[2].toFloat(),
                                     list[3].toFloat()));
        else if(type == "Kd")
            newMtl->setDiffuseColor(QVector3D(list[1].toFloat(),
                                     list[2].toFloat(),
                                     list[3].toFloat()));
        else if(type == "Ks")
            newMtl->setSpecularColor(QVector3D(list[1].toFloat(),
                                     list[2].toFloat(),
                                     list[3].toFloat()));
        else if(type == "map_Kd")
        {

            newMtl->setDiffuseMap(fileInfo.absolutePath() + "/" +list[1]);
        }


    }

    if(newMtl)
        addMaterial(newMtl);

    m_mtlFile.close();

}

