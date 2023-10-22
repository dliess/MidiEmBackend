FROM debian:bookworm

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
RUN apt-get update && apt-get install -y \
	      sudo \
        cmake \
        clang \
        clang-format \
        ninja-build \
        g++ \
        gdb \
        git \
        libasound2-dev \
        libgtest-dev \
        libgmock-dev \
        libusb-1.0-0-dev \
        uuid-dev \
        libspdlog-dev \
        && rm -rf /var/lib/apt/lists/*

# install Qt build common dependencies
RUN apt-get update && apt-get install -y \
        qtbase5-dev \
        qtdeclarative5-dev \
        libqt5websockets5-dev \
        libqt5webchannel5-dev \
        && rm -rf /var/lib/apt/lists/*

# install Qt build dependencies for frontend
RUN apt-get update && apt-get install -y \
        qml-module-qtquick-layouts \
        qml-module-qtquick-controls \
        qml-module-qtquick-controls2 \
        qml-module-qtquick-dialogs \
        qml-module-qtquick-shapes \
        qml-module-qt-websockets \
        && rm -rf /var/lib/apt/lists/*

# some utilities for runtime
RUN apt-get update && apt-get install -y \
        feedgnuplot \
        net-tools \
        curl \
        zsh \
        wget \
        && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
        pkg-config \
        libcapnp-dev \
        capnproto \
        libzmq3-dev \
        python3-toml \
        python3-venv \
        && rm -rf /var/lib/apt/lists/*

RUN     apt-get update && apt-get install -y \
        ripgrep \
        npm \
        nodejs \
        unzip \
        python3-venv \
        && rm -rf /var/lib/apt/lists/*

# Create group and user
RUN groupadd --gid=$GID $BUILD_USER
RUN useradd --create-home --shell /bin/bash $BUILD_USER --uid=$UID -g $BUILD_USER
RUN echo "$BUILD_USER:$BUILD_USER_PWD" | chpasswd
RUN usermod -aG sudo $BUILD_USER

RUN mkdir -p /home/$BUILD_USER/nomidi-backend
RUN mkdir -p /home/$BUILD_USER/CPM_DOWNLOADS

RUN echo "export XDG_CONFIG_HOME=/home/$BUILD_USER/.config" >> /etc/profile.d/xdg_config.sh 

USER $BUILD_USER:$BUILD_USER
WORKDIR /home/build-user

RUN wget -qO- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh | zsh || true
RUN sed -i 's/ZSH_THEME="robbyrussell"/ZSH_THEME="gentoo"/g' ~/.zshrc
RUN echo "alias vim=~/nvim-linux64/bin/nvim" >> ~/.zshrc
RUN echo "alias vi=~/nvim-linux64/bin/nvim" >> ~/.zshrc

WORKDIR /home/build-user/nomidi-backend

