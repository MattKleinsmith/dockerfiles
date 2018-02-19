FROM ubuntu:16.04

RUN apt-get update --fix-missing && apt-get install -y \
    vim \
    wget

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

##########
## Fire ##
##########

RUN pip install fire

RUN wget https://raw.githubusercontent.com/MattKleinsmith/dockerfiles/master/errors/fire/run.py

CMD ["python", "run.py", "--x", "20180217_064501"]
