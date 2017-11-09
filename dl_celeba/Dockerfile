FROM gcr.io/tensorflow/tensorflow:latest-gpu

###########
## Tools ##
###########

RUN apt-get update --fix-missing && apt-get install -y \
    ffmpeg \
    git \
    vim \
    wget

##############
## Anaconda ##
##############

RUN apt-get update --fix-missing && apt-get install -y \
    libglib2.0-0 \
    libsm6 \
    libxext6 \
    libxrender1

RUN echo 'export PATH=/opt/conda/bin:$PATH' > /etc/profile.d/conda.sh && \
    wget --quiet https://repo.continuum.io/archive/Anaconda3-4.3.1-Linux-x86_64.sh -O ~/anaconda.sh && \
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

RUN conda install -y -c conda-forge ipywidgets

############
## OpenCV ##
############

RUN apt-get update --fix-missing && apt-get install -y \
    libcv-dev \
    libopencv-dev

#################
## Facetracker ##
#################

ENV CELEB_PATH /nbs/celeba

ADD FaceTracker $CELEB_PATH/FaceTracker

ADD facetracker $CELEB_PATH/facetracker

RUN $CELEB_PATH/facetracker/make_opencv_lib_links.sh

WORKDIR $CELEB_PATH/facetracker/

RUN python2 $CELEB_PATH/facetracker/setup.py install

#RUN pip install pyreadlines

RUN conda install -y pytorch torchvision cuda80 -c soumith

RUN apt-get install -y \
    python-tk


############################
## wkentaro's pytorch-fcn ##
############################

RUN pip install torchfcn

###########################

# pip or conda?
#RUN pip install qt

RUN pip install tqdm

RUN conda install -y -c anaconda qt

##########################

RUN apt-get install -y font-manager
RUN echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula \
    select true | debconf-set-selections
RUN apt-get install -y ttf-mscorefonts-installer
RUN rm ~/.cache/matplotlib -fr

##################
## Config files ##
##################

ADD .vimrc /root/.vimrc

ADD .vim /root/.vim

ADD .bashrc /root/.bashrc

ENV LANG C.UTF-8

WORKDIR /nbs/Portraitseg
