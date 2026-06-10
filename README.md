QML Native Component Explorer (qml-component-explorer)
=============================

This is an extension for the [XOVI framework](https://github.com/asivery/xovi), which builds a QAbstractListModel of all QML components registered from the C++ side. List models for the enumerations, members and methods of these classes can also be retrieved. This allows developers to explore the interface of these QML components inside an application as a convenient reference or when you do not have access to its source code.

An example front-end is also included in the example directory. Its usage depends on the application whose native components you want to inspect.

## Important note ##

Use at your own risk!

This extension uses XOVI's' `$noLock` mechanism, which can affect the stability of the extended application. Although I have not observed explicit crashes due to this extension, depending on your target system and application you might be less lucky! I do not take responsibility for loss of data or functionality of your device.

Please understand that the content of the list models produced by _qml-component-explorere_ are copyrighted by the original application author. Unless the **license** under which you are using the software you are examining **explicitly allows or mandates source code redistribution**, you should not publish or share any program output produced while _qml-component-explorer_ is loaded. That includes the standard output of the application you are loading the extension in, but also manually copying parts of its output. This is a tool for learning and understanding only.

## Building and installing ##

- Run `XOVI_REPO=/path/to/xovi/repository qmake6` to create the Makefile
- Run `make` to build
- Move resulting extension library `component-explorer.so` into the corresponding `extensions.d` directory

How to install the example front-end depends on how you want to use it. At the very least, you will need to compile the application resources:
- Run `/usr/lib/qt6/libexec/rcc example/application.qrc --binary -o example/resources.rcc` to compile resources

## License and acknowledgements ##

_qml-component-explorer_ (but not its output) is licensed under the GNU Public License 3.0.
Many thanks to asivery for writing and making XOVI available.
