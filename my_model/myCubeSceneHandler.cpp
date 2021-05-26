
#include "myCubeSceneHandler.h"
#include "AxisCube.h"
#include <array>
#include <tuple>

using namespace cs;
using namespace std;


namespace my {


// 	const vec3 CubeSceneHandler::aixs_table[6] = rotateAxisTbl
// 	{
// 		{0,0,1},	// 0 U
// 		{1,0,0},	// 1 R  
// 		{0,-1,0},	// 2 F
// 		{0,0,-1},	// 3 D
// 		{-1,0,0},	// 4 L
// 		{0,1,0}		// 5 B
// 	};


    CubeSceneHandler::CubeSceneHandler(CubeScene &scene)
		: myCubeScene(scene)
		, myCube(scene.cube)
		, myCam(scene.camera)
		, myCubeAni(myCube)
		//, myCoordAni(myCube)
		, myArrowAni(scene)
	{
    }

    bool CubeSceneHandler::init(ESContext *esContext, const CubeModel *srcCube) {
        //AxisCube::init();
        
		
		
		if (!myCubeScene.init(esContext, srcCube))
			return false;

        _update_cam();

		myArrowAni.setOnOff(false);
		myIsInited = true;

		return true;

    }


	void CubeSceneHandler::moveAbsolute(cs::Move mv, double delay)
	{
		bool is_move2n = false;

		if (isMove2n(mv))
		{
			is_move2n = true;
			mv = (Move)((int)mv & 0xff);
		}


		if (!(mv >= Ux1 && mv < Rot_x1))
			return;



		CubieCube cc = myCubeAni.getDstFrameCubeAng().getCubieCube().move(mv);

		int ax = mv / 3;
		int po = mv % 3 + 1;

		array<int, 6> ang_rot_dir = { 0,0,0,0,0,0 };

		if (po == 2)	// Ux2 等
		{
			if (is_move2n)
				ang_rot_dir[ax] = -1;
			else
				ang_rot_dir[ax] = 1;
		}

		myCubeAni.setFrame(delay, CubeAng(cc), ang_rot_dir);
	}

	void CubeSceneHandler::moveAbsolute(Move mv) 
	{
		moveAbsolute(mv, myCubeAni.getDefaultDelay());
    }


