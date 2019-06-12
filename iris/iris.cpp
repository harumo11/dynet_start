#include <iostream>
#include <vector>
#include <dynet/dynet.h>	// 基本的なクラス
#include <dynet/training.h> // トレーナ
#include <dynet/expr.h>		// Expression,parameterのため

int main(int argc, char* argv[])
{
	
	// dynetの初期化
	dynet::initialize(argc, argv);

	// 設定
	const int ITERATIONS = 30;
	const int HIDDEN_LAYER_SIZE = 16;
	const int INPUT_LAYER_SIZE = 4;
	const int OUTPUT_LAYER_SIZE = 3;

	// 計算グラフを構築
	dynet::ParameterCollection model;
	dynet::SimpleSGDTrainer trainer(model);

	// パラメータを設定（パラメータ＝最適化される変数）
	// W1 (8x4) matrix
	dynet::Parameter p_W1 = model.add_parameters({HIDDEN_LAYER_SIZE, INPUT_LAYER_SIZE});
	// b1 (16x1) vector
	dynet::Parameter p_b1 = model.add_parameters({HIDDEN_LAYER_SIZE});
	// W2 (3x16) matrix
	dynet::Parameter p_W2 = model.add_parameters({OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE});
	// b2 (3x1) vector
	dynet::Parameter p_b2 = model.add_parameters({OUTPUT_LAYER_SIZE});

	// ノードの作成と計算グラフへの登録
	dynet::ComputationGraph cg;
	dynet::Expression W1 = dynet::parameter(cg, p_W1);
	dynet::Expression b1 = dynet::parameter(cg, p_b1);
	dynet::Expression W2 = dynet::parameter(cg, p_W2);
	dynet::Expression b2 = dynet::parameter(cg, p_b2);
	// ニューラルネット及び，計算グラフへの入力変数x_valueを宣言し，変更可能なように参照渡しする．
	std::vector<dynet::real> x_value(4);
	dynet::Expression x = dynet::input(cg, {4}, x_value);
	// ニューラルネット及び，計算グラフへの入力変数y_setosa(1,0,0)を宣言し，変更可能なように参照渡しする．
	dynet::Expression y_setosa = dynet::one_hot(cg, 4, 0);
	for (auto&& e : dynet::as_vector(y_setosa.value())){
		std::cout << e << std::endl;
	}
	std::cout << "-----------------------------------" << std::endl;
	// ニューラルネット及び，計算グラフへの入力変数y_versicolor(0,1,0)を宣言し，変更可能なように参照渡しする．
	dynet::Expression y_versicolor = dynet::one_hot(cg, 4, 1);
	for (auto&& e : dynet::as_vector(y_versicolor.value())){
		std::cout << e << std::endl;
	}
	std::cout << "-----------------------------------" << std::endl;
	// ニューラルネット及び，計算グラフへの入力変数y_virginica(0,0,1)を宣言し，変更可能なように参照渡しする．
	dynet::Expression y_virginica = dynet::one_hot(cg, 4, 2);
	for (auto&& e : dynet::as_vector(y_virginica.value())){
		std::cout << e << std::endl;
	}
	// 計算グラフにノードの接続関係を宣言する
	return 0;
}
