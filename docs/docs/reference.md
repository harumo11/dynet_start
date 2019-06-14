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

	void clear()
	: 計算グラフをリセットし，新規に作成された状態にします．

	void checkpoint()
	: チェックポイントを作成します．

	void revert()
	: 最後のチェックポイントときの状態に戻ります．

	const Tensor& forward(VariableIndex i)
	: 事前に計算されている全ての値を無視して最初のノードから指定されたノードへの完全な`forward`（順伝播）を実行．

		Return
		: 実行したあとの最後のノードの値

		Parameters
		: - i: `forword`（順伝播）が計算された後の最新のノードの変数インデックス

	const Tensor& incrimental_forward(const Expression& last)
	: `forward`（順伝播）を最後に計算されたノードから指定されたノードまで計算する．
	もしあなたが，ノードを追加し，そして新しい部分のみを評価したいなら，その場合に便利です．

		Return
		: 実行した後の`last Expression`の値

		Parameters
		: - `last`: 順伝播を計算する必要がある式(Expression)

	const Tensor& incrimental_forward(VariableIndex i)
	: `forward`（順伝播）を最後に計算されたノードから指定されたノードまで計算する．
	もしあなたが，ノードを追加し，そして新しい部分のみを評価したいなら，その場合に便利です．

		Return
		: 実行した後の最後のノードの値

		Parameters
		: - `last`: 順伝播を計算する必要がある式(Expression)

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

	void backward(VariableIndex i, bool full-false)
	: ノード`i`から逆伝播を実行します．（ノードiはすでに評価されていると必要があります)
	`full`パラメータは全てのノード(`true`)に対して傾きを計算するかnon-constantなノードだけ計算を行うべきかどうかを指定します．
	デフォルトでは下記の条件を除いてはノードは普遍です.

		1. パラメータノード
		2. ノードがnon-constantノードに依存している

		したがって，定数の関数と入力は定数として考えられます．

		傾きを取得したい場合は`full`を`true`にしてください．
		デフォルトではバックプロパゲーション時に勾配に影響を与えないノードを無視するように`false`に設定されています．

		Parameters
		: - `i`: どのノードから計算を始めるかを指し示すノードのインデックス
		  - `full`: 全ての勾配を計算するかどうか（定数ノードを含めるかどうか)
		    もしあなたが傾きを検索したいならこれを有効にしてください．
			デフォルト設定では逆伝播に関係ない勾配計算を省き効率を高めるために,
			これはオフになっています，
	
	void print_graphviz()
	: デバッグのために使用されます

	unsigned get_id() const
	: 計算グラフ（ネットワーク）のユニークIDを取得します．
	このIDは計算グラフが各エポックごとに作成され，１づつ増加します．

		Return
		: グラフID

	
		


### Nodes
ノードは`ComputationGraph`の構成要素です．
エンドユーザはノードと直接的にやり取りするのではなく`Expression`を通して
やり取りを行います．
しかしながら，新しい操作の実装のためには以下で説明するノードクラスを継承した
サブクラスを新しく作る必要があります．

struct Node
: `#include<dynet.h>`
計算ノードに関する情報が含まれています．:引数，出力値，関数に関する出力の勾配．
このクラスは新しい操作(operation)の実装時には継承される必要があります．
例はnodes.ccを見てください．新しい式の操作(operation)は`Node`から作成されます．
例はexpr.h/expr.ccを見てください．

**Public Functions**

virtual std::string as_string(const std::vector<std::string>& args) const = 0
: デバッグに重要な情報を返します．詳しくはnodes-conv.ccを見てください．

	Return
	: ノードの説明（string)

	Parameters
	: - `args`: 引数の説明(string)

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

struct Expression
: `#include<expr.h>`
ExpressionはDynetの計算グラフを構成するブロックです．

	**Public Functions**
	
	Expression(ComputationGraph* *pg, VariableIndex i)
	: コンストラクタ．操作（Operation)を作成するときに使用します．
	
		Parameters
		: - `pg`: 計算グラフのポインタ
		  - `i` : 変数インデックス
	
	const Tensor& value() const
	: expression(式)の値を得ます．
	計算グラフが利用可能でない場合はruntime_errorが投げられます．

		Return
		: テンソル形式の式の値

	!!! note
		オリジナルではThrows a runtiime_errorとタイポがあったのでマープルしておく


	const Tensor& gradient() const
	: 式(Expression)の傾きを得る
	計算グラフが利用可能でない場合はruntime_errorが投げられます．

		Return
		: テンソル形式の値

	この関数を呼ぶ前に`backword`を下流の式で呼び出してください．

	もし，式が定数を表現しているならば（これは関数がパラメータを有していないことを意味している）
	dynetは効率のためにその勾配を計算しようとはしません．`backword`の`full=true`引数を加えることによって
	手動で強制的に計算をさせることができます．


	const Dim& dim() const
	: 表現の次元を求めます．
	計算グラフが利用可能でない場合はruntime_errorが投げられます．

		Return
		: 表現(expression)の次元

