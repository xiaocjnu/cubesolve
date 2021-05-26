#include "util.h"
#include "CubieCube.h"
#include <random>
#include <time.h>


using namespace std;

namespace cs
{

	std::default_random_engine random_engine((int)time(nullptr));


	//////////////////////////////////////////////////////////////////////////

	bool isMove2n(Move m)
	{
		int m1 = m & 0xff;
		int m2 = m >> 8;



		return (m2 == 1) && ((m1 % 3) == 1);
	}

	Move getPureMove(Move m)
	{
		if (m == InvalidMove)
			return m;

		return (Move)((int)m & 0xff);
	}

	std::vector<Move> move_decompose(Move m)
	{
		std::vector<Move> result;

		if (Ux1 <= m && m <= Rot_z3)
		{
			result.push_back(m);
		}
		else if (Ex1 <= m && m <= Sx3)
		{
			int off = m - Ex1;
			result.push_back((Move)move_middle_table[off][0]);
			result.push_back((Move)move_middle_table[off][1]);
			result.push_back((Move)move_middle_table[off][2]);
		}
		else if (LowerUx1 <= m && m <= LowerBx3)
		{
			int off = m - LowerUx1;
			result.push_back((Move)move_lower_table[off][0]);
			result.push_back((Move)move_lower_table[off][1]);
		}

		return result;
	}

	std::array<std::vector<Move>, 2> move_decompose2(Move m)
	{
		std::array<std::vector<Move>, 2> result;


		if (Ux1 <= m && m <= Bx1)
		{
			result[0].push_back(m);
		}
		else if (Rot_x1 <= m && m <= Rot_z3)
		{
			result[1].push_back(m);
		}
		else if (Ex1 <= m && m <= Sx3)
		{
			int off = m - Ex1;

			result[0].push_back((Move)move_middle_table[off][0]);
			result[0].push_back((Move)move_middle_table[off][1]);
			result[1].push_back((Move)move_middle_table[off][2]);
		}
		else if (LowerUx1 <= m && m <= LowerBx3)
		{
			int off = m - LowerUx1;

			result[0].push_back((Move)move_lower_table[off][0]);
			result[1].push_back((Move)move_lower_table[off][1]);
		}

		return result;

	}

	int nchoosek(int n, int k)
	{
		int i, j, s;

		if (n < k || k < 0)
			return 0;
		if (k > n / 2)
			k = n - k;
		
		for (s = 1, i = n, j = 1; i != n - k; i--, j++)
		{
			s *= i;
			s /= j;
		}
		return s;
	}


	int getNParity(int idx, int n)
	{
		int p = 0;
		for (int i = n - 2; i >= 0; i--) {
			p ^= idx % (n - i);
			idx /= (n - i);
		}
		return p & 1;
	}

	int randInt(int a, int b)
	{
		uniform_int_distribution<> dis(a, b);

		return dis(random_engine);
	}

	double random(double a, double b)
	{
		uniform_real_distribution<> dis(a, b);

		return dis(random_engine);
	}

	std::vector<int> MoveVector2IntVector(const std::vector<Move>& mm)
	{
		std::vector<int> v(mm.size());

		for (int i = 0; i < (int)mm.size(); ++i)
		{
			v[i] = (int)mm[i];
		}

		return v;
	}


	std::vector<Move> IntVector2MoveVector(const std::vector<int>& vv)
	{
		std::vector<Move> mm(vv.size());

		for (int i = 0; i < (int)vv.size(); ++i)
		{
			mm[i] = (Move)vv[i];
		}

		return mm;
	}

	//////////////////////////////////////////////////////////////////////////

	eo_t::eo_t()
		: val(0)
	{
	}

	eo_t::eo_t(int v)
	{
		set(v);
	}

	eo_t::eo_t(const eo_t & src)
	{
		val = src.val;
	}

	void eo_t::set(int v)
	{
		int c = v % 2;
		c = c < 0 ? c + 2 : c;

		val = c;
	}


	eo_t eo_t::operator+(const eo_t & b) const
	{
		int c = this->val + b.val;
		return eo_t(c);
	}

	bool eo_t::operator==(const eo_t & b) const
	{
		return val == b.val;
	}

	eo_t eo_t::neg() const
	{
		return *this;
		//return eo_t(1 - this->val);
	}


	//////////////////////////////////////////////////////////////////////////
	co_t::co_t()
		: val(0)
	{
	}
	co_t::co_t(int v)
	{
		set(v);
	}
	co_t::co_t(const co_t & src)
		: val(src.val)
	{
	}
	void co_t::set(int v)
	{
		int c = v % 6;
		c = c < 0 ? c + 6 : c;

		val = c;
	}
	co_t co_t::operator+(const co_t & b) const
	{
		int c = this->val;
		c += (c < 3) ? b.val : 6 - b.val;
		c %= 3;
		if ((this->val >= 3) ^ (b.val >= 3))
			c += 3;
		

		return co_t(c);
	}
	bool co_t::operator==(const co_t & b) const
	{
		return val == b.val;
	}
	co_t co_t::neg() const
	{
		const static int a[6] = { 0,2,1,3,4,5 };
		return co_t(a[this->val]);
	}
	edge_orientation::edge_orientation()
	{
		for (int i = 0; i < 12; ++i)
		{
			this->at(i) = 0;
		}
	}
	edge_orientation::edge_orientation(const initializer_list<int>& t)
	{
		int i = 0;
		for (int a : t)
		{
			this->at(i++) = a;
		}

		assert(i == 12);
	}


	int edge_orientation::getIndex() const
	{
		int idx = 0;

		for (int i = 0; i < 11; ++i)
		{
			idx = (idx << 1) + this->at(i).get();
		}

		return idx;
	}

	void edge_orientation::setIndex(int idx)
	{
		int parity = 0;
		for (int i = 10; i >= 0; --i)
		{
			int a = idx >> 1;
			int b = idx & 1;

			this->at(i) = b;
			parity ^= b;
			idx = a;
		}

		this->at(11) = parity;
	}

	//////////////////////////////////////////////////////////////////////////
	corner_orientation::corner_orientation()
	{
		for (int i = 0; i < 8; ++i)
		{
			this->at(i) = 0;
		}
	}
	corner_orientation::corner_orientation(const initializer_list<int>& t)
	{
		int i = 0;
		for (int a : t)
		{
			this->at(i++) = a;
		}

		assert(i == 8);
	}
	int corner_orientation::getIndex() const
	{
		int idx = 0;

		for (int i = 0; i < 7; ++i)
		{
			idx = 3 * idx + this->at(i).get();
		}

		return idx;
	}
	void corner_orientation::setIndex(int idx)
	{
		int sum = 0;
		for (int i = 6; i >= 0; --i)
		{
			int a = idx / 3;
			int b = idx % 3;

			this->at(i) = b;
			sum += b;
			idx = a;
		}

		this->at(7) = (15 - sum) % 3;
	}
}