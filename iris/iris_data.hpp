// This IrisDataTable class parse iris data

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
#include <random>
#include <algorithm>

/**
 * @brief Repesents one flower data.
 */
class IrisData {
	public:
		double sepal_length;
		double sepal_width;
		double petal_length;
		double petal_width;
		std::string class_name;
		unsigned int class_number;
		int index;
};

/**
 * @brief Represents all flower data in table.
 */
class IrisDataTable {
	public:
		IrisDataTable(const std::string iris_data_path);
		void shuffle();
		int get_data_size();
		IrisData at(const int index);
		void print();
		std::pair<std::vector<float>, std::vector<float>> generate_data(const unsigned int training_data_size = 140);

	private:
		IrisData split(const std::string sentence);
		std::vector<IrisData> data_table = std::vector<IrisData>(150);
};

/**
 * @brief A constructor, 
 * In this constructor, getting all data, parsing those data is done.
 *
 * @param iris_data_path A path to iris.data
 */
IrisDataTable::IrisDataTable(const std::string iris_data_path){
	// Open iris data
	std::ifstream iris_file(iris_data_path);
	if (!iris_file) {
		std::cout << "[ERROR] : Couldn't open iris.data file" << std::endl;
	}

	// Split each data from one data
	std::string one_line_sentence;
	int itr = 0;
	//this->data_table.resize(150);
	while (iris_file >> one_line_sentence) {
		// split here
		this->data_table[itr] = this->split(one_line_sentence);
		this->data_table[itr].index = itr;
		itr++;
	}
}

std::pair<std::vector<float>, std::vector<float>> IrisDataTable::generate_data(const unsigned int training_data_size){

	auto copied_table = this->data_table;
	std::vector<float> training_data_vector;
	std::vector<float> verificating_data_vector;

	//shuffle table
	this->shuffle();

	std::cout << "generate_data1" << std::endl;
	std::cout << copied_table.size() << std::endl;
	//generate iterator which represents training data.
	for (int i = 0; i < copied_table.size(); i++) {
		std::cout << i << std::endl;
		auto iris_data = copied_table.at(i);
		copied_table.erase(copied_table.begin()+i);
		training_data_vector.push_back(iris_data.sepal_width);
		training_data_vector.push_back(iris_data.sepal_length);
		training_data_vector.push_back(iris_data.petal_width);
		training_data_vector.push_back(iris_data.petal_length);
	}

	copied_table.shrink_to_fit();

	std::cout << "generate_data2" << std::endl;
	for (auto d : copied_table){
		verificating_data_vector.push_back(d.sepal_width);
		verificating_data_vector.push_back(d.sepal_length);
		verificating_data_vector.push_back(d.petal_width);
		verificating_data_vector.push_back(d.petal_length);
	}
	std::cout << "generate_data3" << std::endl;

	return std::make_pair(training_data_vector, verificating_data_vector);
}

/**
 * @brief Read one line from iris.data, parse it.
 *
 * @param sentence one line from iris.data
 *
 * @return splited data.
 */
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
	std::cout << "parsing result size : " << results.size() << std::endl;
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

	std::cout << "[ Show one data ] : " << "[class number] : " << return_data.class_number << "[class name] : " << return_data.class_name << "\t" << return_data.petal_width << "\t" << return_data.petal_length << "\t" << return_data.sepal_width << "\t" << return_data.sepal_length << std::endl;
	return return_data;
}

IrisData IrisDataTable::at(const int index){
	return this->data_table[index];
}

void IrisDataTable::shuffle(){

	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(this->data_table.begin(), this->data_table.end(), engine);
}

void IrisDataTable::print(){

	this->data_table.shrink_to_fit();
	std::cout << "[ Table information ] " << this->data_table.size() << std::endl;

	for (int itr = 0; itr < this->data_table.size(); itr++) {
		std::cout << "[ " << this->data_table.at(itr).index << " ] " << 
			         "[class]\t" << this->data_table.at(itr).class_name << 
					 "\t: " << this->data_table.at(itr).class_number << 
					 "\t[petal] " << 
					 this->data_table.at(itr).petal_length << 
					 this->data_table.at(itr).petal_width << 
					 "\t[sepal] " <<
					 this->data_table.at(itr).sepal_length << 
					 this->data_table.at(itr).sepal_width << std::endl;
	}
}
