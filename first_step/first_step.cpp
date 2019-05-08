#include <iostream>
#include <vector>
#include <dynet/init.h>

int main(int argc, char const* argv[])
{
	//	パラメータ設定用の構造体
	dynet::DynetParams params;

	//	確保するメモリ量を３つ指定する(MB単位)
	//	それぞれ，forward-path, backword-path, パラメータで使用する値
	params.mem_descriptor = "1024";

	params.random_seed = 0;				//	ランダムシード，０なら適当に初期化
	params.weight_decay = 0.0;			//	L2正則化の強さ
	params.shared_parameters = false;	//	パラメータをshared memory上に載せるかどうか

	//	GPUに関する設定
	//	メモリに余裕があるGPUを一個選んで使う
	const int MAX_GPUS = 1024;	//	刺さっているGPU以上の値にすれば良い
	params.ngpus_requested = false;
	params.ids_requested   = false;
	params.requested_gpus  = -1;
	params.gpu_mask =	   std::vector<int>(MAX_GPUS, 0);


	//	実際に初期化
	dynet::initialize(params);

	//	ここでやりたい処理を書く
	
	return 0;
}

