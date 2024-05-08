FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

USER root

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

# Dependencies to install build dependencies
RUN apt -y update && apt install -y wget software-properties-common

# Build dependencies
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
RUN apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"

RUN apt -y update && apt -y install \
    cmake g++ valgrind \
    tzdata python3-pip lsb-release parallel ninja-build default-jdk

RUN apt -y upgrade cmake g++
RUN apt -y clean all

# Conan stuff
RUN pip install conan
RUN conan profile detect

# Install dependencies
COPY conanfile.py conanfile.py
COPY scripts scripts
COPY ./src/core_server/internal/parsing/ceql_query/autogenerate_script.sh ./src/core_server/internal/parsing/ceql_query/autogenerate_script.sh
COPY ./src/core_server/internal/parsing/stream_declaration/autogenerate_script.sh ./src/core_server/internal/parsing/stream_declaration/autogenerate_script.sh
RUN conan source .
RUN conan install . --build missing -s:h compiler=gcc -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2
