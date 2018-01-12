FROM nvidia/cuda:9.0-cudnn7-devel-ubuntu16.04

ENV LANG C.UTF-8

###########
## Tools ##
###########

RUN apt-get update --fix-missing && apt-get install -y \
    wget \
    vim \
    git

##############
## Anaconda ##
##############

RUN apt-get update --fix-missing && apt-get install -y \
    libglib2.0-0 \
    libsm6 \
    libxext6 \
    libxrender1

RUN echo 'export PATH=/opt/conda/bin:$PATH' > /etc/profile.d/conda.sh && \
    wget --quiet https://repo.continuum.io/archive/Anaconda3-5.0.1-Linux-x86_64.sh -O ~/anaconda.sh && \
    /bin/bash ~/anaconda.sh -b -p /opt/conda && \
    rm ~/anaconda.sh

ENV PATH /opt/conda/bin:$PATH

#####################
## Python packages ##
#####################

# TODO: Find out what needs python-tk
RUN apt-get update --fix-missing && apt-get install -y \
    python-tk

# Fonts for tqdm
RUN apt-get update --fix-missing && apt-get install -y font-manager && \
    echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula \
    select true | debconf-set-selections && \
    apt-get install -y ttf-mscorefonts-installer && \
    rm ~/.cache/matplotlib -fr

# For tqdm
RUN conda install -y qt -c anaconda

RUN pip install --upgrade \
        pip && \
    pip install \
        tqdm \
        bcolz

RUN CC="cc -mavx2" pip install -U --force-reinstall pillow-simd

#############
## Jupyter ##
#############

RUN conda install -y notebook && \
    conda install -y jupyter_contrib_nbextensions -c conda-forge && \
    jupyter nbextensions_configurator enable --user

# Add my Jupyter settings
ADD .jupyter /root/.jupyter

# For creating interactive visualizations in notebooks
RUN apt-get update --fix-missing && apt-get install -y \
    ffmpeg && \
    conda install -y ipywidgets -c conda-forge

################
## PostgreSQL ##
################

RUN apt-get update --fix-missing && apt-get install -y \
    postgresql \
    libpq-dev \
    postgresql-client \
    postgresql-client-common \
    postgresql-contrib

RUN pip install psycopg2

#############
## PyTorch ##
#############

RUN conda install -y numpy pyyaml mkl setuptools cmake cffi

RUN conda install -y magma-cuda90 -c soumith

RUN git clone --recursive https://github.com/pytorch/pytorch /pytorch

WORKDIR /pytorch

RUN python setup.py install

RUN pip install torchfcn

#################
## FaceTracker ##
#################

WORKDIR /

RUN apt-get update --fix-missing && apt-get install -y \
    libcv-dev \
    libopencv-dev && \
    git clone git://github.com/kylemcdonald/FaceTracker.git

WORKDIR /FaceTracker

RUN git checkout opencv2 && \
    wget https://raw.githubusercontent.com/MattKleinsmith/dockerfiles/master/portraitseg/FaceTracker/Makefile -O Makefile && \
    make

WORKDIR /

ENV PYFACETRACKER pyFaceTracker-0.1.1

RUN wget https://bitbucket.org/amitibo/pyfacetracker/downloads/$PYFACETRACKER.tar.gz && \
    tar -x -f $PYFACETRACKER.tar.gz && rm $PYFACETRACKER.tar.gz && \
    wget https://raw.githubusercontent.com/MattKleinsmith/dockerfiles/master/portraitseg/pyfacetracker/setup.py -O /$PYFACETRACKER/setup.py && \
    wget https://raw.githubusercontent.com/MattKleinsmith/dockerfiles/master/portraitseg/pyfacetracker/make_opencv_lib_links.sh -O /$PYFACETRACKER/make_opencv_lib_links.sh && \
    chmod +x /$PYFACETRACKER/make_opencv_lib_links.sh && \
    /$PYFACETRACKER/make_opencv_lib_links.sh

WORKDIR $PYFACETRACKER

RUN apt-get update --fix-missing && apt-get install -y \
        python-dev \
        python-setuptools && \
    wget https://bootstrap.pypa.io/get-pip.py && \
        python2 get-pip.py && \
    python2 -m pip install \
        numpy \
        matplotlib \
        pillow && \
    python2 setup.py install

##################
## Config files ##
##################

ADD .vimrc /root/.vimrc

ADD .vim /root/.vim

ADD .bashrc /root/.bashrc

WORKDIR /code/portraitseg

# TODO:
# apt-get clean && \
# rm -rf /var/lib/apt/lists/*
