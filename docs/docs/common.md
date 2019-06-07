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

# 用語の解説

## 主なプレイヤー
 
- Computation Graph : 計算グラフ
- Expressions (計算グラフ内のノードのこと) : 式
- Parameters : パラメータ
- Model (パラメータの集合のこと） : モデル
- Trainer : トレーナー

## Computation Graph and Expression

1. 基本的なExpression（ノード）を作成する
2. それら（ノード）をoperatorを使って組み合わせる

## Model and Parameters

- Parametersの実態はvectorやmatrixで，最適化を施される変数です
- Modelはパラメータの集合です
- ParametersはComputation graphより寿命が長い

```C++
const int HIDDEN_SIZE = 8;
dynet::ParameterCollection model;
// Create parameter
dynet::Parameter p_W = model.add_parameters({HIDDEN_SIZE, 2});
// Convert params to expression(node) and add to the graph
dynet::Expression W = dynet::parameter(cg, p_W);
```

## Parameter Initialization

```c++
dyent::Parameter p_W = model.add_parameters({HIDDEN_SIZE, 2}, dynet::GlorotInitializer());
dyent::Parameter p_W = model.add_parameters({HIDDEN_SIZE, 2}, dynet::NormalInitializer(0,1));
```

## Trainer and backdrop

1. Trainerをmodel(ParameterCollection)を与えて初期化してください
2. ComputationGraph.backward()をスカラーノードから実行し勾配を求めてください．
3. Trainer.update()を読み出すことでmodelのパラメータを勾配を用いて更新してください．

## Training with DyNet

1. model(ParameterCollection)を作成し，Parametersを加え，Trainerを作成してください
2. それぞれの訓練サンプルごとに
	1. 誤差を求めるためにComputationGraphを作成してください
	2. 前方計算をしてください（誤差を計算）
	3. バックプロパゲーションを計算してください（勾配を計算）
	4. パラメータを更新


