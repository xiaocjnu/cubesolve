#include "CubeFrames.h"
#include "cs.h"
#include <assert.h>
#include <algorithm>

#ifdef USE_SNAPPY
#include "snappy.h"
#include "snappy-sinksource.h"
#endif


using namespace std;
using namespace cs;


const string CubeFrames::INDENTIFY = "FRAMES";
const std::vector<short> CubeFrames::VALID_VERSIONS =
{
	10,		// 6 字节不压缩格式
	11,		// 6 字节压缩格式
	20,		// 5 字节不压缩格式
	21		// 5 字节压缩格式
};


Move cf_move_byte_t::getMoveURF() const
{
	int a = raw & 0x0f;

	if (0 <= a && a < 9)
		return (Move)a;
	else
		return InvalidMove;
}

Move cf_move_byte_t::getMoveDLB() const
{

	int a = (raw >> 4) & 0x0f;

	if (0 <= a && a < 9)
		return (Move)(9 + a);
	else
		return InvalidMove;

}

void cf_move_byte_t::setMoveURF(cs::Move m)
{
	if (Ux1 <= m && m <= Fx3)
	{
		raw &= 0xf0;
		raw |= m;
	}
	else
		raw |= 0x0f;
}

void cf_move_byte_t::setMoveDLB(cs::Move m)
{
	if (Dx1 <= m && m <= Bx3)
	{
		raw &= 0x0f;
		raw |= ((m - 9) << 4);
	}
	else
		raw |= 0xf0;
}

void cf_move_byte_t::setMove(cs::Move m)
{
	if (0 <= m && m < 9)
	{
		setMoveURF(m);
		setMoveDLB(InvalidMove);
	}
	else if (9 <= m && m < 18)
	{
		setMoveURF(InvalidMove);
		setMoveDLB(m);
	}
	else
		raw = (char)0xff;
}

void cf_move_byte_t::setMoves(cs::Move m1, cs::Move m2)
{
	if (m1 == InvalidMove)
		setMove(m2);
	else if (m2 == InvalidMove)
		setMove(m1);
	else
	{
		if (0 <= m1 && m1 < 18 &&
			0 <= m2 && m2 < 18)
		{
			int ax1 = m1 / 3;
			int ax2 = m2 / 3;

			if (ax1 < 3 && ax2 >= 3)
			{
				setMoveURF(m1);
				setMoveDLB(m2);
			}
			else if (ax1 >= 3 && ax2 < 3)
			{
				setMoveDLB(m1);
				setMoveURF(m2);
			}
			else
				raw = (char)0xff;
		}
		else
		{
			raw = (char)0xff;
		}
	}
}

void cf_move_byte_t::setMoves(std::vector<cs::Move> moves)
{
	if (moves.empty() || moves.size() > 2)
	{
		raw = (char)0xff;
		return;
	}

	if (moves.size() == 1)
	{
		setMove(moves[0]);
	}
	else
	{
		setMoves(moves[0], moves[1]);
	}
}

std::vector<Move> cf_move_byte_t::getMoves() const
{
	Move urf = getMoveURF();
	Move dlb = getMoveDLB();

	vector<Move> mm;

	if (urf >= 0)
		mm.push_back(urf);

	if (dlb >= 0)
		mm.push_back(dlb);

	return mm;
}

short CubeFrames::checkVersion(const char * data, size_t length)
{
	if (length < 8)
		return -1;


	const cf_header_t& header = *(const cf_header_t*)data;


	for (int i = 0; i < 6; ++i)
	{
		if (header.identify[i] != INDENTIFY[i])
			return -1;
	}

	short v = header.version;
	
	if (isValidVersion(v))
		return v;
	else
		return -1;
}

short CubeFrames::checkVersion(const std::vector<char>& data)
{
	if (data.empty())
		return -1;
	else
		return checkVersion(&data[0], data.size());
}

bool CubeFrames::isValidVersion(short version)
{
	auto it = find(VALID_VERSIONS.begin(), VALID_VERSIONS.end(), version);

	if (it == VALID_VERSIONS.end())
		return false;
	else
		return true;
}

CubeFrames CubeFrames::load(const char * data, size_t length)
{
	CubeFrames frames;


	short version = checkVersion(data, length);

	if (version < 0)	// 非法, 尝试是否没有头的旧数据
		frames.records = _load_v0020(data, length);
	else if (length >= 8)
	{
		cube_frame_t* cfd = (cube_frame_t*)data;
		size_t record_nchars = length - offsetof(cube_frame_t, records);

		frames.identify.resize(6);
		for (int i = 0; i<6; ++i)
		{
			frames.identify[i] = cfd->header.identify[i];
		}
		
		frames.version = version;

		if (version == 10)
			frames.records = _load_v0010((const char*)cfd->records, record_nchars);
		else if (version == 11)
			frames.records = _load_v0011((const char*)cfd->records, record_nchars);
		else if (version == 20)
			frames.records = _load_v0020((const char*)cfd->records, record_nchars);
		else if (version == 21)
			frames.records = _load_v0021((const char*)cfd->records, record_nchars);
	}

	return std::move(frames);
}