	void CubeSceneHandler::moveRelative(cs::Move mv, double delay)
	{
		bool is_move2n = false;

		if (isMove2n(mv))
		{
			is_move2n = true;
			mv = (Move)((int)mv & 0xff);
		}


		// 查找 AxisCube
		int axisCube_idx = getRelativeAxisCubeIdx();
		const AxisCube &axisCube = AxisCube::valid_table[axisCube_idx];
		CubieCube axisMoveCC = AxisCube::gen_cubie_table[axisCube_idx];


		CubieCube moveCubeInv = axisMoveCC.inv();


		static const int rot_tbl[] = { 1, 0, 2 };

		// 分情况 move
		if (Ux1 <= mv && mv <= Bx3)        // 基本公式
		{
			int mv_a = AxisCube::move_conj_table_i[mv][axisCube_idx];
			moveAbsolute((Move)(is_move2n ? 0x100 | mv_a : mv_a), delay);
		}
		else if (Rot_x1 <= mv && mv <= Rot_z3)        // 整体旋转
		{
			int ax = (mv - (int)Rot_x1) / 3;
			int po = mv % 3 + 1;

			double ang = -90.0 * po;

			Coordinate c = myCubeAni.getDstFrameCoord();


			vec3 rot_ax = c.local2world(CubeModel::rotateAxisTbl[axisCube[rot_tbl[ax]]]);
			c.rotate(rot_ax, ang);

			vec3 ref_ax = { 0,0,0 };

			if (po == 2)
			{
				if (is_move2n)
					ref_ax = rot_ax;
				else
					ref_ax = -1.0 * rot_ax;
			}

			myCubeAni.setFrame(delay, c, ref_ax);
		}
		else if (Ex1 <= mv && mv <= Sx3)    // 中间层
		{
			int m1 = move_middle_table[mv - Ex1][0];
			int m2 = move_middle_table[mv - Ex1][1];
			int m_rot = move_middle_table[mv - Ex1][2];

			int ax = (m_rot - (int)Rot_x1) / 3;
			int po = m_rot % 3 + 1;


			CubieCube cc = myCubeAni.getDstFrameCubeAng().getCubieCube();

			int m1_c = AxisCube::move_conj_table_i[m1][axisCube_idx];
			int m2_c = AxisCube::move_conj_table_i[m2][axisCube_idx];

			int m1_ax = m1_c / 3;
			int m2_ax = m2_c / 3;

			cc = cc.move(m1_c);
			cc = cc.move(m2_c);

			Coordinate c = myCubeAni.getDstFrameCoord();

			vec3 rot_axis = c.local2world(CubeModel::rotateAxisTbl[axisCube[rot_tbl[ax]]]);
			c.rotate(rot_axis, -90.0 * po);


			array<int, 6> ang_rot_dir = { 0,0,0,0,0,0 };
			vec3 ref_axis = { 0,0,0 };



			if (po == 2)
			{
				if (is_move2n)
				{
					ang_rot_dir[m1_ax] = -1;
					ang_rot_dir[m2_ax] = 1;
					ref_axis = -1.0 * rot_axis;
				}
				else
				{
					ang_rot_dir[m1_ax] = 1;
					ang_rot_dir[m2_ax] = -1;
					ref_axis = rot_axis;
				}

				if (mv == Sx2)
				{
					ang_rot_dir[m1_ax] = -ang_rot_dir[m1_ax];
					ang_rot_dir[m2_ax] = -ang_rot_dir[m2_ax];
					ref_axis = -1.0 * ref_axis;
				}

			}


			myCubeAni.setFrame(delay, c, CubeAng(cc, { 0,0,0,0,0,0 }), ref_axis, ang_rot_dir);

		}
		else if (LowerUx1 <= mv && mv <= LowerBx3)    // 转两层
		{
			int m1 = move_lower_table[mv - LowerUx1][0];
			int m_rot = move_lower_table[mv - LowerUx1][1];

			int rot_ax = (m_rot - (int)Rot_x1) / 3;
			int po = m_rot % 3 + 1;


			CubieCube cc = myCubeAni.getDstFrameCubeAng().getCubieCube();

			int m1_c = AxisCube::move_conj_table_i[m1][axisCube_idx];
			cc = cc.move(m1_c);
			int m1_ax = m1_c / 3;


			Coordinate c = myCubeAni.getDstFrameCoord();
			vec3 rot_axis = c.local2world(CubeModel::rotateAxisTbl[axisCube[rot_tbl[rot_ax]]]);
			c.rotate(rot_axis, -90.0 * po);


			array<int, 6> ang_rot_dir = { 0,0,0,0,0,0 };
			vec3 ref_axis = { 0,0,0 };

			if (po == 2)    // 处理180度方向
			{
				if (is_move2n)
				{
					ang_rot_dir[m1_ax] = -1;
					ref_axis = rot_axis;
				}
				else
				{
					ang_rot_dir[m1_ax] = 1;
					ref_axis = -1.0 * rot_axis;
				}

				if (mv >= LowerDx1)
					ref_axis = -1.0 * ref_axis;
			}

			myCubeAni.setFrame(delay, c, CubeAng(cc, { 0,0,0,0,0,0 }),
				ref_axis, ang_rot_dir);
		}

	}

	void CubeSceneHandler::moveRelative(Move mv) {
		moveRelative(mv, myCubeAni.getDefaultDelay());
    }

	void CubeSceneHandler::move(cs::Move mv, double delay)
	{
		if (myRelativeFormulaMode)
			moveRelative(mv, delay);
		else
			moveAbsolute(mv, delay);
	}

