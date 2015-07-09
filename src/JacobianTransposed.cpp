﻿
#include <Utils/Includes.h>
#include <Utils/Utils.h>
#include <Utils/Camera.h>
#include <Utils/IMGUI_V4.h>
#include "common.h"
#include "Robot.h"
#include "JacobianTransposed.h"
#include "Timer.h"
#include "JacobianMatrix.h"

#include "Graph.h"

extern UI::IMGUI ui;
extern Camera camera;
extern float	 mouse_x, mouse_y;

extern int robotPositionsCounter;
extern int robotPositionsMax;
extern vector<glm::vec4> robotPositions;
extern PositionSaver g_mousePositions;
extern PositionSaver g_robotPositions;
extern PositionSaver g_robotDebugPositions;
extern std::list<Plot*> plotList;

// static const float pi = 3.141592f;
static const float hpi = 0.5f * 3.141592f;
static const float pi2 = 2.f * 3.141592f;
// static const double dpi = 3.141592653589793;
static const double hdpi = 0.5 * 3.141592653589793;
static const double sqdpi = 3.141592653589793 * 3.141592653589793;
static const double dpi2 = 2.0 * 3.141592653589793;


Graph jacobianIterations("jacobianIterations", Graph::LastX, 0xFFFF00FF, 250);
Graph jacobianPrecision("jacobianPrecision", Graph::LastX, 0xFF4000FF, 250);
extern Plot mainPlot;

bool JT0::solve(Point aim, Robot &robot){
	if(performIK(aim, robot))
		robot.insertVariables(result);
	return true;
}
bool JT0::performIK(Point aim, Robot &robot){
	int iterationLimit = 2000;
	float minError = 0.001f;

	auto jacobian = buildJacobian(robot);
	auto jjp = jacobian.transposed() * jacobian; // 6xn * nx6 da 6x6

	auto endEffector = robot.endEffector;
	Matrix variables(robot.getSize(), 1);
	Matrix enhancement(robot.getSize(), 1);
	auto force = Matrix(6,1);

	variables.insertColumn(0, robot.getVariables());

	float error = glm::distance(aim.position, endEffector.position);
	for(int i=0; error > minError && i<iterationLimit; i++){
		auto delta  = (aim.position - endEffector.position);
		force.insertColumn(0, {delta.x, delta.y, delta.z,0,0,0});

		auto a = dot(jjp*force, force);
		a = a/dot(jjp*force, jjp*force);
		// a = 0.005f;
		auto q = jacobian*force*a;
		// q = mul(q, enhancement);
		variables = q + variables;
		// robot.clamp(variables.getVector());
		endEffector = robot.simulate(variables.getVector());

		error = glm::distance(endEffector.position, aim.position);
	}

	result = variables.getVector();
	// succes = error > minError;
	succes = true;
	return succes;
}


//TODO: dodać sprawdzanie orientacji jeszcze
bool JT1::solve(Point aim, Robot &robot){
	performIK(aim, robot);
		// robot.insertVariables(result);
	return glm::distance(endPosition, aim.position) < 0.005;
}
bool JT1::performIK(Point aim, Robot &robot){
	int iterationLimit = 2000;
	float minError = 0.001f;

	auto endEffector = robot.endEffector;
	auto force = Matrix(6,1);
	Matrix variables(robot.getSize(), 1);
	Matrix enhancement(robot.getSize(), 1);

	double i = 0.2;
	// double i = 1.5;
	for(auto &it : variables.getVector()){
		it = 0.0;
	}
	for(auto &it : enhancement.getVector()){
		// it = i;
		it = 1;
		// i -= 0.2;
		i += 0.9;
	}
	// enhancement(3) *= 15;

	variables.insertColumn(0, robot.getVariables());

	float positionError = glm::distance(aim.position, robot.endEffector.position);

	g_mousePositions(aim.position);
	// if(positionError > minError*5.f)
		// aim.position = robot.endEffector.position + (aim.position - robot.endEffector.position)*0.1f;

		auto jacobian = buildJacobian(robot,variables.getVector(), endEffector);
		auto jjp = jacobian.transposed() * jacobian; // 6xn * nx6 da 6x6
	u32 iteration = 0;
	for(; positionError > minError && iteration<iterationLimit; iteration++){


		auto positionDelta  = (aim.position - endEffector.position);
		// auto axisDelta  = glm::cross(glm::axis(endEffector.quat), glm::axis(aim.quat));

		force.insertColumn(0, {positionDelta.x, positionDelta.y, positionDelta.z,0, 0, 0});

		auto a = dot(jjp*force, force);
		a = a/dot(jjp*force, jjp*force);
		auto gradient = jacobian*force*a;
		gradient = mul(gradient, enhancement);

		// for(int i=0; i<robot.chain.size(); i++){
			// float val = robot.chain[i]->value;
			// float max = robot.chain[i]->max;
			// float min = robot.chain[i]->min;

			// if(val > max/2)
				// gradient(i) *=  (0.1 + max - val)/max/2 + 0.5;
			// else if(val < min/2)
				// gradient(i) *=  (0.1 + min - val)/min/2 + 0.5;


		// }

		variables = gradient + variables;
		// robot.clamp(variables.getVector());
		endEffector = robot.simulate(variables.getVector());

		positionError = glm::distance(endEffector.position, aim.position);
	}
	endPosition = endEffector.position;
	result = variables.getVector();
	succes = positionError < minError;

	auto l_v = robot.getVariables();
	// jacobianIterations.push(std::min(100u, iteration));
	// jacobianPrecision.push();
	// succes = true;
	return succes;
}

bool JTReversed::performIK(Point aim, Robot &robot){

	return true;
}
bool CCD::performIK(Point aim, Robot &robot){

	return true;
}

extern glm::vec4 robotTarget;
extern glm::vec4 endPos;
extern glm::vec4 normal;
extern glm::vec4 surfaceOfMotion;
extern glm::quat gripperQuat;
// TODO: poprawić newPoint, ma on bć punktm przecięcia ray z płaszczyzną, obecnie jest po chuju
void BADBADBADRobotIKRealtimeTest(Robot &robot){
	if(ui.rClick()){
		endPos = robot.endEffector.position;
		gripperQuat = robot.endEffector.quat;
		normal = glm::normalize(camera.Normal);
		// endPos = camera.eyePosition + normal*2.f;
		surfaceOfMotion = glm::vec4(glm::vec3(normal.xyz()), -glm::dot(normal, endPos));
	}

	// glm::vec4 newPoint = camera.eyePlaneIntersection(surfaceOfMotion);

	JT1 ik;
	// auto res = ik.performIK({ newPoint, gripperQuat }, robot);
	// auto res = ik.performIK({ robotTarget, gripperQuat }, robot);

	if(ui.rRepeat() && ik.succes){
		robotTarget = camera.eyePlaneIntersection(surfaceOfMotion);
		g_mousePositions(robotTarget);
	}
	robot.insertVariables(ik.result);

}


void jacobianTransposeInit(){
	jacobianIterations.setBouds({0,250,0,500});
	jacobianPrecision.setBouds({0,250,0,1});
}
void jacobianTransposeUpdate(){


}
void test(Robot &robot){
}
