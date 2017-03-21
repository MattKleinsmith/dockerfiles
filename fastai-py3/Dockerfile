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
    keras \
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

ADD conda install -c conda-forge ipywidgets

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
ADD .bashrc /root/.bashrc

WORKDIR /nbs
