    1  ls
    2  cat nvidia_install.log 
    3  mkdir Lab
    4  ls
    5  cd Lab/
    6  ls
    7  pwd
    8  ls
    9  nvcc
   10  apt install nvidia-cuda-toolkit
   11  apt unlock
   12  apt install nvidia-cuda-toolkit
   13  sudo rm /var/lib/apt/lists/lock
   14  sudo rm /var/cache/apt/archives/lock
   15  apt install nvidia-cuda-toolkit
   16  sudo apt install nvidia-cuda-toolkit
   17  reboot
   18  ls
   19  sudo apt install nvidia-cuda-toolkit
   20  sudo dpkg --configure -a
   21  sudo apt install nvidia-cuda-toolkit
   22  install cuda
   23  apt install cuda
   24  dpkg -l | grep '^rc' | awk '{print $2}' | sudo xargs dpkg --purge
   25  export CUPIT_LIB_PATH=${OPT_PATH}/cuda/toolkit_9.0/cuda/extras/CUPTI/lib64
   26  export LD_LIBRARY_PATH=${CUPIT_LIB_PATH}:$LD_LIBRARY_PATH
   27  sudo apt install nvidia-cuda-toolkit
   28  sudo apt-cache search cuda-command-line-tool
   29  sudo apt install cuda-command-line-tools-9-1
   30  sudo apt-cache search cuda-command-line-tool
   31  ls
   32  wget https://developer.nvidia.com/compute/cuda/9.2/Prod2/local_installers/cuda_9.2.148_396.37_linux
   33  sudo service lightdm stop
   34  vim /etc/modprobe.d/blacklist-nouveau.conf
   35  sudo update-initramfs -u
   36  sudo sh cuda_7.5.18_linux.run --override
   37  ls
   38  sudo sh cuda_9.2.148_396.37_linux --override
   39  sudo service lightdm start
   40  /usr/local/cuda-9.2/
   41  export PATH=$PATH:/usr/local/cuda-9.2/bin
   42  vim /etc/ld.so.conf
   43  /usr/local/cuda-9.2/bin/nvcc
   44  /usr/local/cuda-9.2/bin/nvcc -version
   45  /usr/local/cuda-9.2/bin/nvcc --version
   46  ls
   47  cd Lab/
   48  ls
   49  /usr/local/cuda-9.2/bin/nvcc -c -arch=sm_20 kernel.cu 
   50  /usr/local/cuda-9.2/bin/nvcc -c  kernel.cu 
   51  ls
   52  /usr/local/cuda-9.2/bin/nvcc  kernel.cu 
   53  ls
   54  ./a.out 
   55  history > command.sh
