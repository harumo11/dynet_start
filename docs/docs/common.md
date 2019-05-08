# 一般的な使い方

## 初期化・片付け

使用するメモリ量とデバイスを最初に指定

```
dynet::initialize();
```

指定するために`DynetParams`構造体に必要な値をセットして渡してやる

- `params.mem_descriptor = "2048"`

	foward-path, backword-path, パラメータで2048が3等分される

- `params.random_seed = 0`
	
	０なら適当に初期化

- `params.weight_decay = 0.0`

	L2正則化の強さ, 未だにL2正則化は使うのか？

- `params.shared_parameters = false`

	パラメータをshared memory上に置くかどうか

下記はcudaを使用する場合のみ設定
下記のように設定するとメモリに余裕があるGPUを勝手に１個選んで使用する

```
params.ngpus_requested = false;
params.ids_requested   = false;
params.requested_gpus  = -1;

const unsigned int MAX_GPUS = 1024;	//適当に大きな値，挿してあるGPUの枚数以上の値にすればよい
params.gpus_mask       = std::vector<int>(MAX_GPUS, 0);
```

