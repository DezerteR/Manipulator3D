﻿#include <Utils/Includes.h>
#include <Utils/Utils.h>
#include <Utils/Camera.h>
#include <Utils/MeshInfo.h>
#include <Utils/Camera.h>
#include <Utils/IMGUI_V4.h>
#include "Common.h"
#include "Robot.h"
#include "CFGParser.h"
#include "ResourceLoader.h"
#include "Engine.h"

std::unordered_map<string, GLuint>	shaders;
void ResourceLoader::loadResources(CFG::Node &cfg){
	count = cfg["Shaders"].size() + cfg["Images"].size() + cfg["Meshes"].size();

	for(int i=0; i<cfg["Shaders"].size(); i++){
		loadShader(cfg["Shaders"][i]);
	}
	for(int i=0; i<cfg["Images"].size(); i++){
		loadImage(cfg["Images"][i]);
	}
	for(int i=0; i<cfg["Meshes"].size(); i++){
		loadMesh(cfg["Meshes"][i]);
	}
	for(int i=0; i<cfg["ImageSets"].size(); i++){
		 loadImageSet(cfg["ImageSets"][i]);
	}

	Engine::genVao(model_vertices, model_coords, model_normals, model_indices, resources);
}

bool ResourceLoader::loadShader(CFG::Node &cfg){

	std::stringstream ss(cfg.value);
	string name, vert, frag;
	ss>>name>>vert>>frag;

	vert += ".vert";
	frag += ".frag";
	if(shaders.find(name) == shaders.end()){
		cout<<"shader: "<<name<<endl;
		shaders[name] = compileShader(vert, frag);
	}
	return true;
}
char* ResourceLoader::loadFile(string fname, GLint &fSize){
	std::ifstream::pos_type size;
	char * memblock;
	string text;

	// file read based on example in cplusplus.com tutorial
	std::ifstream file (fname.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
	if (file.is_open()){
		size = file.tellg();
		fSize = (GLuint) size;
		memblock = new char [size];
		file.seekg (0, std::ios::beg);
		file.read (memblock, size);
		file.close();
		text.assign(memblock);
	}
	else{
		cout << "Unable to open file " << fname << endl;
		exit(1);
	}
	return memblock;
}

bool ResourceLoader::loadImage(CFG::Node &cfg){
	string name = cfg.value;
	loadImage(name);
}

bool ResourceLoader::loadMesh(string meshName){
	string fileName = meshPath+meshName+".obj";
	auto &meshes = resources->meshes;

	Mesh mesh;
	cout<<"mesh: "+meshName<<endl;
	fstream file;
	file.open(fileName, std::ios::in);

		if(!file.is_open()){
			cout<<"error opening: "<<meshName<<endl;
			return false;
		}

		mesh.begin = model_indices.size();
		loadObj(file);
		mesh.end = model_indices.size();
		mesh.offset = (void*)(sizeof(int)*mesh.begin);
		mesh.count = mesh.end - mesh.begin;


	file.close();
	meshes[meshName] = mesh;
	return true;
}
bool ResourceLoader::loadMesh(CFG::Node &cfg){
	string name = cfg["Name"].value;
	loadMesh(name);
	return true;
}
bool ResourceLoader::loadObj(fstream &file){

	// fstream file;
	// file.open(name.c_str(), ios::in);
		string tmp, mpt;
		char ctmp;
		char nop;
		// pozbywamy się poczatkowych śmieci
		while(tmp != "v" && !file.eof())
			file>>tmp;

	vector<float> vertex;
	vector<float> coord;
	vector<float> normal;
	vertex.reserve(600000);
	coord.reserve(400000);
	normal.reserve(600000);
	int u(0), k(0);
	// file>>tmp;
	float x,y,z;
	int a,b,c;
		while(tmp=="v"){
			file>>x>>y>>z;
			vertex[u++]=x;
			vertex[u++]=y;
			vertex[u++]=z;
			file>>tmp;
		}
		u=0;
		while(tmp=="vt"){
			file>>x>>y;
			coord[u++]=x;
			coord[u++]=y;
			file>>tmp;
		}
		u=0;
		while(tmp=="vn"){
			file>>x>>y>>z;
			normal[u++]=x;
			normal[u++]=y;
			normal[u++]=z;
			file>>tmp;
		}
		while(tmp != "f" && !file.eof())
			file>>tmp;
		int i(0);
		if(model_indices.size()>0)
			i = model_indices[model_indices.size()-1]+1;
		// i+=przebieg;

		u=0;//model_vertices.size();
		k=0;//model_coords.size();


		/*
		1_______2
		|            / |
		|         /    |
		|      /       |
		|   /          |
		|/             |
		0             3
		012023
		*/
		while(tmp=="f"&& !file.eof()){
		for(int t=0; t<3; t++){
		file>>a>>nop>>b>>nop>>c;
		// cerr<<a<<" "<<b<<" "<<c<<endl;
			a--;b--;c--;
				model_vertices.push_back(vertex[a*3]);
				model_vertices.push_back(vertex[a*3+1]);
				model_vertices.push_back(vertex[a*3+2]);
				model_coords.push_back(coord[b*2]);
				model_coords.push_back(coord[b*2+1]);
				model_normals.push_back(normal[c*3]);
				model_normals.push_back(normal[c*3+1]);
				model_normals.push_back(normal[c*3+2]);
				u+=3;k+=2;
				model_indices.push_back(i++);
			}
				file>>tmp;
				if(tmp=="f");
				else{
					model_indices.push_back(i-3);
					model_indices.push_back(i-1);
					file>>nop>>b>>nop>>c;
					a=atoi(tmp.c_str());a--;b--;c--;
				model_vertices.push_back(vertex[a*3]);
				model_vertices.push_back(vertex[a*3+1]);
				model_vertices.push_back(vertex[a*3+2]);
				model_coords.push_back(coord[b*2]);
				model_coords.push_back(coord[b*2+1]);
				model_normals.push_back(normal[c*3]);
				model_normals.push_back(normal[c*3+1]);
				model_normals.push_back(normal[c*3+2]);
				u+=3;k+=2;
						model_indices.push_back(i++);
						file>>tmp;
				}
		}

	// file.close();

}

bool ResourceLoader::loadFonts(CFG::Node &_cfg){
	bool success;
	auto &XMLs = _cfg["XML"s];
	for(int i=0; i<XMLs.size(); i++){
		auto cfg = XMLs[i];
		string name = cfg["name"].value;
		string file = cfg["file"].value;
		int size = cfg["size"].asInt();
		bool success = true;

		UI::Font font;
		// font.size = size;
		success &= loadImage(file+".png");
		font.texID = resources->textures[file];

		glBindTexture(GL_TEXTURE_2D, font.texID);
		int w,h;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, 0);
		font.generator = new UI::FontRenderer();
		font.generator->loadFontInfoXML(file, w,h);
		UI::fonts[name + to_string(size)] = font;
	}
	auto &FNTs = _cfg["FNT"s];
	for(int i=0; i<FNTs.size(); i++){
		auto cfg = FNTs[i];
		string name = cfg["name"].value;
		string symbolsName = cfg["symbols"].value;

		if (UI::fonts.find(symbolsName) == UI::fonts.end()){
			UI::Font &symbols = UI::fonts[symbolsName];
			// symbols.size = -1;
			success &= loadImage(symbolsName+".png");
			symbols.texID = resources->textures[symbolsName];

			symbols.generator = new UI::FontRenderer();
			symbols.generator->m_associatedSymbols = symbols.generator;
			symbols.generator->loadFontInfoFNT(name);
			symbols.generator->loadFontInfoSYM(symbolsName);
			// fonts[symbolsName] = symbols;
		}
		if (UI::fonts.find(name) == UI::fonts.end()){
			UI::Font &font = UI::fonts[name];
			// font.size = -1;
			success &= loadImage(name+".png");
			font.texID = resources->textures[name];

			font.generator = new UI::FontRenderer();
			font.generator->loadFontInfoFNT(name);
			font.generator->m_associatedSymbols = UI::fonts[symbolsName].generator;

			// fonts[name] = font;
		}
	}



	return success;
}

