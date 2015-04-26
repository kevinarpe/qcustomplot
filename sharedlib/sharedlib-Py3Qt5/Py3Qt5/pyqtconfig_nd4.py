# Copyright (c) 2014 Riverbank Computing Limited <info@riverbankcomputing.com>
# 
# This file is part of PyQt.
# 
# This file may be used under the terms of the GNU General Public
# License versions 2.0 or 3.0 as published by the Free Software
# Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file.  Alternatively you may (at
# your option) use any later version of the GNU General Public
# License if such license has been publicly approved by Riverbank
# Computing Limited (or its successors, if any) and the KDE Free Qt
# Foundation. In addition, as a special exception, Riverbank gives you
# certain additional rights. These rights are described in the Riverbank
# GPL Exception version 1.1, which can be found in the file
# GPL_EXCEPTION.txt in this package.
# 
# If you are unsure which license is appropriate for your use, please
# contact the sales department at sales@riverbankcomputing.com.
# 
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# This module is intended to be used by the configuration scripts of extension
# modules that %Import PyQt5 modules.


import sipconfig

# Note: This file was hell to update.  I don't know why this file is now excluded from PyQt5.
# It makes building Python bindings much simpler for PyQt4.
# Original Ref: /usr/lib/python3/dist-packages/PyQt4/pyqtconfig_nd4.py

# These are installation specific values created when PyQt5 was configured.
_pkg_config = {
#    'pyqt_bin_dir':      '/usr/bin',
    'pyqt_bin_dir':      '/usr/lib/x86_64-linux-gnu/qt5/bin',
#    'pyqt_config_args':  '--confirm-license --verbose -q /usr/bin/qmake-qt4 -c -j 10 LIBDIR_QT=/usr/lib STRIP= MOC=moc-qt4 LIBS_OPENGL= LIBS_X11= LIBS_THREAD= CFLAGS= -g -O2 -fstack-protector-strong -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2 CXXFLAGS= -g -O2 -fstack-protector-strong -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2 LFLAGS= -Wl,-z,relro -Wl,-O1 -m /usr/lib/python3.4/config-3.4m-x86_64-linux-gnu -d /usr/lib/python3.4/dist-packages --dbus /usr/include/dbus-1.0 --no-designer-plugin',
    'pyqt_config_args':  '--confirm-license --verbose -q /usr/lib/x86_64-linux-gnu/qt5/bin/qmake -c -j 10 LIBDIR_QT=/usr/lib/x86_64-linux-gnu STRIP= MOC=/usr/lib/x86_64-linux-gnu/qt5/bin/moc LIBS_OPENGL= LIBS_X11= LIBS_THREAD= CFLAGS= -g -O2 -fstack-protector-strong -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2 CXXFLAGS= -g -O2 -fstack-protector-strong -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2 LFLAGS= -Wl,-z,relro -Wl,-O1 -m /usr/lib/python3.4/config-3.4m-x86_64-linux-gnu -d /usr/lib/python3.4/dist-packages --dbus /usr/include/dbus-1.0 --no-designer-plugin',
#    'pyqt_mod_dir':      '/usr/lib/python3.4/dist-packages/PyQt4',
    'pyqt_mod_dir':      '/usr/lib/python3/dist-packages/PyQt5',
#    'pyqt_modules':      'QtCore QtGui QtHelp QtNetwork QtDBus QtDeclarative QtScript QtScriptTools QtXml QtOpenGL QtSql QtSvg QtTest QtWebKit QtXmlPatterns phonon QtAssistant QtDesigner',
# $ find  /usr/share/sip/PyQt5 -name '*mod.sip' -exec dirname {} \; | xargs -l -i basename {}  | sort
    'pyqt_modules':      'Enginio QAxContainer _QOpenGLFunctions_2_0 _QOpenGLFunctions_ES2 QtBluetooth QtCore QtDBus QtDesigner QtGui QtHelp QtMacExtras QtMultimedia QtMultimediaWidgets QtNetwork QtOpenGL QtPositioning QtPrintSupport QtQml QtQuick QtQuickWidgets QtSensors QtSerialPort QtSql QtSvg QtTest QtWebKit QtWebKitWidgets QtWebSockets QtWidgets QtWinExtras QtX11Extras QtXmlPatterns',
#    'pyqt_sip_dir':      '/usr/share/sip/PyQt4',
    'pyqt_sip_dir':      '/usr/share/sip/PyQt5',
#    'pyqt_sip_flags':    '-x VendorID -t WS_X11 -x PyQt_NoPrintRangeBug -t Qt_4_8_6 -g',
# >>> import PyQt5.QtCore
# >>> PyQt5.QtCore.PYQT_CONFIGURATION['sip_flags']
# '-t WS_X11 -t Qt_5_3_2'
    'pyqt_sip_flags':    '-t WS_X11 -t Qt_5_3_2',
#    'pyqt_version':      0x040b02,
    'pyqt_version':      0x050302,
#    'pyqt_version_str':  '4.11.2',
    'pyqt_version_str':  '5.3.2',
#    'qt_archdata_dir':   '/usr/share/qt4',
    'qt_archdata_dir':   '/usr/share/qt5',
#    'qt_data_dir':       '/usr/share/qt4',
    'qt_data_dir':       '/usr/share/qt5',
    'qt_dir':            '/usr',
    'qt_edition':        'free',
    'qt_framework':      0,
#    'qt_inc_dir':        '/usr/include/qt4',
    'qt_inc_dir':        '/usr/include/x86_64-linux-gnu/qt5',
#    'qt_inc_dir':        '/usr/include/qt4',
    'qt_inc_dir':        '/usr/include/x86_64-linux-gnu/qt5',
    'qt_lib_dir':        '/usr/lib/x86_64-linux-gnu',
    'qt_threaded':       1,
#    'qt_version':        0x040806,
    'qt_version':        0x050302,
    'qt_winconfig':      'shared'
}

