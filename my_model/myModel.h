#pragma once

#include <vector>
#include <string>
#include "myMatrix.h"
#include "myQuaternion.h"
#include "myTransform.h"
#include <list>

namespace my
{
	struct material_t
	{
		fvec4 ambient_color;
		fvec4 diffuse_color;
		fvec4 specular_color;
		fvec4 emissive_color;
		float specular_exponent;


		static material_t default_material();


		// 获取所有颜色相同但比例不同的颜色
		static material_t colored_material(fvec4 color, float ambient_factor,
			float diffuse_factor, float specular_factor, float emissive_color, float specular_exponent);

	};


	class Model : public Coordinate
	{
	public:
		std::string name;

		std::vector<fvec3> vertex;			// 顶点坐标
		std::vector<fvec3> color;			// 顶点颜色, 可选
		std::vector<fvec3> normal;			// 顶点法线
		std::vector<fvec2> texture;			// 顶点材质坐标, 可选

		std::vector<ivec3> triangles;		// 三角形顶点下标

		material_t material;	// 材质, 可选

		bool en_color;	// 是否使用顶点颜色,当 color 为空时此选项无效
		bool en_texture;	// 是否使用材质,当 texture 为空时此选项无效

		



		Model();


		static Model gen_box(std::string name, vec3 org, fvec3 len);
		static Model gen_sphere(std::string name, vec3 org, fvec3 len, int numLongitude, int numLatitude);
		static Model gen_rectangle(std::string name, vec3 org, fvec2 len, int ndiv = 1);


		bool verify() const;

	};

	// 合并两个模型，简单把所有顶点转换成世界坐标等合并在一起，material 是 m1 的material, 不支持 texture
	Model merge_model(std::string name, const Model& m1, const Model& m2);

	Model merge_model(std::string name, const Model& m1, const Model& m2, const Model& m3);
	Model merge_model(std::string name, const Model& m1, const Model& m2, const Model& m3, const Model& m4);
	Model merge_model(std::string name, const Model& m1, const Model& m2, const Model& m3, const Model& m4, const Model& m5);


	// 读取 obj 文件, scale 是缩放比例
	Model load_obj_file(std::string model_name, std::string filename, float scale = 1.0f, bool isVerify = true);


	class ModelGroupIterator;

	// 模型组，拥有自己的坐标系，组内模型相对于此坐标
	class ModelGroup : public Coordinate
	{
	public:
		std::string name;


		std::vector<Model> models;		// 子模型
		std::vector<ModelGroup> groups;	// 子组


		ModelGroupIterator begin();

		size_t size() const { return models.size(); };

		friend class ModelGroupIterator;

	protected:
		// idx: 当前 group 相对于父组的下标
		void _begin(ModelGroupIterator& it, int idx);
	};


	class ModelGroupIterator
	{
	public:

		bool isEnd() const;

		void next();


		Model* getModel() const { return model; };


// 		// 获取坐标，如果模型更改，不会重新计算
// 		Coordinate getCoor() const { return coor; };

		// 计算模型相对于世界的坐标系
		Coordinate calCoor() const;


		friend class ModelGroup;

	protected:
		struct parents_t
		{
			int idx;
			ModelGroup* ptr;
		};

/*		void _calCoor();*/

	protected:
		int idx_model = -1;			// 当前模型下标
		Model* model = nullptr;		// 当前子模型, = group->models[idx]

		int idx_group = -1;				// 当前组下标(相对于父组)
		ModelGroup* group = nullptr;	// 当前组,即当前模型所处的组

		Coordinate coor = Coordinate::eye();	// 当前模型相对于世界的坐标
	
		std::list<parents_t> fatherList;		// 父组的栈，当前组不压栈
	

	};

}