	void CubeSceneHandler::move(Move mv) {
		move(mv, myCubeAni.getDefaultDelay());
    }

    void CubeSceneHandler::setCamDistance(double d) {
        if (d > 0) {
            myCamDistance = d;
            _update_cam();
        }
    }

    void CubeSceneHandler::setCamPitch(double a) {
        if (a >= -89.0 && a <= 89.0) {
            myCamPitch = a;
            _update_cam();
        }
    }

    void CubeSceneHandler::setCamYaw(double a) {
        myCamYaw = a;
        _update_cam();

    }

    void CubeSceneHandler::setCam(double dist, double pitch, double yaw) {
        if (dist > 0 && pitch >= 89.0 && pitch <= 89.0) {
            myCamDistance = dist;
            myCamPitch = pitch;
            myCamYaw = yaw;
            _update_cam();
        }
    }

    void CubeSceneHandler::setRefPitch(double a) {
        if (a >= 89.0 && a <= 89.0)
            myRefPitch = a;
    }

    void CubeSceneHandler::update(double deltaTime) {
		if (!myIsInited)
			return;

        myCubeAni.update(deltaTime);
		myArrowAni.update(deltaTime);
    }


    void CubeSceneHandler::setDefaultDelay(double t) {
		myCubeAni.setDefaultDelay(t);
    }

    int CubeSceneHandler::getRelativeAxisCubeIdx()
    {
        Coordinate c;
        c.rotate(0, 0, 90);
        c.rotate(0, -(myRefPitchToCam ? myCamPitch : myRefPitch), (myRefYawToCam ? myRefYaw : myCamYaw));

        // 查找相对的 U
        double max_u = -1000;
        int axis_u = -1;

        for (int i = 0; i < 6; ++i)
        {
            double u = dot(myCubeAni.getDstFrameCoord().local2world(CubeModel::rotateAxisTbl[i]), c.local2world({ 0,0,1 }));
            if (u > max_u)
            {
                max_u = u;
                axis_u = i;
            }
        }

        // 查找相对的 R
        double max_r = -1000;
        int axis_r = -1;
        for (int i = 0; i < 6; ++i)
        {
            if (i == axis_u)
                continue;

            double r = dot(myCubeAni.getDstFrameCoord().local2world(CubeModel::rotateAxisTbl[i]), c.local2world({ 1,0,0 }));
            if (r > max_r)
            {
                max_r = r;
                axis_r = i;
            }
        }

        // 查找 AxisCube
        int axisCube_idx = -1;
        for (int i = 0; i < AxisCube::num_valid_table; ++i)
        {
            const AxisCube& ac = AxisCube::valid_table[i];
            if (ac[0] == axis_u && ac[1] == axis_r)
            {
                axisCube_idx = i;
                break;
            }
        }

        return axisCube_idx;
    }

    AxisCube CubeSceneHandler::getRelativeAxisCube() {
        AxisCube axisCube = AxisCube::valid_table[getRelativeAxisCubeIdx()];

        return axisCube;
    }

    void CubeSceneHandler::setCoordAniOriginal() {
        Coordinate c;
        myCubeAni.setFrame(c);

    }

	void CubeSceneHandler::showArrow(bool b)
	{
		myArrowAni.setOnOff(b);
	}