_default_macros = {
    'AIX_SHLIB':                '',
    'AR':                       'ar cqs',
    'CC':                       'gcc',
    'CFLAGS':                   ' -g -O2 -fstack-protector-strong -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2',
    'CFLAGS_APP':               '',
    'CFLAGS_CONSOLE':           '',
    'CFLAGS_DEBUG':             '-g',
    'CFLAGS_EXCEPTIONS_OFF':    '',
    'CFLAGS_EXCEPTIONS_ON':     '',
    'CFLAGS_MT':                '',
    'CFLAGS_MT_DBG':            '',
    'CFLAGS_MT_DLL':            '',
    'CFLAGS_MT_DLLDBG':         '',
    'CFLAGS_RELEASE':           '-O2',
    'CFLAGS_RTTI_OFF':          '',
    'CFLAGS_RTTI_ON':           '',
    'CFLAGS_SHLIB':             '-fPIC',
    'CFLAGS_STL_OFF':           '',
    'CFLAGS_STL_ON':            '',
    'CFLAGS_THREAD':            '-D_REENTRANT',
    'CFLAGS_WARN_OFF':          '-w',
    'CFLAGS_WARN_ON':           '-Wall -W',
    'CHK_DIR_EXISTS':           'test -d',
    'CONFIG':                   'qt warn_on release incremental link_prl gdb_dwarf_index',
    'COPY':                     'cp -f',
    'CXX':                      'g++',
    'CXXFLAGS':                 ' -g -O2 -fstack-protector-strong -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2',
    'CXXFLAGS_APP':             '',
    'CXXFLAGS_CONSOLE':         '',
    'CXXFLAGS_DEBUG':           '-g',
    'CXXFLAGS_EXCEPTIONS_OFF':  '',
    'CXXFLAGS_EXCEPTIONS_ON':   '',
    'CXXFLAGS_MT':              '',
    'CXXFLAGS_MT_DBG':          '',
    'CXXFLAGS_MT_DLL':          '',
    'CXXFLAGS_MT_DLLDBG':       '',
    'CXXFLAGS_RELEASE':         '-O2',
    'CXXFLAGS_RTTI_OFF':        '',
    'CXXFLAGS_RTTI_ON':         '',
    'CXXFLAGS_SHLIB':           '-fPIC',
    'CXXFLAGS_STL_OFF':         '',
    'CXXFLAGS_STL_ON':          '',
    'CXXFLAGS_THREAD':          '-D_REENTRANT',
    'CXXFLAGS_WARN_OFF':        '-w',
    'CXXFLAGS_WARN_ON':         '-Wall -W',
    'DEFINES':                  '',
    'DEL_FILE':                 'rm -f',
    'EXTENSION_PLUGIN':         '',
    'EXTENSION_SHLIB':          '',
    'INCDIR':                   '',
    'INCDIR_OPENGL':            '/usr/X11R6/include',
#    'INCDIR_QT':                '/usr/include/qt4',
    'INCDIR_QT':                '/usr/include/x86_64-linux-gnu/qt5',
    'INCDIR_X11':               '/usr/X11R6/include',
    'LFLAGS':                   ' -Wl,-z,relro -Wl,-O1',
    'LFLAGS_CONSOLE':           '',
    'LFLAGS_CONSOLE_DLL':       '',
    'LFLAGS_DEBUG':             '',
    'LFLAGS_DLL':               '',
    'LFLAGS_OPENGL':            '',
    'LFLAGS_PLUGIN':            '-shared',
    'LFLAGS_RELEASE':           ' -Wl,-O1',
    'LFLAGS_RPATH':             '-Wl,-rpath,',
    'LFLAGS_SHLIB':             '-shared',
    'LFLAGS_SONAME':            '-Wl,-soname,',
    'LFLAGS_THREAD':            '',
    'LFLAGS_WINDOWS':           '',
    'LFLAGS_WINDOWS_DLL':       '',
    'LIB':                      '',
    'LIBDIR':                   '',
    'LIBDIR_OPENGL':            '/usr/X11R6/lib',
    'LIBDIR_QT':                '/usr/lib',
    'LIBDIR_X11':               '/usr/X11R6/lib',
    'LIBS':                     '',
    'LIBS_CONSOLE':             '',
    'LIBS_CORE':                '',
    'LIBS_GUI':                 '',
    'LIBS_NETWORK':             '',
    'LIBS_OPENGL':              '',
    'LIBS_RT':                  '',
    'LIBS_RTMT':                '',
    'LIBS_THREAD':              '',
    'LIBS_WEBKIT':              '',
    'LIBS_WINDOWS':             '',
    'LIBS_X11':                 '',
    'LINK':                     'g++',
    'LINK_SHLIB':               'g++',
    'LINK_SHLIB_CMD':           '',
    'MAKEFILE_GENERATOR':       'UNIX',
    'MKDIR':                    'mkdir -p',
#    'MOC':                      'moc-qt4',
    'MOC':                      '/usr/lib/x86_64-linux-gnu/qt5/bin/moc',
    'RANLIB':                   '',
    'RPATH':                    '',
    'STRIP':                    ''
}


