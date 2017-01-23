FROM gcr.io/tensorflow/tensorflow:latest-gpu

##############
## Anaconda ##
##############

RUN apt-get update --fix-missing && apt-get install -y wget bzip2 ca-certificates \
    libglib2.0-0 libxext6 libsm6 libxrender1 \
    git mercurial subversion vim

RUN echo 'export PATH=/opt/conda/bin:$PATH' > /etc/profile.d/conda.sh && \
    wget --quiet https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86_64.sh -O ~/anaconda.sh && \
    /bin/bash ~/anaconda.sh -b -p /opt/conda && \
    rm ~/anaconda.sh

ENV PATH /opt/conda/bin:$PATH

####################
## TensorFlow GPU ##
####################


RUN pip install --upgrade pip

# Tensorflow
# Only the CPU version of TensorFlow is available at the moment and can be installed in the conda environment for Python 2 or Python 3.
ENV TF_BINARY_URL https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.1-cp35-cp35m-linux_x86_64.whl
RUN pip3 install --ignore-installed --upgrade $TF_BINARY_URL

###########################
## Other Python packages ##
###########################

RUN pip3 install \
    keras \
    bcolz

######################
## Jupyter notebook ##
######################

RUN conda install -y notebook==4.3.1 && \
    conda install -c conda-forge jupyter_contrib_nbextensions && \
    jupyter nbextensions_configurator enable --user

ADD .jupyter /root/.jupyter

WORKDIR /nbs
