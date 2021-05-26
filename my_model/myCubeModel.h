#pragma once


#include <string>
#include "myModel.h"
#include "CubieCube.h"
#include "myCubeAng.h"
#include <map>

namespace my
{

	class CubeModel : public ModelGroup
	{
	public:

		// 模型obj文件，用于加载模型
		std::string fn_piece_corner;		// 角块
		std::string fn_piece_edge;			// 棱块
		std::string fn_piece_centre;		// 中心块
		std::string fn_paster_corner;		// 角块贴纸
		std::string fn_paster_edge;			// 棱块贴纸
		std::string fn_paster_centre;		// 中心块贴纸
		std::string fn_ball;


		std::string fn_logo;		// logo 材质文件


		static const double piece_len;			// 块的边长
		static const double piece_gap;		// 块之间的间隙
		static const double paster_offset;	// 贴纸偏移量


		// 旋转轴表，即魔方轴对应的坐标向量
		static const vec3 rotateAxisTbl[6];

		// 旋转角块表，旋转轴对应的四个角块
		static const int rotateCornerTbl[6][4];

		// 旋转棱块表，旋转轴对应的四个角块
		static const int rotateEdgeTbl[6][4];


		CubeModel();
		~CubeModel();


		// 设置默认的文件名，文件在当前目录
		void set_defaut_filename(std::string path = "");


		// 初始化一些表
		void init_tables();

		void load_model();

		static int modelGroupSize() { return 27; };


		// 设置	CubieCube 和 AxisAng
		void setCubeAng(const CubeAng& ca);

		const CubeAng& getCubeAng() const { return my_ca; };

		cs::CubieCube getCubieCube() const { return my_ca.getCubieCube(); };

		// 设置 CubieCube, AxisAng 自动置 0
		// void setCubieCube(const cs::CubieCube& cc);

		// 设置轴的偏移角度，取值 [-45,45), 只能相对两个轴同时非0值
		// 输入角度超出范围自动置0， 当出现相对两个轴同时非0值，按以下规则自动忽略输入（用0填补）
		// 0 1 2 3 4 5
		// - x x - x x
		//   - x x - x
		//     - x x -
		// void setAxisAng(std::array<double, 6> ang);

		const CubeAng::ang_t& getAxisAng() const { return my_ca.getAxisAng(); };

		// 轴 axis 旋转角度 ang, 自动容错
		// axis 取值 [0,6), ang 任意值adjust
		void rotateAxis(int axis, double ang);


		CubeModel& operator = (const CubeModel& _right);


		// 重置颜色
		void resetColor();

		void resetAllPasterColor();
		void resetBodyColor();

		// 重置贴纸颜色
		void resetPasterColorAbs(cs::Facelet facelet);
		void resetPasterColorAbs(int facelet);
		void resetPasterColorRel(cs::Facelet facelet);
		void resetPasterColorRel(int facelet);

		// 重置角块贴纸颜色
		void resetCornerColorAbs(cs::Corner corner);
		void resetCornerColorAbs(int corner);
		void resetCornerColorRel(cs::Corner corner);
		void resetCornerColorRel(int corner);

		// 重置棱块贴纸颜色
		void resetEdgeColorAbs(cs::Edge edge);
		void resetEdgeColorAbs(int edge);
		void reseEdgeColorRel(cs::Edge edge);
		void resetEdgeColorRel(int edge);

		// 重置中心块贴纸颜色
		void resetCentreColor(cs::Color centre);
		void resetCentreColor(int centre);

		// 重置个面的9个贴纸的颜色，只有绝对。
		void resetFaceColor(cs::Color face);
		void resetFaceColor(int face);


		// 单独设置贴纸的颜色, 贴纸序号是绝对的贴纸，当cubie cube 变化时，贴纸所处位置会变化，但仍然属于原来的块。
		void setPasterColorAbs(cs::Facelet facelet, fvec4 color);
		void setPasterColorAbs(int facelet, fvec4 color);

		// 单独设置贴纸的颜色, 贴纸序号是当前位置贴纸，当cubie cube 变化时，贴纸所处位置不会变化，对应于不同的块。
		void setPasterColorRel(cs::Facelet facelet, fvec4 color);
		void setPasterColorRel(int facelet, fvec4 color);


		// 调整贴纸颜色，颜色相对当前颜色进行调整. 贴纸序号是绝对的贴纸
		//     saturation, >= 0, 饱和度调整倍数
		//     brightness, >= 0, 亮度调整倍数
		void adjustPasterColorAbs(cs::Facelet facelet, double saturation, double brightness);
		void adjustPasterColorAbs(int facelet, double saturation, double brightness);

