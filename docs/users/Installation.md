# Installation Guide for CORE Project

## Easy Install with Docker

### Dependencies

- Docker

#### Steps

1. Pull the repository:
   ```bash
   git pull https://github.com/CER-CORE/CORE
   ```
2. Change to the project directory:
   ```bash
   cd CORE
   ```
3. Start the Docker daemon if it's not running:
   ```bash
   systemctl start docker
   ```
4. Run docker-compose with bash:
   ```bash
   sudo docker-compose run core-terminal bash
   ```
5. Compile for the first time (inside the docker terminal):
   ```bash
   chmod +x ./scripts/* && ./scripts/build.sh && ./scripts/build_grammar.sh
   ```
6. Compile and test:
   ```bash
   ./scripts/compile_and_test.sh
   ```

## Manual Installation

### Dependencies

- Conan
- CMake
- g++ (version 11 or later)

#### Steps

1. Update packages:

   ```bash
   sudo apt update -y
   ```

2. Install Conan:

   ```bash
   sudo apt install python3-pip -y
   pip install conan
   ```

3. Install the latest version of CMake:

    ```bash
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
    source ~/.profile # Or reboot your terminal`
    ```

4. Install g++ version 11 or later:

    ```bash
    sudo apt install software-properties-common
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt update
    sudo apt install g++-11
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 1000
    ```

5. Setup Conan:

   ```
   conan profile detect
   ```

6. Compile the project:

   ```
   mkdir -p build
   mkdir -p build/Debug
   conan source .
   conan install . -s build_type=Debug -s:h compiler=gcc -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2 --build missing -vquiet
   conan build . -s build_type=Debug -s:h compiler=gcc -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2 --build missing -vquiet
   ```

   If changes were made to the grammar, rebuild it using:

   ```
   cd build/Debug
   make grammar
   cd ../../
   conan build . -s build_type=Debug -s:h compiler=gcc -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2 --build missing -vquiet
   ```
