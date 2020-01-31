#include <iostream>
#include <dynet/expr.h>
#include <dynet/model.h>
#include <dynet/io.h>

int main(int argc, char* argv[])
{
	//dynetを初期化
	dynet::initialize(argc, argv);

	//パラメータ
	const unsigned HIDDEN_SIZE = 8;
	dynet::ParameterCollection model;
	dynet::Parameter p_W, p_b, p_V, p_a;

	//計算グラフ
	dynet::ComputationGraph cg;

	p_W = model.add_parameters({HIDDEN_SIZE, 2});
	p_b = model.add_parameters({HIDDEN_SIZE});
	p_V = model.add_parameters({1, HIDDEN_SIZE});
	p_a = model.add_parameters({1});

	dynet::Expression W = dynet::parameter(cg, p_W);
	dynet::Expression b = dynet::parameter(cg, p_b);
	dynet::Expression V = dynet::parameter(cg, p_V);
	dynet::Expression a = dynet::parameter(cg, p_a);

	dynet::TextFileLoader loader("tmp.model");
	loader.populate(model);

	//入出力
	//std::vector<dynet::real> x_value = {-1, -1};
	//std::vector<dynet::real> x_value = {-1, 1};
	//std::vector<dynet::real> x_value = {1, -1};
	std::vector<dynet::real> x_value = {1, 1};
	dynet::Expression x = dynet::input(cg, {2}, x_value);

	//計算グラフの構築
	dynet::Expression h = dynet::tanh(W*x + b);
	dynet::Expression y_pred = V*h + a;
	
	//実行
	cg.forward(y_pred);
	std::cout << "[1, 1] -1 : " << dynet::as_scalar(y_pred.value()) << std::endl;

	std::cout << "======================================" << std::endl;
	cg.print_graphviz();
	std::cout << "======================================" << std::endl;
	
	return 0;
}
