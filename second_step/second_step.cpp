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
	dynet::ComputationGraph cg;
	dynet::Expression input_expr = dynet::input(cg, 1.0f);
	dynet::Expression output_expr = 2.0f * input_expr + 3.0f;
	// dynet::realは今の所floatと等価
	dynet::real output_value = dynet::as_scalar(cg.forward(output_expr));
	std::cout << static_cast<float>(output_value) << std::endl;

	//	vector version
	dynet::ComputationGraph cg;
	std::vector<float> input_value = {1.0, 2.0, 3.0};
	
	//	第二引数で次元を指定する
	dynet::Expression input_expr = dynet::input(cg, {3}, input_value);
	dynet::Expression output_expr = 2.0f * input_expr + 3.0;
	std::vector<dynet::real> output_value = dynet::as_vector(cg.forward(output_expr));
}
