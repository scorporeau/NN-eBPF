# Implementing a Neural Network Using eBPF
## Rebuilding the Kernel
Compile the kernel to version 6.1.43 and enable eBPF.

Fetch and extract the kernel source code
```shell
$ wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.43.tar.xz
$ tar -xvf linux-6.1.43.tar.xz
```

Install the packages required for kernel compilation
```shell
$ sudo apt install make build-essential libncurses5-dev bison flex libssl-dev dwarves libelf-dev
```

Generate the kernel configuration file `.config`
```shell
$ cd linux-6.1.43
$ make mrproper && \
  make clean && \
  make menuconfig
```

Edit the following settings in the kernel configuration file `.config`
```shell
CONFIG_BPF=y
CONFIG_BPF_SYSCALL=y
CONFIG_BPF_JIT=y
CONFIG_TRACEPOINTS=y
CONFIG_BPF_LSM=y
CONFIG_DEBUG_INFO=y
CONFIG_DEBUG_INFO_BTF=y
CONFIG_LSM="bpf"
CONFIG_SYSTEM_TRUSTED_KEYS=“”
CONFIG_SYSTEM_REVOCATION_KEYS=""
```
Compile the kernel
```shell
$ make -j$(nproc)
$ sudo make modules_install -j$(nproc)
$ sudo make install -j$(nproc)
$ sudo reboot now
```
## Recompile libbpf and bpftool
Execute the following in the project directory
```shell
$ make -j$(nproc)
```

## Train the model and quantize model parameters
Navigate to the `src` directory, train the model, and the generated `mlp.th` file will store the model parameters
```shell
python3 mlp_train.py
```

Quantize the model parameters; the generated `mlp_params.bpf.h` file stores the quantization results
```shell
python3 mlp_quant.py ./mlp.th 16
```

The data used for training is stored in the `dataset` folder

## Compile the eBPF XDP code
Navigate to the `src` folder and run
```shell
$ make
```
This compiles the `.output/xdp` and `.output/hot_updating` directories

## Run and hot-update parameters
Navigate to the `src` folder and load the xdp program
```shell
$ sudo .output/xdp
```
After the xdp program loads successfully, open another terminal while still in the `src` folder and load the model parameters using hot updating
```shell
$ sudo .output/hot_updating
```
View real-time output
```shell
sudo cat /sys/kernel/debug/tracing/trace_pipe
```

## Test Instances
The `dataset-instance` folder contains normal and intrusion instances that can be run in real time to evaluate the model's real-time analysis performance

Translated with DeepL.com (free version)