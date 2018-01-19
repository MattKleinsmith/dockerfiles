# Docker for fast.ai

2018-01-12: The Docker image works with [the lesson1 notebook](https://github.com/fastai/fastai/blob/master/courses/dl1/lesson1.ipynb). It's untested for other notebooks.

## Why Docker

To not let dependencies slow you or anyone else down.

- If you make a mistake while sorting out a mess of dependencies, you can just delete the Docker container and start from a fresh one; as opposed to trying to undo it on your operating system.
- Once you sort out a mess of dependencies, you'll never have to do it again. Even if you install a new operating system or move to a new computer, you can quickly recover your environment by downloading the corresponding Docker image or Dockerfile.
- Also, no one else will have to go through that mess, because you can send them the Docker image or Dockerfile.

For more information, check out [this Docker tutorial for data science](https://towardsdatascience.com/how-docker-can-help-you-become-a-more-effective-data-scientist-7fc048ef91d5).


## Assumptions

- You have a machine with an NVIDIA GPU in it.
  - If you don't, check out [Paperspace](http://forums.fast.ai/t/paperspace-setup-help/9290) or consider [building your own deep learning workstation](https://www.topbots.com/deep-confusion-misadventures-in-building-a-machine-learning-server/).
- Your machine is running Ubuntu.
  - nvidia docker only works on Linux, unfortunately.
- You've installed an [NVIDIA driver](http://www.nvidia.com/Download/index.aspx).
- You've installed [docker](https://docs.docker.com/engine/installation/linux/docker-ce/ubuntu/).
- You've installed [nvidia docker](https://github.com/NVIDIA/nvidia-docker).

## How to use the Docker image

If you've followed the setup:

1. Enter `fastai` into a terminal.
2. Enter `j8` into the container's terminal that popped up as a result.

A Jupyter server will now be running in a fastai environment with all of fastai's dependencies.

## Setup

1\. Define your code and data paths by entering the following in a terminal:

```
CODE=~/code
DATA=~/data
```

2\. Clone the fast.ai repo onto your host machine by entering the following in a terminal:

```

git clone https://github.com/fastai/fastai.git $CODE/fastai
mkdir $DATA
wget http://files.fast.ai/data/dogscats.zip -O $DATA/dogscats.zip
unzip -q ~/data/dogscats.zip -d $DATA
```

3\. Spawn a docker container by entering the following in a terminal:

`docker run --runtime=nvidia --init -it -p 8888:8888 -v $CODE:/code -v $DATA:/data -w="/code/fastai" mwksmith/fastai`

4\. Start the Jupyter notebook server by entering the following alias into the container's terminal:

`jupyter_notebook_GPU_0_PORT_8888`

or

`j8`

5\. Authenticate your browser by opening the Jupyter notebook link that you see in the output (e.g. right click and press Open Link). It should look like this (your token value will be different): http://localhost:8888/?token=e73df183317f4d9283ab79fcd2b696220557db843036a1ff

6\. Open a Jupyter notebook in your browser and enjoy the fast.ai library.

---

You can open the notebook for lesson 1 by going to: http://localhost:8888/notebooks/courses/dl1/lesson1.ipynb

---

To allow for faster access next time, enter the following into your host machine's terminal, only once:

`echo "alias fastai=\"docker run --runtime=nvidia --init -it -p 8888:8888 -v $CODE:/code -v $DATA:/data -w="/code/fastai" mwksmith/fastai\"" >> ~/.bashrc`

`source ~/.bashrc`

---

## Explanation

- `docker run`: Runs a container from a Docker image. A container is an instance of a Docker image, like an object is an instance of a class. A Dockerfile defines a Docker image, like a class definition defines a class.

- `--runtime=nvidia`: Gives the container access to the host's NVIDIA driver.

- `--init`: I don't really understand this option, but it's needed to make Jupyter notebooks run when a container is spawned. If you're curious, here are [the docs](https://docs.docker.com/engine/reference/run/#specify-an-init-process).
- `-it`: Creates a terminal for the container that the user on the host can interact with.

- `-rm`: Removes the container once it exits. Otherwise inactive containers pile up.

- `-p 8888:8888`: Binds the host's 8888 port to the container's 8888 port, allowing the host to access the container's Jupyter server through the host's Internet browser.

- `-v $CODE:/code` and `-v $DATA:/data`: Gives the container access to the host's `$CODE` and `$DATA` directories and calls them "/code" and "/data" from the container's perspective. **Note: any changes to the files, either made by the host or the container, will change those files from both the host's perspective and the container's perspective.**

- `-w="/code/fastai"`: Sets the working directory of the container, from the container's perspective.

- `mwksmith/fastai`: Specifies the ID of the Docker image on [Docker Hub](https://hub.docker.com/). Docker Hub is like GitHub but for Docker images instead of code. The repository's name is "[mwksmith](https://hub.docker.com/r/mwksmith/portraitseg/)" and the Docker image's name is "[fastai](https://hub.docker.com/r/mwksmith/portraitseg/)". `docker run` will download the Docker image if it's not already on disk.

For more information, see [the Docker run reference](https://docs.docker.com/engine/reference/run/) and [the Docker run options list](https://docs.docker.com/engine/reference/commandline/run/).

If you'd rather talk to a person, you can [create a GitHub issue on this repo](https://github.com/MattKleinsmith/dockerfiles/issues/new), or post on [forums.fast.ai](forums.fast.ai) and put "@Matthew" in your post so I get an email.

`jupyter_notebook_GPU_0_PORT_8888` is defined in the container's ~/.bashrc:

```
alias jupyter_notebook_GPU_0_PORT_8888=\
      "ln -sf /data /code/fastai/courses/dl1/ && \
       source activate fastai && \
       CUDA_VISIBLE_DEVICES=0 jupyter notebook --port=8888 --allow-root"
```

## Extending the Dockerfile and building your own image

Hamel Husain wrote [a great Docker tutorial](https://towardsdatascience.com/how-docker-can-help-you-become-a-more-effective-data-scientist-7fc048ef91d5) and it'll teach you what you need to know to create your own Dockerfiles and Docker images.

If you have any questions, let me know by [creating a GitHub issue on this repo](https://github.com/MattKleinsmith/dockerfiles/issues/new), or posting on [forums.fast.ai](forums.fast.ai) and putting "@Matthew" in your post so I get an email.
