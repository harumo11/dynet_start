# dynet start documentation in Japanese

## Installation for **Ubuntu 18.04**

### 1. cuda 
(お使いのPCがnvidiaのグラボを積んでない場合は，この章は飛ばしましょう)
#### Nvidia driver

Original web pages are [here](https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html#installdriver)

Download file and details are
[netwok driver](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1804&target_type=debnetwork)

```
sudo dpkg -i cuda-repo-ubuntu1804_10.1.105-1_amd64.deb 
sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/7fa2af80.pub
sudo apt update
sudo apt install cuda
```

After driver installation, try reboot.

```
sudo reboot
```

再起動の後，問題なくインストールできたかどうか確かめましょう．

```
nvidia-smi
```

下記のような表示が出れば成功です．

```
Tue May  7 15:46:06 2019       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 418.56       Driver Version: 418.56       CUDA Version: 10.1     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  GeForce GTX 108...  Off  | 00000000:01:00.0  On |                  N/A |
| 23%   29C    P8    12W / 250W |    256MiB / 11175MiB |      1%      Default |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID   Type   Process name                             Usage      |
|=============================================================================|
|    0      1269      G   /usr/lib/xorg/Xorg                           162MiB |
|    0      1446      G   /usr/bin/gnome-shell                          90MiB |
+-----------------------------------------------------------------------------+
```

#### cuDNN

[詳細](https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html#installdriver)

nvidiaアカウントが必要なので作成してダウンロードしましょう.

[ダウンロードページ](https://developer.nvidia.com/cudnn)
に飛んでcuDNNをダウンロードしましょう．
いくつかダウンロードできるものがありますが，

1. cuDNN Runtime Library for Ubuntu18.04
2. cuDNN Developer Library for Ubuntu18.04
3. cuDNN Code samples and User Guide for Ubuntu18.04

をダウンロードしましょう．
下記のコマンドでインストールしましょう．

```
sudo dpkg -i libcudnn7_7.5.1.10-1+cuda10.1_amd64.deb
sudo dpkg -i libcudnn7-dev_7.5.1.10-1+cuda10.1_amd64.deb
sudo dpkg -i libcudnn7-doc_7.5.1.10-1+cuda10.1_amd64.deb
```

正常にインストールが完了したかどうかを確認するために
試しにディープラーニングを実行してみましょう．

```
cp -r /usr/src/cudnn_samples_v7/ $HOME
cd  $HOME/cudnn_samples_v7/mnistCUDNN
make clean && make
make clean && make
```

うまく実行できると結果が表示されます．
私の環境(Geforce GTX1080Ti)では下記のように表示されました．
最後に`Test passed!`と表示されていればうまくインストールが完了した証拠です．

```
cudnnGetVersion() : 7501 , CUDNN_VERSION from cudnn.h : 7501 (7.5.1)
Host compiler version : GCC 7.4.0
There are 1 CUDA capable devices on your machine :
device 0 : sms 28  Capabilities 6.1, SmClock 1582.0 Mhz, MemSize (Mb) 11175, MemClock 5505.0 Mhz, Ecc=0, boardGroupID=0
Using device 0

Testing single precision
Loading image data/one_28x28.pgm
Performing forward propagation ...
Testing cudnnGetConvolutionForwardAlgorithm ...
Fastest algorithm is Algo 1
Testing cudnnFindConvolutionForwardAlgorithm ...
^^^^ CUDNN_STATUS_SUCCESS for Algo 0: 0.066368 time requiring 0 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 1: 0.077664 time requiring 3464 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 2: 0.102400 time requiring 57600 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 7: 0.110400 time requiring 2057744 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 5: 0.132096 time requiring 203008 memory
Resulting weights from Softmax:
0.0000000 0.9999399 0.0000000 0.0000000 0.0000561 0.0000000 0.0000012 0.0000017 0.0000010 0.0000000 
Loading image data/three_28x28.pgm
Performing forward propagation ...
Resulting weights from Softmax:
0.0000000 0.0000000 0.0000000 0.9999288 0.0000000 0.0000711 0.0000000 0.0000000 0.0000000 0.0000000 
Loading image data/five_28x28.pgm
Performing forward propagation ...
Resulting weights from Softmax:
0.0000000 0.0000008 0.0000000 0.0000002 0.0000000 0.9999820 0.0000154 0.0000000 0.0000012 0.0000006 

Result of classification: 1 3 5

Test passed!

Testing half precision (math in single precision)
Loading image data/one_28x28.pgm
Performing forward propagation ...
Testing cudnnGetConvolutionForwardAlgorithm ...
Fastest algorithm is Algo 1
Testing cudnnFindConvolutionForwardAlgorithm ...
^^^^ CUDNN_STATUS_SUCCESS for Algo 1: 0.019584 time requiring 3464 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 2: 0.032768 time requiring 28800 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 7: 0.091136 time requiring 2057744 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 5: 0.092064 time requiring 203008 memory
^^^^ CUDNN_STATUS_SUCCESS for Algo 4: 0.094176 time requiring 207360 memory
Resulting weights from Softmax:
0.0000001 1.0000000 0.0000001 0.0000000 0.0000563 0.0000001 0.0000012 0.0000017 0.0000010 0.0000001 
Loading image data/three_28x28.pgm
Performing forward propagation ...
Resulting weights from Softmax:
0.0000000 0.0000000 0.0000000 1.0000000 0.0000000 0.0000714 0.0000000 0.0000000 0.0000000 0.0000000 
Loading image data/five_28x28.pgm
Performing forward propagation ...
Resulting weights from Softmax:
0.0000000 0.0000008 0.0000000 0.0000002 0.0000000 1.0000000 0.0000154 0.0000000 0.0000012 0.0000006 

Result of classification: 1 3 5

Test passed!
```

最後にインストール検証テストで使用したディレクトリはもう必要ないので削除しておきましょう．

```
cd ~/
rm -rf cudnn_samples_v7
```

### 2.Eigen
(お使いのPCがnvidiaのグラボを積んでない場合は，この章を実行しましょう)

[Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)から
最新バージョンのEigenをダウンロードしましょう．

ダウンロードしたEigenのディレクトリに移動して下記のコマンドを打ちましょう．

```
mkdir build_dir
cd build_dir
cmake ../
sudo make install
```

### 3. dynet

**cudaとeigenの両方をインストールしておくことが必須条件です．**
**cudaバックエンドを使用する場合でもeigenをインストールしておく必要があります．**

```
git clone https://github.com/clab/dynet.git
cd dynet
mkdir build
cd build
```

バックエンドがcudaの場合はこちら

```
cmake .. -DENABLE_BOOST=ON -DENABLE_CPP_EXAMPLES=ON -DEIGEN3_INCLUDE_DIR=/usr/local/include/eigen3 -DCUDNN_ROOT=/usr/local/cuda/ -DBACKEND=cuda
```

バックエンドがeigenの場合はこちら

```
cmake .. -DENABLE_BOOST=ON -DENABLE_CPP_EXAMPLES=ON -DEIGEN3_INCLUDE_DIR=/usr/local/include/eigen3
```

```
make -j10
sudo make install
```

最後に，インストールの確認のために簡単なディープラーニングのプログラムを実行してみましょう．

```
cd example
./xor-autobatch
```

エラーなく実行された完了です．

はい，これでインストールは完了です．
おつかれさまでした．

