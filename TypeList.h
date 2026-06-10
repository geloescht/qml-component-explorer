/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

#ifndef _TYPE_LIST_
#define _TYPE_LIST_

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQmlEngine>
#include <QAbstractListModel>
#include "MethodList.h"
#include "PropertyList.h"
#include "EnumList.h"

class TypeList: public QAbstractListModel
{
    Q_OBJECT
    
    private:
    std::vector<const QMetaObject*> metaObjects;
    
    public:
    enum PropertyRole { RoleName, RoleNProperties, RolePropertiesPreview, RoleNMethods, RoleMethodsPreview, RoleSuperclassName };
    Q_ENUM(PropertyRole)
    
    
    void addMetaObject(const QMetaObject *metaObject) {
        metaObjects.push_back(metaObject);
        std::printf("[component-explorer] Added MetaObject (%p) with name %s and %i methods\n", metaObject, metaObject->className(), metaObject->methodCount());
        /*
        for(int i = 0; i < metaObject->methodCount(); i++) {
            auto methodSignature = metaObject->method(i).methodSignature();
            std::printf("%s, ", methodSignature.data());
        }
        std::printf("\n");*/
    }
    
    ~TypeList() {
        for(auto iter = metaObjects.begin(), end = metaObjects.end(); iter != end; ++iter) {
            auto metaObject = *iter;
            std::printf("[component-explorer] {%p, \"%s\"}\n", metaObject, metaObject->className());
        }
    }
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        return metaObjects.size();
    }
    
    QHash<int, QByteArray> roleNames() const {
        return QHash{
            std::make_pair((int)RoleName, QByteArray("name")),
            std::make_pair((int)RoleNProperties, QByteArray("nProperties")),
            std::make_pair((int)RolePropertiesPreview, QByteArray("propertiesPreview")),
            std::make_pair((int)RoleNMethods, QByteArray("nMethods")),
            std::make_pair((int)RoleMethodsPreview, QByteArray("methodsPreview")),
            std::make_pair((int)RoleSuperclassName, QByteArray("superclassName"))
        };
    }
    
    QVariant data(const QModelIndex &index, int role = 0) const {
        if(index.row() < 0 || index.row() >= metaObjects.size()) {
            return QVariant();
        }
        auto metaObject = metaObjects.at(index.row());
        switch(role) {
            case 0: //name
                return metaObject->className();
            break;
            case 1: //nProperties
                return metaObject->propertyCount();
            break;
            case 2: //propertiesPreview
                return "not implemented";
            break;
            case 3: //nMethods
                return metaObject->methodCount();
            break;
            case 4: //methodsPreview
                return "not implemented";
            break;
            case RoleSuperclassName:
                auto *superclass = metaObject->superClass();
                if(superclass)
                {
                    return superclass->className();
                } else
                    return "";
            break;
        }
        return QVariant();
    }
    
    Q_INVOKABLE MethodList* methodList(int index, MethodList::ClassMembership membership=MethodList::OwnMethods) const {
        if(index < 0 || index >= metaObjects.size()) {
            return 0;
        }
        return new MethodList(metaObjects[index], membership);
    }
    
    Q_INVOKABLE PropertyList* propertyList(int index, PropertyList::ClassMembership membership=PropertyList::OwnProperties) const {
        if(index < 0 || index >= metaObjects.size()) {
            return 0;
        }
        return new PropertyList(metaObjects[index], membership);
    }
    
    Q_INVOKABLE EnumList* enumList(int index, EnumList::ClassMembership membership=EnumList::OwnEnums) const {
        if(index < 0 || index >= metaObjects.size()) {
            return 0;
        }
        return new EnumList(metaObjects[index], membership);
    }
};
#endif
