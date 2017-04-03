FROM gcr.io/tensorflow/tensorflow:latest-gpu

RUN apt-get update && apt-get install -y \
        git \
        libatk1.0-dev \
        libbonoboui2-dev \
        libcairo2-dev \
        libgnome2-dev \
        libgnomeui-dev \
        libgtk2.0-dev \
        libncurses5-dev \
        libx11-dev \
        libxpm-dev \
        libxt-dev \
        python3-dev \
        wget

WORKDIR /root

RUN wget ftp://ftp.vim.org/pub/vim/unix/vim-7.4.tar.bz2

RUN tar -xf /root/vim-7.4.tar.bz2 && rm /root/vim-7.4.tar.bz2

WORKDIR vim74

RUN ./configure \
    --with-features=huge \
    --enable-multibyte \
    --enable-python3interp=yes \
    --with-python3-config-dir=/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu \
    --enable-gui=gtk2 --enable-cscope --prefix=/usr

RUN make -j$(nproc) VIMRUNTIMEDIR=/usr/share/vim/vim74

RUN make -j$(nproc) install

WORKDIR /root

RUN rm -r vim74

ADD .bashrc /root/.bashrc
ADD .vimrc /root/.vimrc
ADD .vim /root/.vim
