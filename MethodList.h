/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

#ifndef _METHOD_LIST_
#define _METHOD_LIST_

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQmlEngine>
#include <QAbstractListModel>
#include "TypeHandle.h"

class MethodList: public QAbstractListModel
{
    Q_OBJECT
    
    public:
    enum ClassMembership { AllMethods, OwnMethods, InheritedMethods };
    Q_ENUM(ClassMembership)
    
    private:
    const QMetaObject* aMetaObject;
    ClassMembership classMembership;
    
    public:
    MethodList() :aMetaObject(0), classMembership(OwnMethods) {}
    MethodList(const QMetaObject* obj, ClassMembership iMember=OwnMethods) :aMetaObject(obj), classMembership(iMember) {}
    MethodList(const MethodList& other) = default;
    MethodList& operator=(const MethodList& other) =default;
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        if(aMetaObject) {
            switch(classMembership) {
                case AllMethods:
                    return aMetaObject->methodCount();
                break;
                case OwnMethods:
                    return aMetaObject->methodCount() - aMetaObject->methodOffset();
                case InheritedMethods:
                    return aMetaObject->methodOffset();
                break;
            }
        }
        
        return 0;
    }
    
    QHash<int, QByteArray> roleNames() const {
        return QHash{
            std::make_pair(0, QByteArray("name")),
            std::make_pair(1, QByteArray("returnTypeName")),
            std::make_pair(2, QByteArray("signature")),
            std::make_pair(3, QByteArray("access")),
            std::make_pair(4, QByteArray("methodType")),
            std::make_pair(5, QByteArray("header")),
            std::make_pair(6, QByteArray("headerHtml")),
            std::make_pair(7, QByteArray("returnTypeHandle")),
            std::make_pair(8, QByteArray("argumentTypeHandle")),
        };
    }
    
    QVariant data(const QModelIndex &index, int role = 0) const {
        static const char* accessStrings[3] = {"private", "protected", "private"};
        
        if(index.row() < 0 || index.row() >= rowCount()) {
            return QVariant();
        }
        int offset = classMembership == OwnMethods ? aMetaObject->methodOffset() : 0;
        auto method = aMetaObject->method(index.row() + offset);
        switch(role) {
            case 0: //name
                return method.name();
            break;
            case 1: //returnTypeName
                return method.returnMetaType().name();
            break;
            case 2: //signature
                return method.methodSignature();
            break;
            case 3: //access
                return accessStrings[method.access()];
            break;
            case 4: //methodType
                return method.methodType();
            break;
            case 5:   //header
            case 6: { //headerHtml
                bool isHtml = role == 6;
                QByteArray ret;
                bool returnTypeValid = !!method.returnMetaType().metaObject();
                if(isHtml && returnTypeValid) {
                    ret += "<a href=\"returnType\">";
                }
                ret += method.returnMetaType().name();
                if(isHtml && returnTypeValid) {
                    ret += "</a> ";
                } else {
                    ret += " ";
                }
                ret += method.name();
                ret += "(";
                auto parameterNames = method.parameterNames();
                auto parameterTypeNames = method.parameterTypes();
                int count = method.parameterCount();
                for(int i = 0; i < count; i++) {
                    bool argumentTypeValid = !!method.parameterMetaType(i).metaObject();
                    if(isHtml && argumentTypeValid) {
                        ret += "<a href=\"argumentType/";
                        ret += QByteArray::number(i);
                        ret += "\">";
                    }
                    ret += parameterTypeNames[i];
                    if(isHtml && argumentTypeValid) {
                        ret += "</a> ";
                    } else {
                        ret += " ";
                    }
                    ret += parameterNames[i];
                    if(i < count-1) {
                        ret += ", ";
                    }
                }
                ret += ")";
                return ret;
            } break;
            case 7: { //returnTypeHandle
                QVariant ret;
                ret.emplace<TypeHandle>(TypeHandle({method.returnMetaType().metaObject()}));
                return ret;
            } break;
            case 8: { //argumentTypeHandle
                QVariantList ret;
                for(int i = 0; i < method.parameterCount(); ++i) {
                    QVariant variant;
                    variant.emplace<TypeHandle>(TypeHandle({method.parameterMetaType(i).metaObject()}));
                    ret.append(variant);
                }
                return ret;
            } break;
        }
        return QVariant();
    }
};

Q_DECLARE_METATYPE(MethodList);
#endif
