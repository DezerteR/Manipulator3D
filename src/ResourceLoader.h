#pragma once
#include <Utils/BaseStructs.h>

struct Resources
{

	uint32_t VAO;
	uint32_t VBO[3];
	uint32_t IBO;

	std::unordered_map<std::string, GLuint>	textures;
	std::unordered_map<std::string, Mesh>	meshes;
	std::unordered_map<std::string, Image>	images;
	std::unordered_map<std::string, ImageSet>	imageSets;
	std::unordered_map<std::string, ArrayData>	arrayData;

	~Resources();
};

class Scene
{
public:
	std::unordered_map<std::string, Entity> units;
	std::vector<PointLamp> pointLamps;
	unique_ptr<Resources> resources;
	shared_ptr<Robot> robot;

	~Scene();
	Scene() : robot(make_shared<Robot>()), resources(make_unique<Resources>()) {}
};

class ResourceLoader
{
public:

	unique_ptr<Resources> &resources;

	ResourceLoader(unique_ptr<Resources> &_resources):resources(_resources){}
	~ResourceLoader(){
		cerr<<"~ResourceLoader"<<endl;
	}

	string shaderPath { "../shaders/" };
	string imagePath { "../res/textures/" };
	string meshPath { "../res/models/" };
	string soundPath { "../res/sound/" };

	vector<float> model_vertices;
	vector<float> model_coords;
	vector<float> model_normals;
	vector<int> model_indices;

	int progress {0};
	int count {0};

	void loadResources(CFG::Node &cfg);
	void reloadShader(const string &shaderName);
	bool loadShader(CFG::Node &cfg);
	bool loadMesh(string meshName);
	bool loadMesh(CFG::Node &cfg);
	bool loadImage(const string &name);
	bool loadImage(CFG::Node &cfg);
	bool loadFonts(CFG::Node &_cfg);
	bool loadScene(CFG::Node &_cfg);
	bool loadImageSet(CFG::Node &_cfg);
	bool loadObj(fstream &file);
	bool loadScene(Scene &scene, CFG::Node &cfg);
	std::string  loadFile(string fname);
	void printShaderInfoLog(GLint shader);
	void fillBuffers();

	bool loadRobot(Scene &scene, Robot &robot, CFG::Node &cfg);
	GLuint compileShader(const string &shaderName);
};

