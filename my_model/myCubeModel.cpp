#include "myCubeModel.h"
#include "util.h"
#include "myUtils.h"

using namespace std;


namespace my
{

	const double CubeModel::piece_len = 18.66;			// 块的边长
	const double CubeModel::piece_gap = 0.;		// 块之间的间隙
	const double CubeModel::paster_offset = 0.;	// 贴纸偏移量


	// 旋转轴表，即魔方轴对应的坐标向量
	const vec3 CubeModel::rotateAxisTbl[6]
	{
		{0,0,1},	// 0(U)
		{1,0,0},	// 1(R)
		{0,-1,0},	// 2(F)
		{0,0,-1},	// 3(D)
		{-1,0,0},	// 4(L)
		{0,1,0},	// 5(B)
	};

	// 旋转角块表，旋转轴对应的四个角块
	const int CubeModel::rotateCornerTbl[6][4] =
	{
		{0,1,2,3},	// 0(U)
		{0,3,7,4},  // 1(R)
		{0,4,5,1},	// 2(F)
		{6,5,4,7},	// 3(D)
		{6,2,1,5},  // 4(L)
		{6,7,3,2},	// 5(B)
	};

	// 旋转棱块表，旋转轴对应的四个角块
	const int CubeModel::rotateEdgeTbl[6][4]
	{
		{0,1,2,3},	 // 0(U)
		{0,11,4,8},  // 1(R)
		{1,8,5,9},	 // 2(F)
		{4,5,6,7},	 // 3(D)
		{6,10,2,9},  // 4(L)
		{7,11,3,10}, // 5(B)
	};



	CubeModel::CubeModel()
	{
	}


	CubeModel::~CubeModel()
	{
	}

	void CubeModel::set_defaut_filename(std::string path)
	{
		fn_piece_corner = "piece_corner.obj";
		fn_piece_edge = "piece_edge.obj";
		fn_piece_centre = "piece_centre.obj";
		fn_paster_corner = "paster_corner.obj";
		fn_paster_edge = "paster_edge.obj";
		fn_paster_centre = "paster_centre.obj";
		fn_ball = "ball.obj";
		fn_logo = "logo.ktx";

		fn_piece_corner = path + fn_piece_corner;
		fn_piece_edge = path + fn_piece_edge;
		fn_piece_centre = path + fn_piece_centre;
		fn_paster_corner = path + fn_paster_corner;
		fn_paster_edge = path + fn_paster_edge;
		fn_paster_centre = path + fn_paster_centre;
		fn_ball = path + fn_ball;
		fn_logo = path + fn_logo;
	}



