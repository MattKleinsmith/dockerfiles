Caffe + NVIDIA's Pascal GPUs requires:
- CUDA 8.0
- Building Caffe with the Pascal architecture setting:

```
  -DCUDA_ARCH_NAME="Manual" \
  -DCUDA_ARCH_BIN="52 60" \
  -DCUDA_ARCH_PTX="60" \
```
https://github.com/NVIDIA/nvidia-docker/issues/208
