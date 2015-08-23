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
	virtual ~Module(){}
	Module(){
		lastVelocity = 0.0;
		lastAcceleration = 0.0;
		value = 0.0;
	}
	bool goTo(float dt, double jVelocityModifier);
	double computeMaxStep(float dt, double jVelocityModifier);
	glm::vec4 getMainAxis(){
		return glm::normalize(vecToA + vecToB);
	}
	glm::vec3 getMainAxis3(){
		return glm::normalize(vecToA.xyz() + vecToB.xyz());
	}
	void reset(){
		lastVelocity = 0;
		lastAcceleration = 0;
		value = 0;
		targetValue = 0;
	}

	int type;
	glm::vec4 axis;
	glm::vec4 vecToA;
	glm::vec4 vecToB;
	std::string name;
	std::shared_ptr<Entity> entity;
	double min, max;
	double value;
	double targetValue;
	double maxVelocty;
	double maxAcceleration;
	double lastVelocity;
	double lastAcceleration;
};

class Gripper : public Module{};
class IIK {
public:
	std::vector<double> result;
	glm::vec4 endPosition;
	bool succes = false;

	virtual bool solve(Point target, Robot &robot) = 0;
	virtual bool performIK(Point start, Point target, Robot &robot)=0;
	virtual ~IIK(){}
};

class Robot {
public:
	~Robot(){
		for(auto &it : chain)
			it.reset();
		chain.clear();
		std::cerr<<"delete Robot"<<std::endl;
	}

	std::vector<glm::vec4> forward(); // punkt ze wsp��dnych
	void inverse(); // wsp��dne z punktu
	void update(float dt);
	std::vector<double> getVariables();
	bool goTo(float dt, double jVelocityModifier);
	bool goTo(const std::vector<double> &jointPositions);
	Point simulate(std::vector<double> &vec);
	std::vector<Point> simulateFullData(std::vector<double> &variables);
	glm::vec4 clamp(std::vector<double> &vec);
	glm::vec4 insertVariables(std::vector<double> &vec);
	int getSize(){return chain.size();}
	void reset();

	bool isReady { true };
	Point endEffector;
// private:
	std::vector<std::unique_ptr<Module>> chain;
};

/// without orientation
class JT0 : public IIK {
public:
	bool solve(Point aim, Robot &robot);
	bool performIK(Point start, Point target, Robot &robot);
};
/// with orientation
class JT1 : public IIK {
public:
	~JT1(){
		std::cerr<<"delete JT1"<<std::endl;
	}
	bool solve(Point aim, Robot &robot);
	bool performIK(Point start, Point target, Robot &robot);
};

void robotTestInit(Robot &robot);
void robotTest(float dt, Robot &robot);
