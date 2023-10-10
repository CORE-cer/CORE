#!/bin/bash
sudo apt update -y
sudo apt install python3-pip -y
# Install the latest version of cmake:
sudo apt remove --purge --auto-remove cmake

sudo apt update && \
    sudo apt install -y software-properties-common lsb-release cargo && \
    sudo apt clean all

sudo apt install default-jdk -y

wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
sudo apt update

sudo apt install kitware-archive-keyring -y
sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
sudo apt update -y

sudo apt install cmake valgrind -y

cargo install run-clang-tidy

pip install conan

source ~/.profile # Or reboot your terminal

conan profile detect

# Run the scrips:
# ./scripts/install_dependencies.sh
# ./scripts/compile_and_test.sh