	void CubeModel::init_tables()
	{

		// material
		my_material_piece = material_t::colored_material({ 1,1,1,1 }, 0.08f, 0.1f, 0.5f, 0.0f, 20.0f);	// 黑色反光
		my_material_ball = material_t::colored_material({ 1,1,1,1 }, 0.05f, 0.05f, 0.0f, 0.0f, 20.0f);	// 黑色不反光


// 		my_material_paster[0] = material_t::colored_material(vec4{0.9f, 0.9f, 0.9f, 1.0f},
// 			ambient, diffuse, specular, 0.0f, specular_exp);	// U 白色
// 
// 		my_material_paster[1] = material_t::colored_material(vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
// 			ambient, diffuse, specular, 0.0f, specular_exp);	// R 红色
// 
// 		my_material_paster[2] = material_t::colored_material(vec4{ 0.0f, 1.0f, 0.3020f, 1.0f },
// 			ambient, diffuse, specular, 0.0f, specular_exp);	// F 绿色
// 
// 		my_material_paster[3] = material_t::colored_material(vec4{ 1.0f, 1.0f, 0.0f, 1.0f },
// 			ambient, diffuse, specular, 0.0f, specular_exp);	// D 黄色
// 
// 		my_material_paster[4] = material_t::colored_material(vec4{ 1.0f, 112/255.0f, 0.0f, 1.0f },
// 			ambient, diffuse, specular, 0.0f, specular_exp);	// L 橙色
// 
// 		my_material_paster[5] = material_t::colored_material(vec4{ 14/255.00f, 140/255.0f, 254/255.0f, 1.0f },
// 			ambient, diffuse, specular, 0.0f, specular_exp);	// B 蓝色


		my_material_paster[0] = material_t::colored_material(fvec4{ 1.0f, 1.0f, 0.941176f, 1.0f },
			paster_ambient_factor, paster_diffuse_factor, paster_specular_factor, 0.0f, paster_specular_exponent);	// U 白色

		my_material_paster[1] = material_t::colored_material(fvec4{ 1.0f, 0.0177768f, 0.000991836f, 1.0f },
			paster_ambient_factor, paster_diffuse_factor, paster_specular_factor, 0.0f, paster_specular_exponent);	// R 红色

		my_material_paster[2] = material_t::colored_material(fvec4{ 0, 0.75111f, 0.0199283f, 1.0f },
			paster_ambient_factor, paster_diffuse_factor, paster_specular_factor, 0.0f, paster_specular_exponent);	// F 绿色

		my_material_paster[3] = material_t::colored_material(fvec4{ 0.878447f, 1, 0, 1.0f },
			paster_ambient_factor, paster_diffuse_factor, paster_specular_factor, 0.0f, paster_specular_exponent);	// D 黄色

		my_material_paster[4] = material_t::colored_material(fvec4{ 1, 0.379065f, 0, 1.0f },
			paster_ambient_factor, paster_diffuse_factor, paster_specular_factor, 0.0f, paster_specular_exponent);	// L 橙色

		my_material_paster[5] = material_t::colored_material(fvec4{ 0, 0.272038f, 0.792447f, 1.0f },
			paster_ambient_factor, paster_diffuse_factor, paster_specular_factor, 0.0f, paster_specular_exponent);	// B 蓝色

		// myCentrePos
		double c = piece_len + piece_gap;

		myCentrePos[0] = { 0, 0, c };
		myCentrePos[1] = { c, 0, 0 };
		myCentrePos[2] = { 0, -c, 0 };
		myCentrePos[3] = { 0, 0, -c };
		myCentrePos[4] = { -c, 0, 0 };
		myCentrePos[5] = { 0, c, 0 };

		// myCornerPos
		

		myCornerPos[0] = { c, -c, c };
		myCornerPos[1] = { -c, -c, c };
		myCornerPos[2] = { -c, c, c };
		myCornerPos[3] = { c, c, c };
		myCornerPos[4] = { c, -c, -c };
		myCornerPos[5] = { -c, -c, -c };
		myCornerPos[6] = { -c, c, -c };
		myCornerPos[7] = { c, c, -c };


		// myEdgePos
		myEdgePos[0] = { c, 0, c };
		myEdgePos[1] = { 0, -c, c };
		myEdgePos[2] = { -c, 0, c };
		myEdgePos[3] = { 0, c, c };
		myEdgePos[4] = { c, 0, -c };
		myEdgePos[5] = { 0, -c, -c };
		myEdgePos[6] = { -c, 0, -c };
		myEdgePos[7] = { 0, c, -c };
		myEdgePos[8] = { c, -c, 0 };
		myEdgePos[9] = { -c, -c, 0 };
		myEdgePos[10] = { -c, c, 0 };
		myEdgePos[11] = { c, c, 0 };

		//myCentreAng
		myCentreAng[0] = { 0,0,0 };
		myCentreAng[1] = { 0,90,0 };
		myCentreAng[2] = { 90,0,0 };
		myCentreAng[3] = { 180,0,0 };
		myCentreAng[4] = { 0,-90,0 };
		myCentreAng[5] = { -90,0,0 };


		// myCornerAng
		myCornerAng[0][0] = { 0,0,0 };
		myCornerAng[0][1] = { -90,0,-90 };
		myCornerAng[0][2] = {90, -90, 0};

		myCornerAng[1][0] = { 0,0,-90 };
		myCornerAng[1][1] = { -90,0,180 };
		myCornerAng[1][2] = { 0,-90, 0 };

		myCornerAng[2][0] = { 0,0,180 };
		myCornerAng[2][1] = { -90,0,90 };
		myCornerAng[2][2] = { -90,-90,0 };

		myCornerAng[3][0] = { 0,0,90 };
		myCornerAng[3][1] = { -90,0,0 };
		myCornerAng[3][2] = { 180,-90,0 };

		myCornerAng[4][0] = { 180,0,-90 };
		myCornerAng[4][1] = { 90,0,0 };
		myCornerAng[4][2] = { 0,90,0 };

		myCornerAng[5][0] = { 180,0,180 };
		myCornerAng[5][1] = { 90,0,-90 };
		myCornerAng[5][2] = { 90,90,0 };

		myCornerAng[6][0] = { 180,0,90 };
		myCornerAng[6][1] = { 90,0,180 };
		myCornerAng[6][2] = { 180,90,0 };

		myCornerAng[7][0] = { 180,0,0 };
		myCornerAng[7][1] = { 90,0,90 };
		myCornerAng[7][2] = { -90,90,0 };


		// myEdgeAng
		myEdgeAng[0][0] = { 0,0,0 };
		myEdgeAng[0][1] = { 180,-90,0 };

		myEdgeAng[1][0] = { 0,0,-90 };
		myEdgeAng[1][1] = { 90,-90,0 };

		myEdgeAng[2][0] = { 0,0,180 };
		myEdgeAng[2][1] = { 0,-90,0 };

		myEdgeAng[3][0] = { 0,0,90 };
		myEdgeAng[3][1] = { -90,-90,0 };

		myEdgeAng[4][0] = { 180,0,0 };
		myEdgeAng[4][1] = { 0,90,0 };

		myEdgeAng[5][0] = { 180,0,-90 };
		myEdgeAng[5][1] = { 90,90,0 };

		myEdgeAng[6][0] = { 180,0,180 };
		myEdgeAng[6][1] = { 180,90,0 };

		myEdgeAng[7][0] = { 180,0,90 };
		myEdgeAng[7][1] = { -90,90,0 };

		myEdgeAng[8][0] = { 90,0,0 };
		myEdgeAng[8][1] = { -90,0,-90 };

		myEdgeAng[9][0] = { 90,180,0 };
		myEdgeAng[9][1] = { 90,0,-90 };

		myEdgeAng[10][0] = {-90,180,0 };
		myEdgeAng[10][1] = { -90,0,90 };

		myEdgeAng[11][0] = { -90,0,0 };
		myEdgeAng[11][1] = { 90,0,90 };
	}

