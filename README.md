# How to build the project:

## Easy Install Docker

Dependencies: Docker.

First pull repo

`git pull https://github.com/CER-CORE/CORE`

CD in to directory

`cd CORE`

If the docker daemon is not running:

`systemctl start docker`

Run docker-compose with bash

`sudo docker-compose run core-terminal bash`

Now you have a terminal with all the necessary tools installed. To compile for the first time run:

`chmod +x ./scripts/* && ./scripts/build.sh && ./scripts/build_grammar.sh`

Now you can compile and test with

`./scripts/compile_and_test.sh`

## Manual Install

We utilize conan for package managing. In ubuntu this is how it could be done:

## Update packages

`sudo apt update -y`

## Conan

```
sudo apt install python3-pip -y
pip install conan
```

## Install the latest version of cmake:

```
sudo apt remove --purge --auto-remove cmake
sudo apt update && \
    sudo apt install -y software-properties-common lsb-release valgrind && \
    sudo apt clean all
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
sudo apt update
sudo apt install kitware-archive-keyring -y
sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
sudo apt update -y
sudo apt install cmake -y
source ~/.profile # Or reboot your terminal
```

If you are using WSL, you might need to reboot your terminal for this to work.

## Install version 11 of g++, or later.

if `g++ --version` is >= 11, then you can skip this step.

```
sudo apt install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install g++-11
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 1000
```

## Setup conan with autodetected configs of your computer

`conan profile detect`

It is important to note we also provide known working conan profiles, located at `./conan_profiles`.

## Compile the project

First we need to install the third party dependencies:

```
mkdir -p build
mkdir -p build/Debug
conan source .
conan install . -s build_type=Debug -s:h compiler=gcc \
                -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2\
                --build missing -vquiet
```

Next, we need to compile the projects code:

```
conan build . -s build_type=Debug -s:h compiler=gcc \
              -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2\
              --build missing -vquiet
```

If changes were done to the grammar it can be rebuilt using:

```
cd build/Debug
make grammar
cd ../../
conan build . -s build_type=Debug -s:h compiler=gcc \
              -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2\
              --build missing -vquiet
```

There are multiple scripts with self explanatory names in the `./scripts`
directory.  If you are planning on modifying the code, it is recommended
to use the compile_and_test.sh script to make sure the unit tests are
working correctly.

## Project development.

### clang-format

it is crucial that your clang-format version is version 15 so that it
is the same that is used in github actions. To download this version and set it as default you can use these commands in ubuntu:

```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal main" | sudo tee -a /etc/apt/sources.list
sudo apt-get update
sudo apt-get install -y clang-format-15
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-15 1000
```

And after that, `clang-format --version` should be 15. Use the script: `./scripts/clang_format_all_files.sh` before merging with main to pass that individual github action. 

Note that if you wish to not use clang-format somewhere in your code, you just need to add these comments:

```c++
// clang-format off

// ... Code that is not formatted...

// clang-format on
```
