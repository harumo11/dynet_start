# iris

dynetでirisの分類をしてみましょう

irisのデータセットは[ここ](http://archive.ics.uci.edu/ml/datasets/Iris)
からダウンロードできます．

irisはアヤメの分類を行うデータセットです．
４つの情報からアヤメの種類を学習・推測するものになります．

このデータセットはパターン認識のための最もよく知られたものです．
このデータセットは３つの種類のアヤメのデータがそれぞれ５０個ずつ含まれています．
一つのアヤメは他の２つと線形分離可能です．

## データの中身

### アヤメの種類

- setosa 
- versicolor
- virginica

それぞれのデータが５０個ずつ含まれています．

### 含まれている情報

- ガク片（sepal)の幅・長さ(cm)
- 花弁（petal)の幅・長さ(cm)

### 情報量

- 情報数:150
- 欠損情報:無し

### データの構成

1. sepal length(cm)
2. sepal width(cm)
3. petal length(cm)
4. petal width(cm)
5. class:
	- Iris Setosa
	- Iris Versicolor
	- Iris Virginica

### 統計

| |Min|Max|Mean|Standard deviation|Class correlation|
|-|:-:|:-:|:--:|:----------------:|:---------------:|
|sepal length|4.3|7.9|5.84|0.83|0.7826|
|sepal width|2.0|4.4|3.05|0.43|-0.4194|
|petal length|1.0|6.9|3.76|1.76|0.9490(high!)|
|petal width|0.1|2.5|1.2|0.76|0.9565(high!)|