	void CubeModel::load_model()
	{
		init_tables();

		Model piece_corner = load_obj_file("piece", fn_piece_corner);
		Model piece_edge = load_obj_file("piece", fn_piece_edge);
		Model piece_centre = load_obj_file("piece", fn_piece_centre);
		Model paster_corner = load_obj_file("paster", fn_paster_corner);
		Model paster_edge = load_obj_file("paster", fn_paster_edge);
		Model paster_centre = load_obj_file("paster", fn_paster_centre);
		Model ball = load_obj_file("ball", fn_ball);

		piece_centre.material = my_material_piece;
		piece_corner.material = my_material_piece;
		piece_edge.material = my_material_piece;

		ball.material = my_material_ball;


		double paster_z = piece_len / 2.0f + paster_offset;	// 贴纸坐标

		// ball
		models.resize(1);	// ball
		models[0] = ball;


		// groups
		groups.resize(26);	// other
		_setPoints();
// 		myCentres = &(groups[0]);	// [6] URFDLB 排列
// 		myCorners = &(groups[6]);	// [8]
// 		myEdges = &(groups[14]);	// [12]

		// centre
		ModelGroup centre;
		centre.models.resize(2);
		centre.models[0] = piece_centre;
		centre.models[1] = paster_centre;

		centre.models[1].setPos(0, 0, paster_z);

		for (int i = 0; i < 6; ++i)
		{
			myCentres[i] = centre;
			myCentres[i].setPos(myCentrePos[i]);
			myCentres[i].rotate(myCentreAng[i]);
			myCentres[i].name = string("centre") + to_string(i);
			myCentres[i].models[1].material = my_material_paster[i];
		}

		myCentres[0].models[1].en_texture = true;	// U 中心块贴纸使用材质

		// 翻转材质坐标 y 轴
		for (int i = 0; i < (int)myCentres[0].models[1].texture.size(); ++i)
		{
			myCentres[0].models[1].texture[i][1] = 1.0f - myCentres[0].models[1].texture[i][1];
		}



		// corner
		ModelGroup corner;
		corner.models.resize(4);
		corner.models[0] = piece_corner;
		corner.models[1] = paster_corner;
		corner.models[2]= paster_corner;
		corner.models[3] = paster_corner;

		corner.models[1].name = "paster0";
		corner.models[2].name = "paster1";
		corner.models[3].name = "paster2";

		
		corner.models[1].setPos( 0,0, paster_z);		// U,  z

		corner.models[2].setPos(paster_z, 0, 0);		// R,  x
		corner.models[2].rotate(-90, 0, -90);

		corner.models[3].setPos( 0, -paster_z, 0);	// F,  -y
		corner.models[3].rotate(90, -90, 0);
		
		for (int i = 0; i < 8; ++i)
		{
			myCorners[i] = corner;
			myCorners[i].setPos(myCornerPos[i]);
			myCorners[i].rotate(myCornerAng[i][0]);
			myCorners[i].name = string("corner") + to_string(i);
			myCorners[i].models[1].material = my_material_paster[cs::cornerColor[i][0]];
			myCorners[i].models[2].material = my_material_paster[cs::cornerColor[i][1]];
			myCorners[i].models[3].material = my_material_paster[cs::cornerColor[i][2]];
		}


		// edge
		ModelGroup edge;
		edge.models.resize(3);
		edge.models[0] = piece_edge;
		edge.models[1] = paster_edge;
		edge.models[2] = paster_edge;

		edge.models[1].name = "paster0";
		edge.models[2].name = "paster1";

		edge.models[1].setPos(0, 0, paster_z);		// U,  z
		edge.models[2].setPos(paster_z, 0, 0);		// R,  x
		edge.models[2].rotate(0, -90, 180);



		for (int i = 0; i < 12; ++i)
		{
			myEdges[i] = edge;
			myEdges[i].setPos(myEdgePos[i]);
			myEdges[i].rotate(myEdgeAng[i][0]);
			myEdges[i].name = string("edge") + to_string(i);
			myEdges[i].models[1].material = my_material_paster[cs::edgeColor[i][0]];
			myEdges[i].models[2].material = my_material_paster[cs::edgeColor[i][1]];
		}


		myModelLoaded = true;
	}

