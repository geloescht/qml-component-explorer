/*
qml-component-explorer, a tool for listing native QML components and their attributes
    Copyright (C) 2026  Lukas Winter
 is licensed under GPL-3.0 (see LICENSE file)
*/

#ifndef _TYPE_LIST_
#define _TYPE_LIST_

#ifdef QT_NO_DEBUG
#define QT_NO_DEBUG_OUTPUT
#endif

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQmlEngine>
#include <QList>
#include <QBitArray>
#include <QtLogging>
#include <QAbstractListModel>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/ranked_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include "MethodList.h"
#include "PropertyList.h"
#include "EnumList.h"
#include "TypeHandle.h"

using namespace boost::multi_index;

class TypeList: public QAbstractListModel
{
    Q_OBJECT
    
    private:
    struct ClassName{};

    struct TypeInfo {
        const QMetaObject* metaObject;

        const QByteArrayView className() const {
            return QByteArrayView(metaObject->className());
        }

        bool operator<(const TypeInfo& other) const {
            return metaObject < other.metaObject;
        }
    };

    typedef multi_index_container<
        TypeInfo,
        indexed_by<
            ordered_unique<member<TypeInfo, const QMetaObject*, &TypeInfo::metaObject> >,
            ranked_non_unique<tag<ClassName>, const_mem_fun<TypeInfo, const QByteArrayView, &TypeInfo::className> >
        >
    > TypeInfoContainer;
    TypeInfoContainer metaObjects;
    QList<int> filterMaskCache;
    
    public:
    enum PropertyRole { RoleHandle, RoleName, RoleNProperties, RolePropertiesPreview, RoleNMethods, RoleMethodsPreview, RoleSuperclassHandle, RoleSuperclassName, RoleFilterMatch };
    Q_ENUM(PropertyRole)
    enum FilterFlag { FilterName = 1, FilterProperties = 2, FilterMethods = 4, FilterEnums = 8, FilterCaseInsensitive = 16 };
    Q_ENUM(FilterFlag)
    Q_DECLARE_FLAGS(FilterFlags, FilterFlag)
    
    
    int addMetaObject(const QMetaObject *metaObject) {
        auto findIter = metaObjects.get<0>().find(metaObject);
        if(findIter != metaObjects.get<0>().end() || metaObject == NULL) {
            return -1;
        }
        
        int index = metaObjects.get<ClassName>().lower_bound_rank(metaObject->className());
        beginInsertRows(QModelIndex(), index, index);
        TypeInfo typeInfo = {metaObject};
        metaObjects.get<0>().insert(typeInfo);
        qDebug("[component-explorer] Added MetaObject (%p) with name %s and %i methods", metaObject, metaObject->className(), metaObject->methodCount());
        endInsertRows();
        
        return index;
    }
    
