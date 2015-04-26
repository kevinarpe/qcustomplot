# Import the pyqtconfig.py for the correct python3 version and normal or debug
# build.

import sys

if sys.version_info[1] == 4:
    if 'd' in sys.abiflags:
        try:
            from Py3Qt5.pyqtconfig_d4 import *
            from Py3Qt5.pyqtconfig_d4 import _pkg_config, _default_macros
        except ImportError as msg:
            raise ImportError('No module named pyqtconfig; package python3-pyqt5-dbg not installed')
    else:
        from Py3Qt5.pyqtconfig_nd4 import *
        from Py3Qt5.pyqtconfig_nd4 import _pkg_config, _default_macros

