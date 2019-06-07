# XOR

## 論理ゲート

```
A----++++
      ++++++-----Z
B----++++
```

### AND(論理積）

```math
論理式: Z = AB
```

|A|B|Z|
|-|-|-|
|0|0|0|
|0|1|0|
|1|0|0|
|1|1|1|

### OR（論理和）

```math
論理式: Z = A+B
```

|A|B|Z|
|-|-|-|
|0|0|0|
|0|1|1|
|1|0|1|
|1|1|1|

### NOT（論理否定）

```
A-----+++++----Z
```

|A|Z|
|-|-|
|0|1|
|1|0|

### NAND (NOT of AND)

全てが１のときのみ０

|A|B|Z|
|-|-|-|
|0|0|1|
|0|1|1|
|1|0|1|
|1|1|0|

### XOR(eXclusive OR:排他的論理和）

不一致和（A != Bのとき１）

|A|B|Z|
|-|-|-|
|0|0|0|
|0|1|1|
|1|0|1|
|1|1|0|

## Deeplearningから見たXOR

適当な入力ベクトルxを入れると，それが1か0かを判定するネットワークを作成する．

### トレーニングデータ

$$
\begin{equation}
X= 
\begin{bmatrix}
0 & 0 \\
0 & 1 \\
1 & 0 \\
1 & 1 \\
\end{bmatrix}
\end{equation}
$$

$$
\begin{equation}
y= 
\begin{bmatrix}
0  \\
1  \\
1  \\
0  \\
\end{bmatrix}
\end{equation}
$$

### Computation Graph
![image](image/graph.png)

## 実際にやってみるXOR

!!! note
	[xor example](https://github.com/clab/dynet/blob/master/examples/xor/train_xor.cc)


### プログラムの全体像

!!! note 
	[xorの数学的解説](http://gtech.hatenablog.com/entry/2016/07/30/141647)

**モデルの式**
$$
\hat{y} = \sigma ( \textbf{v} \cdot \tanh(\textbf{Ux} + \textbf{b}))
$$

```c++
#include "dynet/training.h"
#include "dynet/expr.h"
#include "dynet/io.h"
#include "dynet/model.h"

#include <fstream>

using namespace std;
using namespace dynet;

int main(int argc, char** argv) {
  dynet::initialize(argc, argv);

  const unsigned ITERATIONS = 30;

  //  ParameterCollection (all the model parameters)
  ParameterCollection m;
  SimpleSGDTrainer trainer(m);
  //MomentumSGDTrainer trainer(m);

  Parameter p_W, p_b, p_V, p_a;
  const unsigned HIDDEN_SIZE = 3;
  p_W = m.add_parameters({HIDDEN_SIZE, 2});
  p_b = m.add_parameters({HIDDEN_SIZE});
  p_V = m.add_parameters({1, HIDDEN_SIZE});
  p_a = m.add_parameters({1});

  // train the parameters
  for (unsigned iter = 0; iter < ITERATIONS; ++iter) {

    ComputationGraph cg;
    Expression W = parameter(cg, p_W);
    Expression b = parameter(cg, p_b);
    Expression V = parameter(cg, p_V);
    Expression a = parameter(cg, p_a);

    vector<Expression> losses;

    for (unsigned mi = 0; mi < 4; ++mi) {

      bool x1 = mi % 2;
      bool x2 = (mi / 2) % 2;
      vector<dynet::real> x_values(2);
      x_values[0] = x1 ? 1 : -1;
      x_values[1] = x2 ? 1 : -1;
      float y_value = (x1 != x2) ? 1 : -1;

      Expression x = input(cg, {2}, x_values);
      Expression y = input(cg, y_value);

      //Expression h = tanh(W*x + b);
      Expression h = tanh(affine_transform({b, W, x}));
      //Expression h = softsign(W*x + b);
      Expression y_pred = affine_transform({a, V, h});
      losses.push_back(squared_distance(y_pred, y));

    }

    Expression loss_expr = sum(losses);

    // Print the graph, just for fun.
    if(iter == 0) {
      cg.print_graphviz();
    }

    // Calculate the loss. Batching will automatically be done here.
    float loss = as_scalar(cg.forward(loss_expr)) / 4;
    cg.backward(loss_expr);
    trainer.update();

    cerr << "E = " << loss << endl;
  }

}
```

!!! TODO
	referenceを調べて，各関数の説明を付け加える

### include

```c++
#include <dynet/training.h>
#include <dynet/expr.h>

#include <iostream>
#include <fstream>
```

### namespace

```c++
using namespace std;
using namespace dynet;
```

### initialize

```c++
dynet::initialize(argc, argv);
```

### parameter setting1

全ての重みパラメータwが登録されている`ParameterCollection`wを作成する．
そして，その重みパラメータを更新するための`SGDTrainer`を作成し，
`SGDTrainer`に`ParameterCollection`を渡す．

```c++
const unsigned ITERATIONS = 30;
ParameterCollection m;
SimpleSGDTrainer trainer(m);
```

### parameter setting

```c++
Prameter p_W, p_b, p_V, p_a;
const unsigned HIDDEN_SIZE = 3;
p_W = m.add_parameters({HIDDEN_SIZE, 2});
p_b = m.add_parameters({HIDDEN_SIZE});
p_V = m.add_parameters({1, HIDDEN_SIZE});
p_a = m.add_parameters({1});
```

### train the parameters

dynetのトレーニングフェーズは`for文`で回します．

```c++
for (unsigned inter = 0 iter < ITERATIONS; ++iter) {
	
	ComputationGraph cg;
	Expression W = parameter(cg, p_W);
	Expression b = parameter(cg, p_b);
	Expression V = parameter(cg, p_V);
	Expression a = parameter(cg, p_a);

	vector<Expression> losses;
}
```

### 教師データ作成

!!! note	"三項演算子"
	[?:文]は条件分岐処理の１つで，条件が真か偽により処理を分岐します．
	[if文]を短縮して書いたような記述方法になります．

	```
	条件式 ? 真の場合の文 : 偽の場合の文
	```

	注意点としては条件の分岐結果が変数やリテラルなどの**値**になっていなければ
	ならないことです．よく代入演算子と一緒に利用されます．

	```
	bool variable = (result) ? true : false;
	```

```c++
for (unsigned mi = 0; mi < 4; ++mi) {
	
	// 入力ベクトルを作成
	bool x1 = mi % 2;		// 0 -> false, 1 -> true,  2 -> false, 3 -> true 偶奇判定
	bool x2 = (mi / 2) % 2;	// 0 -> false, 1 -> false, 2 -> true, 3 -> true

	vector<dynet::real> x_values(2);	// dynet::realはfloatと同等

	x_value[0] = x1 ? 1 : -1;
	x_value[1] = x2 ? 1 : -1;
	float y_value = (x1 != x2) ? 1 : -1;
```

XORを[0,1]ではなく[-1,1]であわらしていると思われる．４イテレーション全部を
計算すると下記の表のようになる．

|(indecs)mi|x_value[0]|x_value[1]|y_value|
|----------|----------|----------|-------|
|0         |-1        |-1        |-1     |
|1         |1         |-1        |1      |
|2         |-1        |1         |1      |
|3         |1         |1         |-1     |

|A|B|Z|
|-|-|-|
|0|0|0|
|0|1|1|
|1|0|1|
|1|1|0|

### 

### expression

```c++
Expression x = input(cg, {2}, x_values);
Expression y = input(cg, y_value);
```
