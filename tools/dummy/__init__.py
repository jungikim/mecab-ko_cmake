import sys

if sys.platform == "win32":
    import ctypes
    import glob
    import os

    import pkg_resources

    module_name = sys.modules[__name__].__name__
    package_dir = pkg_resources.resource_filename(module_name, "")

    add_dll_directory = getattr(os, "add_dll_directory", None)
    if add_dll_directory is not None:
        add_dll_directory(package_dir)

    for library in glob.glob(os.path.join(package_dir, "*.dll")):
        ctypes.CDLL(library)

from dummy._ext import (
    Dummy
)

d = Dummy()
print(d())