	void CubeModel::setCubeAng(const CubeAng & ca)
	{

		// 处理 logo 角度
// 		if (my_ca.getCubieCube().move(cs::Ux1) == cc)
// 		{
// 			myLogoAngIdx = (myLogoAngIdx + 1) % 4;
// 		}
// 		else if (my_ca.getCubieCube().move(cs::Ux2) == cc)
// 		{
// 			myLogoAngIdx = (myLogoAngIdx + 2) % 4;
// 		}
// 		else if (my_ca.getCubieCube().move(cs::Ux3) == cc)
// 		{
// 			myLogoAngIdx = (myLogoAngIdx - 1) % 4;
// 		}
// 
// 		myLogoAng = 90.0f * myLogoAngIdx;


		my_ca = ca;
		_update();
	}

// 	void CubeModel::setCubieCube(const cs::CubieCube & cc)
// 	{
// 		std::array<double, 6> ang = { 0,0,0,0,0,0 };
// 		
// 		CubeAng ca;
// 		ca.setCubieCube(cc);
// 		ca.setAxisAng(ang);
// 
// 
// 		setCubeAng(ca);
// 	}
// 
// 	void CubeModel::setAxisAng(std::array<double,6> ang)
// 	{
// 		my_ca.setAxisAng(ang);
// 		myLogoAng = 90.0f * myLogoAngIdx + ang[0];
// 		_update();
// 	}


	void CubeModel::rotateAxis(int axis, double ang)
	{
		my_ca.rotateAxis(axis, ang);

		_update();
	}


