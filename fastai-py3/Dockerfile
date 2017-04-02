FROM gcr.io/tensorflow/tensorflow:latest-gpu

##############
## Anaconda ##
##############

RUN apt-get update --fix-missing && apt-get install -y wget bzip2 ca-certificates \
    libglib2.0-0 libxext6 libsm6 libxrender1 \
    git mercurial subversion vim

RUN echo 'export PATH=/opt/conda/bin:$PATH' > /etc/profile.d/conda.sh && \
    wget --quiet https://repo.continuum.io/archive/Anaconda3-4.3.0-Linux-x86_64.sh -O ~/anaconda.sh && \
    /bin/bash ~/anaconda.sh -b -p /opt/conda && \
    rm ~/anaconda.sh

ENV PATH /opt/conda/bin:$PATH

#####################
## Python packages ##
#####################

RUN pip install --upgrade pip

RUN pip install \
    tensorflow-gpu \
    keras==1.2.2 \
    bcolz \
    xgboost \
    gensim \
    keras_tqdm

######################
## Jupyter notebook ##
######################

RUN conda install -y notebook==4.4.1 && \
    conda install -y -c conda-forge jupyter_contrib_nbextensions && \
    jupyter nbextensions_configurator enable --user

# Add my jupyter settings
ADD .jupyter /root/.jupyter

RUN conda install -y -c conda-forge ipywidgets

############
## FFmpeg ##
############

RUN add-apt-repository ppa:mc3man/trusty-media && apt-get update && \
    apt-get install -y ffmpeg

###########
## Other ##
###########

RUN conda install -y pytorch torchvision cuda80 -c soumith
RUN CC="cc -mavx2" pip install -U --force-reinstall pillow-simd
RUN conda install -c menpo opencv3=3.2.0

###########
## Torch ##
###########

RUN apt-get update && apt-get install -y \
    software-properties-common \
    libzmq3-dev \
    libssl-dev \
    python-zmq

WORKDIR /root

RUN git clone https://github.com/torch/distro.git torch --recursive && \
    cd torch && \
    bash install-deps && \
    ./install.sh

ENV PATH=/root/torch/install/bin:$PATH

RUN git clone https://github.com/facebook/iTorch.git && \
    cd iTorch && \
    luarocks make

RUN luarocks install unsup

###########

ADD .bashrc /root/.bashrc
ADD .vimrc /root/.vimrc
ADD .vim /root/.vim

WORKDIR /nbs
