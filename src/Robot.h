#pragma once
class Robot;
class Entity;
enum  JointType{
	HINGE = 1,
	PRISMATIC = 2,
};

struct Point {
	glm::vec4 position;
	glm::quat quat;
};

class Module {
public:
	int type;
	glm::vec4 axis;
	glm::vec4 vecToA;
	glm::vec4 vecToB;
	std::string name;
	Entity *entity;
	double min, max;
	double value;
	double maxVelocty;
	double maxAcceleration;

	glm::vec4 getMainAxis(){
		return glm::normalize(vecToA + vecToB);
	}
	glm::vec3 getMainAxis3(){
		return glm::normalize(vecToA.xyz() + vecToB.xyz());
	}
};

class Gripper : public Module{};

class IIK {
public:
	std::vector<double> result;
	glm::vec4 endPosition;
	bool succes = false;

	virtual bool solve(Point aim, Robot &robot) = 0;
	virtual bool performIK(Point aim, Robot &robot)=0;
};


class Robot {
public:
	std::vector<std::unique_ptr<Module>> chain;
	Point endEffector;

	std::vector<glm::vec4> forward(); // punkt ze wsp��dnych
	void inverse(); // wsp��dne z punktu
	void update(float dt);
	std::vector<float> getVariables();
	bool goTo(const std::vector<double> &jointPositions);
	Point simulate(std::vector<double> &vec);
	std::vector<Point> simulateFullData(std::vector<double> &variables);
	glm::vec4 clamp(std::vector<double> &vec);
	glm::vec4 insertVariables(std::vector<double> &vec);
	int getSize(){return chain.size();}
};

class JT0 : public IIK {
public:

	bool solve(Point aim, Robot &robot);
	bool performIK(Point aim, Robot &robot);
};
class JT1 : public IIK {
public:

	bool solve(Point aim, Robot &robot);
	bool performIK(Point aim, Robot &robot);
};
class JTReversed : public IIK {
public:

	bool solve(Point aim, Robot &robot);
	bool performIK(Point aim, Robot &robot);
};
class CCD : public IIK {
public:

	bool solve(Point aim, Robot &robot);
	bool performIK(Point aim, Robot &robot);
};




