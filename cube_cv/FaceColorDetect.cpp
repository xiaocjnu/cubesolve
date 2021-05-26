#include "FaceColorDetect.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

namespace cube_cv {
    //////////////////////////////////////////////////////////////////////////
    template<typename _Tp>
    _Tp _median(const cv::Mat_<_Tp> &I) {
        vector<_Tp> v;

        int num = I.rows * I.cols;
        v.reserve(num);


        for (auto it = I.begin(); it < I.end(); ++it) {
            v.push_back(*it);
        }

        sort(v.begin(), v.end());

        return v[num / 2];
    }


    template<typename _Tp>
    void
    _imkmeans(const cv::Mat_<_Tp> &I, int num_kmeans, double tolX, int max_iter, cv::Mat &labels,
              cv::Mat &centers) {
        typedef cv::Mat_<_Tp> mat_type;
        typedef cv::Mat_<int> label_mat_type;

        assert(I.dims == 2);

        int m = I.rows;
        int dim = I.cols;



        // 随机选取中心
        centers = mat_type(num_kmeans, dim);


        for (int k = 0; k < num_kmeans; ++k) {
// 			int i = cs::randInt(0, m - 1);
// 			centers.row(k) = I.row(k);
        }


        labels = label_mat_type::zeros(m, dim);


        // 迭代



        for (int iter = 0; iter < max_iter; ++iter) {

            // 查找最近点确定label
            mat_type sum_centers = mat_type::zeros(num_kmeans, dim);
            Mat_<int> num_centers = label_mat_type::zeros(num_kmeans, 1);


            for (int i = 0; i < m; ++i) {
                mat_type a = I.row(i);


                _Tp min_dd;
                int min_k;

                for (int k = 0; k < num_kmeans; ++k) {
                    mat_type d = a - centers.row(k);

                    mat_type dd_m = d * d.t();

                    assert(dd_m.rows == 1 && dd_m.cols == 1);

                    _Tp dd = dd_m(0);

                    if (k == 0 || dd < min_dd) {
                        min_dd = dd;
                        min_k = k;
                    }

                }

                labels.at<int>(i) = min_k;

                sum_centers.row(min_k) += a;
                ++num_centers(min_k);
            }// for i

            // 更新中心
            for (int k = 0; k < num_kmeans; ++k) {
                centers.row(k) = sum_centers.row(k) / num_centers(k);
            }


        }// for iter
    }


    //////////////////////////////////////////////////////////////////////////

