FROM nvidia/cuda:9.0-cudnn7-devel-ubuntu16.04

# To allow unicode characters in the terminal
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8

# To make the CUDA device order match the IDs in nvidia-smi
ENV CUDA_DEVICE_ORDER=PCI_BUS_ID

###########
## Tools ##
###########

RUN apt-get update --fix-missing && apt-get install -y \
    wget \
    vim \
    git \
    unzip

##############
## Anaconda ##
##############

# https://github.com/ContinuumIO/docker-images/blob/master/anaconda3/Dockerfile

RUN apt-get update --fix-missing && apt-get install -y bzip2 ca-certificates \
    libglib2.0-0 libxext6 libsm6 libxrender1

RUN echo 'export PATH=/opt/conda/bin:$PATH' > /etc/profile.d/conda.sh && \
    wget --quiet https://repo.continuum.io/archive/Anaconda3-5.0.1-Linux-x86_64.sh -O ~/anaconda.sh && \
    /bin/bash ~/anaconda.sh -b -p /opt/conda && \
    rm ~/anaconda.sh

# Removed Tini because Docker 1.13 has the --init flag.

ENV PATH /opt/conda/bin:$PATH

###############
## Intel MKL ##
###############

# Math Kernel Library

# https://hub.docker.com/r/intelpython/intelpython3_core/~/dockerfile/

ENV ACCEPT_INTEL_PYTHON_EULA=yes

RUN conda config --add channels intel\
    && conda install  -y -q intelpython3_core=2018.0.1 python=3 \
    && apt-get update -qqq \
    && apt-get install -y -q g++

# https://docs.anaconda.com/mkl-service/

# "The main purpose of the package is to allow the user to change the number of CPU’s MKL is using at runtime."
RUN conda install mkl-service

# To allow MKL to see all CPU threads
ENV MKL_DYNAMIC=FALSE

#########################
## Andres dependencies ##
#########################

# https://github.com/antorsae/sp-society-camera-model-identification

RUN apt-get install -y libturbojpeg

RUN pip install \
        numpngw \
        tqdm \
        jpeg4py \
        opencv-python

# Add sym links to:
#   train
#   test
#   models
#   submissions
#   flickr_data
#   val_data

#############
## PyTorch ##
#############

# http://pytorch.org/

RUN conda install pytorch torchvision cuda90 -c pytorch

###########
## Other ##
###########

RUN pip install \
    fire

#############
## Aliases ##
#############

RUN echo "alias p=ipython" >> ~/.bashrc

RUN echo "alias c1=\"ipython -- main.py --num-epochs 2 --sample-percent 0.05 --gpu 1\"" >> ~/.bashrc

CMD bash
