# 保存と読み込み

!!! Note
	`populate`は居住する，生息する，人を居住させる，植民するという意味の動詞です．

dynetのC++インターフェースはユーザにモデルパラメータの保存とその読み込みをサポートしています．
ユーザはモデルを保存する際に２つの方法を選択できます．
基本的な使用においてはほとんどの場合，完全な`ParameterCollection`オブジェクトを保存することが
できます．読み込み時において，ユーザはモデルが保存されたときに存在していたのと
同じパラメータ変数を定義し，割り当てるべきです．(これは通常，両方のコードパスによって
同じパラメータ作成が呼び出されることを意味します．)
そして，`populate`を呼び出し，読み込まれるべきパラーメタを有する
`ParameterCollection`オブジェクトに渡します．

```C++
#include <dynet/io.h>
//save end
ParameterCollection m;
Parameter a = m.add_parameters({100});
LookupParameter b = m.add_lookup_parameters(10, {100});
Parameter c = m.add_parameters({1000});
{
	dynet::TextFileSaver s("/tmp/tmp.model");
	s.save(m);
}
```

```C++
//load end
ParameterCollection m;
m.add_parameters({100});
m.add_lookup_parameters(10, {100});
m.add_parameters({1000});
{
	dynet::TextFileLoader l("/tmp/tmp.model");
	l.populate(m);
}
```

しかしながら，いくつかの場合にはパラメータオブジェクトの一部だけを保存できると便利です．
(例えば，事前学習のためにパラメータを読み出したい場合などです)

ここでは，`Parameter`，もしくは`LookupParameter`オブジェクトが陽に保存を実行することが
できることを説明します．ユーザは部分的な保存と読み込みのためにキーを指定することもできます．

```C++
#include <dynet/io.h>
//save end
ParameterCollection m1, m2;
m1.add_parameters({10}, "a");
m1.add_lookup_parameters{10, {2}, "la");
Parameter param_b = m2.add_parameters({3, 7});
{
	dynet::TextFileSaver s("/tmp/tmp.model");
	s.save(m1, "/namespace_tmp/");
	s.save(param_b, "param_b");
}

//load end
ParameterCollection m;
m.add_parameters({10});
m.add_lookup_paramters(10, {2});
{
	dyent::TextFileLoader l("/tmp/tmp.model");
	l.populate(m, "/namespace_tmp/");
	Parameter param_b = m.add_parameters({3, 7});
	l.populate(param_b, "param_b");
}

//load end
// ユーザーはモデルパラメータを読み込むのと同等のインターフェースを使用可能です．
ParameterCollection m;
Parameter parm_a, param_b;
LookupParameter l_param;
{
	dynet::TextFileLoader l("/tmp/tmp.model");
	param_a = l.load_parm(m, "/namespace_tmp/a");
	l_param = l.load_lookup_param(m, "/namespace_tmp/la");
	param_b = l.load_param(m, "param_b");
}
```

!!! WORN
	以前のdynetのバージョンではビルダーオブジェクトはシリアライズされている必要がありました．
	もはや必要ではありません．（Pythonインターフェースはビルダーオブジェクトのシリアライゼーションを許可します）

現在，dynetは簡易なテキストフォーマットのみをサポートしています．
この簡単なフォーマットはとてもシンプルで可読性が高いです．このモデルファイルは
基本的なストレージブロックから構成されています．一つの基本的なブロックは１行目の
メタデータ情報から始まります．
`#object_type# object_name dimension block_size`と残りの部分の実際のデータから構成
されています．読み込みプロセスの間，dynetはメタデータを使用して
ユーザが読み込みたいオブジェクトの配置を行います．

