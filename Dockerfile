# syntax=docker/dockerfile:1

FROM ubuntu:24.04 AS build

USER root

WORKDIR /CORE

RUN apt-get update

RUN apt-get -y upgrade

RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata

# Install the latest version of cmake:
RUN apt remove --purge --auto-remove cmake

RUN apt-get update && apt install -y software-properties-common lsb-release parallel ninja-build valgrind default-jdk python3 python3-dev lsb-release wget sudo git build-essential curl gnupg zip unzip tar pkg-config autoconf automake libtool

COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
RUN apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
RUN apt update

RUN apt install kitware-archive-keyring -y
RUN rm /etc/apt/trusted.gpg.d/kitware.gpg
RUN sudo apt update -y

RUN apt install cmake -y

RUN install -d -m 0755 /etc/apt/keyrings \
    && curl --proto '=https' --tlsv1.2 --fail --silent --show-error https://apt.llvm.org/llvm-snapshot.gpg.key -o /tmp/llvm-apt.key \
    && echo "8b2a587ffd672c4687e7581dad4b2f6c1bb2ad6b480cd9771ba2ff48e0b8c75d  /tmp/llvm-apt.key" | sha256sum --check - \
    && test "$(gpg --show-keys --with-colons /tmp/llvm-apt.key | awk -F: '$1 == "fpr" { print $10; exit }')" = "6084F3CF814B57C1CF12EFD515CF4D18AF4F7421" \
    && gpg --dearmor --yes --output /etc/apt/keyrings/llvm-apt.gpg /tmp/llvm-apt.key \
    && rm /tmp/llvm-apt.key

RUN printf '%s\n' \
    'Types: deb' \
    'URIs: https://apt.llvm.org/noble/' \
    'Suites: llvm-toolchain-noble-19' \
    'Components: main' \
    'Architectures: amd64' \
    'Signed-By: /etc/apt/keyrings/llvm-apt.gpg' > /etc/apt/sources.list.d/llvm.sources

RUN apt-get update && apt-get install -y clang-19 clang-format-19 clang-tidy-19

RUN mkdir /clang

RUN ln -s /usr/bin/clang-19 /clang/clang
RUN ln -s /usr/bin/clang++-19 /clang/clang++

RUN which clang-19

# ADD clang to PATH
ENV PATH="/clang:$PATH"

COPY . .

RUN chmod +x /CORE/scripts/*.sh

RUN apt-get clean

RUN scripts/install_dependencies.sh

RUN scripts/build_and_test.sh -b Debug

RUN scripts/build_and_test.sh -b Release

# Build Python wheel
RUN uv build --wheel

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

RUN apt install -y python3 python3-dev

COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/

COPY --from=build /CORE/.python-version /CORE/.python-version
COPY --from=build /CORE/build/Debug /CORE/build/Debug
COPY --from=build /CORE/build/Release /CORE/build/Release
COPY --from=build /CORE/dist/*.whl /tmp/

RUN uv venv /CORE/.venv && uv pip install --python /CORE/.venv/bin/python /tmp/*.whl && rm /tmp/*.whl

COPY --from=build /CORE/python_streamer /CORE/python_streamer

RUN cd /CORE/python_streamer && uv sync

ENV PATH="/CORE/.venv/bin:/CORE/python_streamer/.venv/bin:$PATH"

# Set default command to bash for interactive terminal
CMD ["/bin/bash"]