CubeFrames CubeFrames::load(const std::vector<char>& data)
{
	if (data.empty())
		return CubeFrames();

	return std::move(load(&(data[0]), data.size()));
}



std::vector<cf_record_t> CubeFrames::_load_v0010(const char * data, size_t length)
{
	std::vector<cf_record_t> records;

	if (length > 0 && length % sizeof(cf_record_t) == 0)
	{
		size_t num_records = length / sizeof(cf_record_t);
		records.resize(num_records);
		copy((cf_record_t*)data, (cf_record_t*)data + num_records, records.begin());
	}

	return std::move(records);
}

std::vector<cf_record_t> CubeFrames::_load_v0011(const char * data, size_t length)
{
	std::vector<cf_record_t> records;

#ifdef USE_SNAPPY

	size_t uncompressed_length;
	bool success = snappy::GetUncompressedLength(data, length, &uncompressed_length);

	if (success && uncompressed_length > 0 && uncompressed_length % sizeof(cf_record_t) == 0)
	{
		size_t num_records = uncompressed_length / sizeof(cf_record_t);
		records.resize(num_records);

		snappy::ByteArraySource src(data, length);
		snappy::UncheckedByteArraySink sink((char*)&(records[0]));

		snappy::Uncompress(&src, &sink);
	}
#endif

	return std::move(records);
}

std::vector<cf_record_t> CubeFrames::_load_v0020(const char * data, size_t length)
{
	vector<cf_record_t> records;

	if (length >0 && length % 5 == 0)
	{
		records.resize(length / 5);

		size_t k = 0;

		for (size_t i = 0; i <length; i+=5, ++k)
		{
			char axis_cube = data[i];
			char move = data[i + 1];

			int ds_time = 0;
			((char*)&ds_time)[2] = data[i + 2];
			((char*)&ds_time)[1] = data[i + 3];
			((char*)&ds_time)[0] = data[i + 4];


			records[k].axis_cube = axis_cube;
			records[k].move.raw = move;
			records[k].ds_time = ds_time;
		}


		assert(k == records.size());
	}

	return std::move(records);
}

std::vector<cf_record_t> CubeFrames::_load_v0021(const char * data, size_t length)
{
	std::vector<cf_record_t> records;

#ifdef USE_SNAPPY
	size_t uncompressed_length;
	bool success = snappy::GetUncompressedLength(data, length, &uncompressed_length);

	if (success && uncompressed_length > 0 && uncompressed_length % 5 == 0)
	{
		string buf;
		snappy::Uncompress(data, length, &buf);

		assert(buf.size() == uncompressed_length);

		records = _load_v0020(&buf[0], buf.size());
	}
#endif

	return std::move(records);
}

std::vector<char> CubeFrames::dump()
{
	return dumpVersion(DEFAULT_VERSION);
}

std::vector<char> CubeFrames::dumpVersion()
{
	return dumpVersion(version);
}


std::vector<char> CubeFrames::dumpVersion(short v)
{
	std::vector<char> data;

	if (identify != "FRAMES" || !isValidVersion(v))
	{
		return data;
	}

	if (v == 10)
		data = _dump_v0010();
	else if (v == 11)
		data = _dump_v0011();
	else if (v == 20)
		data = _dump_v0020();
	else if (v == 21)
		data = _dump_v0021();

	return std::move(data);
}


int CubeFrames::getPreAxis(Move m, int cur_ax)
{
	AxisCube M;
	M = M.move(m);
	AxisCube B = AxisCube::fromValidIdx(cur_ax);
	AxisCube A = B * M.inv();
	return A.getValidIdx();
}