	CubeModel & CubeModel::operator=(const CubeModel & _right)
	{
		ModelGroup::operator=(_right);

		my_ca = _right.my_ca;


		// 以下是表

		my_material_piece = _right.my_material_piece;
		my_material_ball = _right.my_material_ball;

		for (int i = 0; i < 6; ++i)
		{
			my_material_paster[i] = _right.my_material_paster[i];
			myCentrePos[i] = _right.myCentrePos[i];
			myCentreAng[i] = _right.myCentreAng[i];
		}

		for (int i = 0; i < 8; ++i)
		{
			myCornerPos[i] = _right.myCornerPos[i];

			for (int j = 0; j < 3; ++j)
			{
				myCornerAng[i][j] = _right.myCornerAng[i][j];
			}
		}
		
		for (int i = 0; i < 12; ++i)
		{
			myEdgePos[i] = _right.myEdgePos[i];

			myEdgeAng[i][0] = _right.myEdgeAng[i][0];
			myEdgeAng[i][1] = _right.myEdgeAng[i][1];
		}
		


		//myLogoAng = _right.myLogoAng;
		myModelLoaded = _right.myModelLoaded;

		_setPoints();

		return *this;
	}

	void CubeModel::resetColor()
	{
		resetAllPasterColor();
		resetBodyColor();
	}

	void CubeModel::resetAllPasterColor()
	{
		for (int i = 0; i < 54; ++i)
		{
			_setPasterMaterial(i, my_material_paster[i / 9]);
		}
	}

	void CubeModel::resetBodyColor()
	{
		_setBodyMaterial(my_material_piece, my_material_ball);
	}

	void CubeModel::resetPasterColorAbs(cs::Facelet facelet)
	{
		resetPasterColorAbs(int(facelet));
	}

	void CubeModel::resetPasterColorAbs(int facelet)
	{
		if (facelet < 0 || facelet >= 54)
			return;

		_setPasterMaterial(facelet, my_material_paster[facelet / 9]);
	}

	void CubeModel::resetPasterColorRel(cs::Facelet facelet)
	{
		resetPasterColorRel(int(facelet));
	}

	void CubeModel::resetPasterColorRel(int facelet)
	{
		if (facelet < 0 || facelet >= 54)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();
		cs::FaceCube fc = cc.toFaceCube();

		_setPasterMaterial(fc[facelet], my_material_paster[facelet / 9]);
	}

	void CubeModel::resetCornerColorAbs(cs::Corner corner)
	{
		resetCornerColorAbs((int)corner);
	}

	void CubeModel::resetCornerColorAbs(int corner)
	{
		if (corner < 0 || corner >= 8)
			return;

		for (int j = 0; j < 3; ++j)
		{
			resetPasterColorAbs(cs::cornerFacelet[corner][j]);
		}
	}

	void CubeModel::resetCornerColorRel(cs::Corner corner)
	{
		resetCornerColorRel((int)corner);
	}

	void CubeModel::resetCornerColorRel(int corner)
	{
		if (corner < 0 || corner >= 8)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();

		resetCornerColorAbs(cc.corner.perm[corner]);
	}

	void CubeModel::resetEdgeColorAbs(cs::Edge edge)
	{
		resetEdgeColorAbs((int)edge);
	}

	void CubeModel::resetEdgeColorAbs(int edge)
	{
		if (edge < 0 || edge >= 12)
			return;

		resetPasterColorAbs(cs::edgeFacelet[edge][0]);
		resetPasterColorAbs(cs::edgeFacelet[edge][1]);
	}

	void CubeModel::reseEdgeColorRel(cs::Edge edge)
	{
		resetEdgeColorRel((int)edge);
	}

	void CubeModel::resetEdgeColorRel(int edge)
	{
		if (edge < 0 || edge >= 12)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();

		resetEdgeColorAbs(cc.edge.perm[edge]);
	}

	void CubeModel::resetCentreColor(cs::Color centre)
	{
		resetCentreColor((int)centre);
	}

	void CubeModel::resetCentreColor(int centre)
	{
		if (centre < 0 || centre >= 6)
			return;

		int facelet = 4 + centre * 9;

		return resetPasterColorAbs(facelet);
	}

	void CubeModel::resetFaceColor(cs::Color face)
	{
		resetFaceColor((int)face);
	}

	void CubeModel::resetFaceColor(int face)
	{
		if (face < 0 || face >= 6)
			return;

		int _start = 9 * face;
		int _end = 9 * (face + 1);

		for (int i = _start; i < _end; ++i)
		{
			resetPasterColorAbs(i);
		}
	}

	void CubeModel::setPasterColorAbs(cs::Facelet facelet, fvec4 color)
	{
		setPasterColorAbs((int)facelet, color);
	}

