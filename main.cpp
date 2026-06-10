/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

#include <qqml.h>
#include "xovi.h"
#include <stdio.h>
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
            typeList->addMetaObject(type->metaObject);
         }
    } else if(regtype == QQmlPrivate::TypeAndRevisionsRegistration) {
        auto type = reinterpret_cast<QQmlPrivate::RegisterTypeAndRevisions*>(data);
        if(type->uri) {
            std::printf("[component-explorer] Registering type and revisions %s\n", type->uri);
        }
        else {
            std::printf("[component-explorer] Registering type and revisions without name\n");
        }
    } else if(regtype == QQmlPrivate::SingletonAndRevisionsRegistration) {
        auto type = reinterpret_cast<QQmlPrivate::RegisterSingletonTypeAndRevisions*>(data);
        if(type->uri) {
            std::printf("[component-explorer] Registering singleton type and revisions %s\n", type->uri);
        }
        else {
            std::printf("[component-explorer] Registering singleton type and revisions without name\n");
        }
    } else if(regtype == QQmlPrivate::SingletonRegistration) {
        auto type = reinterpret_cast<QQmlPrivate::RegisterSingletonType*>(data);
        if(typeList && type->instanceMetaObject) {
            typeList->addMetaObject(type->instanceMetaObject);
        }
    } else {
        std::printf("[component-explorer] Registration of type %i\n", regtype);
    }
    
    int ret = ((int (*)(QQmlPrivate::RegistrationType, void*))$_ZN11QQmlPrivate11qmlregisterENS_16RegistrationTypeEPv)(regtype, data);
    
    std::printf("Registration done with id %i\n", ret);
    
    return ret;
}

extern "C" void _xovi_construct(){
    typeList = std::make_unique<TypeList>();
    std::printf("[component-explorer] Loaded\n");
    qmlRegisterSingletonInstance("net.geloescht.ComponentExplorer", 1, 0, "TypeList", typeList.get());
    qmlRegisterType<MethodList>("net.geloescht.ComponentExplorer", 1, 0, "MethodList");
    qmlRegisterType<PropertyList>("net.geloescht.ComponentExplorer", 1, 0, "PropertyList");
    qmlRegisterType<EnumList>("net.geloescht.ComponentExplorer", 1, 0, "EnumList");
}
