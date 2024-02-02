#!/usr/bin/env python3
# coding: UTF-8

import sys

import pybind11
from setuptools import Extension, setup

include_dirs = [pybind11.get_include()]
library_dirs = []
cflags = ["-std=c++17", "-fvisibility=hidden"]
ldflags = []
if sys.platform == "darwin":
    cflags.append("-mmacosx-version-min=10.14")
    ldflags.append("-Wl,-rpath,/usr/local/lib")
elif sys.platform == "win32":
    cflags = ["/std:c++17", "/d2FH4-"]

dummy_module = Extension(
    "dummy._ext",
    sources=["dummy/Python.cc"],
    extra_compile_args=cflags,
    extra_link_args=ldflags,
    include_dirs=include_dirs,
    library_dirs=library_dirs,
)

setup(
    name="dummy",
    version="0.0.1",
    description="dummy package",
    packages=["dummy"],
    python_requires=">=3.6",
    ext_modules=[dummy_module],
)
