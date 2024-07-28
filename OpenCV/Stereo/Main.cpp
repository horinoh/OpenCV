#include <cstdlib>
#include <iostream>
#include <format>
#include <filesystem>

#pragma warning(push)
#pragma warning(disable : 4819)
#include <opencv2/opencv.hpp>
#pragma warning(pop)
#include <opencv2/cudastereo.hpp>

#ifdef _DEBUG           
#pragma comment(lib, "opencv_img_hash4100d.lib")
#pragma comment(lib, "opencv_world4100d.lib")
#else           
#pragma comment(lib, "opencv_img_hash4100.lib")
#pragma comment(lib, "opencv_world4100.lib")
#endif
#pragma comment(lib, "cudart_static.lib")

//!< https://github.com/opencv/opencv ���N���[�����A���̃t�H���_�����ϐ� OPENCV_REPO_PATH �ɐݒ肵�Ă���O��
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

int main() 
{
	//!< CUDA �T�|�[�g���`�F�b�N
	std::cout << std::format("CUDA {} supported\n", 0 < cv::cuda::getCudaEnabledDeviceCount() ? "" : "not") << std::endl;
	for (auto i = 0; i < cv::cuda::getCudaEnabledDeviceCount(); ++i) {
		cv::cuda::printCudaDeviceInfo(i);
	}

	//!< ���|�W�g���� samples / data �ȉ��Ɋe��T���v���摜������
	const auto CVSampleDataPath = GetOpenCVRepoPath() / "samples" / "data";

	auto L = cv::imread((CVSampleDataPath / "aloeL.jpg").string());
	auto R = cv::imread((CVSampleDataPath / "aloeR.jpg").string());

	cv::Mat Concat;
	cv::hconcat(L, R, Concat);
	cv::imshow("Stereo", Concat);

	cv::waitKey();

	exit(EXIT_SUCCESS);
}