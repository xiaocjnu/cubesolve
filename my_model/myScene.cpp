#include "myScene.h"



namespace my
{

	light_t light_t::defaut_light()
	{
		light_t light;
		light.position = { 0,0,1,0};
		light.ambient_color = { 1,1,1,1 };
		light.diffuse_color = { 1,1,1,1 };
		light.specular_color = { 1,1,1,1 };
		light.spot_direction = { 0,0,-1 };
		light.attenuation_factors = { 1, 0, 0 };
		light.spot_exponent = 0;
		light.spot_cutoff_angle = 200.0f;
		light.compute_distance_attenuation = false;

		return light;
	}


	Scene::Scene()
	{
		ambient_color = vec4{ 0,0,0,0 };
		camera = Camera(60.0f);
	}


	Scene::~Scene()
	{
	}

	Model* Scene::findModel(std::string name)
	{
		Model* pm = nullptr;

		for (int i =0; i < (int)models.size(); ++i)
		{
			if (models[i].name == name)
			{
				pm = &(models[i]);
				return pm;
			}
		}

		return pm;
	}

}



