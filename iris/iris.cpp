#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>
#include <fstream>
#include <dynet/dynet.h>	// Basic classes
#include <dynet/training.h> // Trainer
#include <dynet/expr.h>		// Expression, Parameter
#include <gpop/Series.hpp>


// This function splits gevein sentence including comma to each single word.
std::tuple<double, double, double, double, std::string> split(const std::string sentence, const char comma){

	std::stringstream sentence_stream;
	sentence_stream << sentence;
	std::string word;
	std::vector<std::string> results;

	while (std::getline(sentence_stream, word, comma)) {
		results.push_back(word);
	}

	return std::forward_as_tuple(std::stod(results[0]), std::stod(results[1]), std::stod(results[2]), std::stod(results[3]), results[4]);
}

class IrisData {
	public:
		IrisData(std::string iris_data_path);
		double sepal_length;
		double sepal_width;
		double petal_length;
		double petal_width;
		std::string class_name;
		unsigned int class_number;
		std::tuple<double, double, double, double, std::string, unsigned int> split(const std::string sentence, const char comma);
};

std::tuple<double, double, double, double, std::string, unsigned int> IrisData::split(const std::string sentence, const char comma){
	std::stringstream sentence_stream;
	sentence_stream << sentence;
	std::string word;
	unsigned int class_number;
	std::vector<std::string> results;

	while (std::getline(sentence_stream, word, comma)) {
		results.push_back(word);
	}

	// Convert from iris class name(Iris-setosa, Iris-versicolor, Iris-virginica) to iris class number [0,2]
	if (results[4] == "Iris-setosa") {
		class_number = 0;
	}
	else if (results[4] == "Iris-versicolor") {
		class_number = 1;
	}
	else {
		class_number = 2;
	}

	return std::forward_as_tuple(std::stod(results[0]), std::stod(results[1]), std::stod(results[2]), std::stod(results[3]), results[4], class_number);
}

IrisData::IrisData(std::string iris_data_path) {

	// Open the iris data
	std::ifstream iris_file(iris_data_path);
	if (!iris_file) {
		std::cout << "[ERROR] : Couldn't open iris.data file" << std::endl;
	}

	// Load data
	std::string one_line_sentence;

	while(iris_file >> one_line_sentence){
		// split here
		auto [sepal_length, sepal_width, petal_length, petal_width, iris_class, iris_number] = split(one_line_sentence, ',');
	}
	
}

// This function loads file and return vector data of iris.
std::tuple<std::vector<double>,  
	       std::vector<double>, 
		   std::vector<double>, 
		   std::vector<double>, 
		   std::vector<unsigned int>> load_iris_data(){

	std::ifstream iris_file("iris.data");
	if (!iris_file) {
		std::cout << "[ERROR] : Couldn't open iris.data file" << std::endl;
	}

	std::vector<double> sepal_length_vec;
	std::vector<double> sepal_width_vec;
	std::vector<double> petal_length_vec;
	std::vector<double> petal_width_vec;
	std::vector<std::string> iris_class_vec;
	int itr = 0;
	std::string one_line_sentence;

	while(iris_file >> one_line_sentence){
		// split here
		auto [sepal_length, sepal_width, petal_length, petal_width, iris_class] = split(one_line_sentence, ',');

		sepal_length_vec.push_back(sepal_length);
		sepal_width_vec.push_back(sepal_width);
		petal_length_vec.push_back(petal_length);
		petal_width_vec.push_back(petal_width);
		iris_class_vec.push_back(iris_class);

		// print 
		std::cout << "----------------\t" << itr << "\t------------------" << std::endl;
		std::cout << "sepal_length : " << sepal_length << std::endl;
		std::cout << "sepal_width : " << sepal_width << std::endl;
		std::cout << "petal_length : " << petal_length << std::endl;
		std::cout << "petal_width : " << petal_width << std::endl;
		std::cout << "iris_class : " << iris_class << std::endl;
		itr++;
	}

	std::vector<unsigned int> iris_class_num_vec;
	int setosa_num = 0;
	int versicolor_num = 0;
	int virginica_num = 0;
	for (auto&& class_name : iris_class_vec){
		if (class_name == "Iris-setosa") {
			iris_class_num_vec.push_back(0);
			setosa_num++;
		}
		else if (class_name == "Iris-versicolor"){
			iris_class_num_vec.push_back(1);
			versicolor_num++;
		}
		else {
			iris_class_num_vec.push_back(2);
			virginica_num++;
		}
	}
	std::cout << "setosa number : "		<< setosa_num << std::endl;
	std::cout << "versicolor number : " << versicolor_num << std::endl;
	std::cout << "virginica number : "	<< virginica_num << std::endl;

	return std::forward_as_tuple(sepal_length_vec, sepal_width_vec, petal_length_vec, petal_width_vec, iris_class_num_vec);
}

int main(int argc, char* argv[])
{
	// Loading of iris data
	auto[sepal_length_vec, sepal_width_vec, petal_length_vec, petal_width_vec, iris_class_num_vec] = load_iris_data();
	
	// Initialize of dynet
	dynet::initialize(argc, argv);

	// Configuration
	const int HIDDEN_LAYER_SIZE = 8;
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
	dynet::Expression loss_expr_setosa     = dynet::pickneglogsoftmax(y_pred, y_label_setosa);		// Output layer (setosa)
	dynet::Expression loss_expr_versicolor = dynet::pickneglogsoftmax(y_pred, y_label_versicolor);	// Output layer (versicolor)
	dynet::Expression loss_expr_virginica  = dynet::pickneglogsoftmax(y_pred, y_label_virginica);	// Output layer (virginica)

	// Drawing our computational graph, just for fun.
	cg.print_graphviz();

	// Training
	double loss_value = 0;
	std::vector<double> loss_vector;
	for (int itr = 0; itr < iris_class_num_vec.size(); itr++) {

		// Setting concrete x_value
		x_value[0] = sepal_length_vec[itr];
		x_value[1] = sepal_width_vec[itr];
		x_value[2] = petal_length_vec[itr];
		x_value[3] = petal_width_vec[itr];
		
		// setosa	  : 0   - 49
		if (0 <= itr && itr < 50) {
			loss_value = dynet::as_scalar(cg.forward(loss_expr_setosa));
			cg.backward(loss_expr_setosa);
		}
		// versicolor : 50  - 99
		if (50 <= itr && itr < 100) {
			loss_value = dynet::as_scalar(cg.forward(loss_expr_versicolor));
			cg.backward(loss_expr_versicolor);
		}
		// virginica  : 100 - 149
		if (100 <= itr && itr < 150) {
			loss_value = dynet::as_scalar(cg.forward(loss_expr_virginica));
			cg.backward(loss_expr_virginica);
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
	x_value[0] = sepal_length_vec[20];
	x_value[1] = sepal_width_vec[20];
	x_value[2] = petal_length_vec[20];
	x_value[3] = petal_width_vec[20];

	// Showing probability of each iris class.
	cg.forward(y_pred);
	auto result = dynet::as_vector(y_pred.value());
	for (auto&& e : result) {
		std::cout << e << std::endl;
	}
	
	return 0;
}
