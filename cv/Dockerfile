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

########################
## Intel MKL settings ##
########################

# Anaconda comes with MKL. Using Intel's conda channel doesn't seem needed.

# To allow setting the number of CPU threads
RUN conda install mkl-service

# To allow MKL to see all CPU threads
ENV MKL_DYNAMIC=FALSE

# To set number of CPU threads:
#   import mkl
#   mkl.set_num_threads(NUM_THREADS)

# Sometimes allowing more threads slows things down.
# Consider these results from my system (6 cores, 2 threads per core, Intel i7-6850K):
#   Eigendecomposition of a 2048x2048 matrix in X s.
#     num threads | X (seconds)
#     4           | 4.04
#     5           | 3.95
#     6           | 4.09
#     7           | 4.90
#     12          | 6.78
# Benchmark code from: http://markus-beuckelmann.de/blog/boosting-numpy-blas.html
# Discussion of slow downs from more threads: https://unix.stackexchange.com/a/80427/201733

#############
## Jupyter ##
#############

RUN conda update -y -n base conda
RUN conda install -y notebook
# Error: IsADirectoryError(21, 'Is a directory'):
RUN conda install -y jupyter_contrib_nbextensions -c conda-forge; exit 0
# The error doesnt appear again if you try to install again.
RUN conda install -y jupyter_contrib_nbextensions -c conda-forge
RUN jupyter nbextensions_configurator enable --user
RUN jupyter nbextension enable collapsible_headings/main
RUN pip install ipywidgets
RUN jupyter nbextension enable --py widgetsnbextension --sys-prefix


RUN jupyter notebook --generate-config --allow-root && \
    echo "c.NotebookApp.ip = '*'" >> ~/.jupyter/jupyter_notebook_config.py && \
    echo "c.NotebookApp.open_browser = False" >> ~/.jupyter/jupyter_notebook_config.py

RUN echo "alias j8=\"CUDA_VISIBLE_DEVICES=0 jupyter notebook --port=8888 --allow-root\"" >> ~/.bashrc

RUN echo "alias jupyter_notebook_GPU_0_PORT_8888=j8" >> ~/.bashrc

RUN echo "alias j9=\"CUDA_VISIBLE_DEVICES=1 jupyter notebook --port=8889 --allow-root\"" >> ~/.bashrc

RUN echo "alias jupyter_notebook_GPU_1_PORT_8889=j9" >> ~/.bashrc

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

# Using Python Fire instead of argparse.
# https://github.com/google/python-fire

RUN pip install \
    fire \
    zarr \
    numcodecs

RUN apt-get update --fix-missing && apt-get install -y \
    libgl1-mesa-glx

#############
## Aliases ##
#############

RUN echo "alias p=\"ipython --no-confirm-exit\"" >> ~/.bashrc

RUN echo "alias c1=\"ipython -- main.py --gpu 1 --test-code --test\"" >> ~/.bashrc

CMD bash