	void CubeModel::setPasterColorAbs(int facelet, fvec4 color)
	{
		if (facelet < 0 || facelet >= 54)
			return;

		_setPasterColor(facelet, color);
	}

	void CubeModel::setPasterColorRel(cs::Facelet facelet, fvec4 color)
	{
		setPasterColorRel((int)facelet, color);
	}

	void CubeModel::setPasterColorRel(int facelet, fvec4 color)
	{
		if (facelet < 0 || facelet >= 54)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();
		cs::FaceCube fc = cc.toFaceCube();

		_setPasterColor(fc[facelet], color);
	}

	void CubeModel::adjustPasterColorAbs(cs::Facelet facelet, double saturation, double brightness)
	{
		adjustPasterColorAbs((int)facelet, saturation, brightness);
	}

	void CubeModel::adjustPasterColorAbs(int facelet, double saturation, double brightness)
	{
		Model* mm = _getPaster(facelet);

		if (mm)
		{
			material_t& material = mm->material;
			material.ambient_color = _adjust_color(material.ambient_color, saturation, brightness);
			material.diffuse_color = _adjust_color(material.diffuse_color, saturation, brightness);
			material.emissive_color = _adjust_color(material.emissive_color, saturation, brightness);
			material.specular_color = _adjust_color(material.specular_color, saturation, brightness);
		}
	}

	void CubeModel::adjustPasterColorRel(cs::Facelet facelet, double saturation, double brightness)
	{
		adjustPasterColorRel((int)facelet, saturation, brightness);
	}

	void CubeModel::adjustPasterColorRel(int facelet, double saturation, double brightness)
	{
		if (facelet < 0 || facelet >= 54)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();
		cs::FaceCube fc = cc.toFaceCube();

		adjustPasterColorAbs(fc[facelet], saturation, brightness);
	}

	void CubeModel::setCornerColorAbs(cs::Corner corner, fvec4 color)
	{
		setCornerColorAbs((int)corner, color);
	}

	void CubeModel::setCornerColorAbs(int corner, fvec4 color)
	{
		if (corner < 0 || corner >= 8)
			return;

		for (int j = 0; j < 3; ++j)
		{
			setPasterColorAbs(cs::cornerFacelet[corner][j], color);
		}
	}

	void CubeModel::setCornerColorRel(cs::Corner corner, fvec4 color)
	{
		setCornerColorRel((int)corner, color);
	}

	void CubeModel::setCornerColorRel(int corner, fvec4 color)
	{
		if (corner < 0 || corner >= 8)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();

		setCornerColorAbs(cc.corner.perm[corner], color);
	}

	void CubeModel::adjustCornerColorAbs(cs::Corner corner, double saturation, double brightness)
	{
		adjustCornerColorAbs((int)corner, saturation, brightness);
	}

	void CubeModel::adjustCornerColorAbs(int corner, double saturation, double brightness)
	{
		if (corner < 0 || corner >= 8)
			return;

		for (int j = 0; j < 3; ++j)
		{
			adjustPasterColorAbs(cs::cornerFacelet[corner][j], saturation, brightness);
		}
	}

	void CubeModel::adjustCornerColorRel(cs::Corner corner, double saturation, double brightness)
	{
		adjustCornerColorRel((int)corner, saturation, brightness);
	}

	void CubeModel::adjustCornerColorRel(int corner, double saturation, double brightness)
	{
		if (corner < 0 || corner >= 8)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();

		adjustCornerColorAbs(cc.corner.perm[corner], saturation, brightness);
	}

	void CubeModel::setEdgeColorAbs(cs::Edge edge, fvec4 color)
	{
		setEdgeColorAbs((int)edge, color);
	}

	void CubeModel::setEdgeColorAbs(int edge, fvec4 color)
	{
		if (edge < 0 || edge >= 12)
			return;

		setPasterColorAbs(cs::edgeFacelet[edge][0], color);
		setPasterColorAbs(cs::edgeFacelet[edge][1], color);
	}

	void CubeModel::setEdgeColorRel(cs::Edge edge, fvec4 color)
	{
		setEdgeColorRel((int)edge, color);
	}

	void CubeModel::setEdgeColorRel(int edge, fvec4 color)
	{
		if (edge < 0 || edge >= 12)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();

		setEdgeColorAbs(cc.edge.perm[edge], color);
	}