    bool FaceColorDetect::detect(const cv::Mat &I, cv::Rect rect) {

        myRect = rect;


        if (rect.width < 30 || rect.height < 30 || rect.width != rect.height)    // 矩形太小或者不是正方形
        {
            myFound = false;
            return myFound;
        }

        if (I.channels() < 3)    // 不是 RGB 颜色
        {
            myFound = false;
            return myFound;
        }


        // 转为double, 提出RGB 3通道
        Mat I2 = Mat(I.rows, I.cols, CV_MAKETYPE(I.depth(), 3));
        mixChannels(I, I2, vector<int>{0, 0, 1, 1, 2, 2});
        I2.convertTo(I2, CV_32FC3);
        I2 /= 255.0;

        myRaw = I2;


        if (rect.x < 0 || rect.x + rect.width > myRaw.cols ||
            rect.y < 0 || rect.y + rect.height > myRaw.rows)    // 矩形超出图像范围
        {
            myFound = false;
            return myFound;
        }

        cvtColor(myRaw, myLab, COLOR_RGB2Lab);    // 转为 Lab 空间


        // 分割图片区域

        myPieceGridX = {rect.x, rect.x + rect.width / 3, rect.x + 2 * rect.width / 3,
                        rect.x + rect.width};
        myPieceGridY = {rect.y, rect.y + rect.height / 3, rect.y + 2 * rect.height / 3,
                        rect.y + rect.height};


        int k = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j, ++k) {
                myPieceRect[k] = Rect(myPieceGridX[j], myPieceGridY[i],
                                      myPieceGridX[j + 1] - myPieceGridX[j],
                                      myPieceGridY[j + 1] - myPieceGridY[j]);
                myPiece[k] = myRaw(myPieceRect[k]);
            }
        }

		Mat se = Mat::ones(5, 5, CV_32F);

        for (int k = 0; k < 9; ++k) {
            Mat pk;

            int m = myPiece[k].rows;
            int n = myPiece[k].cols;

            pk = myPiece[k];

			Mat piece_erode;
			if (k == 4)
			{
				erode(1.0f - pk, piece_erode, se);
				piece_erode = 1.0f - piece_erode;
			}
			else
			{
				piece_erode = pk;
			}


			Vec3f color_lab = pieceColor(piece_erode);
				

            myPieceColor[k] = color_lab;
        }// for k



        myFound = true;

        return myFound;

    }


    //////////////////////////////////////////////////////////////////////////
    bool FaceColorDetect::detect_kmeans(const cv::Mat &I, cv::Rect rect) {

        myRect = rect;


        if (rect.width < 30 || rect.height < 30 || rect.width != rect.height)    // 矩形太小或者不是正方形
        {
            myFound = false;
            return myFound;
        }

        if (I.channels() < 3)    // 不是 RGB 颜色
        {
            myFound = false;
            return myFound;
        }


        // 转为double, 提出RGB 3通道
        Mat I2 = Mat(I.rows, I.cols, CV_MAKETYPE(I.depth(), 3));
        mixChannels(I, I2, vector<int>{0, 0, 1, 1, 2, 2});
        I2.convertTo(I2, CV_32FC3);
        I2 /= 255.0;

        myRaw = I2;


        if (rect.x < 0 || rect.x + rect.width > myRaw.cols ||
            rect.y < 0 || rect.y + rect.height > myRaw.rows)    // 矩形超出图像范围
        {
            myFound = false;
            return myFound;
        }

        cvtColor(myRaw, myLab, COLOR_RGB2Lab);    // 转为 Lab 空间


        // 分割图片区域

        myPieceGridX = {rect.x, rect.x + rect.width / 3, rect.x + 2 * rect.width / 3,
                        rect.x + rect.width};
        myPieceGridY = {rect.y, rect.y + rect.height / 3, rect.y + 2 * rect.height / 3,
                        rect.y + rect.height};


        int k = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j, ++k) {
                myPieceRect[k] = Rect(myPieceGridX[j], myPieceGridY[i],
                                      myPieceGridX[j + 1] - myPieceGridX[j],
                                      myPieceGridY[j + 1] - myPieceGridY[j]);
                myPiece[k] = myLab(myPieceRect[k]);
            }
        }


        for (int k = 0; k < 9; ++k) {
            Mat X, Y;

            Mat pk;
            myPiece[k].convertTo(pk, CV_32FC3);


            int m = myPiece[k].rows;
            int n = myPiece[k].cols;

            meshgrid(Range(0, myPiece[k].cols), Range(0, myPiece[k].rows), X, Y);
            X.convertTo(X, CV_32FC1);
            Y.convertTo(Y, CV_32FC1);


            merge(vector<Mat>{pk, X, Y}, myFeatureSet[k]);

            Mat ft = myFeatureSet[k].reshape(1, m * n);

            TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 100, 0.1);


            // kmeans 分类
            Mat labels, centers;

            int num_kmeans = 3;

            //kmeans(ft, num_kmeans, labels, criteria, 100, KMEANS_RANDOM_CENTERS, centers);
            imkmeans(ft, num_kmeans, 0.1, 10, labels, centers);

            labels = Mat::zeros(m * n, 1, CV_32SC1);
            centers = Mat::zeros(3, 5, CV_32FC1);
            labels = labels.reshape(0, m);

            vector<int> h = histcounts(labels, num_kmeans);
            vector<int> ix(h.size());
            for (int i = 0; i < (int)h.size(); ++i)
                ix[i] = i;


            sort(ix.begin(), ix.end(), [h](int a, int b) {
                return h[a] > h[b];
            });


            int label_max = ix[0];
            Scalar lab_max = {centers.at<float>(label_max, 0), centers.at<float>(label_max, 1),
                              centers.at<float>(label_max, 2)};

            if (lab_max[0] < 10.0 &&
                sqrt(lab_max[1] * lab_max[1] + lab_max[2] * lab_max[2]) < 10)    // 忽略黑色
            {
                lab_max = ix[1];
            }

            // 计算 myPieceColor = 平均
            Vec3f _sum = {0.0, 0.0, 0.0};
            int _num = 0;
            for (int i = 0; i < labels.rows; ++i) {
                for (int j = 0; j < labels.cols; ++j) {
                    if (labels.at<int>(i, j) == label_max) {
                        ++_num;
                        _sum += myPiece[k].at<Vec3f>(i, j);
                    }
                }
            }

            Vec3f _av = _sum / _num;
            myPieceColor[k] = _av;
        }// for k



        myFound = true;

        return myFound;

    }

    std::vector<int> FaceColorDetect::histcounts(const cv::Mat_<int> &I, int k) {
        std::vector<int> cc(k, 0);

        for (auto it = I.begin(); it < I.end(); ++it) {
            int a = *it;

            if (0 <= a && a < k) {
                ++cc[a];
            }

        }

        return cc;
    }


    void FaceColorDetect::imkmeans(const cv::Mat &I, int num_kmeans, double tolX, int max_iter,
                                   cv::Mat &labels, cv::Mat &centers) {
        int dp = I.depth();
        int cn = I.channels();


        switch (dp) {
            case CV_8U:
                _imkmeans((const Mat_<uchar> &) I, num_kmeans, tolX, max_iter, labels, centers);
                break;
            case CV_8S:
                _imkmeans((const Mat_<char> &) I, num_kmeans, tolX, max_iter, labels, centers);
                break;

            case CV_16U:
                _imkmeans((const Mat_<ushort> &) I, num_kmeans, tolX, max_iter, labels, centers);
                break;

            case CV_16S:
                _imkmeans((const Mat_<short> &) I, num_kmeans, tolX, max_iter, labels, centers);
                break;

            case CV_32S:
                _imkmeans((const Mat_<int> &) I, num_kmeans, tolX, max_iter, labels, centers);
                break;

            case CV_32F:
                _imkmeans((const Mat_<float> &) I, num_kmeans, tolX, max_iter, labels, centers);
                break;

            case CV_64F:
                _imkmeans((const Mat_<double> &) I, num_kmeans, tolX, max_iter, labels, centers);
                break;

// 		case CV_16F:
// 			break;

            default:
                throw ("FaceColorDetect::imkmeans: error type of I");

        }


    }

	cv::Vec3f FaceColorDetect::pieceColor(const cv::Mat & I)
	{
		Mat rgb[3];

		split(I, rgb);


		float md_r = _median((const Mat_<float> &) rgb[0]);
		float md_g = _median((const Mat_<float> &) rgb[1]);
		float md_b = _median((const Mat_<float> &) rgb[2]);


		Vec3f color_rgb = Vec3f(md_r, md_g, md_b);

		Mat_<Vec3f> m_color_rgb = Mat_<Vec3f>::zeros(1, 1);
		m_color_rgb(0) = color_rgb;

		Mat color_lab;
		cvtColor(m_color_rgb, color_lab, COLOR_RGB2Lab);

		return color_lab.at<Vec3f>(0);
	}

    std::vector<paster_color_t> FaceColorDetect::color_recognize(const std::array<cv::Vec3f, 54>& colors) {
        std::vector<paster_color_t> result(54);
        std::vector<paster_color_t> no_result;


        // 到(a,b)原点的距离
        vector<double> dd(54);
        vector<int> idx(54);
        vector<bool> occupied(54);
        vector<double> L(54);
        vector<double> a(54);
        vector<double> b(54);
        vector<double> ang(54);


        for (int i = 0; i < 54; ++i) {
            idx[i] = i;
            occupied[i] = false;
            L[i] = colors[i][0];
            a[i] = colors[i][1];
            b[i] = colors[i][2];
            ang[i] = atan2(b[i], a[i]) * 180.0 / M_PI - (-90.0);
			ang[i] = ang[i] < 0 ? ang[i] + 360.0 : ang[i];

            dd[i] = sqrt(a[i] * a[i] + b[i] * b[i]);
        }


        // 白色， 对 dd 进行排序，只改变下标

        std::vector<int> ix = idx;
        sort(ix.begin(), ix.end(), [dd](int i, int j) {
            return dd[i] < dd[j];
        });


        // 最靠近0点的是白色
        for (int i = 0; i < 9; ++i) {
            result[ix[i]] = White;
            occupied[ix[i]] = true;
            b[ix[i]] = 1e10;    // 把白色的b值忽略
            ang[ix[i]] = -1e10;    // 把绿色，黄色的角度忽略

            if (colors[ix[i]][0] < 30)    // L 值太小，亮度不够
                return no_result;
        }


        if (!(dd[ix[7]] < 20))    // 有颜色，不是白色, 允许 logo 不白
            return no_result;


        if (!(dd[ix[9]] > 20))        // 除了白色，其余必须有颜色
            return no_result;



        // 蓝色，对 b 进行排序
        ix = idx;
        sort(ix.begin(), ix.end(), [b](int i, int j) {
            return b[i] < b[j];
        });


        // b 最小的是蓝色
        for (int i = 0; i < 9; ++i) {
            result[ix[i]] = Blue;
            occupied[ix[i]] = true;
            ang[ix[i]] = -1e10;    // 把绿色，黄色的角度忽略
        }


        if (!(b[ix[8]] < -10))    // 蓝色 b 值
            return no_result;


        if (!(b[ix[9]] > 10))        // 其他颜色 b 值
            return no_result;


        // 绿色和黄色，比较角度, 降序排序

        ix = idx;
        sort(ix.begin(), ix.end(), [ang](int i, int j) {
            return ang[i] > ang[j];
        });


        // 最大角度的9个是绿色
        for (int i = 0; i < 9; ++i) {
            result[ix[i]] = Green;
            occupied[ix[i]] = true;


            if (!(a[ix[i]] < 0) || !(b[ix[i]] > 10))    // 必须在第二象限, 小部分第三象限
                return no_result;
        }

        // 黄色次之
        for (int i = 9; i < 18; ++i) {
            result[ix[i]] = Yellow;
            occupied[ix[i]] = true;

            if (!(b[ix[i]] > 0))    // 必须在第一或二象限
                return no_result;
        }


        // 同种亮色角度范围必须小于其间隙
        double gap = ang[ix[8]] - ang[ix[9]];
        if (!(ang[ix[8]] - ang[ix[0]] < gap && ang[ix[17]] - ang[ix[9]] < gap))
            return no_result;


        // 红色，橙色，使用 kmeans 分类
        vector<Vec3f> colors_ro;
        vector<int> idx_ro;
        for (int i = 0; i < 54; ++i) {
            if (!occupied[i]) {
                idx_ro.push_back(i);
                colors_ro.push_back(colors[i]);
            }

        }

        assert(colors_ro.size() == 18);

        cv::Mat m_colors_ro(colors_ro);


        cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 50, 1.0);
        cv::Mat labels, centers;
        kmeans(m_colors_ro, 2, labels, criteria, 50, KMEANS_RANDOM_CENTERS, centers);


        vector<int> idx0, idx1;

        for (int i = 0; i < labels.rows; ++i) {
            if (labels.at<int>(i) == 0)
                idx0.push_back(idx_ro[i]);
            else
                idx1.push_back(idx_ro[i]);
        }


        if (idx0.size() != 9)
            return no_result;


		double ro_ang[2];
		
		for (int i = 0; i < 2; ++i)
		{
			ro_ang[i] = atan2(centers.at<float>(i, 2), centers.at<float>(i, 1)) * 180.0 / M_PI - (-90.0);
			ro_ang[i] = ro_ang[i] < 0 ? ro_ang[i] + 360.0 : ro_ang[i];
        };



        double ro_dd[2] = {
                sqrt(centers.at<float>(0, 1) * centers.at<float>(0, 1) +
                     centers.at<float>(0, 2) * centers.at<float>(0, 2)),
                sqrt(centers.at<float>(1, 1) * centers.at<float>(1, 1) +
                     centers.at<float>(1, 2) * centers.at<float>(1, 2))
        };

        int idx_red;

        if (abs(ro_ang[0] - ro_ang[1]) > 5) // 颜色能区别, 角度大的是橙色
        {
            if (ro_ang[0] > ro_ang[1]) {
                idx_red = 1;
            } else {
                idx_red = 0;
            }
        } else // 颜色不能区别，看级饱和度, 饱和度低的是红色(更暗)
        {
            if (ro_dd[0] < ro_dd[1]) {
                idx_red = 0;
            } else {
                idx_red = 1;
            }
        }


        if (idx_red == 0) {
            for (int i = 0; i < 9; ++i) {
                result[idx0[i]] = Red;
                result[idx1[i]] = Orange;
            }
        } else {
            for (int i = 0; i < 9; ++i) {
                result[idx0[i]] = Orange;
                result[idx1[i]] = Red;
            }
        }

        return result;
    }


}
	
