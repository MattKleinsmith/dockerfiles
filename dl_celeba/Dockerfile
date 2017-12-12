# FROM nvidia/cuda:8.0-cudnn6-runtime-ubuntu16.04
FROM gcr.io/tensorflow/tensorflow:latest-gpu

###########
## Tools ##
###########

RUN apt-get update --fix-missing && apt-get install -y \
    ffmpeg \
    git \
    vim \
    wget

# TODO:
# apt-get clean && \
# rm -rf /var/lib/apt/lists/*

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

RUN pip install --upgrade \
        pip && \
    pip install \
        bcolz \
        gensim \
        keras==1.2.2 \
        keras_tqdm \
        tensorflow-gpu \
        xgboost

RUN CC="cc -mavx2" pip install -U --force-reinstall pillow-simd

#############
## Jupyter ##
#############

RUN conda install -y notebook && \
    conda install -y -c conda-forge jupyter_contrib_nbextensions && \
    jupyter nbextensions_configurator enable --user

# Add my Jupyter settings
ADD .jupyter /root/.jupyter

# For creating interactive visualizations in notebooks
RUN conda install -y -c conda-forge ipywidgets

############
## OpenCV ##
############

# Needed for FaceTracker
RUN apt-get update --fix-missing && apt-get install -y \
    libcv-dev \
    libopencv-dev

#################
## FaceTracker ##
#################

ENV CELEB_PATH /code/celeba

ADD FaceTracker $CELEB_PATH/FaceTracker

# Filename discrepancy due to Dropbox's case insensitivity
# TODO: Rename facetracker to pyfacetracker in all files. Case sensitive.
ADD pyfacetracker $CELEB_PATH/facetracker

RUN $CELEB_PATH/facetracker/make_opencv_lib_links.sh

WORKDIR $CELEB_PATH/facetracker/

RUN python2 $CELEB_PATH/facetracker/setup.py install

RUN conda install -y pytorch torchvision cuda80 -c soumith

RUN apt-get update --fix-missing && apt-get install -y \
    python-tk

############################
## wkentaro's pytorch-fcn ##
############################

RUN pip install torchfcn

##########
## TQDM ##
##########

RUN pip install tqdm
RUN conda install -y -c anaconda qt

###########
## Fonts ##
###########

RUN apt-get install -y font-manager
RUN echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula \
    select true | debconf-set-selections
RUN apt-get install -y ttf-mscorefonts-installer
RUN rm ~/.cache/matplotlib -fr

################
## Networking ##
################

RUN apt-get update --fix-missing && apt-get install -y \
    net-tools

################
## PostgresQL ##
################

RUN apt-get update --fix-missing && apt-get install -y \
    postgresql \
    libpq-dev \
    postgresql-client \
    postgresql-client-common \
    postgresql-contrib

RUN pip install psycopg2

##################
## Config files ##
##################

ADD .vimrc /root/.vimrc

ADD .vim /root/.vim

ADD .bashrc /root/.bashrc

ENV LANG C.UTF-8

WORKDIR /code/portraitseg
