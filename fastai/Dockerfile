# 2018-01-12: This Dockerfile is based on http://files.fast.ai/setup/paperspace
# README: https://github.com/MattKleinsmith/dockerfiles/tree/master/fastai

FROM nvidia/cuda:9.0-cudnn7-devel-ubuntu16.04

# To allow unicode characters in the terminal
ENV LANG C.UTF-8

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

#########################
## fastai dependencies ##
#########################

# You should have a fastai clone on your host machine.
# The following is just to get the dependencies for the clone onto the container.
# You'll give the container access to the clone on your host machine,
# so that the changes you make in the notebooks persist even if the container
# dies.

RUN git clone https://github.com/fastai/fastai.git /code/fastai

WORKDIR /code/fastai

RUN conda env update

# Solves: `libjpeg.so.8: cannot open shared object file: No such file or directory`
#          after `from PIL import Image`
RUN apt-get install -y libjpeg-turbo8

RUN jupyter notebook --generate-config --allow-root && \
    echo "c.NotebookApp.ip = '*'" >> ~/.jupyter/jupyter_notebook_config.py && \
    echo "c.NotebookApp.open_browser = False" >> ~/.jupyter/jupyter_notebook_config.py

RUN /bin/bash -c "\
    source activate fastai && \
    conda install -y notebook && \
    conda install -y jupyter_contrib_nbextensions -c conda-forge && \
    jupyter nbextensions_configurator enable --user && \
    jupyter nbextension enable collapsible_headings/main"

RUN /bin/bash -c "\
    source activate fastai && \
    pip install ipywidgets && \
    jupyter nbextension enable --py widgetsnbextension --sys-prefix"

RUN echo export CUDA_DEVICE_ORDER="PCI_BUS_ID" >> ~/.bashrc

RUN echo "alias j8=\"ln -sf /data /code/fastai/courses/dl1/ && source activate fastai && CUDA_VISIBLE_DEVICES=0 jupyter notebook --port=8888 --allow-root\"" >> ~/.bashrc
RUN echo "alias jupyter_notebook_GPU_0_PORT_8888=j8" >> ~/.bashrc

RUN echo "alias j9=\"ln -sf /data /code/fastai/courses/dl1/ && source activate fastai && CUDA_VISIBLE_DEVICES=1 jupyter notebook --port=8889 --allow-root\"" >> ~/.bashrc
RUN echo "alias jupyter_notebook_GPU_1_PORT_8889=j9" >> ~/.bashrc

# https://software.intel.com/en-us/mkl
RUN /bin/bash -c "\
    source activate fastai && \
    conda install mkl-service"
RUN echo "export MKL_DYNAMIC=FALSE" >> ~/.bashrc
