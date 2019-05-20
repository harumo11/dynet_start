# APIリファレンス

## Core functionalities

### Computation Graph
`ComputationGraph`はdynetの主力です．
`Computation graph`は計算をシンボル的に表現します．そしてその計算は遅延評価されます．
つまり，計算はユーザーが陽にそれを命令したときにだけ実行されます(`forward`計算をトリガーとして)
スカラー値(例えば，loss valueなど)を評価するための`Expression`は`backword`計算をトリガー
として使用されます．そして，パラメータに関して勾配が計算されます．

### Nodes
ノードは`ComputationGraph`の構成要素です．
エンドユーザはノードと直接的にやり取りするのではなく`Expression`を通して
やり取りを行います．
しかしながら，新しい操作の実装のためには以下で説明するノードクラスを継承した
サブクラスを新しく作る必要があります．

### Parameters and Model
パラメータは最適化されるべき数値です．計算モジュールが自身のパラメータを有する
Torchのシステムとは対象的に,dynetではパラメータはただのパラメータです．

疎なアップデートを処理するために，２つのパラメータクラスが存在します．

- `Parameters`はパラメータのvectorやmatrixを表現します．（最終的により高い次元のテンソル）
  これらは密に更新されます．
- `LookupParameters`はvectorのテーブルを表現し，離散オブジェクトの集合を組み込むために
  使用されます．これらは疎に更新されます．

### Tensor
`Tensor`オブジェクトはC++のデータ構造とEigenのTensorsクラスの間の橋渡しをするために
使用されます．

具体的には，エンドユーザは`.value()`をExpressionクラス上で呼んだ後に`tensor`オブジェクト
を得ることができます．あなたは以下で説明されているような関数を使用して，それらのtensorを`float`や
`float`のarrayに変換することが可能です．また，それらの値を保存や読み込むことも可能です．

逆に，ローレベルのノードを実装するときには(例えば新しいoperationsのために)
あなたはEigenのtensorsからdynetのtensorsに逆変換する必要があります．これは
Eigenのtensorsを使用すると効率的に計算を実行できるからです．

### Dimensions
`Dim`クラスはtensorの形状に関する情報を有しています．
[正統派でないデザイン](https://dynet.readthedocs.io/en/latest/unorthodox.html)
で説明されているように，dynetにおいてdimensionsは一般的な次元+バッチの次元を
表現しています．このようにすることで計算の透明性を高めています．

## Operations
### Operation Interface
下記の関数はdynetの`Expressions`を定義します．
`Expression`は`ComputationGraph`を作成するときに使用される様々な関数へのインターフェースとして
使用されます．

### Input Operation
これらの操作はあなたに何かをcomputationgraphに投入することを可能にします．
それは簡単なfloat型のscalar/vector/matrixの入力か,
もしくはdynet parameter objectからのパラメータの入力のどちらかです．
それらは全てcomputationgraphに入力として渡される必要があるので
あなたはこのgraphがどの特定の計算に使用されているかを知ることができます．

### Arithmetic Operations

!!! note "Arighmetic"
	Arithmeticは算術，数学，算数という意味

これらの操作はComputationGraph内の値に対して基本的な算術演算を実行します．

### Probability/Loss Operations
これらの操作は確率を計算するために使用されます．
もしくはトレーニング中に損失関数を計算するために使用されます．

### Flow/Shaping Operations
これらの操作は`ComputationGraph`を流れる情報の流れを制御します．
もしくは，graph内で使用されているvector/tensorhの形を制御します．

### Noise Operations
これらの操作は`ComputationGraph`にノイズを加えるために使用されます．
この目的は学習をより頑強なものにするためです．

### Tensor Operations
これらの操作はより高次元のテンソルに対する操作を実行するために使用されます．

!!! note
	**contraction operation**をコンパイルすることはCUDAにおいて多くの時間を要します．
	CPU実装を使用しているときはデフォルト設定でコンパイルされます．
	もし，あなたがこの操作を必要としているならば，
	ソースコードの
	[この行](https://github.com/clab/dynet/blob/master/dynet/nodes-contract.cc#L11)を
	コメント解除してコンパイルしてください．

### Linear Algebra Operations
これらの操作は様々な一般的な線形代数の操作を実行します．

### Convolution Operations
これらの操作は`convolution`に関係しています．

### Normalization Operation
これはバッチ正規化やそれに関係したものを含みます．

### Device Operation
これらの操作はデバイスに関係したものです．

## Builders
### Softmax Builders
ソフトマックス層を作成するためのインターフェースです．

**class** SoftmaxBuilder

```C++
#include <cfsm-builder.h>
```

## Optimizers
あなたがパラメータを調整するための様々な最適化関数(optimizers)
