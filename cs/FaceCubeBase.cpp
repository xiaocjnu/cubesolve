#include "FaceCubeBase.h"
#include "CubieCube.h"
#include <iostream>
#include <vector>
#include <array>

using namespace std;

namespace cs
{
	//////////////////////////////////////////////////////////////////////////
	// FaceCubeBase
	//////////////////////////////////////////////////////////////////////////

	FaceCubeBase::FaceCubeBase()
	{
	}

	FaceCubeBase::FaceCubeBase(const array<int, 54>& _face)
		: _base(_face)
	{
	}

	FaceCubeBase::FaceCubeBase(const FaceCubeBase & src)
		: _base(src)
	{
	}


	FaceCubeBase FaceCubeBase::move(Move mv) const
	{
		FaceCubeBase ret = *this;
		FaceCubeBase tmp = *this;

		int po = (mv % 3) + 1;
		int ax = mv / 3;
		for (int k = 0; k < po; ++k)
		{
			for (int i = 0; i < 54; ++i)
			{
				ret[i] = tmp[face_move_table[ax][i]];
			}

			tmp = ret;
		}

		return ret;
	}


	string FaceCubeBase::toString() const
	{
		string ret("                                                      ");   // 54 chars

		assert(ret.size() == 54);

		for (int i = 0; i < 54; ++i)
		{
			ret[i] = colorNames[(*this)[i]];
		}

		return ret;
	}


	string FaceCubeBase::toString2() const
	{
		string ret("                                                      ");   // 54 chars

		assert(ret.size() == 54);

		for (int i = 0; i < 54; ++i)
		{
			ret[i] = colorNames2[(*this)[i]];
		}

		return ret;
	}

	void FaceCubeBase::printIndex()
	{
		_print("%d ", &((*this)[0]));
	}

	std::string FaceCubeBase::sprintIndex()
	{
		return _sprint("%d ", &((*this)[0]));
	}


	//////////////////////////////////////////////////////////////////////////
	// FaceColorCube
	//////////////////////////////////////////////////////////////////////////
	ColorCube::ColorCube()
		: FaceCubeBase({
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5 })
	{

	}

	ColorCube::ColorCube(const ColorCube& src)
		: FaceCubeBase(src)
	{
	}

	ColorCube::ColorCube(const _array_t& _face)
		: FaceCubeBase(_face)
	{
	}

	ColorCube::ColorCube(const string& _face)
	{
		assert(_face.size() == 54);

		try
		{
			for (int i = 0; i < 54; ++i)
			{
				char c = _face[i];

				int idx = -1;
				for (int j = 0; j < 6; ++j)
				{
					if (c == colorNames[j])
					{
						idx = j;
						break;
					}
				}


				(*this)[i] = idx;
			}
		}
		catch (const exception& e)
		{
			cerr << "Error: FaceCube::FaceCube(string _face)" << endl << e.what() << endl;
			throw e;
		}
		
	}

	FaceCube ColorCube::toFaceCube() const
	{
		
		CubieCube cc(*this);
		FaceCube fc = cc.toFaceCube();
		return fc;
	}


	void ColorCube::printColor() const
	{
		string s = toString();
		_print("%c ", &(s[0]));
	}

	void ColorCube::printColor2() const
	{
		string s = toString2();
		_print("%c ", &(s[0]));
	}

	std::string ColorCube::sprintColor() const
	{
		string s = toString();
		return _sprint("%c ", &(s[0]));
	}

	std::string ColorCube::sprintColor2() const
	{
		string s = toString2();
		return _sprint("%c ", &(s[0]));
	}

	//////////////////////////////////////////////////////////////////////////
	// FaceletsCube
	//////////////////////////////////////////////////////////////////////////

	FaceCube::FaceCube()
	{
		for (int i = 0; i < 54; ++i)
		{
			(*this)[i] = i;
		}
	}

	FaceCube::FaceCube(const FaceCube & src)
		: FaceCubeBase(src)
	{
	}


	FaceCube::FaceCube(const _array_t& _facelets)
		: FaceCubeBase(_facelets)
	{
	}

	ColorCube FaceCube::toColorCube() const
	{
		ColorCube cc;

		for (int i = 0; i < 54; ++i)
		{
			cc[i] = this->at(i) / 9;
		}

		return cc;
	}

	SingleFaceColor singleFaceColorRotate90(const SingleFaceColor & ff, int n)
	{
		const permutation<9> p9_f1 = { 6,3,0,7,4,1,8,5,2 };
		permutation<9> pp = ff;

		for (int i = 0; i < n; ++i)
		{
			pp = pp * p9_f1;
		}

		SingleFaceColor result = pp;

		return result;
	}

	SixFacesColor sixFacesColorSortCenter(const SixFacesColor & ff)
	{
		SixFacesColor aa;

		for (int i = 0; i < (int)ff.size(); ++i)
		{
			aa[ff[i][4]] = ff[i];
		}

		return aa;
	}

	SixFacesColor colorCubeToSixFacesColor(const ColorCube & cc)
	{
		SixFacesColor ff;

		for (int i = 0; i < (int)ff.size(); ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				ff[i][j] = cc[9 * i + j];
			}

		}

		return ff;
	}

	std::vector< ColorCube> sixFacesColorToColorCube(const SixFacesColor & ff, int max_num)
	{
		vector<ColorCube> result;

		ColorCube cc;


		// 合法的旋转数，避免重复
		array<vector<SingleFaceColor>, 6> valid_power;
		for (int i = 0; i < 6; ++i)
		{
			valid_power[i].push_back(ff[i]);

			for (int j = 1; j < 4; ++j)
			{
				SingleFaceColor fj = singleFaceColorRotate90(ff[i], j);

				bool eq = false;

				for (int k = 0; k < j; ++k)
				{
					if (valid_power[i][k] == fj)
					{
						eq = true;
						break;
					}
				}

				if (!eq)
				{
					valid_power[i].push_back(fj);
				}
			}
		}


		array<int, 6> power = { 0,0,0,0,0,0};
		array<int, 6> power_max;
		for (int i = 0; i < 6; ++i)
		{
			power_max[i] = (int)valid_power[i].size() - 1;
		}


		int num = 0;


		while (true)
		{
			for (int i = 0; i < 6; i++)
			{
				SingleFaceColor fi = valid_power[i][power[i]];

				for (int j = 0; j < 9; ++j)
				{
					cc[i * 9 + j] = fi[j];
				}
			}

			

			CubieCube cubie(cc);
			ColorCube cc2 = cubie.toColorCube();
			// 避免镜像出现， 因为 CubieCube 无法处理镜像


			if (cubie.verify() == 0 && cc == cc2)
			{
				++num;
				result.push_back(cc);
				if (cubie.iseye() || num >= max_num)	// eye 不再查找
					break;
			}



			// 累加 power

			if (power == power_max)
			{
				break;
			}
			else
			{
				for (int j = 5; j >= 0; --j)
				{
					++power[j];

					if (power[j] < (int)valid_power[j].size())
						break;
					else
						power[j] = 0;
				}
			}

		}



		return result;
	}

}