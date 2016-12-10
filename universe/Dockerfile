FROM ubuntu:14.04

RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake \
        gcc \
        git \
        libav-tools \
        libboost-all-dev \
        libjpeg-dev \
        libjpeg-turbo8-dev \
        libsdl2-dev \
        libzbar-dev \
        libzbar0 \
        make \
        python-opengl \
        python-pip \
        software-properties-common \
        swig \
        xorg-dev \
        xvfb \
        zlib1g-dev

RUN add-apt-repository ppa:ubuntu-lxc/lxd-stable && \
    apt-get update && apt-get install -y --no-install-recommends \
        golang && \
    apt-get clean && \
    rm -rf /var/lib /apt/lists/*

RUN pip install --upgrade pip && \
    pip install \
        setuptools \
        numpy

RUN git clone https://github.com/openai/universe.git && \
    cd universe && \
    pip install -e .