std::vector<cf_rich_record_t> CubeFrames::toRichRecords(int init_axis) const
{
	std::vector<cf_rich_record_t> result(records.size());

	if (result.empty())
		return result;

	AxisCubieCube acc;
	acc.axis = AxisCube::fromValidIdx(init_axis);

	for (size_t i = 0; i < records.size(); ++i)
	{
		auto abs_moves = records[i].move.getMoves();

		result[i].ds_time = records[i].ds_time;
		result[i].axis_cube = records[i].axis_cube;

		AxisCubieCube pre_acc = acc;
		acc = acc.rotateToAxis(records[i].axis_cube);


		auto rot_moves = (pre_acc.axis.inv() * acc.axis).getGenFormula();



		if (abs_moves.empty())	// 没有绝对公式
		{
			result[i].abs_move = InvalidMove;
			result[i].abs_move2 = InvalidMove;

			if (rot_moves.size() == 1)
				result[i].rel_move = rot_moves[0];
			else
				result[i].rel_move = InvalidMove;

		}
		else if (abs_moves.size() == 1)  // 一步绝对公式
		{
			result[i].abs_move = abs_moves[0];
			result[i].abs_move2 = InvalidMove;
			result[i].rel_move = InvalidMove;

			Move rm = pre_acc.formulaA2R(abs_moves[0]);

			if (rot_moves.size() == 0)
			{
				result[i].rel_move = rm;
			}
			else if (rot_moves.size() == 1)	// 转成转两层公式
			{
				for (int j = 0; j < 18; ++j)
				{
					if (move_lower_table[j][0] == rm && move_lower_table[j][1] == rot_moves[0])
					{
						result[i].rel_move = Move(LowerUx1 + j);
						break;
					}
				}
			}
		}
		else  // 两步绝对公式
		{
			result[i].abs_move = abs_moves[0];
			result[i].abs_move2 = abs_moves[1];
			result[i].rel_move = InvalidMove;


			Move rm1 = pre_acc.formulaA2R(abs_moves[0]);
			Move rm2 = pre_acc.formulaA2R(abs_moves[1]);

			if (rot_moves.size() == 1)	// 转成转中间层公式
			{

				for (int j = 0; j < 9; ++j)
				{
					if (move_middle_table[j][0] == rm1 && move_middle_table[j][1] == rm2 
						&& move_middle_table[j][2] == rot_moves[0])
					{
						result[i].rel_move = Move(Ex1 + j);
						break;
					}
				}
			}
		}
	}

	return result;
}



void CubeFrames::_dump_header(std::string identify, short version, cf_header_t& header)
{
	for (size_t i = 0; i < 6; ++i)
	{
		if (i < identify.size())
			header.identify[i] = identify[i];
		else
			header.identify[i] = 0;
	}

	header.version = version;
}

std::vector<char> CubeFrames::_dump_v0010()
{
	std::vector<char> data;

	if (records.empty())
		return data;

	data.resize(sizeof(cf_header_t) + sizeof(cf_record_t) * records.size());
	cube_frame_t* cfd = (cube_frame_t*)&data[0];
	_dump_header(identify, 10, cfd->header);
	copy(records.begin(), records.end(), (cf_record_t*)cfd->records);

	return data;
}

std::vector<char> CubeFrames::_dump_v0011()
{
	std::vector<char> data;

	if (records.empty())
		return data;


#ifdef USE_SNAPPY
	string compressed;
	snappy::Compress((const char*)&(records[0]), records.size() * sizeof(cf_record_t), &compressed);


	data.resize(sizeof(cf_header_t) + compressed.size());
	cube_frame_t* cfd = (cube_frame_t*)&(data[0]);
	_dump_header(identify, 11, cfd->header);
	copy(compressed.begin(), compressed.end(), (char*)(cfd->records));

#endif

	return data;
}

std::vector<char> CubeFrames::_dump_v0020()
{
	std::vector<char> data;

	if (records.empty())
		return data;


	data.resize(sizeof(cf_header_t) + 5 * records.size());
	cube_frame_t* cfd = (cube_frame_t*)&(data[0]);
	_dump_header(identify, 20, cfd->header);
	
	char* buf = (char*)cfd->records;
	int k = 0;
	for (size_t i = 0; i < records.size(); ++i, k+=5)
	{
		const cf_record_t& r = records[i];

		buf[k] = r.axis_cube;
		buf[k + 1] = r.move.raw;
		buf[k + 2] = ((char*)&r.ds_time)[2];
		buf[k + 3] = ((char*)&r.ds_time)[1];
		buf[k + 4] = ((char*)&r.ds_time)[0];
	}

	return data;
}

std::vector<char> CubeFrames::_dump_v0021()
{
	std::vector<char> data;

	auto raw = _dump_v0020();

	if (raw.size() <= 8)
		return data;


#ifdef USE_SNAPPY
	size_t uncompress_length = raw.size() - 8;
	char* uncompress = &raw[8];

	string compressed;
	snappy::Compress(uncompress, uncompress_length, &compressed);


	data.resize(sizeof(cf_header_t) + compressed.size());
	cube_frame_t* cfd = (cube_frame_t*)&(data[0]);
	_dump_header(identify, 21, cfd->header);
	copy(compressed.begin(), compressed.end(), (char*)(cfd->records));
#endif

	return data;
}
