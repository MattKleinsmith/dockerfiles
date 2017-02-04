FROM gcr.io/tensorflow/tensorflow:latest-gpu

##############
## Anaconda ##
##############

RUN apt-get update --fix-missing && apt-get install -y wget bzip2 ca-certificates \
    libglib2.0-0 libxext6 libsm6 libxrender1 \
    git mercurial subversion

RUN echo 'export PATH=/opt/conda/bin:$PATH' > /etc/profile.d/conda.sh && \
    wget --quiet https://repo.continuum.io/archive/Anaconda2-4.2.0-Linux-x86_64.sh -O ~/anaconda.sh && \
    /bin/bash ~/anaconda.sh -b -p /opt/conda && \
    rm ~/anaconda.sh

ENV PATH /opt/conda/bin:$PATH

#############
## fast.ai ##
#############

RUN pip install --upgrade pip

# Tensorflow
ENV TF_BINARY_URL https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0-cp27-none-linux_x86_64.whl
RUN pip install --ignore-installed --upgrade $TF_BINARY_URL

RUN pip install \
    keras \
    bcolz

RUN conda install -y -c menpo dlib=18.18

RUN conda install -y numpy

RUN conda install -y -c menpo opencv3=3.2.0 && \
    apt-get install -y libgtk2.0-0

WORKDIR /nbs
