#include "myCubeScene.h"
#include "myShader.h"
#include "ktx.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>


#ifdef min
#undef min
#endif


using namespace std;

namespace my
{
	static void _glUniform_fvec(GLint location, fvec4 a)
	{
		glUniform4f(location, a[0], a[1], a[2], a[3]);
	}

	static void _glUniform_fvec(GLint location, fvec3 a)
	{
		glUniform3f(location, a[0], a[1], a[2]);
	}


	CubeScene::model_info_t * CubeScene::getModelInfoByName(std::string name)
	{
		model_info_t* info = nullptr;

		for (model_info_t& _info : model_infos)
		{
			if (_info.name == name)
			{
				info = &_info;
				break;
			}
		}


		return info;
	}



	CubeScene::CubeScene()
	{
	}


	CubeScene::~CubeScene()
	{
		glReleaseAll();
	}


	static string _load_txt(string fn)
	{
		ifstream file(fn);

		stringstream buffer;
		buffer << file.rdbuf();

		return buffer.str();
	}

	bool CubeScene::init(ESContext* esContext, const CubeModel* srcCube)
	{

		this->esContext = esContext;

		//cube.set_defaut_filename();

		if (srcCube == nullptr)
			cube.load_model();
		else
			cube = *srcCube;

#ifdef WIN32

// 		Model ground = load_obj_file("ground", fn_ground_obj);
// 		ground.setPos(0, 0, -60);
// 		ground.en_texture = true;
// 		modelgroups.models.push_back(ground);
#endif

		Model arrow = load_obj_file("arrow", fn_cube_arrow_obj);
		arrow.material = material_t::colored_material(cube_arrow_color, 0.8f, 1.0f, 0.0f, 0.0f, 20.0f);
		ModelGroup arrow_group;
		arrow_group.name = "arrow_group";
		arrow_group.models.push_back(arrow);
		arrow_group.setPos(0, 0, cube_arrow_offset);
		modelgroups.groups.push_back(arrow_group);
		this->arrow_group = &(*(modelgroups.groups.rbegin()));


		if (is_draw_test_box)
		{
			float _len = 5.0f;
			float _x = 35.0f;
			float _z = -32.0f;

			Model box1 = Model::gen_box("test_box1", { _x,_x,_z }, { _len,_len,_len });
			Model box2 = Model::gen_box("test_box1", { _x,-_x,_z }, { _len,_len,_len });
			Model box3 = Model::gen_box("test_box1", { -_x,-_x,_z }, { _len,_len,_len });
			Model box4 = Model::gen_box("test_box1", { -_x,_x,_z }, { _len,_len,_len });

			box1.material = material_t::colored_material({ 1,0,0,1 }, 1, 1, 0.3f, 0, 20);
			box2.material = material_t::colored_material({ 0,1,0,1 }, 1, 1, 0.3f, 0, 20);
			box3.material = material_t::colored_material({ 0,0,1,1 }, 1, 1, 0.3f, 0, 20);
			box4.material = material_t::colored_material({ 1,0.5f,0.5f,1 }, 1, 1, 0.3f, 0, 20);

			modelgroups.models.push_back(box1);
			modelgroups.models.push_back(box2);
			modelgroups.models.push_back(box3);
			modelgroups.models.push_back(box4);
		}


		textureId_logo = load_texture(cube.fn_logo);
		textureId_background = load_texture(fn_background);
		textureId_ground = load_texture(fn_ground_ktx);


		if (!initVBO())	// 初始化顶点缓冲区
			return false;
		
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFbo);

		if (use_msaa)
			createFramebuffer();


		int rc = glGetError();

		// 加载 shader
		string vShaderStr = vertexShader_es11; //_load_txt("vertexShader_es11.txt");
		string fShaderStr = fragmentShader_es11; //_load_txt("fragmentShader_es11.txt");
		programObject = esLoadProgram(vShaderStr.c_str(), fShaderStr.c_str());


		rc = glGetError();
		if (rc != GL_NO_ERROR)
			return false;


		if (programObject == 0)
			return false;


