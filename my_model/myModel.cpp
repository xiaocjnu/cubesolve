#include "myModel.h"
#include <assert.h>
#include <fstream>
#include <vector>
#include <map>

using namespace std;


namespace my {


    material_t material_t::default_material() {
        material_t material;

        material.ambient_color = {0.5f, 0.5f, 0.5f, 1.0f};
        material.diffuse_color = {0.5f, 0.5f, 0.5f, 1.0f};
        material.specular_color = {0.1f, 0.1f, 0.1f, 1.0f};
        material.emissive_color = {0.0f, 0.0f, 0.0f, 1.0f};
        material.specular_exponent = 20.0f;

        return material;
    }

    my::material_t material_t::colored_material(fvec4 color, float ambient_factor,
                                                float diffuse_factor, float specular_factor,
                                                float emissive_factor, float specular_exponent) {
        material_t material;

        material.ambient_color = ambient_factor * color;
        material.diffuse_color = diffuse_factor * color;
        material.specular_color = specular_factor * color;
        material.emissive_color = emissive_factor * color;

        material.ambient_color[3] = color[3];
        material.diffuse_color[3] = color[3];
        material.specular_color[3] = color[3];
        material.emissive_color[3] = color[3];

        material.specular_exponent = specular_exponent;

        return material;
    }

    Model::Model()
            : en_color(false), en_texture(false) {
        material = material_t::default_material();
    }


    Model Model::gen_box(string name, vec3 org, fvec3 len) {
        Model mdl;
        mdl.name = name;
        mdl.setPos(org);

        // 基本点坐标
        vector<fvec3> base_vertex(8);

        base_vertex[0] = fvec3{0.5f * len[0], 0.5f * len[1], 0.5f * len[2]};
        base_vertex[1] = fvec3{-0.5f * len[0], 0.5f * len[1], 0.5f * len[2]};
        base_vertex[2] = fvec3{-0.5f * len[0], -0.5f * len[1], 0.5f * len[2]};
        base_vertex[3] = fvec3{0.5f * len[0], -0.5f * len[1], 0.5f * len[2]};

        for (int i = 4; i < 8; ++i) {
            base_vertex[i][0] = base_vertex[i - 4][0];
            base_vertex[i][1] = base_vertex[i - 4][1];
            base_vertex[i][2] = -base_vertex[i - 4][2];
        }

        vector<ivec3> base_triangles
                {
                        // face 0 1 2 3
                        ivec3{0, 1, 2},
                        ivec3{0, 2, 3},

                        // face 0 4 5 1
                        ivec3{0, 4, 5},
                        ivec3{0, 5, 1},

                        // face 0 3 7 4
                        ivec3{0, 3, 7},
                        ivec3{0, 7, 4},

                        // face 6 5 4 7
                        ivec3{6, 5, 4},
                        ivec3{6, 4, 7},

                        // face 6 7 3 2
                        ivec3{6, 7, 3},
                        ivec3{6, 3, 2},

                        // face 6 2 1 5
                        ivec3{6, 2, 1},
                        ivec3{6, 1, 5}
                };


        // 生成节点和三角形下标
        mdl.triangles.resize(12);
        mdl.vertex.resize(6 * 4);
        mdl.texture.resize(6 * 4);
        mdl.normal.resize(6 * 4);

        int idx[] = {0, 1, 2, 3};

        for (int i = 0; i < 12; i += 2) {
            mdl.vertex[idx[0]] = base_vertex[base_triangles[i][0]];
            mdl.vertex[idx[1]] = base_vertex[base_triangles[i][1]];
            mdl.vertex[idx[2]] = base_vertex[base_triangles[i][2]];
            mdl.vertex[idx[3]] = base_vertex[base_triangles[i + 1][2]];


            fvec3 normal = normalize(cross(mdl.vertex[idx[1]] - mdl.vertex[idx[0]],
                                          mdl.vertex[idx[2]] - mdl.vertex[idx[0]]));

            mdl.texture[idx[0]] = {0.0f, 0.0f};
            mdl.texture[idx[1]] = {0.0f, 1.0f};
            mdl.texture[idx[2]] = {1.0f, 1.0f};
            mdl.texture[idx[3]] = {1.0f, 0.0};

            mdl.normal[idx[0]] = normal;
            mdl.normal[idx[1]] = normal;
            mdl.normal[idx[2]] = normal;
            mdl.normal[idx[3]] = normal;

            mdl.triangles[i] = {idx[0], idx[1], idx[2]};
            mdl.triangles[i + 1] = {idx[0], idx[2], idx[3]};


            for (int j = 0; j < 4; ++j) {
                idx[j] += 4;
            }
        }


        return mdl;

    }


