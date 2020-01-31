#include <iostream>
#include <vector>

#include <dynet/training.h>
#include <dynet/expr.h>

int main(int argc, char* argv[])
{
	// DyNetの初期化
	dynet::DynetParams params;
	params.autobatch = 1;
	dynet::initialize(params);

	// パラメータ
	const unsigned ITERATIONS = 50;
	dynet::ParameterCollection model;
	dynet::SimpleSGDTrainer trainer(model);

	const unsigned HIDDEN_SIZE = 8;
	dynet::Parameter p_W, p_b, p_V, p_a;
	p_W = model.add_parameters({HIDDEN_SIZE, 2});
	p_b = model.add_parameters({HIDDEN_SIZE});
	p_V = model.add_parameters({1, HIDDEN_SIZE});
	p_a = model.add_parameters({1});

	// パラメータの学習
	std::vector<dynet::real> x_value(2);
	dynet::ComputationGraph cg;
	dynet::Expression y_pred;
	for (unsigned iter = 0; iter < ITERATIONS; ++iter) {
		
		cg.clear();
		dynet::Expression W = dynet::parameter(cg, p_W);
		dynet::Expression b = dynet::parameter(cg, p_b);
		dynet::Expression V = dynet::parameter(cg, p_V);
		dynet::Expression a = dynet::parameter(cg, p_a);

		std::vector<dynet::Expression> losses;

		for (unsigned mi = 0; mi < 4; mi++) {

			bool x1 = mi % 2;
			bool x2 = (mi / 2) % 2;
			x_value[0] = x1 ? 1 : -1;
			x_value[1] = x2 ? 1 : -1;
			float y_value = (x1 != x2) ? 1 : -1;

			dynet::Expression x = dynet::input(cg, {2}, x_value);
			dynet::Expression y = dynet::input(cg, y_value);

			//下記の式は間違い．誤差が永遠に１のまま.というか活性化関数がかかっていない
			//dynet::Expression h = dynet::affine_transform({b, W, x});
			//dynet::Expression h = dynet::tanh(W*x + b);
			dynet::Expression h = dynet::rectify(W*x + b);
			y_pred = dynet::affine_transform({a, V, h});
			losses.push_back(dynet::squared_distance(y_pred, y));
		}

		dynet::Expression loss_expr = dynet::sum(losses);

		// 計算グラフを表示する．僕にはそれが楽しかったから
		if (iter == 0) {
			std::cout << "=======================================" << std::endl;
			cg.print_graphviz();
			std::cout << "=======================================" << std::endl;
		}

		// 損失を計算する．バッチ処理は自動でここで行われます
		//float loss = dynet::as_scalar(cg.forward(loss_expr)) / 4; 
		float loss = dynet::as_scalar(cg.forward(loss_expr)); 
		cg.backward(loss_expr);
		trainer.update();

		std::cout << "E = " << loss / 4.0 << std::endl;
	}

	x_value[0] = -1;
	x_value[1] = -1;
	cg.forward(y_pred);
	std::cout << "[-1, -1] -1 : " << dynet::as_scalar(y_pred.value()) << std::endl;

	return 0;
}
