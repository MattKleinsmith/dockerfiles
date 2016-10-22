FROM ros:kinetic-ros-core

# install ros packages
RUN apt-get update && apt-get install -y \
                ros-kinetic-ros-base=1.3.0-0* \
                ros-kinetic-ros-tutorials \
                vim \
                module-init-tools \
        && rm -rf /var/lib/apt/lists/*

## http://wiki.ros.org/ROS/Tutorials
# Create a ROS workspace
RUN rosws init /root/kinetic_workspace /opt/ros/kinetic
# Create a sandbox
WORKDIR /root/kinetic_workspace/sandbox
RUN /bin/bash -c 'source /root/kinetic_workspace/setup.bash ; \
                  rosws set -y /root/kinetic_workspace/sandbox'
# Create and build a ROS Package
RUN /bin/bash -c 'source /root/kinetic_workspace/setup.bash ; \
                  roscreate-pkg beginner_tutorials std_msgs rospy roscpp ; \
                  rosmake beginner_tutorials' 
WORKDIR /root/kinetic_workspace
# Install the NVIDIA driver
ENV NVIDIA_DRIVER NVIDIA-Linux-x86_64-367.57.run
COPY $NVIDIA_DRIVER /tmp/$NVIDIA_DRIVER
RUN sh /tmp/$NVIDIA_DRIVER -a -N --ui=none --no-kernel-module; \
    rm /tmp/$NVIDIA_DRIVER
# Run ROS core
CMD ["roscore"]
