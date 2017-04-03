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

RUN conda install -y pytorch torchvision cuda80 -c soumith

RUN CC="cc -mavx2" pip install -U --force-reinstall pillow-simd

###########
## Torch ##
###########

RUN apt-get update --fix-missing && apt-get install -y \
    libssl-dev \
    libzmq3-dev \
    python-zmq \
    software-properties-common

WORKDIR /root

RUN git clone https://github.com/torch/distro.git torch --recursive && \
    cd torch && \
    apt-get install -y sudo && \
    bash install-deps && \
    printf "y\n" | ./install.sh

ENV PATH=/root/torch/install/bin:$PATH

# For the arbitrary style transfer repo
RUN luarocks install unsup

#############
## Jupyter ##
#############

RUN conda install -y notebook==4.4.1 && \
    conda install -y -c conda-forge jupyter_contrib_nbextensions && \
    jupyter nbextensions_configurator enable --user

# Add my Jupyter settings
ADD .jupyter /root/.jupyter

RUN conda install -y -c conda-forge ipywidgets

RUN git clone https://github.com/facebook/iTorch.git && \
    cd iTorch && \
    luarocks make

##################
## Config files ##
##################

ADD .vimrc /root/.vimrc

ADD .vim /root/.vim

ADD .bashrc /root/.bashrc

WORKDIR /root
