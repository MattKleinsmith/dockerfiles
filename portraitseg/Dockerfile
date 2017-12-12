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

RUN conda install -y pytorch torchvision cuda90 -c pytorch

RUN pip install torchfcn

#################
## FaceTracker ##
#################

RUN apt-get update --fix-missing && apt-get install -y \
    libcv-dev \
    libopencv-dev

RUN git clone git://github.com/kylemcdonald/FaceTracker.git

WORKDIR /FaceTracker

RUN git checkout opencv2 && \
    wget https://raw.githubusercontent.com/MatthewKleinsmith/dockerfiles/master/portraitseg/FaceTracker/Makefile -O Makefile && \
    make

WORKDIR /

RUN wget https://bitbucket.org/amitibo/pyfacetracker/downloads/pyFaceTracker-0.1.1.tar.gz && \
    tar -x -f pyFaceTracker-0.1.1.tar.gz

RUN wget https://raw.githubusercontent.com/MatthewKleinsmith/dockerfiles/master/portraitseg/pyfacetracker/setup.py -O /pyFaceTracker-0.1.1/setup.py

#ADD FaceTracker /code/FaceTracker
#
#ADD pyfacetracker /code/facetracker
#
#RUN /code/facetracker/make_opencv_lib_links.sh
#
#WORKDIR /code/facetracker/
#
#
## Unfortunately pyfacetracker required Python 2
#RUN apt-get update --fix-missing && apt-get install -y \
#    python-setuptools
#
#RUN wget https://bootstrap.pypa.io/get-pip.py && \
#    python2 get-pip.py
#
#RUN python2 -m pip install numpy
#
#RUN python2 /code/facetracker/setup.py install

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
