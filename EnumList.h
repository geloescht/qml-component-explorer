/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

#ifndef _ENUM_LIST_
#define _ENUM_LIST_

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQmlEngine>
#include <QAbstractListModel>

class EnumList: public QAbstractListModel
{
    Q_OBJECT
    
    public:
    enum ClassMembership { AllEnums, OwnEnums, InheritedEnums };
    Q_ENUM(ClassMembership)
    
    private:
    const QMetaObject* aMetaObject;
    ClassMembership classMembership;
    
    public:
    EnumList() :aMetaObject(0), classMembership(OwnEnums) {}
    EnumList(const QMetaObject* obj, ClassMembership iMember=OwnEnums) :aMetaObject(obj), classMembership(iMember) {}
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        if(aMetaObject) {
            switch(classMembership) {
                case AllEnums:
                    return aMetaObject->enumeratorCount();
                break;
                case OwnEnums:
                    return aMetaObject->enumeratorCount() - aMetaObject->enumeratorOffset();
                case InheritedEnums:
                    return aMetaObject->enumeratorOffset();
                break;
            }
        }
        
        return 0;
    }
    
    QHash<int, QByteArray> roleNames() const {
        return QHash{
            std::make_pair(0, QByteArray("name")),
            std::make_pair(1, QByteArray("values")),
        };
    }
    
    QVariant data(const QModelIndex &index, int role = 0) const {
        if(index.row() < 0 || index.row() >= rowCount()) {
            return QVariant();
        }
        int offset = classMembership == OwnEnums ? aMetaObject->enumeratorOffset() : 0;
        auto enumerator = aMetaObject->enumerator(index.row() + offset);
        switch(role) {
            case 0: //name
                return enumerator.enumName();
            break;
            case 1: { //values
                QVariantMap ret;
                for(int i = 0; i < enumerator.keyCount(); ++i) {
                    ret.insert(enumerator.key(i), enumerator.value(i));
                }
                return ret;
            }
            break;
        }
        return QVariant();
    }
};

Q_DECLARE_METATYPE(EnumList);
#endif
