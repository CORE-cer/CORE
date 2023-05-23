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

## Setup conan with autodetected configs of your computer

`conan profile detect`

## Run the project

There are multiple scrips to run the project:

1. `./scripts/install_dependencies.sh`
2. `./scripts/build.sh`
3. `./scripts/compile_and_test.sh`

Run `./scripts/install_dependencies.sh` before any other. Once compiled,
there are two demo main files: server and client, that model a very simple
communication between a server and a client. More detailed versions can
be seen in test cases inside `src/unit_tests/core_server`
