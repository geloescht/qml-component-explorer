#ifndef _META_OBJECT_HANDLE_
#define _META_OBJECT_HANDLE_

#include <QObject>
#include <QMetaObject>

struct TypeHandle {
    Q_GADGET
    public:
    const QMetaObject *metaObject;
};

Q_DECLARE_METATYPE(TypeHandle)

#endif