    Q_INVOKABLE int addType(const TypeHandle type) {
        return addMetaObject(type.metaObject);
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
            std::make_pair((int)RoleSuperclassHandle, QByteArray("superclassHandle")),
            std::make_pair((int)RoleSuperclassName, QByteArray("superclassName")),
            std::make_pair((int)RoleFilterMatch, QByteArray("filterMatch"))
        };
    }
    
    QVariant data(const QModelIndex &index, int role = 0) const {
        if(index.row() < 0 || index.row() >= metaObjects.size()) {
            return QVariant();
        }
        auto metaObject = metaObjects.get<ClassName>().nth(index.row())->metaObject;
        switch(role) {
            case RoleHandle: {
                QVariant ret;
                ret.emplace<TypeHandle>(TypeHandle({metaObject}));
                return ret;
            } break;
            case RoleName:
                return metaObject->className();
            break;
            case RoleNProperties:
                return metaObject->propertyCount();
            break;
            case RolePropertiesPreview:
                return "not implemented";
            break;
            case RoleNMethods:
                return metaObject->methodCount();
            break;
            case RoleMethodsPreview:
                return "not implemented";
            break;
            case RoleSuperclassHandle: {
                auto *superclass = metaObject->superClass();
                QVariant ret;
                if(superclass) {
                    ret.emplace<TypeHandle>(TypeHandle({superclass}));
                }
                return ret;
            } break;
            case RoleSuperclassName: {
                auto *superclass = metaObject->superClass();
                if(superclass)
                {
                    return superclass->className();
                } else
                    return "";
            } break;
            case RoleFilterMatch:
                if(index.row() < filterMaskCache.length()) {
                    return filterMaskCache[index.row()];
                }
                else
                    return 1;
            break;
        }
        return QVariant();
    }
    
    Q_INVOKABLE MethodList* methodList(int index, MethodList::ClassMembership membership=MethodList::OwnMethods) const {
        if(index < 0 || index >= metaObjects.size()) {
            return 0;
        }
        return new MethodList(metaObjects.get<ClassName>().nth(index)->metaObject, membership);
    }
    
    Q_INVOKABLE PropertyList* propertyList(int index, PropertyList::ClassMembership membership=PropertyList::OwnProperties) const {
        if(index < 0 || index >= metaObjects.size()) {
            return 0;
        }
        return new PropertyList(metaObjects.get<ClassName>().nth(index)->metaObject, membership);
    }
    
    Q_INVOKABLE EnumList* enumList(int index, EnumList::ClassMembership membership=EnumList::OwnEnums) const {
        if(index < 0 || index >= metaObjects.size()) {
            return 0;
        }
        return new EnumList(metaObjects.get<ClassName>().nth(index)->metaObject, membership);
    }
    
    Q_INVOKABLE int indexOf(TypeHandle handle) {
        auto findIter = metaObjects.get<0>().find(handle.metaObject);
        auto tellIter = metaObjects.project<ClassName>(findIter);
        if(tellIter != metaObjects.get<ClassName>().end()) {
            return static_cast<int>(metaObjects.get<ClassName>().rank(tellIter));
        }
        return -1;
    }

    Q_INVOKABLE void setFilter(const QList<QByteArray>& filterTerms, FilterFlags filterItems = FilterName) {
        qInfo("[component-explorer] filtering...");

        QList<QByteArray> mandatoryTerms, excludedTerms, alternativeTerms;
        for (auto& termUntrimmed : filterTerms) {
            auto term = termUntrimmed.trimmed();
            if(term.length() < 1)
                continue;
            
            if(term.startsWith("+")) {
                mandatoryTerms.append(term.sliced(1));
                qDebug("mandatory %s, ", term.sliced(1).constData());
            }
            else if(term.startsWith("-")) {
                excludedTerms.append(term.sliced(1));
                qDebug("excluded %s, ", term.sliced(1).constData());
            } else {
                alternativeTerms.append(term);
                qDebug("alternative %s, ", term.constData());
            }
        }
        
        emit layoutAboutToBeChanged();
        
        if(mandatoryTerms.isEmpty() && excludedTerms.isEmpty() && alternativeTerms.isEmpty()) {
            filterMaskCache.fill(1);
            emit dataChanged(index(0, 0), index(metaObjects.size(), 0));
            emit layoutChanged();
            return;
        }
        
        filterMaskCache.resize(metaObjects.size());
        
        QList<QByteArrayView> haystack;
        QBitArray alternativeFound(alternativeTerms.length());
        
        int i = 0;
        for (auto& typeInfo : metaObjects.get<ClassName>()) {
            const QMetaObject* metaObject = typeInfo.metaObject;
            
            // collect all the strings to be examined in haystack
            haystack.clear();
            if(filterItems & FilterName) {
                haystack.append(metaObject->className());
            }
            for(int i = 0; filterItems & FilterProperties && i < metaObject->propertyCount(); ++i) {
                haystack.append(metaObject->property(i).name());
            }
            /*for(int i = 0; filterItems & FilterMethods && i < metaObject->methodCount(); ++i) {
                haystack.append(metaObject->method(i).nameView());
            }*/
            for(int i = 0; filterItems & FilterEnums && i < metaObject->enumeratorCount(); ++i) {
                haystack.append(metaObject->enumerator(i).name());
            }
            
            bool haveHope = true;
            alternativeFound.fill(false);
            
            for (auto& term : excludedTerms) {
                for(auto& toBeFiltered : haystack) {
                    if(toBeFiltered.contains(term)) {
                        haveHope = false;
                        break;
                    }
                }
                if(!haveHope)
                    break;
            }
            
            if(haveHope) {
                for (int i = 0; i < mandatoryTerms.length(); i++) {
                    auto term = mandatoryTerms[i];
                    bool found = false;
                    for(auto& toBeFiltered : haystack) {
                        if(toBeFiltered.contains(term)) {
                            found = true;
                            break;
                        }
                    }
                    if(!found) {
                        haveHope = false;
                        break;
                    }
                }
            }
            
            if(haveHope) {
                for (int i = 0; i < alternativeTerms.length(); i++) {
                    auto term = alternativeTerms[i];
                    for(auto& toBeFiltered : haystack) {
                        if(toBeFiltered.contains(term)) {
                            alternativeFound.setBit(i);
                            break;
                        }
                    }
                }

                if(alternativeTerms.length() > 0)
                  filterMaskCache[i] = alternativeFound.count(true);
                else
                  filterMaskCache[i] = mandatoryTerms.length() + excludedTerms.length();
            } else {
                // giving up
                filterMaskCache[i] = 0;
            }

            ++i;
        }
        
        qInfo("[component-explorer] filtering done.");
        emit dataChanged(index(0, 0), index(metaObjects.size(), 0));
        emit layoutChanged();
    }
};
#endif
