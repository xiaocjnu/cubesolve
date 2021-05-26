#pragma once

#include "myMatrix.h"
#include "myModel.h"
#include "myCamera.h"
#include <vector>




#define MAX_NUM_LIGHTS 8

namespace my
{
	struct light_t
	{
		vec4 position; 	// light position for a point/spotlight or normalized dir. for a directional light
		fvec4 ambient_color;
		fvec4 diffuse_color;
		fvec4 specular_color;
		fvec3 spot_direction;
		fvec3 attenuation_factors;
		float spot_exponent;
		float spot_cutoff_angle;
		bool compute_distance_attenuation;


		static light_t defaut_light();
	};

	class Scene
	{
	public:
		std::vector<Model> models;
		std::vector<light_t> lights;
		vec4 ambient_color;
		Camera camera;
		

		// 根据名字查找模型，没有返回 nullptr, 名字重复返回找到的第一个
		Model* findModel(std::string name);


		Scene();
		~Scene();
	};
}



