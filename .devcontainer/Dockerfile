FROM debian:bullseye

# Users and passwords
ARG BUILD_USER=build-user
ARG UID=1000
ARG GID=1000
ARG BUILD_USER_PWD=pwd

RUN rm -rf /var/lib/apt/lists/*

# Generate and set locale
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y locales \
    && sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen \
    && dpkg-reconfigure --frontend=noninteractive locales

RUN locale-gen --purge en_US.UTF-8
RUN update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8

# Package installs
RUN apt-get install -y \
	sudo \
        cmake \
        clang-format \
        ninja-build \
        g++ \
        git \
        libasound2-dev \
        libgtest-dev \
        libusb-1.0-0-dev \
        uuid-dev

# install gRPC build dependencies
RUN apt-get install -y \
        libssl-dev

# install Qt build common dependencies
RUN apt-get install -y \
        qtbase5-dev \
        qtdeclarative5-dev \
        libqt5websockets5-dev \
        libqt5webchannel5-dev

# install Qt build dependencies for frontend
RUN apt-get install -y \
        qml-module-qtquick-layouts \
        qml-module-qtquick-controls \
        qml-module-qtquick-controls2 \
        qml-module-qtquick-dialogs \
        qml-module-qtquick-shapes \
        qml-module-qt-websockets

# some utilities for runtime
RUN apt-get install -y \
        vim \
        feedgnuplot \
        netcat \
        zsh \
        curl

#RUN sh -c "$(curl -fsSL https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

RUN apt-get install -y \
        pkg-config \
        libcapnp-dev \
        capnproto \
        libzmq3-dev

RUN rm -rf /var/lib/apt/lists/*

# Create group and user
RUN groupadd --gid=$GID $BUILD_USER
RUN useradd --create-home --shell /bin/bash $BUILD_USER --uid=$UID -g $BUILD_USER
RUN echo "$BUILD_USER:$BUILD_USER_PWD" | chpasswd
RUN usermod -aG sudo $BUILD_USER

USER $BUILD_USER
WORKDIR /home/$BUILD_USER




