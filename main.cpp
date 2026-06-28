/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

#ifdef QT_NO_DEBUG
#define QT_NO_DEBUG_OUTPUT
#endif

#include <qqml.h>
#include <QFont>
#include "xovi.h"
#include <QtLogging>
#include "TypeList.h"
#include "MethodList.h"
#include "PropertyList.h"
#include "EnumList.h"

std::unique_ptr<TypeList> typeList;

extern "C" int override$_ZN11QQmlPrivate11qmlregisterENS_16RegistrationTypeEPv(
        QQmlPrivate::RegistrationType regtype, void* data
) {
    
    if(regtype == QQmlPrivate::TypeRegistration) {
        auto type = reinterpret_cast<QQmlPrivate::RegisterType*>(data);
        if(typeList && type->metaObject) {
            TypeList::QmlTypeInfo qmlTypeInfo = {type->uri, type->elementName, type->version, type->revision};
            typeList->addMetaObject(type->metaObject, &qmlTypeInfo);
         }
    } else if(regtype == QQmlPrivate::TypeAndRevisionsRegistration) {
        auto type = reinterpret_cast<QQmlPrivate::RegisterTypeAndRevisions*>(data);
        if(type->uri) {
            qDebug("[component-explorer] Registering type and revisions %s", type->uri);
        }
        else {
            qDebug("[component-explorer] Registering type and revisions without name");
        }
    } else if(regtype == QQmlPrivate::SingletonAndRevisionsRegistration) {
        auto type = reinterpret_cast<QQmlPrivate::RegisterSingletonTypeAndRevisions*>(data);
        if(type->uri) {
            qDebug("[component-explorer] Registering singleton type and revisions %s", type->uri);
        }
        else {
            qDebug("[component-explorer] Registering singleton type and revisions without name");
        }
    } else if(regtype == QQmlPrivate::SingletonRegistration) {
        auto type = reinterpret_cast<QQmlPrivate::RegisterSingletonType*>(data);
        if(typeList && type->instanceMetaObject) {
            TypeList::QmlTypeInfo qmlTypeInfo = {type->uri, type->typeName, type->version, type->revision};
            typeList->addMetaObject(type->instanceMetaObject, &qmlTypeInfo, TypeList::SingletonType);
        }
    } else {
        qDebug("[component-explorer] Unhandled registration of type %i", regtype);
    }
    
    int ret = ((int (*)(QQmlPrivate::RegistrationType, void*))$_ZN11QQmlPrivate11qmlregisterENS_16RegistrationTypeEPv)(regtype, data);
    
    qDebug("Registration done with id %i", ret);
    
    return ret;
}

extern "C" void _xovi_construct(){
    typeList = std::make_unique<TypeList>();
    qInfo("[component-explorer] Loaded");
    qmlRegisterSingletonInstance("net.geloescht.ComponentExplorer", 1, 0, "TypeList", typeList.get());
    qmlRegisterType<MethodList>("net.geloescht.ComponentExplorer", 1, 0, "MethodList");
    qmlRegisterType<PropertyList>("net.geloescht.ComponentExplorer", 1, 0, "PropertyList");
    qmlRegisterType<EnumList>("net.geloescht.ComponentExplorer", 1, 0, "EnumList");

    QFont::insertSubstitution("monospace", "Noto Mono");
    QFont::insertSubstitution("monospace", "Noto Sans Mono");
}
