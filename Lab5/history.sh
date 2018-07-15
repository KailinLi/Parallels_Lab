    1  wget https://developer.nvidia.com/compute/cuda/9.2/Prod2/local_installers/cuda_9.2.148_396.37_linux
    2  ls
    3  sudo sh cuda_9.2.148_396.37_linux --override
    4  sudo service lightdm stop
    5  sudo add-apt-repository ppa:graphics-drivers/ppa
    6  sudo apt-get purge nvidia-cuda*
    7  sudo apt-get purge nvidia-*
    8  sudo service lightdm stop
    9  vim /etc/modprobe.d/blacklist-nouveau.conf
   10  sudo update-initramfs -u
   11  sudo sh cuda_9.2.148_396.37_linux --override
   12  export PATH=$PATH:/usr/local/cuda-9.2/bin
   13  vim /etc/ld.so.conf
   14  ldconfig
   15  pwd
   16  mkdir Lab
   17  cd Lab/
   18  ls
   19  /usr/local/cuda-9.2/bin/nvcc kernel.cu 
   20  ls
   21  ./a.out 
   22  sudo apt install libopencv-dev
   23  ls
   24  /usr/local/cuda-9.2/bin/nvcc cuda.cu 
   25  /usr/local/cuda-9.2/bin/nvcc cuda.cu `pkg-config --cflags --libs opencv`
   26  ls
   27  g++ pthread.cc -lpthread `pkg-config --cflags --libs opencv`
   28  ls
   29  ./a.out 
   30  /usr/local/cuda-9.2/bin/nvcc cuda.cu `pkg-config --cflags --libs opencv` -lopencv_gpu
   31  ls
   32  vim kernel.cu 
   33  /usr/local/cuda-9.2/bin/nvcc kernel.cu 
   34  ls
   35  nvcc -c cuda.cu
   36  ls
   37  /usr/local/cuda-9.2/lib64/
   38  g++ -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart cuda.cc cuda.o `pkg-config --cflags --libs opencv`
   39  nvcc -c cuda.cu
   40  g++ -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart cuda.cc cuda.o `pkg-config --cflags --libs opencv`
   41  export PATH=/usr/local/cuda/bin:$PATH
   42  export LD_LIBRARY_PATH=/usr/local/cuda/lib64:/usr/local/cuda/lib:$LD_LIBRARY_PATH
   43  g++ -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart cuda.cc cuda.o `pkg-config --cflags --libs opencv`
   44  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   45  ls
   46  ./a.out 
   47  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   48  ./a.out 
   49  ./a.out ./output.jpg 
   50  ls
   51  nvcc -c cuda.cu
   52  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   53  ./a.out output.jpg 
   54  ls
   55  g++ `pkg-config --cflags --libs opencv` handle.cc 
   56  g++ handle.cc `pkg-config --cflags --libs opencv`
   57  ls
   58  ./a.out output.jpg 
   59  vim handle.cc 
   60  g++ handle.cc `pkg-config --cflags --libs opencv`
   61  ./a.out output.jpg 
   62  ls
   63  vim handle.cc 
   64  g++ handle.cc `pkg-config --cflags --libs opencv`
   65  ./a.out output.jpg 
   66  history > history.sh
   67  nvcc --version
   68  nvcc -c cuda.cu
   69  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   70  ./a.out output.jpg 
   71  nvcc -c cuda.cu
   72  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   73  ./a.out output.jpg 
   74  nvcc -c cuda.cu
   75  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   76  ./a.out output.jpg 
   77  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   78  ./a.out output.jpg 
   79  nvcc -c cuda.cu
   80  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   81  ./a.out output.jpg 
   82  ls
   83  nvcc -c cuda.cu
   84  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   85  ./a.out output.jpg 
   86  nvcc -c cuda.cu
   87  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   88  ./a.out output.jpg 
   89  nvcc -c cuda.cu
   90  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   91  ./a.out output.jpg 
   92  nvcc -c cuda.cu
   93  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv`
   94  ./a.out output.jpg 
   95  g++ handle.cc `pkg-config --cflags --libs opencv`
   96  ./a.out output.jpg 
   97  g++ handle.cc -o handle `pkg-config --cflags --libs opencv`
   98  ./handle Sierra.jpg 
   99  nvcc -c cuda.cu
  100  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  101  ./cuda Sierra.jpg 
  102  nvcc -c cuda.cu
  103  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  104  ./cuda Sierra.jpg 
  105  vim cuda.cu
  106  nvcc -c cuda.cu
  107  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  108  ./cuda Sierra.jpg 
  109  vim cuda.cu
  110  nvcc -c cuda.cu
  111  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  112  ./cuda Sierra.jpg 
  113  vim cuda.cu
  114  nvcc -c cuda.cu
  115  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  116  ./cuda Sierra.jpg 
  117  nvcc -c cuda.cu
  118  vim cuda.cu
  119  nvcc -c cuda.cu
  120  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  121  ./cuda Sierra.jpg 
  122  vim cuda.cu
  123  nvcc -c cuda.cu
  124  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  125  ./cuda Sierra.jpg 
  126  g++ handle.cc -o handle `pkg-config --cflags --libs opencv`
  127  ./handle Sierra.jpg 
  128  ./cuda Sierra.jpg 
  129  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  130  ./handle Sierra.jpg 
  131  ./cuda Sierra.jpg 
  132  nvcc -c cuda.cu
  133  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  134  ./handle Sierra.jpg 
  135  ./cuda Sierra.jpg 
  136  vim cuda.cu
  137  nvcc -c cuda.cu
  138  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  139  ./cuda Sierra.jpg 
  140  vim cuda.cu
  141  nvcc -c cuda.cu
  142  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  143  ./cuda Sierra.jpg 
  144  vim cuda.cu
  145  nvcc -c cuda.cu
  146  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  147  ./cuda Sierra.jpg 
  148  vim cuda.cu
  149  nvcc -c cuda.cu
  150  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  151  ./cuda Sierra.jpg 
  152  vim cuda.cu
  153  nvcc -c cuda.cu
  154  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  155  ./cuda Sierra.jpg 
  156  vim cuda.cu
  157  nvcc -c cuda.cu
  158  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  159  ./cuda Sierra.jpg 
  160  vim cuda.cu
  161  nvcc -c cuda.cu
  162  g++ cuda.cc cuda.o -L/usr/local/cuda-9.2/lib64/ -lcuda -lcudart `pkg-config --cflags --libs opencv` -o cuda
  163  ./cuda Sierra.jpg 
  164  history > history.sh 