    Model Model::gen_sphere(string name, vec3 org, fvec3 len, int numLongitude, int numLatitude) {
        Model mdl;
        mdl.name = name;

        if (numLongitude < 3 || numLatitude < 3)
            return mdl;


        mdl.setPos(org);


        double rx = len[0] / 2.0;
		double ry = len[1] / 2.0;
		double rz = len[2] / 2.0;


        // 生成经纬度
        vector<double> longitude(numLongitude);    // 经度
        vector<double> latitude(numLatitude);    // 纬度

        for (int i = 0; i < numLongitude; ++i) {
            longitude[i] = 360.0 * i / numLongitude;
        }

        for (int i = 0; i < numLatitude; ++i) {
            latitude[i] = -90.0 + 180.0 * i / numLatitude;
        }

        // 生成顶点
        // 非极地顶点下标 = (i - 1) * numLongitude + j, i 是纬度下标， j 是经度下标
        // 南极顶点下标 vertex.size()-2,  北极顶点下标 vertex.size()-1,

        mdl.vertex.resize(numLongitude * (numLatitude - 2) + 2);

        int idx = 0;
        for (int i = 1; i < numLatitude - 1; ++i) {
            double z = sin(MY_PI * latitude[i] / 180.0);
            double a = cos(MY_PI * latitude[i] / 180.0);

            for (int j = 0; j < numLongitude; ++j) {

                double x = a * cos(MY_PI * longitude[j] / 180.0);
                double y = a * sin(MY_PI * longitude[j] / 180.0);

                fvec3 v{(float)(rx * x), (float)(ry * y), (float)(rz * z)};
                mdl.vertex[idx++] = v;
            }
        }

        mdl.vertex[idx++] = fvec3{0.f, 0.f, -(float)rz};
        mdl.vertex[idx++] = fvec3{0.f, 0.f, (float)rz};

        assert(idx == mdl.vertex.size());

        // 生成法线
        mdl.normal.resize(mdl.vertex.size());
        fvec3 n1{(float)(ry * rz), (float)(rx * rz), (float)(rx * ry)};

        for (int k = 0; k < (int) mdl.vertex.size(); ++k) {
            mdl.normal[k] = normalize(elemMul(n1, mdl.vertex[k]));
        }


        // 生成三角形
        mdl.triangles.resize((numLatitude - 2) * numLongitude * 2);

        idx = 0;

        // 生成非极地的三角形
        for (int i = 1; i < numLatitude - 2; ++i) {

            int idx1 = (i - 1) * numLongitude;
            int idx2 = i * numLongitude;


            for (int j = 0; j < numLongitude; ++j) {

                int j2 = j + 1;
                if (j == numLongitude - 1)
                    j2 = 0;

                //(i,j), (i+1,j+1), (i+1,j)
                mdl.triangles[idx++] = ivec3{idx1 + j, idx2 + j2, idx2 + j};

                //(i,j), (i,j+1), (i+1,j+1)
                mdl.triangles[idx++] = ivec3{idx1 + j, idx1 + j2, idx2 + j2};
            }
        }

        // 极地的三角形
        int idx_s = mdl.vertex.size() - 2;
        int idx_n = mdl.vertex.size() - 1;

        int idx2 = (numLatitude - 3) * numLongitude;

        for (int j = 0; j < numLongitude; ++j) {

            int j2 = j + 1;
            if (j == numLongitude - 1)
                j2 = 0;

            // (idx_s, (1,j+1), (1,j)
            mdl.triangles[idx++] = ivec3{idx_s, j2, j};

            // idx_n, (numLatitude - 2,j), (numLatitude - 2,j+1)
            mdl.triangles[idx++] = ivec3{idx_n, idx2 + j, idx2 + j2};
        }

        assert(idx == mdl.triangles.size());


        return mdl;
    }

