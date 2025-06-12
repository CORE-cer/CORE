# syntax=docker/dockerfile:1

FROM ubuntu:24.04 AS build

USER root

WORKDIR /CORE

RUN apt-get update

RUN apt-get -y upgrade

RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata

# Install the latest version of cmake:
RUN apt remove --purge --auto-remove cmake

RUN apt install -y software-properties-common lsb-release parallel ninja-build valgrind default-jdk python3 python3-pip lsb-release wget sudo pipx git

RUN pipx ensurepath

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
RUN apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
RUN apt update

RUN apt install kitware-archive-keyring -y
RUN rm /etc/apt/trusted.gpg.d/kitware.gpg
RUN sudo apt update -y

RUN apt install cmake -y

RUN python3 -m venv py

ENV PATH="/CORE/py/bin:$PATH"

RUN pip install conan

RUN conan profile detect

RUN conan remote add artifactory https://conan.buzeta.net/artifactory/api/conan/conan-local

RUN wget https://apt.llvm.org/llvm.sh

RUN chmod +x llvm.sh

RUN ./llvm.sh 19

RUN mkdir /clang

RUN ln -s /usr/bin/clang-19 /clang/clang
RUN ln -s /usr/bin/clang++-19 /clang/clang++

RUN which clang-19

# ADD clang to PATH
ENV PATH="/clang:$PATH"

COPY . .

RUN chmod +x /CORE/scripts/*.sh

RUN apt-get clean

RUN scripts/build_and_test.sh -b Debug

RUN scripts/build_and_test.sh -b Release

RUN cp -r python_streamer/coinbase/* /CORE/build/Debug
RUN cp -r python_streamer/coinbase/* /CORE/build/Release


FROM ubuntu:24.04 AS final

# Install minimal runtime dependencies
RUN apt-get update && apt-get install -y \
    bash \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

USER root

WORKDIR /CORE

RUN apt-get update

RUN apt-get -y upgrade

RUN apt install -y python3 python3-pip python3.12-venv

RUN python3 -m venv py

ENV PATH="/CORE/py/bin:$PATH"

COPY --from=build /CORE/requirements.txt /CORE/requirements.txt

RUN pip install -r requirements.txt

COPY --from=build /CORE/build/Debug /CORE/build/Debug
COPY --from=build /CORE/build/Release /CORE/build/Release

# Set default command to bash for interactive terminal
CMD ["/bin/bash"]