### Input Operation
これらの操作はあなたに何かをcomputationgraphに投入することを可能にします．
それは簡単なfloat型のscalar/vector/matrixの入力か,
もしくはdynet parameter objectからのパラメータの入力のどちらかです．
それらは全てcomputationgraphに入力として渡される必要があるので
あなたはこのgraphがどの特定の計算に使用されているかを知ることができます．

Expression dynet::input(ComputationGraph& g, real s, Device* devide = dynet::default_device)
: scalar input
スカラ値を表現するexpression(式）を作成します．

	Return
	: sを表現するExpression

	Parameters
	: - `g`: 計算グラフ
	  - `s`: 実数
	  - `device`: 入力値が置かれるデバイス，default_deviceがデフォルトで選択される．

Expression dynet::(ComputationGraph& g, const real* ps, Device* device = dynet::default_device)
: 更新可能なスカラー値の入力

	スカラ値`*ps`を表現するExpression(式）を作成する．もし，`*ps`が変更され，かつ，計算グラフが再計算された場合，次の順伝播(forward)はその値を反映します．

	Return
	: `*ps`を表すExpression（式）

	Parameters
	: - `g`: 計算グラフ
	  - `ps`: 実数のポインタ
	  - `device`: 入力値が置かれるデバイス，default_deviceがデフォルトで選択される．
	
Expression dynet::input(ComputationGraph& g, const Dim& d, const std::vector<float>& data, Device* divice = dynet::default_device)
: ベクター，行列，テンソルの入力

	vecotr, matrix, tensorの入力を表現するExpression(式）を作成する．
	入力の次元は`d`によって定義される．よって例えば，```input(g, {50}, data)```:
	という引数は要素数50のベクターを表す．```input(g, {50, 30}, data)```:という
	引数は50x30の行列を表現している．これにより任意の次元を表すことができます．
	この関数はミニバッチに分割された入力の読み込みにも使用できます．
	たとえば，１つのミニバッチに１０個の教師データ(サイズは50x30)があった場合，
	```input(g, Dim({50, 30}, 10) data)```とすることで表現できます．
	データベクター**data**は入力で使用されるための値で満たされており，
	列優先ベクトル形式です．`d`は全てのデータの個数の掛け算で表される要素のサイズです．

	Return
	: データを表現したExpression(式)

	Parameters
	: - `g`: 計算グラフ
	  - `d`:入力行列の次元
	  - `data`:データ点(一つのデータ）のベクター
	  - `device`: 入力値が置かれるデバイス，default_deviceがデフォルトで選択される．

	Expression dynet::input(ComputationGraph& g, const Dim& d, const std::vector<float>* padata, Device* device = dynet::default_device)
	: 更新可能なvector/matrix/tensorの入力
	vectorを参照渡しで渡す入力と似ています．しかし，この関数はポインタ渡しをすることによってデータを更新することが出来ます．

	Return
	: *pdataの表現のExpression(式)

	Parameters
	: - `g`: 計算グラフ
	  - `d`: 行列の次元
	  - `pdata`: ポインタ渡しの(更新可能な)データ点のベクトル
	  - `device`: 入力値が置かれるデバイス，default_deviceがデフォルトで選択される．

Expression dynet::input(ComputationGraph& g, const Dim& d, const std::vector<unsinged int>& ids, const std::vector<float>& data, float defdata = 0.f, Device* device = dynet::default_device)
: スパース（疎）なベクトルの入力
この操作は入力としてインデックスと値がペアになったスパース行列を取ります．
これはベクターの参照渡しの標準入力と全く同じです．しかし，指定していない全ての値を
**defdata**に設定し，その他の入力の値を適切にリセットしてください．

	Return
	: dataの表現のExpression（式）

	Parameters
	: - `g`: 計算グラフ
	  - `d`: 入力行列の次元
	  - `ids`: 更新するデータ点のインデックス
	  - `defdata`: 未指定のデータ点を設定するためのデフォルトデータ
	  - `device`: 入力値が置かれるデバイス，default_deviceがデフォルトで選択される．

Expression dynet::one_hot(ComputaionGraph&g , unsigned int d, unsigned int idx, Device* device = dynet::default_devaice)
: ワンーホットベクトル
	ワンホット表現のベクトルを作成します．

	Return
	: データを表現したExpression(式）

	Parameters
	: - `g`: 計算グラフ
	  - `d`: 入力ベクトルの次元
	  - `idx`: 1をセットしたいインデックス
	  - `device`: 入力値が置かれるデバイス，default_deviceがデフォルトで選択される．

Expression dyent::one_hot(ComputationGraph& g, unsigned int d, const std::vector<unsigned int>& ids, Device* device = dynet::default_device)
: バッチ用のワンホットベクトル

	この操作はワンホットベクトルのバッチを作成します．`ids`の大きさによりバッチサイズを決定します．

	Return
	: dataを表現したExpression

	Parameters
	: - `g`: 計算グラフ
	  - `d`: 入力ベクトルの次元
	  - `ids`: １を設定したいインデックス，バッチエレメントごとに一つ
	  - `device`: 入力値が置かれるデバイス，default_deviceがデフォルトで選択される．

Expression dynet::parameter(ComputationGraph &g, Parater p)
: パラメータを読み込みます．
計算グラフの中にパラメータを読み込み反映します．

	Return
	: パラメータpを表現したExpression(式)

	Parameters
	: - `g`: 計算グラフ
	  - `p`: 読み込まれるパラメータオブジェクト

Expression dynet::parameter(ComputationGraph& g, LookupParameter lp)
: ルックアップパラメータを読み込みます．

	ルックアップパラメータの完全なテンソルを計算グラフに読み込みます．
	通常，ルックアップパラメータには`lookup()`関数を使用してアクセスします．
	しかし，いくつかの場合では，何らかの理由によりルックアップパラメータ全部のパラメータにアクセスしたいことがあります．
	このような場合には，返されたテンソルの最初の次元は`lookup()`関数を読んだ場合に示す次元と同じになります．
	そして，最後の次元のサイズはボキャブラリのサイズと等しくなります．

	Return
	: lpが表現されたExpression（式）

	Parameters
	: - `g`: 計算グラフ
	  - `lp`: 読み込まれるLookupParametersオブジェクト

Expression dynet::const_parameter(ComputationGraph& g, Parameter p)
: 変更不可なパラメータの読み込み
	パラメータを計算グラフに読み込みます．しかし，パラメータアップデートが行われた際に
	アップデートを防ぐことができます．

	Return
	: 定数pが表現されたExpression(式)

	Parameters
	: - `g`: 計算グラフ
	  - `p`: 読み込まれるパラメータオブジェクト

Expression dynet::const_parameter(ComputatinGraph& g, LookupParameter lp)
: ルックアップパラメータを読み込みます．
ルックアップパラメータを計算グラフの中に読み込みます．しかし，パラメータ更新による変更
を避けることができ，パラメータは変更されません．

	Return
	: lpが表現されたExpression（式）

	Parameters
	: - `g`: 計算グラフ
	  - `lp`: 読み込まれるLookupParameterオブジェクト

Expression dynet::lookup(ComputationGraph& g, LookupParameter p, unsigned index)
: パラメータを調べる．
インデックスを参照してパラメータを調べます．そして，それを計算グラフの中に読み込みます．

	Return
	: p[index]が表現されたExpression(式）

	Parameters
	: - `g`: 計算グラフ
	  - `p`: 読み込まれるLookupParameterオブジェクト
	  - `index`: ルックアップパラメータpの中にあるパラメータのインデックス


Expression dynet::lookup(ComputatinGraph& g, LookupParameter p, const std::vector<unsigned>& indics)
: パラメータを調べます.
**ミニバッチバージョン**の`lookup()`.結果のExpression(式）はミニバッチに対応したパラメータになります．
ここでは，バッチのi番目の要素はi番目の要素のインデックスによって指定された位置にあるパラメータに対応します．

	Return
	: p[indics[i]]を表現するi番目の要素のExpression

	Parameters
	: - `g`: 計算グラフ
	  - `p`: 読み込まれるLookupParameterオブジェクト
	  - `indices`: バッチのそれぞれの位置におけるパラメータのインデックス．

### Arithmetic Operations

!!! note "Arighmetic"
	Arithmeticは算術，数学，算数という意味

これらの操作はComputationGraph内の値に対して基本的な算術演算を実行します．

### Probability/Loss Operations
これらの操作は確率を計算するために使用されます．
もしくはトレーニング中に損失関数を計算するために使用されます．

#### Probability Operation

Expression dynet::softmax(const Expression &x, unsigned d = 0)
: ソフトマックス関数
  このソフトマックス関数は各列を正規化して，全ての値が０から１の間を取るように
  します．これには$\frac{e^{x_i}}{\sum_j e^{x_j}}$という式を利用します．

	Return
	: softmaxを計算した後のvectorかmatrix

	Parameters
	: - `x` : vectorかmatrix
      - `d` : 正規化する次元(デフォルト = 0)

Expression dynet::rectify(const Expression &x)
: 正規化線形関数(rectified linear function) **ReLU**
  要素ごとに正規化関数(ReLU)$y_i = \max(x_i, 0)$を実行します．

	Return
	: i番目の要素が$\max(x_i, 0)$である`Expression`

	Parameters
	: - `x` : 入力されるExpression

#### Loss Operation

Expression dynet::pickneglogsoftmax(const Expression &x, unsigned v)
: Negative softmax log likelihood
  この関数はスコアxのベクトルを取り込みます．そして，log softmaxを実行し
  -1を掛けます．そして，要素vに対応した確立を選びます．返り値は損失関数の`Expression`です．
  この関数はもしかすると集合から１つの要素を予測するニューラルネットのトレーニングにおいて
  最も有名な誤差関数かもしれせん．
	
	Return
	: softmaxを実行した後の要素`v`のnegative logの確立

	Parameters
	: - `x` : スコアのvector
	  - `v` : 損失を計算するための要素

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