	int CubeSceneHandler::hitFacelet_norm(double x, double y)
	{

		static auto cross_2d = [](const vec2& a, const vec2& b) -> double
		{
			return a[0] * b[1] - b[0] * a[1];
		};
		

		// 判断三角形，属于正面返回 true， 按顺时针顺序
		static auto triangleDir = [](const vec2& point1, const vec2& point2, const vec2& point3) -> bool
		{
			vec2 a = point3 - point1;
			vec2 b = point2 - point1;


			double c = cross_2d(a, b);

			return c > 0;
		};

		// 每个角的标准化坐标
		static array<vec3, 8> corner_pos = 
		{
			vec3{ 1, -1, 1 },	// 0 URF 
			vec3{ -1, -1, 1 },	// 1 UFL
			vec3{ -1, 1, 1 },	// 2 ULB
			vec3{ 1, 1, 1 },	// 3 UBR
			vec3{ 1, -1, -1 },	// 4 DFR
			vec3{ -1, -1, -1 },	// 5 DLF
			vec3{ -1, 1, -1 },	// 6 DBL
			vec3{ 1, 1, -1 }	// 7 DRB
		};

		// 每个面对应的四个角，按顺时针方向
		static array<array<int,4>,6> face_idx = 
		{
			array<int,4>{2,3,0,1},	// 0 U
			array<int,4>{0,3,7,4},	// 1 R
			array<int,4>{1,0,4,5},	// 2 F
			array<int,4>{5,4,7,6},	// 3 D
			array<int,4>{2,1,5,6},	// 4 L
			array<int,4>{3,2,6,7}	// 5 B
		};

		// 计算每个面的坐标
		array<array<vec3, 4>, 6> faces;

		double halfSize = (CubeModel::piece_len * 3.0 + CubeModel::piece_gap * 2.0) / 2.0;

		for (int i = 0; i < 6; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				faces[i][j] = halfSize * corner_pos[face_idx[i][j]];
			}
			
		}

		// 计算每个贴纸的坐标，按顺时针方向
		array<array<vec3, 4>, 54> facelets;

		for (int i = 0; i < 54; ++i)
		{
			int color = i / 9;
			int idx = i % 9;

			int row = idx / 3;
			int col = idx % 3;


			array<vec3, 4> face = faces[color];

			vec3 p_col = face[1] - face[0];
			vec3 p_row = face[3] - face[0];
			

			array<vec3, 4> ff
			{
				face[0] + (row + 0) / 3.0 * p_row + (col + 0) / 3.0 * p_col,
				face[0] + (row + 0) / 3.0 * p_row + (col + 1) / 3.0 * p_col,
				face[0] + (row + 1) / 3.0 * p_row + (col + 1) / 3.0 * p_col,
				face[0] + (row + 1) / 3.0 * p_row + (col + 0) / 3.0 * p_col,
			};

			facelets[i] = ff;

		}

		// 投影相关矩阵

		double aspect = (double)myCubeScene.esContext->width / (double)myCubeScene.esContext->height;

		mat4 viewMatrix = myCubeScene.camera.getViewMatrix();
		mat4 projMatrix = myCubeScene.camera.getProjMatrix(aspect, myCubeScene.cam_nearZ, myCubeScene.cam_farZ);

		Coordinate coor = myCube;
		mat4 modelMatrix = coor.getTransfrom();
		mat4 mv = viewMatrix * modelMatrix;
		mat4 mvp = projMatrix * mv;



		// 遍历每个面，判断是正面的面  (idx, 坐标)
		vector<tuple<int, array<vec2,4>>> pos_facelets;

		for (int i = 0; i < 6; ++i)
		{
			auto face = faces[i];


			array<vec3, 4> fv;

			for (int j = 0; j < 4; ++j)
			{
				vec4 a = vec3to4(face[j], 1.0);
				vec4 p = mvp * a;
				fv[j] = { p[0] / p[3], p[1] / p[3], p[2] / p[3] };
			}


			bool is_pos = triangleDir(
				vec2{ fv[0][0], fv[0][1] },
				vec2{ fv[1][0], fv[1][1] },
				vec2{ fv[2][0], fv[2][1] }
			);


			if (is_pos)
			{
				for (int j = 0; j < 9; ++j)
				{
					int idx = i * 9 + j;
					array<vec2,4> points;

					for (int k = 0; k < 4; ++k)
					{
						vec4 a = vec3to4(facelets[idx][k], 1.0);
						vec4 p = mvp * a;
						points[k] = { p[0] / p[3], p[1] / p[3] };
					}

					pos_facelets.push_back({ idx, points });
				}
			}

		}