	void CubeModel::adjustEdgeColorAbs(cs::Edge edge, double saturation, double brightness)
	{
		adjustEdgeColorAbs((int)edge, saturation, brightness);
	}

	void CubeModel::adjustEdgeColorAbs(int edge, double saturation, double brightness)
	{
		if (edge < 0 || edge >= 12)
			return;

		adjustPasterColorAbs(cs::edgeFacelet[edge][0], saturation, brightness);
		adjustPasterColorAbs(cs::edgeFacelet[edge][1], saturation, brightness);
	}

	void CubeModel::adjustEdgeColorRel(cs::Edge edge, double saturation, double brightness)
	{
		adjustEdgeColorRel((int)edge, saturation, brightness);
	}


	void CubeModel::adjustEdgeColorRel(int edge, double saturation, double brightness)
	{
		if (edge < 0 || edge >= 12)
			return;

		const cs::CubieCube& cc = my_ca.getCubieCube();

		adjustEdgeColorAbs(cc.edge.perm[edge], saturation, brightness);
	}

	void CubeModel::adjustCentreColor(cs::Color centre, double saturation, double brightness)
	{
		adjustCentreColor((int)centre, saturation, brightness);
	}

	void CubeModel::setCentreColor(cs::Color centre, fvec4 color)
	{
		setCentreColor((int)centre, color);
	}

	void CubeModel::setCentreColor(int centre, fvec4 color)
	{
		if (centre < 0 || centre >= 6)
			return;

		int facelet = 4 + centre * 9;

		return setPasterColorAbs(facelet, color);
	}

	void CubeModel::adjustCentreColor(int centre, double saturation, double brightness)
	{
		if (centre < 0 || centre >= 6)
			return;

		int facelet = 4 + centre * 9;

		adjustPasterColorAbs(facelet, saturation, brightness);
	}

	void CubeModel::setFaceColor(cs::Color face, fvec4 color)
	{
		setFaceColor((int)face, color);
	}

	void CubeModel::setFaceColor(int face, fvec4 color)
	{
		if (face < 0 || face >= 6)
			return;

		int _start = 9 * face;
		int _end = 9 * (face + 1);

		for (int i = _start; i < _end; ++i)
		{
			setPasterColorAbs(i, color);
		}
	}

	void CubeModel::adjustFaceColor(cs::Color face, double saturation, double brightness)
	{
		adjustFaceColor((int)face, saturation, brightness);
	}

	void CubeModel::adjustFaceColor(int face, double saturation, double brightness)
	{
		if (face < 0 || face >= 6)
			return;

		int _start = 9 * face;
		int _end = 9 * (face + 1);

		for (int i = _start; i < _end; ++i)
		{
			adjustPasterColorAbs(i, saturation, brightness);
		}
	}

	void CubeModel::setBodyMaterial(material_t material)
	{

		const float ball_factor = 0.5f;

		material_t mat_b = material;
		mat_b.ambient_color = ball_factor * material.ambient_color;
		mat_b.diffuse_color = ball_factor * material.diffuse_color;
		mat_b.specular_color = { 0,0,0,0 };

		_setBodyMaterial(material, mat_b);
	}


	void CubeModel::_update()
	{
		if (!myModelLoaded)
			return;

		const cs::CubieCube cc = my_ca.getCubieCube();
		std::array<double, 6> ang = my_ca.getAxisAng();

		// 角块位置和姿态
		for (int i = 0; i<8; ++i)
		{

			myCorners[cc.corner.perm[i]].setPos(myCornerPos[i]);
			myCorners[cc.corner.perm[i]].setEuler(myCornerAng[i][cc.corner.ori[i].get()]);
		}


		// edge pos ang
		for (int i = 0; i < 12; ++i)
		{
			myEdges[cc.edge.perm[i]].setPos(myEdgePos[i]);
			myEdges[cc.edge.perm[i]].setEuler(myEdgeAng[i][cc.edge.ori[i].get()]);
		}

		// centre
		for (int i = 0; i < 6; ++i)
		{
			myCentres[i].setEuler(myCentreAng[i]);
		}

		// 旋转角度
		for (int i = 0; i < 6; ++i)
		{
			if (ang[i] != 0)
			{
				vec3 n = rotateAxisTbl[i];
				
				if (i != 0)	// not logo
				{
					myCentres[i].rotate(n, -ang[i]);
				}
				

				for (int j = 0; j < 4; ++j)
				{
					myCorners[cc.corner.perm[rotateCornerTbl[i][j]]].rotate({ 0,0,0 },n, -ang[i]);
					myEdges[cc.edge.perm[rotateEdgeTbl[i][j]]].rotate({ 0,0,0 }, n, -ang[i]);
				}
			}
		}

		// logo

		double logoAng = 90.0 * my_ca.getAxisAngIdx()[0] + my_ca.getAxisAng()[0];
		logoAng = fixAng180(logoAng);

		myCentres[0].rotate(rotateAxisTbl[0], -logoAng);

	}