bool ResourceLoader::loadImageSet(CFG::Node &cfg){
	string name = cfg.value;
	auto descr = CFG::Load("../res/misc/"+name+".yml")["root"];
	loadImage(name+".png");
	auto tmp = resources->images[name];

	ImageSet set;
	int w = set.w = tmp.w;
	int h = set.h = tmp.h;
	set.ID = tmp.ID;
	int count = descr.size();

	for(int i=0; i<count; i++){
		string n = descr[i]["name"].value;
		glm::vec4 r = descr[i]["rect"].asVec4();
		r.y = h - r.y;
		glm::vec4 d = glm::vec4(0,0,r.z, r.w);
		r = glm::vec4(r.x/w, r.y/h, r.z/w, r.w/h);
		set.set.emplace(n, Icon {r, d, 0xffffffff});
	}

	resources->imageSets[name] = set;

}

bool ResourceLoader::loadScene(Scene &scene, CFG::Node &cfg){
	string dirname = cfg["dirname"].value;
	meshPath = cfg["dirname"].value+"\\";

	auto &meshes = cfg["Meshes"];
	for(auto &it : meshes.Vector){
		loadMesh(it);
		Material material {it["Color"].asVec31()};
		scene.units.emplace(it["Name"].value, Entity {&resources->meshes[it["Name"].value], material, it["Position"].asVec31(), it["Quaternion"].asQuat()});
	}

	auto &lamps = cfg["Lamps"];
	for(auto &lamp : lamps.Vector){
		if((string)lamp["Type"] == "POINT"){
			scene.pointLamps.emplace_back(PointLamp {
				lamp["Position"].asVec31(),
				lamp["Color"].asVec31(),
				lamp["Falloff_distance"].asFloat(),
				lamp["Energy"].asFloat()
			});

		}
	}

	Engine::genVao(model_vertices, model_coords, model_normals, model_indices, scene.resources);

	loadRobot(scene, scene.robot, cfg["Robot"]);
}
bool ResourceLoader::loadRobot(Scene &scene, Robot &robot, CFG::Node &cfg){
	for(auto &it : cfg.Vector){
		int type;
		if(it["Type"].value == "prismatic")
			type = PRISMATIC;
		else if(it["Type"].value == "hinge")
			type = HINGE;

		Module *module = new Module;
		cout<<"-- "+it["Name"].value<<endl;
		module->type = type;
		module->vecToA = it["ParentJoint"]["Vec"].asVec30();
		module->axis = it["ParentJoint"]["Axis"].asVec30();
		module->vecToB = it["ChildJoint"]["Vec"].asVec30();
		module->min = it["ParentJoint"]["Min"].asFloat()*toRad;
		module->max = it["ParentJoint"]["Max"].asFloat()*toRad;
		module->name = it["Name"].value;
		module->entity = &scene.units[it["Name"].value];


		robot.chain.push_back(module);

	}

}