		// 判断某个点 P 是否在四边形内
		static auto is_in_quadrangle = [](const vec2 point, const array<vec2, 4>& quadrangle) -> bool
		{
			bool b1 = cross_2d(quadrangle[3] - quadrangle[0], point - quadrangle[0]) > 0;
			bool b2 = cross_2d(quadrangle[2] - quadrangle[3], point - quadrangle[3]) > 0;
			bool b3 = cross_2d(quadrangle[1] - quadrangle[2], point - quadrangle[2]) > 0;
			bool b4 = cross_2d(quadrangle[0] - quadrangle[1], point - quadrangle[1]) > 0;

			return b1 && b2 && b3 && b4;
		};

		// 判断击中那个贴纸
		int hit = -1;

		vec2 point{ x, y };

		for (int i = 0; i < (int)pos_facelets.size(); ++i)
		{
			const auto& pf = pos_facelets[i];
			int idx = get<0>(pf);
			auto qq = get<1>(pf);

			bool b = is_in_quadrangle(point, qq);

			if (b)
			{
				hit = idx;
				break;
			}
		}


		return hit;
	}

	int CubeSceneHandler::hitFacelet(int x, int y)
	{
		double xx = -1.0 + 2.0 * x / (myCubeScene.esContext->width - 1);
		double yy = 1.0 - 2.0 * y / (myCubeScene.esContext->height - 1);

		return hitFacelet_norm(xx, yy);
	}

	int CubeSceneHandler::hitCorner_norm(double x, double y)
	{
		int facelet = hitFacelet_norm(x, y);

		if (!(0 <= facelet && facelet < 54))
			return -1;


		int hit = -1;

		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (facelet == cornerFacelet[i][j])
				{
					hit = i;
					break;
				}
			}
		}

		return hit;
	}

	int CubeSceneHandler::hitCorner(int x, int y)
	{
		double xx = -1.0 + 2.0 * x / (myCubeScene.esContext->width - 1);
		double yy = 1.0 - 2.0 * y / (myCubeScene.esContext->height - 1);

		return hitCorner_norm(xx, yy);
	}

	int CubeSceneHandler::hitEdge_norm(double x, double y)
	{
		int facelet = hitFacelet_norm(x, y);

		if (!(0 <= facelet && facelet < 54))
			return -1;


		int hit = -1;

		for (int i = 0; i < 12; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				if (facelet == edgeFacelet[i][j])
				{
					hit = i;
					break;
				}
			}
		}

		return hit;
	}

	int CubeSceneHandler::hitEdge(int x, int y)
	{
		double xx = -1.0 + 2.0 * x / (myCubeScene.esContext->width - 1);
		double yy = 1.0 - 2.0 * y / (myCubeScene.esContext->height - 1);

		return hitEdge_norm(xx, yy);
	}

	int CubeSceneHandler::hitCenter_norm(double x, double y)
	{
		int facelet = hitFacelet_norm(x, y);

		if (!(0 <= facelet && facelet < 54))
			return -1;


		int hit = -1;

		for (int i = 0; i < 6; ++i)
		{
			if (facelet == 4 + i * 9)
			{
				hit = i;
				break;
			}

		}

		return hit;
	}

	int CubeSceneHandler::hitCenter(int x, int y)
	{
		double xx = -1.0 + 2.0 * x / (myCubeScene.esContext->width - 1);
		double yy = 1.0 - 2.0 * y / (myCubeScene.esContext->height - 1);

		return hitCenter_norm(xx, yy);
	}

	void CubeSceneHandler::_update_cam() {
        Coordinate c;
        c.rotate(90, 0, 90);
        c.rotate(0, -myCamPitch, myCamYaw);
        c.translateLocal({0, 0, myCamDistance});
        (Coordinate &) myCam = c;
    }

}