import os

from skbuild_conan import setup
from setuptools import find_packages

PROJECT_NAME = "pycore"
ROOT_DIR = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(ROOT_DIR, "python/pycore/README.md"), "r") as f:
    LONG_DESCRIPTION = f.read()


def get_version():
    with open(os.path.join(ROOT_DIR, "python/pycore/__init__.py")) as f:
        for line in f:
            if line.startswith("__version__"):
                delim = '"' if '"' in line else "'"
                return line.split(delim)[1]
        else:
            raise RuntimeError("Unable to find version string.")


setup(
    name=PROJECT_NAME,
    version=get_version(),
    packages=find_packages("src"),
    conan_requirements = [
        "antlr4-cppruntime/[4.12.0]",
        "catch2/[3.7.1]",
        "cppzmq/[4.9.0]",
        "cereal/[1.3.2]",
        "libpqxx/[7.9.2]",
        "gmp/[6.3.0]",
        "re2/[20230602]",
        "quill/[3.7.0]",
        "readerwriterqueue/[1.0.6]",
        "uwebsockets/[20.70.0]",
        "glaze/[4.0.1]",
        "pybind11/[2.13.5]",
        "argparse/[3.1]"
    ],

    description=(
        "Python bindings for CORE, an engine designed for the efficient evaluation of complex event queries"
        " over large data streams in real time."
    ),
    long_description=LONG_DESCRIPTION,
    long_description_content_type="text/markdown",
    url="https://rematch.cl/",
    conan_generators="cmake_find_package", 
    author="Kyle Bossonney, Nicolás Buzeta, Vicente Calisto, Juan-Eduardo López, Cristian Riveros, Stijn Vansummeren",
    author_email=(
        "kyle@bossonney.com, nicolas.buzeta@uc.cl, vecalisto@uc.cl, juan.lpez@uc.cl, cristian.riveros@uc.cl, stijn.vansummeren@uhasselt.be"
    ),
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
    ],
    keywords=(
        "complex events, core, streams, automaton, query evaluation"
    ),
    license="MIT",
    python_requires=">=3.9",
    package_dir={"": "python"},
    cmake_install_dir=f"{PROJECT_NAME}",
    build_options={
        'CMAKE_BUILD_PARALLEL_LEVEL': 6,
    },
)