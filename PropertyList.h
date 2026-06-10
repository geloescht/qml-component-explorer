/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

#ifndef _PROPERTY_LIST_
#define _PROPERTY_LIST_

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQmlEngine>
#include <QAbstractListModel>

class PropertyList: public QAbstractListModel
{
    Q_OBJECT
    
    public:
    enum ClassMembership { AllProperties, OwnProperties, InheritedProperties };
    Q_ENUM(ClassMembership)
    enum Flags { Readable = 1, Writable = 2, Constant = 4, Stored = 8, EnumType = 16, FlagType = 32, Final = 64, Required = 128, Bindable = 256, Notify = 512 };
    Q_ENUM(Flags)
    
    private:
    const QMetaObject* aMetaObject;
    ClassMembership classMembership;
    
    public:
    PropertyList() :aMetaObject(0), classMembership(OwnProperties) {}
    PropertyList(const QMetaObject* obj, ClassMembership iMember=OwnProperties) :aMetaObject(obj), classMembership(iMember) {}
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        if(aMetaObject) {
            switch(classMembership) {
                case AllProperties:
                    return aMetaObject->propertyCount();
                break;
                case OwnProperties:
                    return aMetaObject->propertyCount() - aMetaObject->propertyOffset();
                case InheritedProperties:
                    return aMetaObject->propertyOffset();
                break;
            }
        }
        
        return 0;
    }
    
    QHash<int, QByteArray> roleNames() const {
        return QHash{
            std::make_pair(0, QByteArray("name")),
            std::make_pair(1, QByteArray("type")),
            std::make_pair(2, QByteArray("flags"))
        };
    }
    
    QVariant data(const QModelIndex &index, int role = 0) const {
        if(index.row() < 0 || index.row() >= rowCount()) {
            return QVariant();
        }
        int offset = classMembership == OwnProperties ? aMetaObject->propertyOffset() : 0;
        auto property = aMetaObject->property(index.row() + offset);
        switch(role) {
            case 0: //name
                return property.name();
            break;
            case 1: //type
                return property.typeName();
            break;
            case 2: //flags
                return property.isReadable() * Readable + property.isWritable() * Writable + property.isConstant() * Constant + property.isStored() * Stored + property.isEnumType() * EnumType + property.isFlagType() * FlagType + property.isFinal() * Final + property.isRequired() * Required + property.isBindable() * Bindable + property.hasNotifySignal() * Notify;
            break;
        }
        return QVariant();
    }
};

Q_DECLARE_METATYPE(PropertyList);
#endif
