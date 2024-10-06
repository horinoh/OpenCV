#include <cstdlib>
#include <iostream>
#include <format>
#include <filesystem>

#include "../CV.h"

//!< https://github.com/opencv/opencv をクローンし、そのフォルダを環境変数 OPENCV_REPO_PATH に設定してある前提
static auto GetOpenCVRepoPath() {
	char* RepoPath;
	size_t Size = 0;
	if (!_dupenv_s(&RepoPath, &Size, "OPENCV_REPO_PATH")) {
		const auto Path = std::filesystem::path(RepoPath);
		free(RepoPath);
		return Path;
	}
	return std::filesystem::path();
}

static cv::Mat& ToGrayScale(cv::Mat& Disparity) {
	//!< [0, 255] にしてグレースケール表示
	double MinVal, MaxVal;
	cv::minMaxLoc(Disparity, &MinVal, &MaxVal);
	const auto Tmp = 255.0 / (MaxVal - MinVal);
	Disparity.convertTo(Disparity, CV_8UC1, Tmp, -Tmp * MinVal);

	//!< 黒が凹、白が凸になるように反転
	//Disparity = ~Disparity;

	return Disparity;
}

int main() 
{
	//!< ビルド情報
	std::cout << cv::getBuildInformation() << std::endl;

	//!< CUDA サポートをチェック
	const auto HasCuda = 0 < cv::cuda::getCudaEnabledDeviceCount();
	std::cout << std::format("CUDA {} supported\n", HasCuda ? "" : "not") << std::endl;
	for (auto i = 0; i < cv::cuda::getCudaEnabledDeviceCount(); ++i) {
		cv::cuda::printCudaDeviceInfo(i);
	}

	//!< レポジトリの samples / data 以下に各種サンプル画像がある
	const auto CVSampleDataPath = GetOpenCVRepoPath() / "samples" / "data";

	auto L = cv::imread((CVSampleDataPath / "aloeL.jpg").string());
	auto R = cv::imread((CVSampleDataPath / "aloeR.jpg").string());

	cv::Mat Concat;
	cv::hconcat(L, R, Concat);
	cv::imshow("Stereo", Concat);

	if (HasCuda) {
		int NumDisparities, NumIters, NumLevels;
#if false
		//!< [ StereoBeliefPropagation ]
		cv::cuda::StereoBeliefPropagation::estimateRecommendedParams(640, 240, NumDisparities, NumIters, NumLevels);
		auto Matcher = cv::cuda::createStereoBeliefPropagation(NumDisparities, NumIters, NumLevels);
#else
		//!< [ StereoConstantSpaceBP ]
		int NrPlane;
		cv::cuda::StereoConstantSpaceBP::estimateRecommendedParams(640, 240, NumDisparities, NumIters, NumLevels, NrPlane);
		auto Matcher = cv::cuda::createStereoConstantSpaceBP(NumDisparities, NumIters, NumLevels, NrPlane);
#endif

		//!< GPU 用マトリクス
		cv::cuda::GpuMat GL, GR;
		cv::cuda::GpuMat GDisparity(L.size(), CV_8U);

		//!< GPU へアップロード
		GL.upload(L);
		GR.upload(R);
		{
			//!< GPU で計算
			Matcher->compute(GL, GR, GDisparity);
		}
		//!< GPU からダウンロード
		auto Disparity = cv::Mat();
		GDisparity.download(Disparity);

		//!< 表示用にグレースケール変換
		ToGrayScale(Disparity);
		cv::imshow("Disparity", Disparity);

		cv::waitKey();
	}
	else {
		//!< [ StereoBM ]
		auto Matcher = cv::cuda::createStereoBM();

		const auto Size = cv::Size(640, 240);
		cv::resize(L, L, Size);
		cv::resize(R, R, Size);
		cv::cvtColor(L, L, cv::COLOR_BGR2GRAY);
		cv::cvtColor(R, R, cv::COLOR_BGR2GRAY);

		auto Disparity = cv::Mat();
		//auto Disparity = cv::Mat(Size, CV_8UC1);
		Matcher->compute(L, R, Disparity);

		//!< 表示用にグレースケール変換
		ToGrayScale(Disparity);
		cv::imshow("Disparity", Disparity);

		cv::waitKey();
	}

	exit(EXIT_SUCCESS);
}