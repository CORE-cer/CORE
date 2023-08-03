# syntax=docker/dockerfile:1

FROM ubuntu:22.04

USER root

WORKDIR /dependencies

COPY . .

RUN apt-get update

RUN apt-get -y upgrade

RUN apt-get -y install sudo

RUN apt-get -y install wget

RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata

RUN chmod +x /dependencies/scripts/*.sh

RUN /dependencies/scripts/setup_in_ubuntu.sh

RUN conan remote add artifactory https://conan.buzeta.net/artifactory/api/conan/conan-local

RUN /dependencies/scripts/install_dependencies.sh

WORKDIR /

RUN rm -rf /dependencies

RUN mkdir ~/.local

RUN mkdir ~/.local/bin

WORKDIR /CORE