	void CubeModel::_setPoints()
	{
		myCentres = &(groups[0]);	// [6] URFDLB 排列
		myCorners = &(groups[6]);	// [8]
		myEdges = &(groups[14]);	// [12]
	}

	Model * CubeModel::_getPaster(int facelet)
	{
		// 定位角块
		int corner_idx = -1;
		int sub_idx = -1;

		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (cs::cornerFacelet[i][j] == facelet)
				{
					corner_idx = i;
					sub_idx = j;
					break;
				}
			}
		}

		if (corner_idx >= 0)	// 属于角块
		{
			ModelGroup& gg = myCorners[corner_idx];
			Model& mm = gg.models[sub_idx + 1];

			return &mm;
		}

		// 定位棱块

		int edge_idx = -1;
		sub_idx = -1;

		for (int i = 0; i < 12; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				if (cs::edgeFacelet[i][j] == facelet)
				{
					edge_idx = i;
					sub_idx = j;
					break;
				}
			}
		}

		if (edge_idx >= 0)	// 属于角块
		{
			ModelGroup& gg = myEdges[edge_idx];
			Model& mm = gg.models[sub_idx + 1];

			return &mm;
		}

		// 定位中心块
		int centre_idx = (facelet - 4) / 9;

		if (0 <= centre_idx && centre_idx < 6)
		{
			ModelGroup& gg = myCentres[centre_idx];
			Model& mm = gg.models[1];
			return &mm;
		}


		return nullptr;
	}

	void CubeModel::_setPasterColor(int facelet, fvec4 color)
	{
		material_t material = material_t::colored_material(color,
			paster_ambient_factor, paster_diffuse_factor, paster_specular_factor, 0.0f, paster_specular_exponent);

		_setPasterMaterial(facelet, material);

	}

	void CubeModel::_setPasterMaterial(int facelet, material_t material)
	{
		Model* mm = _getPaster(facelet);

		if (mm)
			mm->material = material;
	}

	void CubeModel::_setBodyMaterial(material_t mat_piece, material_t mat_ball)
	{
		for (int i = 0; i < 6; ++i)
		{
			ModelGroup& gg = myCentres[i];
			gg.models[0].material = mat_piece;
		}

		for (int i = 0; i < 8; ++i)
		{
			ModelGroup& gg = myCorners[i];
			gg.models[0].material = mat_piece;
		}

		for (int i = 0; i < 12; ++i)
		{
			ModelGroup& gg = myEdges[i];
			gg.models[0].material = mat_piece;
		}

		models[0].material = mat_ball;
	}

	fvec4 CubeModel::_adjust_color(fvec4 color, double saturation, double brightness)
	{
		fvec4 res = color;

		const double coef_r = 0.298936021293775;
		const double coef_g = 0.587043074451121;
		const double coef_b = 0.114020904255103;

		double gray = coef_r * color[0] + coef_g * color[1] + coef_b * color[2];

		// 饱和度调整
		double rr = gray + saturation * (color[0] - gray);
		double gg = gray + saturation * (color[1] - gray);
		double bb = gray + saturation * (color[2] - gray);

		// 亮度调整
		rr *= brightness;
		gg *= brightness;
		bb *= brightness;

		rr = max(0.0, min(1.0, rr));
		gg = max(0.0, min(1.0, gg));
		bb = max(0.0, min(1.0, bb));

		res[0] = (float)rr;
		res[1] = (float)gg;
		res[2] = (float)bb;

		return res;
	}

}