		char bg_vshader[] =
			"#version 300 es                          \n"
			"layout(location = 0) in vec4 vPosition;  \n"
			"layout(location = 1) in vec2 a_texcoord; \n"
			"out vec2 v_texcoord;                     \n"
			"void main()                              \n"
			"{                                        \n"
			"   gl_Position = vPosition;              \n"
			"   v_texcoord = a_texcoord;              \n"
			"}                                        \n";

		char bg_fshader[] =
			"#version 300 es                              \n"
			"uniform sampler2D sampler;                   \n"
			"precision mediump float;                     \n"
			"in vec2 v_texcoord;                          \n"
			"out vec4 fragColor;                          \n"
			"void main()                                  \n"
			"{                                            \n"
			"   fragColor = texture(sampler, v_texcoord); \n"
			"}                                            \n";
		programObject_background = esLoadProgram(bg_vshader, bg_fshader);

		rc = glGetError();
		if (rc != GL_NO_ERROR)
			return false;


		if (programObject_background == 0)
			return false;


		// 		char env_vshader[] =
		// 			"#version 300 es                          \n"
		// 			"layout(location = 0) in vec3 a_texcoord; \n"
		// 			"out vec3 v_texcoord;                     \n"
		// 			"void main()                              \n"
		// 			"{                                        \n"
		// 			"   gl_Position = vPosition;              \n"
		// 			"   v_texcoord = a_texcoord;              \n"
		// 			"}                                        \n";
		// 		char env_fshader[] =
		// 			"#version 300 es                              \n"
		// 			"uniform samplerCube sampler;                 \n"
		// 			"precision mediump float;                     \n"
		// 			"in vec3 v_texcoord;                          \n"
		// 			"out vec4 fragColor;                          \n"
		// 			"void main()                                  \n"
		// 			"{                                            \n"
		// 			"   fragColor = texture(sampler, v_texcoord); \n"
		// 			"}                                            \n";
		// 		programObject_environment = esLoadProgram(env_vshader, env_fshader);


		char tri_vshader[] =
			"#version 300 es                          \n"
			"layout(location = 0) in vec4 vPosition;  \n"
			"void main()                              \n"
			"{                                        \n"
			"   gl_Position = vPosition;              \n"
			"}                                        \n";

		char tri_fshader[] =
			"#version 300 es                              \n"
			"precision mediump float;                     \n"
			"out vec4 fragColor;                          \n"
			"void main()                                  \n"
			"{                                            \n"
			"   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
			"}                                            \n";


		programObject_triangle = esLoadProgram(tri_vshader, tri_fshader);


		rc = glGetError();
		if (rc != GL_NO_ERROR)
			return false;


		if (programObject_triangle == 0)
			return false;


		calUniformInfo();
		calAttribInfos();


// 		if (use_msaa)
// 		{
// 			createFramebuffer();   // ios 暂时有问题
// 		}

		// 点光
		light_t light0 = light_t::defaut_light();
		//light0.position = normalize(vec4{ 1, -1, 1.0f, 0 });
		light0.position = vec4{ 200.0f, -200.0f, 250.0f, 1.0f };


		fvec4 light0_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		light0.ambient_color = 0.2f*light0_color;
		light0.diffuse_color = 0.6f*light0_color;
		light0.specular_color = 0.6f*light0_color;
// 
// 		light0.ambient_color = 0.2f*light0_color;
// 		light0.diffuse_color = 0.0f*light0_color;
// 		light0.specular_color = 0.0f*light0_color;


		light0.ambient_color[3] = 1.0f;
		light0.diffuse_color[3] = 1.0f;
		light0.specular_color[3] = 1.0f;
		light0.attenuation_factors = { 1.0f, 0.002f, 0.00001f };
		light0.compute_distance_attenuation = false;
		



		light_t light1 = light0;
		//light1.position = normalize(vec4{ -1, 1, -1.0f, 0 });
		light1.position = -1.0*light0.position;
		light1.position[3] = 1.0f;

		lights.push_back(light0);
		lights.push_back(light1);


