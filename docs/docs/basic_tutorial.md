# 基本的なチュートリアル

どのようにパラメータ集合(parameter collections)をトレーニングするのかについての説明をします．
この説明には，ロジスティック回帰モデルを用います．

まずはじめに，我々はパラメータ集合(parameter collection)の構造をセットアップします．

パラメータ集合(parameter collection)を作ってください．そして，それらを更新するための
SDGも同様に作成してください．

```C++
ParameterCollection pc;
SimpleSGDTrainer trainer(pc);
```

**computation graph**を作成しましょう．それは情報の流れを定義するものです．

```C++
ComputationGraph cg;
```

1x3のベクトル型パラメータを初期化し，それを`computation graph`の一部に加えます．

```C++
Expression W = parameter(cg, pc.add_parameters({1,3});
```

ロジスティック回帰の入力と出力を定義する変数を作成しましょう．
そして，それらを`computation graph`に読み込ませます．
我々はまだ具体的な値をそれらの変数にセットする必要がないことに注意してください．

```C++
vector<dynet::read> x_values(3);
Expression x = input(cg, {3}, &x_values);
dynet::real y_value;
Expression y = input(cg, &y_value);
```

次に，入力ベクトル`x`(3x1)と重み行列`W`(1x3)を掛け合わせるための構造をセットアップしましょう．
そして，ロジスティックシグモイド関数を通して推測出力`y_pred`を計算しましょう．

```C++
Expression y_pred = logistic(W*x);	// (1x3) x (3x1)
```

最後に，損失関数を作成しましょう．*computation graph*の最後の関数の値を最小化するために
モデル(conputation graph)は最適化されます．

```C++
Expression l = binary_log_loss(y_pred, y);
```

これで，*computation graph*の作成が完了しました．
そして，我々はその構造を表示することが可能です．

```C++
cg.print_graphviz();
```

さあ，１つの例として，パラメータを更新してみましょう．
入力/出力にトレーニング用の具体的な値をセットしてください．

```C++
x_value = {0.5, 0.3, 0.7};
y_value = 1.0;
```

`forward`を実行することによって，*computation graph*の中を具体的な値が流れ，計算が行われます．
そして，損失が帰ってきます．

```C++
dynet::real loss = as_scalar(cg.forward(l));
```

`backward`を実行することによって逆伝播が行われ，`ParameterCollection`の中に格納されている
パラメータの勾配が計算されます．

```C++
cg.backward(l);
```

`trainer.update`を実行することにより，`trainer`のコンストラクタの引数として渡された
*parameter collection*の中のパラメータが更新されます．ここでの1.0はスケーリングファクタと
呼ばれるもので，それは更新のサイズを調整するときに使用されます．

```C++
trainer.update();
```

これはとてもシンプルな例なので*memory initalization*, *reading/writing parameter collection*, *recurrent/LSTM network*, もしくは，*baiasを関数に加えること*
などについて言及していないことに注意してください．
実際にDyNetをどのように使うかについて学ぶ理想的な用法は`example`ディレクトリを見ることです．
特に，最もシンプルな例である`xor`から始めると良いでしょう．
