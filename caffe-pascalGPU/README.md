We'll need CUDA 8.0 to use Caffe with NVIDIA's Pascal GPUs (GTX 10XX).
Caffe needs to be built with the Pascal architecture setting:
```
  -DCUDA_ARCH_NAME="Manual" \
  -DCUDA_ARCH_BIN="52 60" \
  -DCUDA_ARCH_PTX="60" \
```
https://github.com/NVIDIA/nvidia-docker/issues/208
