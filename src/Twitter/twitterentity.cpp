#include "twitterentity.h"

TwitterEntity::TwitterEntity(const QJsonObject &jsonObj)
{
    obj_ = jsonObj;
    //Indices
    QJsonArray jsonIndices = obj_.value("indices").toArray();
    foreach (QJsonValue index, jsonIndices) {
        indices_.push_back(index.toInt());
    }

}

TwitterEntity::TwitterEntity(const TwitterEntity &other)
{
    obj_     = other.obj_;
    indices_ = other.indices_;
}

TwitterEntity::~TwitterEntity()
{

}

const QList<int> &TwitterEntity::indices()
{
    return indices_;
}

const QJsonObject &TwitterEntity::jsonObj()
{
    return obj_;
}
