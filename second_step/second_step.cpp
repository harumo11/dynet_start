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
	//	scalar
	dynet::ComputationGraph cg;

}
