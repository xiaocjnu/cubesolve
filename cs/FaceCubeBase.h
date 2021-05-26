#pragma once
#include "util.h"
#include <array>
#include <string>
#include <sstream>

#undef max

namespace cs
{


	// Face level Cube, 可以表示 facelet 和 color
	class FaceCubeBase : public std::array<int, 54>
	{
	public:
		using _base = std::array<int, 54>;
		using _array_t = std::array<int, 54>;

	public:
		FaceCubeBase();
		FaceCubeBase(const std::array<int, 54>& _face);
		FaceCubeBase(const FaceCubeBase& src);

		FaceCubeBase move(Move mv) const;
		FaceCubeBase move(int mv) const { return move(Move(mv)); };

		std::string toString() const;	// 使用 URFDLB 名字
		std::string toString2() const;	// 使用 WRGYOB 名字

		// 以易于阅读的方式打印
		void printIndex();

		std::string sprintIndex();

	protected:
		template<typename T>
		static void _print(const char* fmt, T* s);

		template<typename T>
		static std::string _sprint(const char* fmt, T* s);

	};


	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	void FaceCubeBase::_print(const char* fmt, T* s)
	{
		static const int idx[54] = 
		{
			0, 1, 2,    3, 4, 5,     6, 7, 8,
			36, 37, 38, 18, 19, 20, 9, 10, 11, 45, 46, 47,
			39, 40, 41, 21, 22, 23, 12, 13, 14, 48, 49, 50,
			42, 43, 44, 24, 25, 26, 15, 16, 17, 51, 52, 53,
			27, 28, 29,   30, 31, 32,   33, 34, 35
		};

// 		static const vector<vector<int>> idx =
// 		{
// 			vector<int>{0, 1, 2}, vector<int>{3, 4, 5}, vector<int>{6, 7, 8},
// 			vector<int>{36, 37, 38, 18, 19, 20, 9, 10, 11, 45, 46, 47},
// 			vector<int>{39, 40, 41, 21, 22, 23, 12, 13, 14, 48, 49, 50},
// 			vector<int>{42, 43, 44, 24, 25, 26, 15, 16, 17, 51, 52, 53},
// 			vector<int>{27, 28, 29}, vector<int>{30, 31, 32}, vector<int>{33, 34, 35} };


		int ind = 0;

		for (int i = 0; i < 3; ++i)
		{
			printf("      | ");

			for (int j = 0; j < 3; ++j)
			{
				printf(fmt, s[idx[ind++]]);
			}
			printf("|\n");
		}

		printf("------|-------|-------|------\n");

		for (int i = 3; i < 6; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				printf(fmt, s[idx[ind++]]);
			}

			printf("| ");
			for (int j = 3; j < 6; ++j)
			{
				printf(fmt, s[idx[ind++]]);
			}
			printf("| ");
			for (int j = 6; j < 9; ++j)
			{
				printf(fmt, s[idx[ind++]]);
			}
			printf("| ");
			for (int j = 9; j < 12; ++j)
			{
				printf(fmt, s[idx[ind++]]);
			}
			printf("\n");
		}


		printf("------|-------|-------|------\n");

		for (int i = 6; i < 9; ++i)
		{
			printf("      | ");

			for (int j = 0; j < 3; ++j)
			{
				printf(fmt, s[idx[ind++]]);
			}
			
			printf("|\n");
		}


		printf("\n");

		assert(ind == 54);

	}

	template<typename T>
	inline std::string FaceCubeBase::_sprint(const char * fmt, T * s)
	{
		static const int idx[54] =
		{
			0, 1, 2,    3, 4, 5,     6, 7, 8,
			36, 37, 38, 18, 19, 20, 9, 10, 11, 45, 46, 47,
			39, 40, 41, 21, 22, 23, 12, 13, 14, 48, 49, 50,
			42, 43, 44, 24, 25, 26, 15, 16, 17, 51, 52, 53,
			27, 28, 29,   30, 31, 32,   33, 34, 35
		};

		int ind = 0;

		std::stringstream ss;

		const size_t buf_size = 32;
		char buf[32];
		

		for (int i = 0; i < 3; ++i)
		{
			ss << "      | ";

			for (int j = 0; j < 3; ++j)
			{
				snprintf(buf, buf_size, fmt, s[idx[ind++]]);
				ss << buf;
			}
			ss << "|\n";
		}

		ss << "------|-------|-------|------\n";

		for (int i = 3; i < 6; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				snprintf(buf, buf_size, fmt, s[idx[ind++]]);
				ss << buf;
			}

			ss << "| ";
			for (int j = 3; j < 6; ++j)
			{
				snprintf(buf, buf_size, fmt, s[idx[ind++]]);
				ss << buf;
			}
			ss << "| ";
			for (int j = 6; j < 9; ++j)
			{
				snprintf(buf, buf_size, fmt, s[idx[ind++]]);
				ss << buf;
			}
			
			ss << "| ";
			for (int j = 9; j < 12; ++j)
			{
				snprintf(buf, buf_size, fmt, s[idx[ind++]]);
				ss << buf;
			}
			ss << "\n";
		}


		ss << "------|-------|-------|------\n";

		for (int i = 6; i < 9; ++i)
		{
			ss << "      | ";

			for (int j = 0; j < 3; ++j)
			{
				snprintf(buf, buf_size, fmt, s[idx[ind++]]);
				ss << buf;
			}

			ss << "|\n";
		}


		ss << "\n";

		assert(ind == 54);

		return ss.str();
	}

	class FaceCube;

	//////////////////////////////////////////////////////////////////////////
	// 0 - 5, 表示颜色
	class ColorCube : public FaceCubeBase
	{
	public:

		ColorCube();
		ColorCube(const ColorCube& src);
		ColorCube(const _array_t& _face);
		ColorCube(const std::string& _face);


		void printColor() const;		// 使用 URFDLB 名字
		void printColor2() const;		// 使用 WRGYOB 名字

		std::string sprintColor() const;		// 使用 URFDLB 名字
		std::string sprintColor2() const;		// 使用 WRGYOB 名字

		FaceCube toFaceCube() const;
	};


	//////////////////////////////////////////////////////////////////////////
	// 0 - 53， 表示 facelet
	class FaceCube : public FaceCubeBase
	{
	public:
		FaceCube();
		FaceCube(const FaceCube& src);
		FaceCube(const _array_t& _facelets);

		ColorCube toColorCube() const;
	};

	//////////////////////////////////////////////////////////////////////////
	// 不分方向的6面颜色数据
	typedef std::array<int, 9> SingleFaceColor;
	typedef std::array<SingleFaceColor, 6> SixFacesColor;



	// single_face_color_t 顺时针旋转 n 个 90度
	SingleFaceColor singleFaceColorRotate90(const SingleFaceColor& ff, int n);


	// 排序中心块，使之按顺序排列,不做唯一性判断
	SixFacesColor sixFacesColorSortCenter(const SixFacesColor& ff);


	SixFacesColor colorCubeToSixFacesColor(const ColorCube& cc);

	// 把不分方向的6面颜色数据转为 ColorCube
	// max_num 最大搜索数量
	std::vector< ColorCube> sixFacesColorToColorCube(const SixFacesColor& ff, int max_num = std::numeric_limits<int>::max());




}