#include <iostream>
#include <vector>
#include <dynet/training.h>
#include <dynet/expr.h>
#include <dynet/io.h>
#include <dynet/model.h>

//	ベクトルをprintするためのヘルパ関数
void print_vector(const std::vector<dynet::real>& values){
	for (auto&& val : values){
		std::cout << static_cast<float>(val) << "\t";
	}
	std::cout << std::endl;
}

void run(){
	//	scalar version
	std::cout << "D0 " << std::endl;
	dynet::ComputationGraph cg;
	std::cout << "D1 " << std::endl;
	dynet::Expression input_expr = dynet::input(cg, 1.0f);
	std::cout << "D2 " << std::endl;
	dynet::Expression output_expr = 2.0f * input_expr + 3.0f;
	// dynet::realは今の所floatと等価
	std::cout << "D3 " << std::endl;
	dynet::real output_value = dynet::as_scalar(cg.forward(output_expr));
	std::cout << static_cast<float>(output_value) << std::endl;

	//	vector version
	//std::vector<float> input_values = {1.0, 2.0, 3.0};
	
	//	第二引数で次元を指定する
	//	一次元の場合は縦ベクトルになる
	//input_expr = dynet::input(cg, {3}, input_values);
	//output_expr = 2.0f * input_expr + 3.0;
	//std::vector<dynet::real> output_values = dynet::as_vector(cg.forward(output_expr));
	//print_vector(output_values);

	//	行列の場合はベクトルにpackした値をやり取りする
	//	値は縦方向に入る．つまり
	//	[[1, 3],
	//	 [2, 4]]
	//input_values = {1.0, 2.0, 3.0, 4.0};
	//input_expr = dynet::input(cg, {2, 2}, input_values);
	//output_expr = 2.0 * input_expr + 3;
	//output_values = dynet::as_vector(cg.forward(output_expr));
	//print_vector(output_values);
}

int main(int argc, char const* argv[])
{
	run();
	return 0;
}
