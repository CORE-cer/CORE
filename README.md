# How to build the project:

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
    sudo apt install -y software-properties-common lsb-release && \
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

## Setup conan with autodetected configs of your computer

`conan profile detect`

## Compile the project

First we need to install the third party dependencies:

```
mkdir -p build
mkdir -p build/Debug
conan source .
conan install . -s build_type=${BUILD_TYPE} -s:h compiler=gcc \
                -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2\
                --build missing -vquiet
```

Next, we need to compile the projects code:

```
conan build . -s build_type=${BUILD_TYPE} -s:h compiler=gcc \
              -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2\
              --build missing -vquiet
```

If changes were done to the grammar it can be rebuilt using:

```
cd build/${BUILD_TYPE}
make grammar
cd ../../
conan build . -s build_type=${BUILD_TYPE} -s:h compiler=gcc \
              -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2\
              --build missing -vquiet
```

There are multiple scripts with self explanatory names in the `./scripts`
directory.  If you are planning on modifying the code, it is recommended
to use the compile_and_test.sh script to make sure the unit tests are
working correctly.
