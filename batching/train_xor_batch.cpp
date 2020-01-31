#include <iostream>

#include <dynet/training.h>
#include <dynet/expr.h>
#include <dynet/io.h>
#include <Gpop/Series.hpp>

int main(int argc, char* argv[])
{
	// dynetを初期化
	dynet::initialize(argc, argv);

	// パラメータ
	const unsigned HIDDEN_SIZE = 8;
	const unsigned ITERATION = 200;
	dynet::ParameterCollection model;
	dynet::SimpleSGDTrainer trainer(model);

	dynet::ComputationGraph cg;
	dynet::Parameter p_W, p_b, p_V, p_a;

	p_W = model.add_parameters({HIDDEN_SIZE, 2});
	p_b = model.add_parameters({HIDDEN_SIZE});
	p_V = model.add_parameters({1, HIDDEN_SIZE});
	p_a = model.add_parameters({1});

	dynet::Expression W = dynet::parameter(cg, p_W);
	dynet::Expression b = dynet::parameter(cg, p_b);
	dynet::Expression V = dynet::parameter(cg, p_V);
	dynet::Expression a = dynet::parameter(cg, p_a);

	// x_valueを参照渡し可能なように設定する
	// ミニバッチのサイズを設定
	dynet::Dim x_dim({2}, 4), y_dim({1}, 4);
	std::cout << "x_dim : " << x_dim << " y_dim : " << y_dim << std::endl;
	std::vector<dynet::real> x_value = {1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0};
	dynet::Expression x = dynet::input(cg, x_dim, x_value);
	std::vector<dynet::real> y_value = {-1.0, 1.0, 1.0, -1.0};
	dynet::Expression y = dynet::input(cg, y_dim, y_value);

	// computation graphの構築
	dynet::Expression h = dynet::tanh(W*x + b);
	dynet::Expression y_pred = V*h + a;
	dynet::Expression loss_expr = dynet::squared_distance(y_pred, y);
	//dynet::Expression sum_loss = dynet::sum_batches(loss_expr);
	dynet::Expression sum_loss = dynet::mean_batches(loss_expr);

	// 計算グラフを描画
	std::cout << "============================================" << std::endl;
	cg.print_graphviz();
	std::cout << "============================================" << std::endl;

	// プロットの準備
	Gpop::Series plot;
	std::vector<double> losses;

	// 学習を実行
	for (unsigned iter = 0; iter < ITERATION; iter++) {
		float loss = dynet::as_scalar(cg.forward(sum_loss)) / 4;
		cg.backward(sum_loss);
		trainer.update();
		std::cout << "E : " << loss << std::endl;
		losses.push_back(loss);
	}

	plot.plot(losses);
	plot.show();
	std::cin.get();

	// 計算グラフを保存
	dynet::TextFileSaver s("tmp.model");
	s.save(model);
	std::cout << "save model" << std::endl;

	return 0;
}
