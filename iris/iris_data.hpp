// This IrisDataTable class parse iris data

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
#include <random>
#include <algorithm>

class IrisData {
	public:
		double sepal_length;
		double sepal_width;
		double petal_length;
		double petal_width;
		std::string class_name;
		unsigned int class_number;
};

class IrisDataTable {
	public:
		IrisDataTable(const std::string iris_data_path);
		void shuffle();
		int get_data_size();
		IrisData at(const int index);
		IrisData split(const std::string sentence);
		std::vector<IrisData> data_table;
		void print();
};

IrisDataTable::IrisDataTable(const std::string iris_data_path){
	// Open iris data
	std::ifstream iris_file(iris_data_path);
	if (!iris_file) {
		std::cout << "[ERROR] : Couldn't open iris.data file" << std::endl;
	}

	// Split each data from one data
	std::string one_line_sentence;
	int itr = 0;
	this->data_table.resize(150);
	while (iris_file >> one_line_sentence) {
		// split here
		this->data_table[itr] = this->split(one_line_sentence);
		itr++;
	}
}

IrisData IrisDataTable::split(const std::string sentence){
	std::stringstream sentence_stream;
	sentence_stream << sentence;
	std::string word;
	std::vector<std::string> results;
	IrisData return_data;

	// split one sentence to each word
	while (std::getline(sentence_stream, word, ',')) {
		results.push_back(word);
	}

	// Convert from std::string to double for each data
	return_data.sepal_length = std::stod(results[0]);
	return_data.sepal_width  = std::stod(results[1]);
	return_data.petal_length = std::stod(results[2]);
	return_data.petal_width  = std::stod(results[3]);

	// Convert from iris class name(Iris-setosa, Iris-versicolor, Iris-virginica) to iris class number[0,2]
	if (results[4] == "Iris-setosa") {
		return_data.class_name = results[4];
		return_data.class_number = 0;
	}
	else if (results[4] == "Iris-versicolor"){
		return_data.class_name = results[4];
		return_data.class_number = 1;
	}
	else {
		return_data.class_name = results[4];
		return_data.class_number = 2;
	}

	return return_data;
}

IrisData IrisDataTable::at(const int index){
	return this->data_table[index];
}

void IrisDataTable::shuffle(){
	std::random_device rnd;
	std::mt19937 engine(rnd());
	std::shuffle(this->data_table.begin(), this->data_table.end(), engine);
}

void IrisDataTable::print(){

	int itr = 0;
	for (auto&& data : this->data_table){
		std::cout << "---------------\t" <<  itr++ << "\t----------------" << std::endl;
		std::cout << "sepal_length : " << data.sepal_length << "\t sepal_width : " << data.sepal_width << "\t petal_length : " << data.petal_length << "\t petal_width : " << data.petal_width << "\t class name : " << data.class_name << std::endl;
	}
}