		// 调整贴纸颜色，颜色相对当前颜色进行调整. 贴纸序号是相对的贴纸
		void adjustPasterColorRel(cs::Facelet facelet, double saturation, double brightness);
		void adjustPasterColorRel(int facelet, double saturation, double brightness);


		// 调整角块贴纸颜色
		void setCornerColorAbs(cs::Corner corner, fvec4 color);
		void setCornerColorAbs(int corner, fvec4 color);

		void setCornerColorRel(cs::Corner corner, fvec4 color);
		void setCornerColorRel(int corner, fvec4 color);

		// 调整角块贴纸颜色
		void adjustCornerColorAbs(cs::Corner corner, double saturation, double brightness);
		void adjustCornerColorAbs(int corner, double saturation, double brightness);

		void adjustCornerColorRel(cs::Corner corner, double saturation, double brightness);
		void adjustCornerColorRel(int corner, double saturation, double brightness);

		// 设置棱块贴纸颜色
		void setEdgeColorAbs(cs::Edge edge, fvec4 color);
		void setEdgeColorAbs(int edge, fvec4 color);

		void setEdgeColorRel(cs::Edge edge, fvec4 color);
		void setEdgeColorRel(int edge, fvec4 color);

		// 调整棱块贴纸颜色
		void adjustEdgeColorAbs(cs::Edge edge, double saturation, double brightness);
		void adjustEdgeColorAbs(int edge, double saturation, double brightness);

		void adjustEdgeColorRel(cs::Edge edge, double saturation, double brightness);
		void adjustEdgeColorRel(int edge, double saturation, double brightness);


		// 设置中心块贴纸颜色，只有绝对
		void setCentreColor(cs::Color centre, fvec4 color);
		void setCentreColor(int centre, fvec4 color);

		// 调整中心块贴纸颜色，只有绝对。
		void adjustCentreColor(cs::Color centre, double saturation, double brightness);
		void adjustCentreColor(int centre, double saturation, double brightness);


		// 设置整个面的9个贴纸的颜色，只有绝对。
		void setFaceColor(cs::Color face, fvec4 color);
		void setFaceColor(int face, fvec4 color);

		// 调整整个面的9个贴纸的颜色，只有绝对。
		void adjustFaceColor(cs::Color face, double saturation, double brightness);
		void adjustFaceColor(int face, double saturation, double brightness);


		// 设置主体颜色
		void setBodyMaterial(material_t material);


	protected:

		void _update();		// 更新位置和姿态

		void _setPoints(); // 设置 myCentres 等指针，用于复制函数


		// 获取贴纸模型
		Model* _getPaster(int facelet);

		// 单独设置贴纸的颜色
		void _setPasterColor(int facelet,fvec4 color);


		// 单独设置贴纸的材质
		void _setPasterMaterial(int facelet, material_t material);


		// 设置主体材质
		void _setBodyMaterial(material_t mat_piece, material_t mat_ball);

		// 调整颜色饱和度和亮度
		fvec4 _adjust_color(fvec4 color, double saturation, double brightness);

	protected:

		CubeAng my_ca;

// 
// 
// 		// 轴的偏移角度，取值 [-45,45), 只能相对两个轴同时非0值, 顺时针是正方向
// 		double myAxisAng[6];

		ModelGroup* myCentres = nullptr;
		ModelGroup* myCorners = nullptr;
		ModelGroup* myEdges = nullptr;

		// 以下是表

		material_t my_material_piece;		// 块的材质
		material_t my_material_paster[6];	// 贴纸颜色 URFDLB
		material_t my_material_ball;

		vec3 myCentrePos[6];	// 中心块标准位置， z -> U, x -> R, -y -> F
		vec3 myCornerPos[8];	// 角块的标准位置
		vec3 myEdgePos[12];		// 棱块标准位置

		vec3 myCentreAng[6];		// 中心块标准欧拉角
		vec3 myCornerAng[8][3];		// 角块的标准欧拉角
		vec3 myEdgeAng[12][2];		// 棱块标准欧拉角


		//int myLogoAngIdx = 0;			// logo 角度, 取值 0, 1, 2, 3
		//double myLogoAng = 0.0;			// logo 角度， 顺时针为正
		bool myModelLoaded = false;


		// 贴纸颜色相关参数
		float paster_ambient_factor = 0.8f;
		float paster_diffuse_factor = 1.0f;
		float paster_specular_factor = 0.2f;
		float paster_specular_exponent = 20.0f;


	};

}

