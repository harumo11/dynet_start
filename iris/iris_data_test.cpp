#include <iostream>
#include "iris_data.hpp"

int main(int argc, char const* argv[])
{
	IrisDataTable iris_data_table("./iris.data");
	iris_data_table.print();
	iris_data_table.shuffle();
	iris_data_table.print();

	auto input_data_pair = iris_data_table.generate_data();
	auto train_data = input_data_pair.first;
	auto verif_data = input_data_pair.second;

	int i = 0;
	for(auto e : train_data){
		std::cout << "[ " << i << " ] " <<  e << std::endl;
		if (i % 4 == 0) {
			i++;
		}
	}

	std::cout << "=======================================" << std::endl;

	for(auto e : verif_data){
		std::cout << e << std::endl;
	}
	
	return 0;
}
