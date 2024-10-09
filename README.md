# OpenCV

## [OpenCV](https://github.com/opencv/opencv)
- [CMake](https://cmake.org/)
    - "Where is the source code" に [opencv](https://github.com/opencv/opencv) をクローンしたレポジトリを指定
    - "Where to build the binaries" に適当なフォルダ (以下 **OPENCV_SDK_PATH** とする) を指定する
    - "Configure"
    - オプション
        - ライブラリが大量にできて使用時に大変なのでまとめる
            - BUILD_opencv_world にチェックを入れる
        - CUDA を使用する場合
            - CUDA は予めインストールしておく
            - [contrib](https://github.com/opencv/opencv_contrib) をクローンする
                - OPENCV_EXTRA_MODULES_PATH に contrib/mudules フォルダを指定する
            - WITH_CUDA にチェックを入れる
            - CMAKE_INSTALL_PREFIX に **OPENCV_SDK_PATH** と同じフォルダを指定しておく
        - GSreamer を使用する場合
            - WIDTH_GSTREAMER、OPENCV_GAPI_GSTREAMER にチェックを入れる
            - GSTREAMER_XXX 系を設定する
                ~~~
                GSTREAMER_app_LIBRARY               C:/gstreamer/1.0/msvc_x86_64/lib/gstapp-1.0.lib
                GSTREAMER_audio_LIBRARY             C:/gstreamer/1.0/msvc_x86_64/lib/gstaudio-1.0.lib
                GSTREAMER_base_LIBRARY              C:/gstreamer/1.0/msvc_x86_64/lib/gstbase-1.0.lib
                GSTREAMER_glib_INCLUDE_DIR          C:/gstreamer/1.0/msvc_x86_64/include/glib-2.0
                GSTREAMER_glib_LIBRARY              C:/gstreamer/1.0/msvc_x86_64/lib/glib-2.0.lib
                GSTREAMER_glibconfig_INCLUDE_DIR    C:/gstreamer/1.0/msvc_x86_64/lib/glib-2.0/include
                GSTREAMER_gobject_LIBRARY           C:/gstreamer/1.0/msvc_x86_64/lib/gobject-2.0.lib
                GSTREAMER_gst_INCLUDE_DIR           C:/gstreamer/1.0/msvc_x86_64/include/gstreamer-1.0
                GSTREAMER_gstreamer_LIBRARY         C:/gstreamer/1.0/msvc_x86_64/lib/gstreamer-1.0.lib
                GSTREAMER_pbutils_LIBRARY           C:/gstreamer/1.0/msvc_x86_64/lib/gstpbutils-1.0.lib
                GSTREAMER_riff_LIBRARY              C:/gstreamer/1.0/msvc_x86_64/lib/gstriff-1.0.lib
                GSTREAMER_video_LIBRARY             C:/gstreamer/1.0/msvc_x86_64/lib/gstvideo-1.0.lib
                ~~~
        - 再度 "Configure"
    - "Generate"
        - OpenCV.sln ができる
- ビルド
    - OpenCV.sln を開く
    - Build - Batch Build - ALL_BUILD (Debug, Release) を行う
    - Build - Batch Build - INSTALL (Debug, Release) を行う
        - **OPENCV_SDK_PATH**\ 以下へインストールされる
- 使用
    - Visutal Studio プロパティの指定
        - 追加のインクルードディレクトリ
            - **OPENCV_SDK_PATH**\include
        - 追加のライブラリディレクトリ
            - **OPENCV_SDK_PATH**\x64\vc17\lib
        - 以下のように .lib を追加する
            ~~~
            #ifdef _DEBUG           
            #pragma comment(lib, "opencv_img_hash4100d.lib")
            #pragma comment(lib, "opencv_world4100d.lib")
            #else           
            #pragma comment(lib, "opencv_img_hash4100.lib")
            #pragma comment(lib, "opencv_world4100.lib")
            #endif
            ~~~
        - DLL の場所を環境変数 Path に通しておく
            - **OPENCV_SDK_PATH**\x64\vc17\bin

    - CUDA 使用時
        - 追加のライブラリディレクトリ
            - $(CUDA_PATH)\lib\x64
        - cudart_static.lib を追加する
            ~~~
            #pragma comment(lib, "cudart_static.lib")
            ~~~
    ## [Gstreamer](https://gstreamer.freedesktop.org/documentation/installing/on-windows.html?gi-language=c)

    - [ダウンロード](https://gstreamer.freedesktop.org/download/#windows)
        - Gstreamerの開発目的では runtime, development の両方をインストール
            - デフォルト設定ではインストールされない項目がある、全選択して全てインストールしておく
        - デフォルトでは、以下のような場所にインストールされるので、環境変数 Path を通しておく
            ~~~
            C:\gstreamer\1.0\msvc_x86_64\bin
            ~~~
        - 確認
            ~~~
            $gst-inspect-1.0 --version
            $gst-launch-1.0 videotestsrc ! autovideosink
            ~~~
