#include <iostream>
#include <vector>
#include <dynet/training.h>
#include <dynet/expr.h>
#include <dynet/model.h>
#include <gpop/Series.hpp>

int main(int argc, char* argv[])
{

	// dynetの初期化
	dynet::initialize(argc, argv);

	// 設定
	const int ITERATIONS = 30;
	const int HIDDEN_L1_SIZE = 2;

	// 計算グラフを構築
	dynet::ParameterCollection model;
	dynet::SimpleSGDTrainer trainer(model);

	// パラメータを設定 (パラメータ = 最適化される変数）
	// W1 (2x2) のパラメータを作成
	dynet::Parameter p_W1 = model.add_parameters({HIDDEN_L1_SIZE, 2});
	// b1 (2x1) のパラメータを作成
	dynet::Parameter p_b1 = model.add_parameters({HIDDEN_L1_SIZE});
	// W2 (1x2) のパラメータを作成
	dynet::Parameter p_W2 = model.add_parameters({1, HIDDEN_L1_SIZE});
	// b2 (1x1) のパラメータを作成
	dynet::Parameter p_b2 = model.add_parameters({1});

	// ノードの作成と計算グラフへの登録
	dynet::ComputationGraph cg;
	dynet::Expression W1 = dynet::parameter(cg, p_W1);
	dynet::Expression b1 = dynet::parameter(cg, p_b1);
	dynet::Expression W2 = dynet::parameter(cg, p_W2);
	dynet::Expression b2 = dynet::parameter(cg, p_b2);
	// ニューラルネット及び，計算グラフへの入力変数x_valueを宣言し，変更可能にするために参照渡しをします．
	std::vector<dynet::real> x_value(2);
	dynet::Expression x  = dynet::input(cg, {2}, &x_value);
	// ニューラルネット及び，計算グラフへの入力変数y_valueを宣言し，変更可能にするために参照渡しをします．
	dynet::real y_value;
	dynet::Expression y = dynet::input(cg, &y_value);
	// 計算グラフにノードの接続関係を宣言する．
	dynet::Expression z1 = dynet::rectify(W1*x+b1);
	dynet::Expression y_pred = W2*z1+b2;
	dynet::Expression loss_expr = dynet::squared_distance(y_pred, y);

	// 構築した計算グラフを描いてみる．それが僕には楽しかったから
	cg.print_graphviz();

	// 誤差を記録する
	std::vector<double> loss_vec;

	// 教師データを作成し，学習させてみる
	for (int iter = 0; iter < ITERATIONS; iter++) {
		double loss_value = 0;
		for (int mi = 0; mi < 4; mi++) {
			//教師データの作成開始(x,y)
			bool x1 = mi%2;
			bool x2 = (mi / 2)%2;
			std::cout << "x1 : x2 " << std::boolalpha << x1 << "\t" << x2 << std::endl;
			x_value[0] = x1 ? 1 : -1;
			x_value[1] = x2 ? 1 : -1;
			std::cout << "x_value[0] : " << x_value[0] << "\t" << "x_value[1] : " << x_value[1] << std::endl;
			y_value = (x1 != x2) ? 1 : -1;
			//教師データの作成終了(x,y)
			//学習実行
			loss_value += dynet::as_scalar(cg.forward(loss_expr));
			cg.backward(loss_expr);
			trainer.update();
		}
		loss_value /= 4;
		std::cout << "E = " << loss_value << std::endl;
		loss_vec.push_back(loss_value);
	}

	// 誤差をプロットする
	Series plot("error");
	plot.plot(loss_vec);
	plot.show();
	plot.save_as_png("error");

	// 結果を確かめる
	x_value[0] = -1;
	x_value[1] = -1;
	//cg.forward(loss_expr);
	cg.forward(y_pred);
	std::cout << "[-1,-1] -1 : " << dynet::as_scalar(y_pred.value()) << std::endl;
	x_value[0] = -1;
	x_value[1] = 1;
	//cg.forward(loss_expr);
	cg.forward(y_pred);
	std::cout << "[-1,1] 1 : " << dynet::as_scalar(y_pred.value()) << std::endl;
	x_value[0] = 1;
	x_value[1] = -1;
	//cg.forward(loss_expr);
	cg.forward(y_pred);
	std::cout << "[1,-1] 1 : " << dynet::as_scalar(y_pred.value()) << std::endl;
	x_value[0] = 1;
	x_value[1] = 1;
	//cg.forward(loss_expr);
	cg.forward(y_pred);
	std::cout << "[1,1] -1 : " << dynet::as_scalar(y_pred.value()) << std::endl;

	return 0;
}