    Model Model::gen_rectangle(string name, vec3 org, fvec2 len, int ndiv) {
        Model mdl;
        mdl.name = name;
        mdl.setPos(org);


        int num_row = ndiv + 1;


        mdl.vertex.resize(num_row * num_row);
        mdl.normal.resize(mdl.vertex.size());

        int idx = 0;
        for (int i = 0; i <= ndiv; ++i) {
            double x = len[0] * (-0.5 + 1.0 * i / ndiv);
            for (int j = 0; j <= ndiv; ++j) {
                double y = len[1] * (-0.5 + 1.0 * j / ndiv);
                mdl.vertex[idx] = fvec3{(float)x, (float)y, 0};
                mdl.normal[idx] = {0, 0, 1};
                ++idx;
            }
        }


        mdl.triangles.resize(ndiv * ndiv * 2);
        idx = 0;
        for (int i = 0; i < ndiv; ++i) {
            int off = i * num_row;
            int off2 = (i + 1) * num_row;
            for (int j = 0; j < ndiv; ++j) {
                mdl.triangles[idx++] = {off + j, off2 + j, off2 + j + 1};
                mdl.triangles[idx++] = {off + j, off2 + j + 1, off + j + 1};
            }
        }

        assert(idx = mdl.triangles.size());


        return mdl;
    }

	bool Model::verify() const
	{

		const float inf = 1e7f;

		size_t num_v = vertex.size();

		for (const auto& a : vertex)
		{
			if (a[0] < -inf || a[0] > inf)
				return false;

			if (a[1] < -inf || a[1] > inf)
				return false;

			if (a[2] < -inf || a[2] > inf)
				return false;
		}


		if (en_color)
		{
			if (color.size() != num_v)
				return false;

		}
		else
		{
			if (!color.empty() && color.size() != num_v)
				return false;
		}

		if (normal.size() != num_v)
			return false;

		for (const auto& a : normal)
		{
			if (a[0] < -inf || a[0] > inf)
				return false;

			if (a[1] < -inf || a[1] > inf)
				return false;

			if (a[2] < -inf || a[2] > inf)
				return false;
		}


		if (en_texture)
		{
			if (texture.size() != num_v)
				return false;
		}
		else
		{
			if (!texture.empty() && texture.size() != num_v)
				return false;
		}



		for (size_t i = 0; i < triangles.size(); ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (triangles[i][j] < 0 || triangles[i][j] >= (int)num_v)
					return false;
			}
		}


