#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>
#include <fstream>
#include <dynet/dynet.h>	// Basic classes
#include <dynet/training.h> // Trainer
#include <dynet/expr.h>		// Expression, Parameter
#include <gpop/Series.hpp>

#include "iris_data.hpp"




int main(int argc, char* argv[])
{
	// Loading of iris data
	IrisDataTable iris_data_table("iris.data");
	iris_data_table.print();
	iris_data_table.shuffle();
	iris_data_table.print();
	
	// Initialize of dynet
	dynet::initialize(argc, argv);

	// Configuration
	const int HIDDEN_LAYER_SIZE = 16;
	const int INPUT_LAYER_SIZE = 4;
	const int OUTPUT_LAYER_SIZE = 3;

	// Building computational graph
	dynet::ParameterCollection model;
	dynet::SimpleSGDTrainer trainer(model);

	// Set parameters
	// W1 (8x4) matrix
	dynet::Parameter p_W1 = model.add_parameters({HIDDEN_LAYER_SIZE, INPUT_LAYER_SIZE});
	// b1 (16x1) vector
	dynet::Parameter p_b1 = model.add_parameters({HIDDEN_LAYER_SIZE});
	// W2 (3x16) matrix
	dynet::Parameter p_W2 = model.add_parameters({OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE});
	// b2 (3x1) vector
	dynet::Parameter p_b2 = model.add_parameters({OUTPUT_LAYER_SIZE});

	// Making nodes and registering to computational graph
	dynet::ComputationGraph cg;
	dynet::Expression W1 = dynet::parameter(cg, p_W1);
	dynet::Expression b1 = dynet::parameter(cg, p_b1);
	dynet::Expression W2 = dynet::parameter(cg, p_W2);
	dynet::Expression b2 = dynet::parameter(cg, p_b2);
	// Making input variable x_value
	std::vector<dynet::real> x_value(4);
	dynet::Expression x = dynet::input(cg, {4}, x_value);
	// Defintions of relationship of nodes
	dynet::Expression z1 = dynet::rectify(W1*x+b1);		// Input layer
	dynet::Expression y_pred = dynet::softmax(W2*z1+b2);// Hidden layer

	// Making output value y_label_*
	int y_label_setosa     = 0; //: setosa
	int y_label_versicolor = 1; //: versicolor
	int y_label_virginica  = 2; //: virginica
	unsigned* y_label = 0;
	//dynet::Expression loss_expr_setosa     = dynet::pickneglogsoftmax(y_pred, y_label_setosa);		// Output layer (setosa)
	//dynet::Expression loss_expr_versicolor = dynet::pickneglogsoftmax(y_pred, y_label_versicolor);	// Output layer (versicolor)
	//dynet::Expression loss_expr_virginica  = dynet::pickneglogsoftmax(y_pred, y_label_virginica);	// Output layer (virginica)
	dynet::Expression loss_expr = dynet::pickneglogsoftmax(y_pred, y_label);

	// Drawing our computational graph, just for fun.
	cg.print_graphviz();

	// Training
	double loss_value = 0;
	std::vector<double> loss_vector;
	for (int itr = 0; itr < iris_data_table.data_table.size(); itr++) {

		// Setting concrete x_value
		x_value[0] = iris_data_table.data_table[itr].sepal_length;
		x_value[1] = iris_data_table.data_table[itr].sepal_width;
		x_value[2] = iris_data_table.data_table[itr].petal_length;
		x_value[4] = iris_data_table.data_table[itr].petal_width;



		// setosa	  : 0   - 49
		if (iris_data_table.data_table[itr].class_name == "Iris-setosa"){
			*y_label = 0;
			loss_value = dynet::as_scalar(cg.forward(loss_expr));
			cg.backward(loss_expr);
		}
		// versicolor : 50  - 99
		if (iris_data_table.data_table[itr].class_name == "Iris-versicolor") {
			*y_label = 1;
			loss_value = dynet::as_scalar(cg.forward(loss_expr));
			cg.backward(loss_expr);
		}
		// virginica  : 100 - 149
		if (iris_data_table.data_table[itr].class_name == "Iris-virginica") {
			*y_label = 2;
			loss_value = dynet::as_scalar(cg.forward(loss_expr));
			cg.backward(loss_expr);
		}
		trainer.update();
		loss_vector.push_back(loss_value);
		std::cout << "[ " <<  itr << " ] " << " E = " << loss_value << std::endl;
	}

	// Show loss plot
	Series plot;
	plot.plot(loss_vector);
	plot.show();
	std::cin.get();

	// Prediction
	// Setting concrete x_value 
	// NOTE Ordinary, we should not use trainging data. This time, we use it for easy.
	x_value[0] = iris_data_table.data_table[20].sepal_length;
	x_value[1] = iris_data_table.data_table[20].sepal_width;
	x_value[2] = iris_data_table.data_table[20].petal_length;
	x_value[3] = iris_data_table.data_table[20].petal_width;
	int y_label_answer = iris_data_table.data_table[20].class_number;

	// Showing probability of each iris class.
	cg.forward(y_pred);
	auto result = dynet::as_vector(y_pred.value());
	for (auto&& e : result) {
		std::cout << e << std::endl;
	}
	std::cout << "answer : " << y_label_answer << std::endl;
	
	return 0;
}
