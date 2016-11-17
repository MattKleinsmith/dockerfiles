FROM ubuntu:14.04

##########
## CUDA ##
##########

# https://github.com/NVIDIA/nvidia-docker/blob/master/ubuntu-14.04/cuda/8.0/runtime/Dockerfile
LABEL com.nvidia.volumes.needed="nvidia_driver"

RUN NVIDIA_GPGKEY_SUM=d1be581509378368edeec8c1eb2958702feedf3bc3d17011adbf24efacce4ab5 && \
    NVIDIA_GPGKEY_FPR=ae09fe4bbd223a84b2ccfce3f60f4b3d7fa2af80 && \
    apt-key adv --fetch-keys http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1404/x86_64/7fa2af80.pub && \
    apt-key adv --export --no-emit-version -a $NVIDIA_GPGKEY_FPR | tail -n +2 > cudasign.pub && \
    echo "$NVIDIA_GPGKEY_SUM  cudasign.pub" | sha256sum -c --strict - && rm cudasign.pub && \
    echo "deb http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1404/x86_64 /" > /etc/apt/sources.list.d/cuda.list

ENV CUDA_VERSION 8.0
LABEL com.nvidia.cuda.version="8.0"

ENV CUDA_PKG_VERSION 8-0=8.0.44-1
RUN apt-get update && apt-get install -y --no-install-recommends \
        cuda-nvrtc-$CUDA_PKG_VERSION \
        cuda-nvgraph-$CUDA_PKG_VERSION \
        cuda-cusolver-$CUDA_PKG_VERSION \
        cuda-cublas-$CUDA_PKG_VERSION \
        cuda-cufft-$CUDA_PKG_VERSION \
        cuda-curand-$CUDA_PKG_VERSION \
        cuda-cusparse-$CUDA_PKG_VERSION \
        cuda-npp-$CUDA_PKG_VERSION \
        cuda-cudart-$CUDA_PKG_VERSION && \
    ln -s cuda-$CUDA_VERSION /usr/local/cuda

RUN echo "/usr/local/cuda/lib" >> /etc/ld.so.conf.d/cuda.conf && \
    echo "/usr/local/cuda/lib64" >> /etc/ld.so.conf.d/cuda.conf && \
    ldconfig

RUN echo "/usr/local/nvidia/lib" >> /etc/ld.so.conf.d/nvidia.conf && \
    echo "/usr/local/nvidia/lib64" >> /etc/ld.so.conf.d/nvidia.conf

ENV PATH /usr/local/nvidia/bin:/usr/local/cuda/bin:${PATH}
ENV LD_LIBRARY_PATH /usr/local/nvidia/lib:/usr/local/nvidia/lib64

# https://github.com/NVIDIA/nvidia-docker/blob/master/ubuntu-14.04/cuda/8.0/devel/Dockerfile
RUN apt-get install -y --no-install-recommends \
        cuda-core-$CUDA_PKG_VERSION \
        cuda-misc-headers-$CUDA_PKG_VERSION \
        cuda-command-line-tools-$CUDA_PKG_VERSION \
        cuda-nvrtc-dev-$CUDA_PKG_VERSION \
        cuda-nvml-dev-$CUDA_PKG_VERSION \
        cuda-nvgraph-dev-$CUDA_PKG_VERSION \
        cuda-cusolver-dev-$CUDA_PKG_VERSION \
        cuda-cublas-dev-$CUDA_PKG_VERSION \
        cuda-cufft-dev-$CUDA_PKG_VERSION \
        cuda-curand-dev-$CUDA_PKG_VERSION \
        cuda-cusparse-dev-$CUDA_PKG_VERSION \
        cuda-npp-dev-$CUDA_PKG_VERSION \
        cuda-cudart-dev-$CUDA_PKG_VERSION \
        cuda-driver-dev-$CUDA_PKG_VERSION

ENV LIBRARY_PATH /usr/local/cuda/lib64/stubs:${LIBRARY_PATH}

###########
## cuDNN ##
###########

# https://github.com/NVIDIA/nvidia-docker/blob/master/ubuntu-14.04/cuda/8.0/devel/cudnn5/Dockerfile
RUN apt-get install -y --no-install-recommends \
        curl

ENV CUDNN_VERSION 5
LABEL com.nvidia.cudnn.version="5"

