# APIリファレンス

!!! quote 
	[API Reference](https://dynet.readthedocs.io/en/latest/core.html)

## Core functionalities

### Computation Graph
`ComputationGraph`はdynetの主力です．
`Computation graph`は計算をシンボル的に表現します．そしてその計算は遅延評価されます．
つまり，計算はユーザーが陽にそれを命令したときにだけ実行されます(`forward`計算をトリガーとして).
スカラー値(例えば，loss valueなど)を評価するための`Expression`は`backword`計算をトリガー
として使用されます．そして，パラメータに関して勾配が計算されます．

int dynet::get_number_of_active_graph()
: 有効なグラフの数を得る．
これは０か１です．１以上のグラフを同時に作成できません．

	Return
	: 有効なグラフの数

unsigned dynet::get_current_graph_id()
: 現在有効なグラフのidを得ます．
これはグラフが古くなった(stale)かどうかをチェックするのに役立ちます．

	Return 
	: 現在のグラフのID

struct ComputationGraph
: `#include<dynet.h>`
   <br>ノードが`forward`と`backword`の中間値を表し，エッジが多変数関数を表す計算グラフ

	この事実を表現するために関数は複数の引数を取るでしょう．また，エッジは１つのheadと
	０，１，２，もしくはそれ以上のtailを持ちます.[有向グラフにおけるheadとtail](https://en.wikipedia.org/wiki/Directed_graph)
	(定数，入力，また，パラメータは引数が０の関数として表現されます．)
	例：$z=f(x,y)$という関数が与えらた，x,zとyはノードであり，そしてzノードを指し示す
	fを表すエッジがあったとすると，xとyはエッジのtailです．あなたは`ComputationGraph`の
	`backword`以外のメソッドの使用を必要とすべきではない．なぜならそれらの殆どは
	`Expression`クラスから直接利用可能であるため．

	**Public Functions**
	
	ComputaionGraph()
	: デフォルトコンストラクタ

	VariableIndex add_input(real s, Device *device)	
	: スカラ値の入力を加える.
	<br>
	計算グラフはユーザーのデータ構造から入力をもらい，それを計算に利用する．
		
		Return
		: 作られた変数のインデックス

		Parameters
		: - s: 実数
		  - device: 入力値がおかれるデバイス
	
	VariableIndex add_input(const Dim &d, const std::vector<float>& data, Device *device)
	: 複数次元の入力を加える
	この計算ネットワークはユーザーのデータ構造から入力を受取，それらを計算に利用する．
		
		Return
		: 作成された変数のインデックス

		Parameters
		: - d: 所望の入力の形
		 - data: 入力データ(1次元配列としての）
		 - data: それぞれのインデックスに対応したデータポイント
		 - device: 入力データが置かれるデバイス

	VariableIndex add_input(const Dim &d, const std::vector<float> *pdata, Device *device)
	: 複数次元の入力のポインタを加える.
	計算ネットワークはユーザのデータ構造から入力を受取り，それらを計算に利用する．
	
		Return
		: 作成された変数のインデックス

		Parameters
		: - d: 所望の入力の形
		  - pdata: 入力データへのポインタ（１次元配列としての）
		  - device: 入力値が置かれるデバイス

	VariableIndex add_input(const Dim &d, const std::vector<unsigned int>& ids, const std::vector<float> &data, Device* device float defdata=0.f)
	: スパースな入力を加える
	計算ネットワークはユーザのデータ構造から入力を受取り，それらを計算に利用する．
	オプションのデフォルト値を使って,計算ネットワークへの入力はスパース配列フォーマットで表現してください．

		Return
		: 作成された変数へのインデックス

		Parameters
		: - d: 所望の入力の形
		  - ids: 更新するデータポイントのインデックス
		  - data: それぞれのインデックスに対応したデータポイント
		  - device: 入力データが置かれるデバイス
		  - defdata: 未指定のデータポイントを設定するためのデフォルトデータ

		VariableIndex add_parameters(Parameter p)
		: 計算グラフにパラメータを加える
			
			Return
			: 作成された変数のインデックス

			Parameters
			: - p: 加えられたパラメータ

	VariableIndex add_parameters(LookupParameter p)
	: 計算グラフにルックアップパラメータの完全な行列を加える.

		Return
		: 作成された変数のインデックス

		Parameters
		: - p: 加えられるルックアップパラメータ

	VariableIndex add_const_parameters(Parameter p)
	: 計算グラフに**更新されない**パラメータを加える

		Return
		: 作成された変数のインデックス

		Parameters
		: - p: 加えられるパラメータ

	VariableIndex add_const_parameters(LookupParameter p)
	: **更新されない**ルックアップパラメータの完全な行列を計算グラフに加える

		Return
		: 作成された変数のインデックス

		Parameters
		: - p: 加えられるルックアップパラメータ

	VariableIndex add_lookup(LookupParameter p, const unsigned *pindex)
	: 計算グラフにルックアップパラメータ(シングル)を加える
	呼び出し元が所有しているインデックスのメモリ位置を指すには`pindex`を使用してください．

		Return
		: 作成された変数のインデックス

		Parameters
		: - p: 拾われるルックアップパラメータ
		: - pindex: ルックアップを指し示すポインタ

	VariableIndex add_lookup(LookupParameter p, unsinged index)
	: 計算グラフにルックアップパラメータを加える

		Return
		: 作成された変数のインデックス

		Parameters
		: - p: 拾われるルックアップパラメータ
		: - index: ルックアップを指すインデックス

	VariableIndex add_lookup(LookupParameter p, const std::vector<unsigned> *pindecs)
	: 計算グラフにルックアップパラメータ(複数)を加えましょう
	呼び出し元が所有しているインデックスのメモリ位置を指すには`pindecs`を使用してください．

		Return
		: 作成された変数のインデックス

		Parameters
		: - p: 拾われるルックアップパラメータ
		: - indeces: ルックアップを指すインデックス(複数)

	VariableIndex add_lookup(LookupParameter p, const std::vector<unsigned> &indices)
	: ルックアップパラメータを計算グラフに加えます.

		Return
		: 作成された変数のインデックス

		Parameters
		: - p: 拾われるルックアップパラメータ
		: - indeces: ルックアップを指すインデックス(複数)

	!!! note
		大量の省略あり，余裕があったら和訳しましょうね．


	void backward(const Expression &last, bool full = false)
	: 出力層から順々にバックプロパゲーションを実行します．
	`full`パラメータは全てのノード(`true`)に対して傾きを計算するかnon-constantなノードだけ計算を行うべきかどうかを指定します．
	デフォルトでは下記の条件を除いてはノードは普遍です.

		1. パラメータノード
		2. ノードがnon-constantノードに依存している

		したがって，定数の関数と入力は定数として考えられます．

		傾きを取得したい場合は`full`を`true`にしてください．
		デフォルトではバックプロパゲーション時に勾配に影響を与えないノードを無視するように`false`に設定されています．

	Parameters
	: - `last`: 勾配を計算する元の式（`Expression`)
	  - `full`: 全ての勾配を計算するかどうか（定数ノードを含めるかどうか)




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
