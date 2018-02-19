FROM ubuntu:16.04

RUN apt-get update --fix-missing && apt-get install -y \
    python3 \
    python3-pip \
    vim \
    wget

RUN pip3 install fire

RUN wget https://raw.githubusercontent.com/MattKleinsmith/dockerfiles/master/errors/fire/run.py

CMD ["python3", "run.py", "--x", "20180217_064501"]
