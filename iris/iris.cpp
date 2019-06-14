#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <dynet/dynet.h>	// 基本的なクラス
#include <dynet/training.h> // トレーナ
#include <dynet/expr.h>		// Expression,parameterのため

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
	double sepal_length;
	double sepal_width;
	double petal_length;
	double petal_width;
	std::string iris_class;
	int itr = 0;
	char delim = ',';
	while(iris_file >> sepal_length >> delim >> sepal_width >> delim >> petal_length >> delim >> petal_width >> delim >> iris_class){
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
	// iris データの読み込み
	auto[sepal_length_vec, sepal_width_vec, petal_length_vec, petal_width_vec, iris_class_num_vec] = load_iris_data();
	
	// dynetの初期化
	dynet::initialize(argc, argv);

	// 設定
	const int HIDDEN_LAYER_SIZE = 16;
	const int INPUT_LAYER_SIZE = 4;
	const int OUTPUT_LAYER_SIZE = 3;

	// 計算グラフを構築
	dynet::ParameterCollection model;
	dynet::SimpleSGDTrainer trainer(model);

	// パラメータを設定（パラメータ＝最適化される変数）
	// W1 (8x4) matrix
	dynet::Parameter p_W1 = model.add_parameters({HIDDEN_LAYER_SIZE, INPUT_LAYER_SIZE});
	// b1 (16x1) vector
	dynet::Parameter p_b1 = model.add_parameters({HIDDEN_LAYER_SIZE});
	// W2 (3x16) matrix
	dynet::Parameter p_W2 = model.add_parameters({OUTPUT_LAYER_SIZE, HIDDEN_LAYER_SIZE});
	// b2 (3x1) vector
	dynet::Parameter p_b2 = model.add_parameters({OUTPUT_LAYER_SIZE});

	// ノードの作成と計算グラフへの登録
	dynet::ComputationGraph cg;
	dynet::Expression W1 = dynet::parameter(cg, p_W1);
	dynet::Expression b1 = dynet::parameter(cg, p_b1);
	dynet::Expression W2 = dynet::parameter(cg, p_W2);
	dynet::Expression b2 = dynet::parameter(cg, p_b2);
	// ニューラルネット及び，計算グラフへの入力変数x_valueを宣言し，変更可能なように参照渡しする．
	std::vector<dynet::real> x_value(4);
	dynet::Expression x = dynet::input(cg, {4}, x_value);
	// ニューラルネット及び，計算グラフへの入力変数y_labelを宣言し，変更可能なようにポインタ渡しをする．
	// *y_label = 0 : setosa
	// *y_label = 1 : versicolor
	// *y_label = 2 : virginica
	unsigned int* y_label = 0;
	// 計算グラフにノードの接続関係を宣言する
	dynet::Expression z1 = dynet::rectify(W1*x+b1);		// 入力層
	dynet::Expression y_pred = dynet::softmax(W2*z1+b2);// 第１層

	dynet::Expression loss_expr = dynet::pickneglogsoftmax(y_pred, y_label); // 出力層

	// 構築した計算グラフを描いてみる．それが僕には楽しかったから
	cg.print_graphviz();

	// 学習させてみる.
	double loss_value = 0;
	for (int itr = 0; itr < iris_class_num_vec.size(); itr++) {
		// 正解ラベルの設定
		*y_label = iris_class_num_vec[itr];

		// 入力を設定
		x_value[0] = sepal_length_vec[itr];
		x_value[1] = sepal_width_vec[itr];
		x_value[2] = petal_length_vec[itr];
		x_value[3] = petal_width_vec[itr];
		
		// setosa	  : 0   - 49
		if (0 <= itr && itr < 50) {
			loss_value = dynet::as_scalar(cg.forward(loss_expr));
		}
		// versicolor : 50  - 99
		if (50 <= itr && itr < 100) {
			loss_value = dynet::as_scalar(cg.forward(loss_expr));
		}
		// virginica  : 100 - 149
		if (100 <= itr && itr < 150) {
			loss_value = dynet::as_scalar(cg.forward(loss_expr));
		}
		cg.backward(loss_expr);
		trainer.update();
		std::cout << "E = " << loss_value << std::endl;
	}
	return 0;
}