class Configuration(sipconfig.Configuration):
    """The class that represents PyQt configuration values.
    """
    def __init__(self, sub_cfg=None):
        """Initialise an instance of the class.

        sub_cfg is the list of sub-class configurations.  It should be None
        when called normally.
        """
        if sub_cfg:
            cfg = sub_cfg
        else:
            cfg = []

        cfg.append(_pkg_config)

        sipconfig.Configuration.__init__(self, cfg)


class QtCoreModuleMakefile(sipconfig.SIPModuleMakefile):
    """The Makefile class for modules that %Import QtCore.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore"]

        sipconfig.SIPModuleMakefile.__init__(self, *args, **kw)


class QtGuiModuleMakefile(QtCoreModuleMakefile):
    """The Makefile class for modules that %Import QtGui.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui"]

        QtCoreModuleMakefile.__init__(self, *args, **kw)


class QtHelpModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QtHelp.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtHelp"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtMultimediaModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QtMultimedia.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtMultimedia"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtNetworkModuleMakefile(QtCoreModuleMakefile):
    """The Makefile class for modules that %Import QtNetwork.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtNetwork"]

        QtCoreModuleMakefile.__init__(self, *args, **kw)


class QtDeclarativeModuleMakefile(QtNetworkModuleMakefile):
    """The Makefile class for modules that %Import QtDeclarative.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtNetwork", "QtDeclarative"]

        QtNetworkModuleMakefile.__init__(self, *args, **kw)


class QtAssistantModuleMakefile(QtNetworkModuleMakefile):
    """The Makefile class for modules that %Import QtAssistant.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtNetwork", "QtAssistant"]

        QtNetworkModuleMakefile.__init__(self, *args, **kw)


class QtOpenGLModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QtOpenGL.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        kw["opengl"] = 1

        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtOpenGL"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtScriptModuleMakefile(QtCoreModuleMakefile):
    """The Makefile class for modules that %Import QtScript.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtScript"]

        QtCoreModuleMakefile.__init__(self, *args, **kw)


class QtScriptToolsModuleMakefile(QtScriptModuleMakefile):
    """The Makefile class for modules that %Import QtScriptTools.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtScript", "QtScriptTools"]

        QtScriptModuleMakefile.__init__(self, *args, **kw)


class QtSqlModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QtSql.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtSql"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtSvgModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QtSvg.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtSvg"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtTestModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QtTest.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtTest"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtWebKitModuleMakefile(QtNetworkModuleMakefile):
    """The Makefile class for modules that %Import QtWebKit.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtNetwork", "QtWebKit"]

        QtNetworkModuleMakefile.__init__(self, *args, **kw)


class QtXmlModuleMakefile(QtCoreModuleMakefile):
    """The Makefile class for modules that %Import QtXml.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtXml"]

        QtCoreModuleMakefile.__init__(self, *args, **kw)


class QtXmlPatternsModuleMakefile(QtCoreModuleMakefile):
    """The Makefile class for modules that %Import QtXmlPatterns.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtXmlPatterns"]

        QtCoreModuleMakefile.__init__(self, *args, **kw)


class phononModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import phonon.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "phonon"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtDesignerModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QtDesigner.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QtDesigner"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QAxContainerModuleMakefile(QtGuiModuleMakefile):
    """The Makefile class for modules that %Import QAxContainer.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtGui", "QAxContainer"]

        QtGuiModuleMakefile.__init__(self, *args, **kw)


class QtDBusModuleMakefile(QtCoreModuleMakefile):
    """The Makefile class for modules that %Import QtDBus.
    """
    def __init__(self, *args, **kw):
        """Initialise an instance of a module Makefile.
        """
        if "qt" not in kw:
            kw["qt"] = ["QtCore", "QtDBus"]

        QtCoreModuleMakefile.__init__(self, *args, **kw)

