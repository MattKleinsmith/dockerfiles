FROM ros:kinetic-ros-core

# install ros packages
RUN apt-get update && apt-get install -y \
    ros-kinetic-ros-base=1.3.0-0*

# Manual additions
RUN apt-get install -y \
    vim

# Remove APT lists to save space    
RUN rm -rf /var/lib/apt/lists/*

#### http://wiki.ros.org/ROS/Tutorials
## Enable ROS commands
RUN source /opt/ros/kinetic/setup.bash
## Create a ROS workspace
RUN rosws init /root/kinetic_workspace /opt/ros/kinetic
RUN source /root/kinetic_workspace/setup.bash
## Create a sandbox
RUN mkdir /root/kinetic_workspace/sandbox
RUN rosws set -y /root/kinetic_workspace/sandbox 
RUN source /root/kinetic_workspace/setup.bash
## Create a ROS Package
RUN roscd
RUN cd sandbox
RUN roscreate-pkg beginner_tutorials std_msgs rospy roscpp
## Build a ROS Package
RUN rosmake beginner_tutorials 
## Run ROS core
RUN roscore


# # Running ros core
# http://wiki.ros.org/docker/Tutorials/Docker#Running_ROS_containers
# docker run -it ros
# roscore

# # Ensuring commands like "rostopic" are found
# docker ps
# docker exec -it nostalgic_morse bash
# source /opt/ros/kinetic/setup.bash
# rostopic list

# # Creating a ROS package
# 
