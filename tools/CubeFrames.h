///////////////////////////////////////////////////////////////////////////
// 魔方记录类型
// 数据结构：
// header 8 字节, 其中
//   -- 标识符，6字节ASCII字符  FRAMES, 注意不要当成字符串来处理，因为字符串要求最后一个字节是'\0', 这里没有可能会引起错误。应该当成字节数组来处理。
//   -- 版本号, 2字节，short 类型，小端。目前合法版本号 10,11,20,21. -1表示非法
// 数据（根据版本号不同而不同）
//
// 不同版本数据结构, 当前默认版本为 20
// 版本 10:
//   每条记录6个字节，总数据长度是 6 * N.每条记录结构如下
//   -- char axis_cube;		// AxisCube 编号
//   -- cf_move_byte_t move;	// 把两个绝对公式合并到一个字节，详情见 cf_move_byte_t
//   -- int ds_time;		// 时间，毫秒，4字节小端
//
// 版本 11:
//   版本 10  的数据，采用 snappy 压缩
//
// 版本 20:
//   每条记录5个字节，总数据长度是 5 * N.每条记录结构如下
//   -- char axis_cube;		// AxisCube 编号
//   -- cf_move_byte_t move;			// 把两个绝对公式合并到一个字节，详情见 cf_move_byte_t
//   -- ((char*)&ds_time)[2];		// ds_time 低3字节按大端排列
//   -- ((char*)&ds_time)[1];
//   -- ((char*)&ds_time)[0];
//
// 版本 21：
//   版本 20 的数据，采用 snappy 压缩
//
// 
// 注意：如果定义 USE_SNAPPY, 则压缩不能使用，返回空



#pragma once

#include <string>
#include <vector>
#include "tools_typedef.h"
#include "cs.h"


// frame 记录

#pragma pack(push, 1)


struct cf_move_byte_t
{
	// 低四位是 U R F 三个面的公式，取值  0 - 8, 0xf 表示不转, 高四位表示 D L B三个面，取值 0-8(对应 9-17), 0xf 表示不转. 第四位与高四位独立
	char raw;


	// 获取 U R F 的公式，如不动返回 InvalidMove
	cs::Move getMoveURF() const;

	// 获取 D L B 部分公式，如不动返回 InvalidMove
	cs::Move getMoveDLB() const;

	// 获取转动公式，长度可以是0,1,2, 当相对两个面同时转动时，可能出现 长度是2
	std::vector<cs::Move> getMoves() const;

	// 设置 U R F 部分公式，如果非法则设置为 0xf 不转动
	void setMoveURF(cs::Move m);

	// 设置 D L B 部分公式，如果非法则设置为 0xf 不转动 
	void setMoveDLB(cs::Move m);

	// 设置为一步转动，m 取值 0 - 17, 或 -1
	void setMove(cs::Move m);

	// 设置为2步转动，2步必须是相对的面, 非法设为 -1
	void setMoves(cs::Move m1, cs::Move m2);

	// 设置为一步或者2步转动，2步必须是相对的面, 非法设为 -1
	void setMoves(std::vector<cs::Move> moves);

};


struct cf_record_t
{
	char axis_cube;		// AxisCube 编号
	cf_move_byte_t move;
	int ds_time;
};


#ifndef SWIG
static_assert(sizeof(cf_record_t) == 6,
	"结构体 cf_record_t 长度不等于6，可能编译器设置有问题！");
#endif

// frame 头， 8 个字节
struct cf_header_t
{
	char identify[6];	// 类型表示, 6字节，一般是 FRAMES
	short version;		// 版本号，目前两个版本 10: 不压缩  11: snappy 压缩, -1 表示未知版本
};

#ifndef SWIG
static_assert(sizeof(cf_header_t) == 8, 
	"结构体 cf_header 长度不等于8，可能编译器设置有问题！");
#endif

// 原始 frame 结构体，不完整
struct cube_frame_t
{
	cf_header_t header;
	cf_record_t records[1];
};


#ifndef SWIG
static_assert(offsetof(cube_frame_t, records) == 8,
	"records 在结构体 cube_frame_t 中的偏移量不是8，可能编译器设置有问题！");
#endif


#pragma pack(pop)



// 更加丰富的 cube frame 信息, 不用来保存
struct cf_rich_record_t
{
	int axis_cube;		// 当前，AxisCube
	cs::Move abs_move;	// 绝对转动公式, 0-17 或 -1
	cs::Move abs_move2;	// 绝对转动公式, 0-17 或 -1, 当转中间层时出出现两步绝对 move
	cs::Move rel_move;	// 相对转动公式，相对于上一步的 axis_cube
	int ds_time;		// 这步所花时间(ms)
};






//////////////////////////////////////////////////////////////////////////
// CubeFrames
//////////////////////////////////////////////////////////////////////////

class CubeFrames
{
public:
	static const std::string INDENTIFY;	// 标识符
	static const std::vector<short> VALID_VERSIONS;		// 可用的版本号
	static const short DEFAULT_VERSION = 20;		// 默认版本


	std::string identify = INDENTIFY;
	short version = DEFAULT_VERSION;		// 默认版本	
	std::vector<cf_record_t> records;


	// 是否有记录
	bool empty() const { return records.empty(); };
	

	// 检查字节流头部信息，标识符正确且版本合法，返回版本号，否则返回-1
	static short checkVersion(const char* data, size_t length);
	static short checkVersion(const std::vector<char>& data);

	// 测试版本是否合法
	static bool isValidVersion(short version);


	// 反序列化，从字节流读取
	static CubeFrames load(const char* data, size_t length);
	static CubeFrames load(const std::vector<char>& data);


	
	// 序列化，强制使用默认版本
	std::vector<char> dump();


	// 序列化，使用原版本
	std::vector<char> dumpVersion();

	// 序列化，使用指定版本
	std::vector<char> dumpVersion(short v);

	// 已知 move(相对) 和 当前 axis cube, 获取上一次的 axis cube,
	static int getPreAxis(cs::Move m, int cur_ax);


	// 转换成 cf_rich_record_t 数组, init_axis 是第一帧记录之前的 axis_cube
	// 数组长度长度和原来一样，当不能用一步相对公式表示时，则 rel_move = -1
	std::vector<cf_rich_record_t> toRichRecords(int init_axis = 0) const;


protected:

	// 读取记录
	static std::vector<cf_record_t> _load_v0010(const char* data, size_t length);
	static std::vector<cf_record_t> _load_v0011(const char* data, size_t length);
	static std::vector<cf_record_t> _load_v0020(const char* data, size_t length);
	static std::vector<cf_record_t> _load_v0021(const char* data, size_t length);


	// 序列化头
	static void _dump_header(std::string identify, short version, cf_header_t& header);


	// 序列化记录
	std::vector<char> _dump_v0010();
	std::vector<char> _dump_v0011();
	std::vector<char> _dump_v0020();
	std::vector<char> _dump_v0021();


};