		ambient_color = fvec4{ 0.3f, 0.3f, 0.3f, 1.0f };

		// 相机
		Camera cam(38.0f);
		cam.rotate(60, 0, 30);
		cam.translateLocal(vec3{ 0,0,200 });
		cam_nearZ = 20;
		cam_farZ = 1000;

		//cam.setPos(vec3{ 0, -10, 3 });
		//cam.setPos(vec3{ 0,0,10 });


		camera = cam;


		is_inited = true;

		rc = glGetError();
		if (rc != GL_NO_ERROR)
			return false;

		return true;
	}

	bool CubeScene::initVBO()
	{

// 		vector<float> array_buf;
// 		vector<unsigned int> elem_buf;

		array_buf.clear();
		elem_buf.clear();

		// 解析模型
		vector<ModelGroup*> gg = { &cube, &modelgroups };

		for (ModelGroup* g : gg)
		{
			for (ModelGroupIterator it = g->begin(); !it.isEnd(); it.next())
			{
				Model& model = *(it.getModel());
				Coordinate coor = it.calCoor();

				if (model.vertex.size() == 0 || model.triangles.size() == 0)
					continue;

				model_info_t info;
				info.name = model.name;
				info.is_enable = true;
				info.it = it;
				info.material = model.material;
				info.array_off = array_buf.size() * sizeof(float);
				info.elem_off = elem_buf.size() * sizeof(unsigned int);
				info.num_elem = model.triangles.size();
				info.use_tex = model.en_texture && model.texture.size() == model.vertex.size();

				model_infos.push_back(info);

				// 顶点和法线
				vector<vec4> vertex(model.vertex.size());
				vector<vec3> normal(model.normal.size());


				for (int j = 0; j < (int)vertex.size(); ++j)
				{
					fvec4 v = vec3to4(model.vertex[j], 1.0f);
					fvec3 n = model.normal[j];

					array_buf.push_back(v[0]);
					array_buf.push_back(v[1]);
					array_buf.push_back(v[2]);
					array_buf.push_back(v[3]);

					array_buf.push_back(n[0]);
					array_buf.push_back(n[1]);
					array_buf.push_back(n[2]);


					fvec2 t;
					if (info.use_tex)	// 有材质
					{
						t = model.texture[j];

						array_buf.push_back(t[0]);
						array_buf.push_back(t[1]);
					}
				}

				// triangles

				for (int j = 0; j < (int)model.triangles.size(); ++j)
				{
					ivec3 n = model.triangles[j];

					elem_buf.push_back(n[0]);
					elem_buf.push_back(n[1]);
					elem_buf.push_back(n[2]);
				}
			}// for it
		}// for g


		int rc = glGetError();

		// 生成缓冲
		glGenBuffers(1, &vboId_array);
		glGenBuffers(1, &vboId_element);


		// 顶点
		glBindBuffer(GL_ARRAY_BUFFER, vboId_array);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*array_buf.size(), &array_buf[0], GL_STATIC_DRAW);


		// 元素
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId_element);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*elem_buf.size(), &elem_buf[0], GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		rc = glGetError();


		if (rc != GL_NO_ERROR)
			return false;



		return true;
		
	}

	GLenum CubeScene::load_texture(std::string fn)
	{
		GLuint textureId;

		// 2x2 Image, 3 bytes per pixel (R, G, B)
		GLubyte pixels[4 * 3] =
		{
		   255,   0,   0, // Red
			 0, 255,   0, // Green
			 0,   0, 255, // Blue
		   255, 255,   0  // Yellow
		};

		// Use tightly packed data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Generate a texture object
		glGenTextures(1, &textureId);

		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Load the texture
		KTX_dimensions dim;
		GLboolean isMip;
		GLenum err;
		GLenum target;
		unsigned int kvdLen = 0;
		unsigned char ** ppKvd = nullptr;

		ktxLoadTextureN(fn.c_str(), &textureId, &target, &dim, &isMip, &err, &kvdLen, ppKvd);

		// Set the filtering mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return textureId;

	}

	void CubeScene::createFramebuffer()
	{
		// 		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// 		GLint params;
		// 		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &params);

				// Create the framebuffer and bind it so that future OpenGL ES framebuffer commands are directed to it.
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer.
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

		// Create the color renderbuffer and call the rendering context to allocate the storage on our Core Animation layer.
		// The width, height, and format of the renderbuffer storage are derived from the bounds and properties of the CAEAGLLayer object
		// at the moment the renderbufferStorage:fromDrawable: method is called.

		//[mContext renderbufferStorage : GL_RENDERBUFFER fromDrawable : self];


		//glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &params);
		GLenum color_format = GL_RGBA8;
		GLenum depth_format = GL_DEPTH_COMPONENT16;
		int num_samples = 4;


		glRenderbufferStorage(GL_RENDERBUFFER, color_format, esContext->width, esContext->height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

		//glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &params);

		// Retrieve the height and width of the color renderbuffer.
		int width, height;
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

		// Perform similar steps to create and attach a depth renderbuffer.
		glGenRenderbuffers(1, &depthRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, depth_format, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

		// The following is MSAA settings
		glGenFramebuffers(1, &MSAAFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, MSAAFramebuffer);

		glGenRenderbuffers(1, &MSAARenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, MSAARenderbuffer);


		// 4 samples for color
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, num_samples, color_format, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, MSAARenderbuffer);

		glGenRenderbuffers(1, &MSAADepthRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, MSAADepthRenderbuffer);
		// 4 samples for depth
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, num_samples, depth_format, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, MSAADepthRenderbuffer);

		// Test the framebuffer for completeness.
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "CubeScene:createFramebuffer: glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE";
			return;
			//return false;
		}

		// glViewport(0, 0, mBackingWidth, mBackingHeight);

		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Do other settings...

	}

	void CubeScene::update()
	{
	}

	void CubeScene::calUniformInfo()
	{
		// 获取 Uniform 信息
		int numUnifroms;
		glGetProgramiv(programObject, GL_ACTIVE_UNIFORMS, &numUnifroms);

		uniformInfos.resize(numUnifroms);

		for (int i = 0; i < numUnifroms; ++i)
		{
			glGetActiveUniform(programObject, i, sizeof(infos_t::name),
				&(uniformInfos[i].length), &(uniformInfos[i].size), &(uniformInfos[i].type), uniformInfos[i].name);
			uniformInfos[i].loc = glGetUniformLocation(programObject, uniformInfos[i].name);
			uniformLoc.insert({ uniformInfos[i].name, uniformInfos[i].loc });
		}
	}

	void CubeScene::calAttribInfos()
	{
		// 获取 Vertex Attributes 信息
		int numAttrib;
		glGetProgramiv(programObject, GL_ACTIVE_ATTRIBUTES, &numAttrib);

		attribInfos.resize(numAttrib);

		for (int i = 0; i < numAttrib; ++i)
		{
			glGetActiveAttrib(programObject, i, sizeof(infos_t::name),
				&(attribInfos[i].length), &(attribInfos[i].size), &(attribInfos[i].type), attribInfos[i].name);
			attribInfos[i].loc = glGetAttribLocation(programObject, attribInfos[i].name);

			attribLoc.insert({ attribInfos[i].name, attribInfos[i].loc });
		}
	}

	void CubeScene::set_default_filename(std::string path)
	{
		fn_vShader = path + fn_vShader;
		fn_fShader = path + fn_fShader;
		fn_ground_obj = path + fn_ground_obj;
		fn_ground_ktx = path + fn_ground_ktx;
		fn_background = path + fn_background;
		fn_environment_ktx = path + fn_environment_ktx;
		fn_cube_arrow_obj = path + fn_cube_arrow_obj;
	}

	void CubeScene::glReleaseAll()
	{
		if (programObject)
		{
			glDeleteProgram(programObject);
			programObject = 0;
		}

		if (programObject_background)
		{
			glDeleteProgram(programObject_background);
			programObject_background = 0;
		}

		if (programObject_environment)
		{
			glDeleteProgram(programObject_environment);
			programObject_environment = 0;
		}

		if (textureId_logo)
		{
			glDeleteTextures(1, &textureId_logo);
			textureId_logo = 0;
		}

		if (textureId_background)
		{
			glDeleteTextures(1, &textureId_background);
			textureId_background = 0;
		}

		if (textureId_ground)
		{
			glDeleteTextures(1, &textureId_ground);
			textureId_ground = 0;
		}

		glReleaseVBO();
		glReleaseFBO();
	}

	void CubeScene::glReleaseVBO()
	{
		if (vboId_array)
		{
			glDeleteBuffers(1, &vboId_array);
			vboId_array = 0;
		}

		if (vboId_element)
		{
			glDeleteBuffers(1, &vboId_element);
			vboId_element = 0;
		}
	}

	void CubeScene::glReleaseFBO()
	{


		if (framebuffer)
		{
			glDeleteFramebuffers(1, &framebuffer);
			framebuffer = 0;
		}

		if (renderbuffer)
		{
			glDeleteFramebuffers(1, &renderbuffer);
			renderbuffer = 0;
		}

		if (depthRenderbuffer)
		{
			glDeleteFramebuffers(1, &depthRenderbuffer);
			depthRenderbuffer = 0;
		}

		if (MSAAFramebuffer)
		{
			glDeleteFramebuffers(1, &MSAAFramebuffer);
			MSAAFramebuffer = 0;
		}

		if (MSAARenderbuffer)
		{
			glDeleteFramebuffers(1, &MSAARenderbuffer);
			MSAARenderbuffer = 0;
		}

		if (MSAADepthRenderbuffer)
		{
			glDeleteFramebuffers(1, &MSAADepthRenderbuffer);
			MSAADepthRenderbuffer = 0;
		}
	}

	void CubeScene::onPause()
	{
// 		if (!is_inited)
// 			return;
// 
// 		glReleaseFBO();
// 		is_paused = true;

	}

	void CubeScene::onResume()
	{
// 		if (!is_inited)
// 			return;
// 
// 		if (is_paused && vboId_array == 0)
// 		{
// 			// Make the context current
// 			int rc = eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
// 				esContext->eglSurface, esContext->eglContext);
// 
// 
// 			//initVBO();
// 
// 			if (use_msaa)
// 				createFramebuffer();
// 		}
// 
// 		is_paused = false;
	}

	void CubeScene::onSurfaceChanged()
	{
        if (use_msaa)
        {
            glReleaseFBO();
            createFramebuffer();
        }
	}

	void CubeScene::showArrow(bool b)
	{
		model_info_t* info = getModelInfoByName("arrow");
		

		if (info)
		{
			info->is_enable = b;
		}

	}

	void CubeScene::setArrowPoseAsCube()
	{
		if (arrow_group)
		{
			arrow_group->setPos(cube.getPos());
			arrow_group->setQuaternion(cube.getQuaternion());
		}
	}


	void CubeScene::draw(ESContext* esContext)
	{
		if (!is_inited || is_paused)
			return;


		GLenum rc = glGetError();

		if (use_msaa)
			glBindFramebuffer(GL_FRAMEBUFFER, MSAAFramebuffer);
		else
		{
#ifndef __APPLE__
			glBindFramebuffer(GL_FRAMEBUFFER, defaultFbo);
#endif
		}
        
			
		rc = glGetError();
		// 		glBindRenderbuffer(GL_RENDERBUFFER, MSAARenderbuffer);

			//	glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Clear the color buffer
		glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
		glClearDepthf(1.0);

        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

		//glClearDepthf(1.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);

		if (enable_polygon_offset)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(polygon_offset_factor, polygon_offset_units);
		}
		else
			glDisable(GL_POLYGON_OFFSET_FILL);

		// 		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		// 		glEnable(GL_SAMPLE_COVERAGE);
		// 		glEnable(GL_STENCIL_TEST);
		// 		glEnable(GL_DITHER);
		// 		glEnable(GL_BLEND);
		// 
		// 		glSampleCoverage(0.5, false);

		

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);

		if (enable_environment) // 画环境
		{

		}
		else // 画背景
		{
			// Use the program object

			// TODO: 使用局部变量有问题
// 			const float bg_vertex[] =
// 			{
// 				1, 1, 0.999f,
// 				-1, 1, 0.999f,
// 				-1, -1, 0.999f,
// 				1, -1, 0.999f
// 			};
// 
// 			const float bg_texcoord[] =
// 			{
// 				1,0,
// 				0,0,
// 				0,1,
// 				1,1
// 			};
// 
// 			glUseProgram(programObject_background);
// 			//glUniform1i(0, 0);	// sampler
// 
// 			glBindBuffer(GL_ARRAY_BUFFER, 0);
// 			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// 
// 			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &(bg_vertex));
// 			glEnableVertexAttribArray(0);
// 
// 			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, &(bg_texcoord));
// 			glEnableVertexAttribArray(1);
// 
// 			glActiveTexture(GL_TEXTURE0);
// 			glBindTexture(GL_TEXTURE_2D, textureId_background);


			//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		// 画模型
		// Use the program object
		glUseProgram(programObject);


		// 设置 Uniform

		// mvp_matrix, modelview_matrix, inv_transpose_modelview_matrix
		double aspect = (double)esContext->width / (double)esContext->height;

		mat4 viewMatrix = camera.getViewMatrix();
		mat4 projMatrix = camera.getProjMatrix(aspect, cam_nearZ, cam_farZ);



		map<int, int> mm;

		int a = 1;
		auto b = mm.find((const map<int, int>::key_type&)(a));

		// texture matrices not assigned
	// 	glUniformMatrix4fv(uniformLocMap["tex_matrix[0]"], 1, p);
	// 	glUniformMatrix4fv(uniformLocMap["tex_matrix[1]"], 1, p);

		// enable_tex
		glUniform1i(uniformLoc.find(string("enable_tex[0]"))->second, true);
		//glUniform1i(uniformLoc.find("enable_tex[1]")->second, false);


		// enable_tex_matrix
		glUniform1i(uniformLoc.find("enable_tex_matrix[0]")->second, false);
		//glUniform1i(uniformLoc.find("enable_tex_matrix[1]")->second, false);

		// ambient_scene_color
		_glUniform_fvec(uniformLoc.find("ambient_scene_color")->second, ambient_color);


		// light_state, light_enable_state

		
		int light_en[MAX_NUM_LIGHTS];
		int num_lights = std::min(MAX_NUM_LIGHTS, (int)lights.size());
		for (int i = 0; i < num_lights; ++i)
		{
			light_t& light = lights[i];
			light_en[i] = 1;

			string name = string("light_state[") + to_string(i) + "]";
			string name_en = string("light_enable_state[") + to_string(i) + "]";

			//glUniform1i(uniformLoc.find(name_en)->second, true);

			fvec4 light_pos = mat_d2f(viewMatrix * light.position);

 			_glUniform_fvec(uniformLoc.find(name + ".position")->second, light_pos);
 			_glUniform_fvec(uniformLoc.find(name + ".ambient_color")->second, light.ambient_color);
			_glUniform_fvec(uniformLoc.find(name + ".diffuse_color")->second, light.diffuse_color);
			_glUniform_fvec(uniformLoc.find(name + ".specular_color")->second, light.specular_color);
			_glUniform_fvec(uniformLoc.find(name + ".spot_direction")->second, light.spot_direction);
			_glUniform_fvec(uniformLoc.find(name + ".attenuation_factors")->second, light.attenuation_factors);
			glUniform1f(uniformLoc.find(name + ".spot_exponent")->second, light.spot_exponent);
			glUniform1f(uniformLoc.find(name + ".spot_cutoff_angle")->second, light.spot_cutoff_angle);
			glUniform1i(uniformLoc.find(name + ".compute_distance_attenuation")->second, light.compute_distance_attenuation);
		}

		for (int i = num_lights; i < MAX_NUM_LIGHTS; ++i)
		{
			light_en[i] = 0;
		}

		//glUniform1i(uniformLoc.find("light_enable_state[0]")->second, (1<<num_lights)-1);

// 		glUniform1i(uniformLoc.find("light_enable_state[0]")->second, true);
// 		glUniform1i(uniformLoc.find("light_enable_state[0]")->second + 1, true);

		//glUniform1iv(uniformLoc.find("light_enable_state[0]")->second, num_lights, light_en);

		// num_lights
		glUniform1i(uniformLoc.find("num_lights")->second, num_lights);

		// enable_lighting
		if (num_lights > 0)
			glUniform1i(uniformLoc.find("enable_lighting")->second, true);
		else
			glUniform1i(uniformLoc.find("enable_lighting")->second, false);

		// others
		glUniform1i(uniformLoc.find("light_model_two_sided")->second, false);
		glUniform1i(uniformLoc.find("enable_color_material")->second, false);
		glUniform1i(uniformLoc.find("enable_fog")->second, false);


		//glUniform1f(uniformLocMap["fog_density"], 0.0f);
		//glUniform1f(uniformLocMap["fog_start"], 0.0f);
		//glUniform1f(uniformLocMap["fog_end"], 0.0f);
		//glUniform1i(uniformLocMap["fog_mode"], 0.0f);

		glUniform1i(uniformLoc.find("xform_eye_p")->second, true);
		glUniform1i(uniformLoc.find("rescale_normal")->second, false);
		glUniform1i(uniformLoc.find("normalize_normal")->second, true);

		glUniform1f(uniformLoc.find("rescale_normal_factor")->second, 1.0f);

		//glUniform4fv(uniformLocMap["ucp_eqn"], 1, nullptr);
		glUniform1i(uniformLoc.find("enable_ucp")->second, false);


		glUniform1i(uniformLoc.find("sampler")->second, 0);


		// 画模型
		//for (ModelGroupIterator it = cube.begin(); !it.isEnd(); it.next())



		glBindBuffer(GL_ARRAY_BUFFER, vboId_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId_element);



		for (model_info_t& info : model_infos)
		{
			if (!info.is_enable)
				continue;

			Model& model = *(info.it.getModel());
			Coordinate coor = info.it.calCoor();


			if (model.vertex.size() == 0 || model.triangles.size() == 0)
				continue;


			// mvp

			mat4 modelMatrix = coor.getTransfrom();
			mat4 mv = viewMatrix * modelMatrix;
			fmat4 fmv = mat_d2f(mv);
			fmat4 mvp = mat_d2f(projMatrix * mv);
			fmat3 inv_transpose_mv = mat_d2f(inv(submat3(mv).transpose()));

			info.mvp = mvp;
			info.fmv = fmv;
			info.inv_transpose_mv = inv_transpose_mv;

			glUniformMatrix4fv(uniformLoc.find("mvp_matrix")->second, 1, GL_FALSE, &(info.mvp(0, 0)));
			glUniformMatrix4fv(uniformLoc.find("modelview_matrix")->second, 1, GL_FALSE, &(info.fmv(0, 0)));
			glUniformMatrix3fv(uniformLoc.find("inv_transpose_modelview_matrix")->second, 1, GL_FALSE, &(info.inv_transpose_mv(0, 0)));

			// material_state uniform
			_glUniform_fvec(uniformLoc.find("material_state.ambient_color")->second, model.material.ambient_color);
			_glUniform_fvec(uniformLoc.find("material_state.diffuse_color")->second, model.material.diffuse_color);
			_glUniform_fvec(uniformLoc.find("material_state.specular_color")->second, model.material.specular_color);
			_glUniform_fvec(uniformLoc.find("material_state.emissive_color")->second, model.material.emissive_color);
			glUniform1f(uniformLoc.find("material_state.specular_exponent")->second, model.material.specular_exponent);



			int stride;
			if (info.use_tex)
				stride = 9 * sizeof(float);		// 4 + 3 + 2
			else
				stride = 7 * sizeof(float);		// 4 + 3



			glVertexAttribPointer(attribLoc.find("a_position")->second, 3, GL_FLOAT, GL_FALSE, stride,
				(const void*)info.array_off);


			glEnableVertexAttribArray(attribLoc.find("a_position")->second);


			// 		in vec4 a_texcoord0; // available if enable_tex[0] is true
			// 		in vec4 a_texcoordl; // available if enable_tex[1] is true


			size_t off1 = 4 * sizeof(float);

			// 		glVertexAttrib4fv(attribLoc.find("a_color")->second, &(model.material.diffuse_color[0]));
			// 		glDisableVertexAttribArray(attribLoc.find("a_color")->second);

					//glVertexAttribPointer(attribLoc.find("a_normal")->second, 3, GL_FLOAT, GL_FALSE, 0, &(normal[0]));


			glVertexAttribPointer(attribLoc.find("a_normal")->second, 3, GL_FLOAT, GL_FALSE, stride,
					(const void*)(info.array_off + off1));

			glEnableVertexAttribArray(attribLoc.find("a_normal")->second);



			if (model.en_texture && model.texture.size() > 0)
			{
				glUniform1i(uniformLoc.find("enable_tex[0]")->second, true);

				glActiveTexture(GL_TEXTURE0);


				if (model.name == "ground")
					glBindTexture(GL_TEXTURE_2D, textureId_ground);
				else
					glBindTexture(GL_TEXTURE_2D, textureId_logo);


				off1 += 3 * sizeof(float);

				glVertexAttribPointer(attribLoc.find("a_texcoord0")->second, 2, GL_FLOAT, GL_FALSE, stride,
						(const void*)(info.array_off + off1));

				
				glEnableVertexAttribArray(attribLoc.find("a_texcoord0")->second);
			}
			else
			{
				glUniform1i(uniformLoc.find("enable_tex[0]")->second, false);
				glDisableVertexAttribArray(attribLoc.find("a_texcoord0")->second);

			}



			// 分次画
			const int max_triangles_per_time = 10000;
			int tri_offset = 0;

			while (tri_offset < (int)model.triangles.size()) {
				size_t elem_off = info.elem_off + tri_offset * sizeof(unsigned int) * 3;
				int count = std::min(max_triangles_per_time, (int)model.triangles.size() - tri_offset);


				// 画
				glDrawElements(GL_TRIANGLES, 3 * count, GL_UNSIGNED_INT,
						(const void *)elem_off);

				tri_offset += count;

			}

           
		}

		if (!use_msaa)
        {
            return;
        }



		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFbo);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, MSAAFramebuffer);

		// OpenGL ES3.0 Core multisampling

		// Discard the depth buffer from the read fbo. It is no more necessary.
		GLenum tmp = GL_DEPTH_ATTACHMENT;
		glInvalidateFramebuffer(GL_READ_FRAMEBUFFER, 1, &tmp);

		// Copy the read fbo(multisampled framebuffer) to the draw fbo(single-sampled framebuffer)
		glBlitFramebuffer(0, 0, esContext->width, esContext->height, 0, 0, esContext->width, esContext->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		tmp = GL_COLOR_ATTACHMENT0;
		glInvalidateFramebuffer(GL_READ_FRAMEBUFFER, 1, &tmp);
		// 
		// 	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		// 
		// 	//glPresentRenderbuffer
		// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		// 	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		// 	glBlitFramebuffer(0, 0, esContext->width, esContext->height, 0, 0, esContext->width, esContext->height,
		// 		GL_COLOR_BUFFER_BIT, GL_NEAREST);



		//glFinish();

	}


	void CubeScene::draw_triangle(ESContext *esContext)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFbo);


		triangle_vertex = { 0.0f,  0.5f, 0.0f,
								 -0.5f, -0.5f, 0.0f,
								 0.5f, -0.5f, 0.0f
		};


		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);

		// Clear the color buffer
		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the program object
		glUseProgram(programObject_triangle);

		// Load the vertex data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)&(triangle_vertex[0]));

		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

}


