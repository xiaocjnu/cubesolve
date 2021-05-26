#pragma once
#include "esUtil.h"
#include "myModel.h"
#include "myCubeModel.h"
#include "myScene.h"
#include <vector>
#include <map>

#define CUBESCENE_NUM_LIGHT	2



namespace my
{
	class CubeScene
	{
	public:

		// 查询 uniforms, vertex attributes 信息
		struct infos_t {
			int loc;
			int length;
			int size;
			GLenum type;
			char name[128];
		};

		// 模型画图信息
		struct model_info_t
		{
			std::string name;
			bool is_enable;	// 是否画
			ModelGroupIterator it;
			material_t material;
			int	num_elem;
			int array_off;	// 在 vboId_array 的偏移量
			int elem_off;	// 在 vboId_element 的偏移量
			bool use_tex;	// 是否有材质


			// 临时缓存
			fmat4 mvp;
			fmat4 fmv;
			fmat3 inv_transpose_mv;


// 			std::vector<fvec3> plainVertex;
// 			std::vector<fvec3> plainNormal;
// 			std::vector<fvec2> plainTex;
		};

		


		CubeModel cube;				// 魔方
		ModelGroup modelgroups;		// 其他物体


		std::vector<light_t> lights;
		fvec4 ambient_color;
		Camera camera;

		float cam_nearZ;
		float cam_farZ;
		
		bool enable_environment = false;	// 使用环境立方体贴图，否则使用背景
		float environment_cube_len = 1000;

		fvec4 background_color = { 0.5f, 0.4f, 0.7f, 0 };	// 背景颜色
		bool is_draw_test_box = false;		// 画几个box, 测试用

		fvec4 cube_arrow_color = { 0.8667f, 0.8863f, 1.0f, 1.f }; //{ 0.f, 0.6863f, 0.8588f, 1.f};
		double cube_arrow_offset = 18.66;	// 箭头偏移量


		std::string fn_vShader = "vertexShader_es11.txt";
		std::string fn_fShader = "fragmentShader_es11.txt";
		std::string fn_ground_obj = "ground.obj";
		std::string fn_ground_ktx = "ground.ktx";
		std::string fn_background = "background.ktx";
		std::string fn_environment_ktx = "environment.ktx";
		std::string fn_cube_arrow_obj = "arrow.obj";


		ESContext* esContext = nullptr;
		GLuint programObject;
		GLuint programObject_background;
		GLuint programObject_environment;
		GLuint programObject_triangle;
		GLuint textureId_logo = 0;		// logo 材质 id
		GLuint textureId_background = 0;		// 背景材质 id
		GLuint textureId_ground = 0;		// 地面材质
		std::vector<float> array_buf;
		std::vector<unsigned int> elem_buf;

		GLuint vboId_array = 0;		// 顶点数组缓冲区
		GLuint vboId_element = 0;	// 顶点元素缓冲区
		GLint defaultFbo = 0;
		GLuint framebuffer = 0;
		GLuint renderbuffer = 0;
		GLuint depthRenderbuffer = 0;
		GLuint MSAAFramebuffer = 0;
		GLuint MSAARenderbuffer = 0;
		GLuint MSAADepthRenderbuffer = 0;

		bool use_msaa = false;


		bool enable_polygon_offset = false;
		float polygon_offset_factor = -2.0f;
		float polygon_offset_units = -2.0f;


		// 画三角形的资源
		std::vector<float> triangle_vertex;

	protected:
		std::vector<infos_t> uniformInfos;
		std::map<std::string, int> uniformLoc;


		std::vector<infos_t> attribInfos;
		std::map<std::string, int> attribLoc;

		std::vector<model_info_t> model_infos;

		bool is_inited = false;
		bool is_paused = false;


		ModelGroup* arrow_group = nullptr;
		model_info_t* getModelInfoByName(std::string name);

	public:
	


		CubeScene();
		~CubeScene();

		// srcCube == nullptr 会重新加载，否则复制
		bool init(ESContext* esContext, const CubeModel* srcCube = nullptr);

		bool initVBO();	// 设置顶点缓冲区，并写入数据

		GLenum load_texture(std::string fn);
		void createFramebuffer();

		void update();
		
		void draw(ESContext* esContext);

		void draw_triangle(ESContext* esContext);

		// 获取统一变量信息
		void calUniformInfo();

		// 获取顶点变量信息
		void calAttribInfos();


		void set_default_filename(std::string path);

		// 释放所有 opengl 资源
		void glReleaseAll();


		// 释放所有 opengl 缓冲区资源: VBO
		void glReleaseVBO();

		// 释放所有 opengl 缓冲区资源: FBO
		void glReleaseFBO();


		bool isInited() const { return is_inited; };
		bool isPaused() const { return is_paused; };

		void onPause();
		void onResume();
		void onSurfaceChanged();


		// 设置是否显示箭头
		void showArrow(bool b);

		// 设置根据魔方姿态设置箭头中心的姿态
		void setArrowPoseAsCube();

		// 重设箭头旋转角度
		ModelGroup* getArrowGroup() const { return arrow_group; };




		const std::vector<infos_t>& getUniformInfos() const { return uniformInfos; };
		const std::map<std::string, int>& getUniformLoc() const { return uniformLoc; };


		const std::vector<infos_t>& getAttribInfos() const { return attribInfos; };
		const std::map<std::string, int>& getAttribLoc() const { return attribLoc; };

		const std::vector<model_info_t>& getModelInfos() const { return model_infos; };
	};


}

