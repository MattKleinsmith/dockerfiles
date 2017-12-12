#!/bin/bash

declare -a libs=("libopencv_core"
                 "libopencv_imgproc"
                 "libopencv_calib3d"
                 "libopencv_video"
                 "libopencv_features2d"
                 "libopencv_ml"
                 "libopencv_highgui"
                 "libopencv_objdetect"
                 "libopencv_contrib"
                 "libopencv_legacy")
for lib in "${libs[@]}"
do
    ln -sf /usr/lib/x86_64-linux-gnu/"$lib".so /usr/lib/x86_64-linux-gnu/lib"$lib".so
done



