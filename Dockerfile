# syntax=docker/dockerfile:1

FROM ubuntu:20.04

USER root

WORKDIR /dependencies

COPY . .

RUN apt-get update

RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata

RUN apt-get install -y sudo

RUN chmod +x /dependencies/scripts/*.sh

RUN /dependencies/scripts/setup_in_ubuntu.sh

RUN conan remote add artifactory https://conan.buzeta.net/artifactory/api/conan/conan-local

RUN /dependencies/scripts/install_dependencies.sh

WORKDIR /

RUN rm -rf /dependencies

WORKDIR /CORE
