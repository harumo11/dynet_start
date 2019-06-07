#include <iostream>
#include <vector>
#include <dynet/training.h>
#include <dynet/expr.h>
#include <dynet/io.h>
#include <dynet/model.h>

int main(int argc, char* argv[])
{
	
	// dynetの初期化
	dynet::initialize(argc, argv);

	// 設定
	const int ITERATIONS = 30;

	// パラメータコレクションの作成
	// パラメータとは我々が最適化するもののこと(vector/matrix)
	// モデルはパラメータの集合体
	dynet::ParameterCollection m;
	dynet::SimpleSGDTrainer trainer(m);

	const unsigned int HIDEEN_SIZE = 8;
	// 8x2のサイズのパラメータ（matrix)を作成
	dynet::Parameter p_W = m.add_parameters({HIDEEN_SIZE, 2});
	// 8x1のサイズのパラメータ（vector)を作成
	dynet::Parameter p_b = m.add_parameters({HIDEEN_SIZE});
	// 1x8のサイズのパラメータ（matrix)を作成
	dynet::Parameter p_V = m.add_parameters({1, HIDEEN_SIZE});
	// 1のスカラーパラメータを作成
	dynet::Parameter p_a = m.add_parameters({1});

	if (argc == 2) {
		// もし，ファイルからパラメータとモデルがロード可能ならばロードする
		dynet::TextFileLoader loader(argv[1]);
		loader.populate(m);
	}

	// 静的な計算グラフの宣言
	dynet::ComputationGraph cg;
	dynet::Expression W = dynet::parameter(cg, p_W);
	dynet::Expression b = dynet::parameter(cg, p_b);
	dynet::Expression V = dynet::parameter(cg, p_V);
	dynet::Expression a = dynet::parameter(cg, p_a);

	// ネットワークへの入力x_valueを宣言し，変更可能にするために参照渡しをします．
	std::vector<dynet::real> x_value(2);
	dynet::Expression x = dynet::input(cg, {2}, &x_value);
	// 出力の真値y_valueを宣言し，変更可能にするために参照渡しをします．
	dynet::real y_value;
	dynet::Expression y = dynet::input(cg, &y_value);

	dynet::Expression h = dynet::tanh(W*x+b);
	dynet::Expression y_pred = V*h+a;
	dynet::Expression loss_expr = dynet::squared_distance(y_pred, y);

	// 作成したグラフを描いてみせる，それが僕には楽しかったから
	cg.print_graphviz();

	// パラメータをトレーニングする
	for (unsigned iter = 0; iter < ITERATIONS; ++iter) {
		double loss = 0;
		for (unsigned mi = 0; mi < 4; ++mi) {
			// トレーニングデータ作成開始（x,y)
			bool x1 = mi%2;			//0->false, 1->true,  2->false, 3->true(奇数or偶数)
			bool x2 = (mi / 2)%2;	//0->false, 1->false, 2->true,  3->true
			std::cout << "x1 : x2 " << x1 << "\t" << x2 << std::endl;
			x_value[0] = x1 ? 1 : -1;
			x_value[1] = x2 ? 1 : -1;
			std::cout << "x_value[0] : " << x_value[0] << "\t" << "x_value[1] : " << x_value[1] << std::endl;
			y_value = (x1 != x2) ? 1 : -1;
			// トレーニングデータ作成終了（x,y)
			// 学習実行
			loss += dynet::as_scalar(cg.forward(loss_expr)); 
			cg.backward(loss_expr);
			trainer.update();
		}
		loss /= 4;
		std::cout << "E = " << loss << std::endl;
	}

	for (auto&& e : dynet::as_vector(V.value())){
		std::cout << e << " : ";
	}
	std::cout << std::endl;

	// トレーニングした結果を試してみる
	x_value[0] = -1;
	x_value[1] = -1;
	cg.forward(loss_expr);
	std::cout << "[-1,-1] -1 : " << dynet::as_scalar(y_pred.value()) << std::endl;;
	x_value[0] = -1;
	x_value[1] = 1;
	cg.forward(loss_expr);
	std::cout << "[-1, 1]  1 : " << dynet::as_scalar(y_pred.value()) << std::endl;;
	x_value[0] = 1;
	x_value[1] = -1;
	cg.forward(loss_expr);
	std::cout << "[ 1,-1]  1 : " << dynet::as_scalar(y_pred.value()) << std::endl;;
	x_value[0] = 1;
	x_value[1] = 1;
	cg.forward(loss_expr);
	std::cout << "[ 1, 1] -1 : " << dynet::as_scalar(y_pred.value()) << std::endl;;
	return 0;
}
