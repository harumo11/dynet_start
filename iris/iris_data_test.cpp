#include <iostream>
#include "iris_data.hpp"

int main(int argc, char const* argv[])
{
	IrisDataTable iris_data_table("./iris.data");
	iris_data_table.print();
	iris_data_table.shuffle();
	iris_data_table.print();
	
	return 0;
}