RUN CUDNN_DOWNLOAD_SUM=a87cb2df2e5e7cc0a05e266734e679ee1a2fadad6f06af82a76ed81a23b102c8 && \
    curl -fsSL http://developer.download.nvidia.com/compute/redist/cudnn/v5.1/cudnn-8.0-linux-x64-v5.1.tgz -O && \
    echo "$CUDNN_DOWNLOAD_SUM  cudnn-8.0-linux-x64-v5.1.tgz" | sha256sum -c --strict - && \
    tar -xzf cudnn-8.0-linux-x64-v5.1.tgz -C /usr/local && \
    rm cudnn-8.0-linux-x64-v5.1.tgz && \
    ldconfig

################
## TensorFlow ##
################

# https://www.tensorflow.org/versions/r0.11/get_started/os_setup.html#pip-installation
RUN apt-get install -y --no-install-recommends \
        build-essential \
        libfreetype6-dev \
        libpng12-dev \
        libzmq3-dev \
        pkg-config \
		python \
        python-dev \
        software-properties-common \
        unzip
RUN curl -O https://bootstrap.pypa.io/get-pip.py && \
    python get-pip.py && \
    rm get-pip.py
# Ubuntu/Linux 64-bit, GPU enabled, Python 2.7
# Requires CUDA toolkit 8.0 and CuDNN v5.
ENV TF_BINARY_URL https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.11.0-cp27-none-linux_x86_64.whl
RUN pip install --upgrade $TF_BINARY_URL
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/extras/CUPTI/lib64
ENV CUDA_HOME /usr/local/cuda

###########
## Keras ##
###########

# https://keras.io/#installation
RUN pip install keras
RUN pip install h5py

############
## OpenCV ##
############

# http://www.pyimagesearch.com/2016/07/11/compiling-opencv-with-cuda-support/
# https://github.com/chipgarner/opencv3-webcam-docker/blob/master/Dockerfile
# https://github.com/opencv/opencv/pull/6510
RUN apt-get install -y --no-install-recommends \
		libjpeg8-dev \
		libtiff5-dev \
		libjasper-dev \
		libgtk2.0-dev \
		libavcodec-dev \
		libavformat-dev \
		libswscale-dev \
		libv4l-dev \
		libatlas-base-dev \
		libhdf5-serial-dev \
		libjpeg-dev \
		libavformat-dev \
		libswscale-dev \
		libdc1394-22-dev \
		libxine-dev \
		libgstreamer0.10-dev \
		libgstreamer-plugins-base0.10-dev \
		libv4l-dev \
		libtbb-dev \
		libeigen3-dev \
		libqt4-dev \
		libmp3lame-dev \
		libopencore-amrnb-dev \
		libopencore-amrwb-dev \
		libtheora-dev \
		libvorbis-dev \
		libxvidcore-dev \
		# Doesn't work libfaac-dev \
		# Doesn't work ffmpeg \
		#  zlib1g-dev \
		#  libavcodec-dev \
		x264 \ 
		v4l-utils \
		gfortran \
		yasm \
		python-numpy \
		checkinstall \
		wget \
		unzip 
RUN wget https://cmake.org/files/v3.7/cmake-3.7.0-Linux-x86_64.sh
RUN chmod +x cmake-3.7.0-Linux-x86_64.sh
RUN ./cmake-3.7.0-Linux-x86_64.sh --skip-license && rm cmake-3.7.0-Linux-x86_64.sh
WORKDIR opencv_compile
RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/3.1.0.zip
RUN unzip opencv.zip && rm opencv.zip
WORKDIR opencv-3.1.0
WORKDIR build
RUN cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D WITH_CUDA=ON \
    -D ENABLE_FAST_MATH=1 \
    -D CUDA_FAST_MATH=1 \
    -D WITH_CUBLAS=1 ..
RUN make -j$(nproc)
RUN make install
RUN ldconfig
WORKDIR / 
RUN rm -rf opencv_compile

# This works:
# apt-get install -y python-opencv
# But it's only 2.4.8.
# Let's try Anaconda

#############################
### Other Python libraries ##
#############################
#
##############
#
#RUN apt-get clean && \
#    rm -rf /var/lib/apt/lists/*
#CMD ["python"]