		return true;
	}

    Model merge_model(std::string name, const Model &m1, const Model &m2) {
        Model mdl;

        mdl.name = name;
        mdl.material = m1.material;

        assert(m1.vertex.size() == m1.normal.size());
        assert(m2.vertex.size() == m2.normal.size());

        mdl.vertex.resize(m1.vertex.size() + m2.vertex.size());
        mdl.normal.resize(mdl.vertex.size());


        int idx = 0;
        for (int i = 0; i < (int) m1.vertex.size(); ++i) {
            fvec3 v = mat_d2f(m1.getPos()) + m1.vertex[i];
            mdl.vertex[idx] = v;
            mdl.normal[idx] = m1.normal[i];

            ++idx;
        }


        for (int i = 0; i < (int) m2.vertex.size(); ++i) {
            fvec3 v = mat_d2f(m2.getPos()) + m2.vertex[i];
            mdl.vertex[idx] = v;
            mdl.normal[idx] = m2.normal[i];

            ++idx;
        }


        assert(idx == mdl.vertex.size());


        // color

        mdl.color.resize(m1.color.size() + m2.color.size());

        idx = 0;
        for (int i = 0; i < (int) m1.color.size(); ++i) {
            mdl.color[idx++] = m1.color[i];
        }

        for (int i = 0; i < (int) m2.color.size(); ++i) {
            mdl.color[idx++] = m2.color[i];
        }

        assert(mdl.color.size() == idx);

        // triangles
        mdl.triangles.resize(m1.triangles.size() + m2.triangles.size());

        for (int i = 0; i < (int) m1.triangles.size(); ++i) {
            mdl.triangles[i] = m1.triangles[i];
        }

        int off_tr = m1.triangles.size();
        int off_vt = m1.vertex.size();

        for (int i = 0; i < (int) m2.triangles.size(); ++i) {
            ivec3 v = m2.triangles[i];

            mdl.triangles[off_tr + i] = off_vt + v;
        }

        return mdl;
    }

    Model merge_model(std::string name, const Model &m1, const Model &m2, const Model &m3) {
        return merge_model(name, merge_model("", m1, m2), m3);
    }

    Model merge_model(std::string name, const Model &m1, const Model &m2, const Model &m3,
                      const Model &m4) {
        return merge_model(name, merge_model("", m1, m2, m3), m4);
    }

    Model merge_model(std::string name, const Model &m1, const Model &m2, const Model &m3,
                      const Model &m4, const Model &m5) {
        return merge_model(name, merge_model("", m1, m2, m3, m4), m5);
    }


    // 分割字符串
    static vector<string> _split_str(const string &s, const string &seperator) {
        vector<string> result;
        typedef string::size_type string_size;
        string_size i = 0;

        while (i != s.size()) {
            //找到字符串中首个不等于分隔符的字母；
            int flag = 0;
            while (i != s.size() && flag == 0) {
                flag = 1;
                for (string_size x = 0; x < seperator.size(); ++x)
                    if (s[i] == seperator[x]) {
                        ++i;
                        flag = 0;
                        break;
                    }
            }

            //找到又一个分隔符，将两个分隔符之间的字符串取出；
            flag = 0;
            string_size j = i;
            while (j != s.size() && flag == 0) {
                for (string_size x = 0; x < seperator.size(); ++x)
                    if (s[j] == seperator[x]) {
                        flag = 1;
                        break;
                    }
                if (flag == 0)
                    ++j;
            }
            if (i != j) {
                result.push_back(s.substr(i, j - i));
                i = j;
            }
        }
        return result;
    }


    Model load_obj_file(std::string model_name, std::string filename, float scale, bool isVerify) {
        struct _less_ivec3 {
            bool operator()(const ivec3 &a, const ivec3 &b) const {
                if (a[0] < b[0])
                    return true;
                if (a[0] > b[0])
                    return false;

                if (a[1] < b[1])
                    return true;
                if (a[1] > b[1])
                    return false;

                return (a[2] < b[2]);
            }
        };


        // 读文件
        ifstream file(filename);

        if (!file.is_open())
            return Model();


        vector<string> lines;


        try {
            string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
        }
        catch (...) {
            file.close();
            return Model();
        }


        // 处理文本文件
        vector<fvec3> v;
        vector<fvec3> vt;
        vector<fvec3> vn;

        vector<ivec3> f[3];

        for (string line : lines) {
            vector<string> words = _split_str(line, " ");

            // 顶点
            if (words.size() >= 4 && words[0] == "v") {
                float x = stof(words[1]);
				float y = stof(words[2]);
				float z = stof(words[3]);

                v.push_back(scale * fvec3{x, y, z});
            }

            // 材质
            if (words.size() >= 4 && words[0] == "vt") {
                float x = stof(words[1]);
                float y = stof(words[2]);
                float z = stof(words[3]);

                vt.push_back(fvec3{x, y, z});
            }

            // 法线
            if (words.size() >= 4 && words[0] == "vn") {
                float x = stof(words[1]);
                float y = stof(words[2]);
				float z = stof(words[3]);

                vn.push_back(normalize(fvec3{x, y, z}));
            }

            // 面
            if (words.size() >= 4 && words[0] == "f") {
                vector<string> w1 = _split_str(words[1], "/");
                vector<string> w2 = _split_str(words[2], "/");
                vector<string> w3 = _split_str(words[3], "/");


                int f1v = stoi(w1[0]);
                int f1t = stoi(w1[1]);
                int f1n = stoi(w1[2]);

                int f2v = stoi(w2[0]);
                int f2t = stoi(w2[1]);
                int f2n = stoi(w2[2]);

                int f3v = stoi(w3[0]);
                int f3t = stoi(w3[1]);
                int f3n = stoi(w3[2]);

                f[0].push_back(ivec3{f1v, f1t, f1n});
                f[1].push_back(ivec3{f2v, f2t, f2n});
                f[2].push_back(ivec3{f3v, f3t, f3n});
            }

        }

        // 转换模型
        Model mdl;

// 		assert(v.size() == vt.size() && v.size() == vn.size());
// 
// 		mdl.vertex = v;
// 		//mdl.vertex_texture0 = vt;
// 		mdl.normal = vn;
// 
// 		for (int i = 0; i<(int)mdl.vertex.size(); ++i)
// 		{
// 			mdl.vertex[i] = scale * mdl.vertex[i];
// 		}
// 
// 
// 		mdl.triangles.resize(f[0].size());
// 
// 		for (int i = 0; i < (int)f[0].size(); ++i)
// 		{
// 			mdl.triangles[i] = ivec3{ f[0][i][0], f[1][i][0], f[2][i][0] };
// 		}


        map<ivec3, int, _less_ivec3> idx_map;


        for (int i = 0; i < (int) f[0].size(); ++i) {
            ivec3 tri;

            for (int j = 0; j < 3; ++j) {
                ivec3 fji = f[j][i];


                auto it = idx_map.find(fji);
                if (it != idx_map.end()) {
                    tri[j] = it->second;
                } else {
                    mdl.vertex.push_back(v[fji[0] - 1]);

                    fvec3 vt_ji = vt[fji[1] - 1];
                    mdl.texture.push_back({vt_ji[0], vt_ji[1]});
                    mdl.normal.push_back(vn[fji[2] - 1]);

                    tri[j] = mdl.vertex.size() - 1;
                    idx_map.insert(std::pair<ivec3, int>(fji, tri[j]));

                }
            }

            mdl.triangles.push_back(tri);

        }

		if (isVerify)
		{
			bool isValid = mdl.verify();

			if (!isValid)
			{
				mdl = Model();
			}
		}

		mdl.name = model_name;

        return std::move(mdl);


    }

    ModelGroupIterator ModelGroup::begin() {
        ModelGroupIterator it;
        _begin(it, 0);
        //it._calCoor();
        return it;
    }

    void ModelGroup::_begin(ModelGroupIterator &it, int idx) {
        if (models.size() > 0) {
            it.idx_model = 0;
            it.model = &(models[0]);
            it.group = this;
            it.idx_group = idx;
            //it._calCoor();
            return;
        }


        it.fatherList.push_back({idx, this});

        for (int i = 0; i < (int) groups.size(); ++i) {
            groups[i]._begin(it, i);

            if (!it.isEnd()) {
                return;
            }

        }

        it.fatherList.pop_back();
    }

    bool ModelGroupIterator::isEnd() const {
        return idx_model < 0;
    }

    void ModelGroupIterator::next() {
        if (isEnd())
            return;

        // 子模型
        if (idx_model + 1 < (int) group->models.size()) {
            ++idx_model;
            model = &(group->models[idx_model]);
            //_calCoor();
            return;
        }

        idx_model = -1;
        model = nullptr;

        // 子组


        fatherList.push_back({idx_group, group});
        for (int i = 0; i < (int) group->groups.size(); ++i) {
            group->groups[i]._begin(*this, i);

            if (!isEnd()) {
                return;
            }
        }
        fatherList.pop_back();


        // 兄弟组

        while (!fatherList.empty()) {

            parents_t pp = *(fatherList.rbegin());

            ModelGroup *father = pp.ptr;

            for (int idx_bro = idx_group + 1; idx_bro < (int) father->groups.size(); ++idx_bro) {
                ModelGroup *bro = &(father->groups[idx_bro]);

                bro->_begin(*this, idx_bro);

                if (!isEnd()) {
                    return;
                }
            }

            idx_group = pp.idx;
            group = pp.ptr;
            fatherList.pop_back();
        }
    }

    Coordinate ModelGroupIterator::calCoor() const {
        Coordinate coor;

        if (isEnd())
            return coor;

        coor = Coordinate::eye();

        for (auto a : fatherList) {
            coor = coor * (*(a.ptr));
        }

        coor = coor * (*group) * (*model);

        return coor;
    }
// 	void ModelGroupIterator::_calCoor()
// 	{
// 		if (isEnd())
// 			return;
// 
// 		coor = Coordinate::eye();
// 
// 		for (auto a : fatherList)
// 		{
// 			coor = coor * (*(a.ptr));
// 		}
// 
// 		coor = coor * (*group) * (*model);
// 
// 		
// 	}
}