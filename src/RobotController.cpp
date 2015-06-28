﻿#include <Utils/Includes.h>
#include <Utils/Utils.h>
#include <Utils/BaseStructs.h>
#include <Utils/IMGUI_V4.h>
#include "Robot.h"
#include "JacobianTransposed.h"
#include "IInterpolator.h"
#include "RobotController.h"
extern UI::IMGUI ui;
/*
	Robot ma w IK wbudowane sledzenie punktu z okresloną predkością, nie udaje się do zadanego punktu od razu. wiec jak interpolator wypluje kolejny punkt robot dojedzie do niego
	- ze "stałą" prędkością(po prostej)
	- wyznaczy ik tego punktu i poleci interpolacją złączową( choć bedzie mu to trudno zrobić(musiałby co klatkę liczyć ik tego punktu, w sumie dałoby to lepsze przybliżenie tego punktu) )


*/
extern float g_timeStep;

void RCTest(RobotController &rc){
	glm::vec4 p0(0, 4, 2, 1);
	glm::vec4 p1(0.5, 4, 4, 1);
	glm::vec4 p2(1.5, 4, 2, 1);
	glm::vec4 p3(3, 3, 4, 1);
	glm::vec4 p4(3, 4, 2, 1);
	glm::vec4 p5(4, 2, 4, 1);
	glm::vec4 p6(5, 3, 2, 1);
	glm::vec4 p7(6, 1, 4, 1);

	glm::vec4 c1(6, 1, 5, 1);
	glm::vec4 c2(1, -6, 6, 1);
	glm::vec4 c3(-6, -1, 6, 1);
	glm::vec4 c4(-1, 6, 6, 1);
	glm::vec4 c5(6, 1, 6, 1);
	glm::vec4 c6(1, -6, 6, 1);
	glm::vec4 c7(-6, -1, 6, 1);
	glm::vec4 c8(-1, 6, 6, 1);

	std::cout<<"Start test"<<std::endl;
	// rc.path(new BezierCurveNonUniform({p1, p2, p3, p4, p5, p1}, {0.5f, 0.5f, 1.5f, 2.f, 1.f, 0.5f}))();
	// rc.path(new BSpline({p0, p1, p2, p3, p4, p5, p6, p0}))();
	rc.move(new Linear({ c1, c2, c3, c4, c5 }), "move 1");
	rc.wait(5000);
	rc.move(new BSpline({ c5, c4, c2, c1 }), "move 2");
	rc.move(new BSpline({ c5, c4, c3, c2, c1 }), "move 3");
	rc.move(new BSpline({ c4, c3, c1, c2, c1 }), "move 4");

}

//TODO: Całą odpowiedzialność za wykonanie akcji i tworzenie przerzucić do commandsów, robotControler niech zajmuje się tylko przechowywaniem commandsów, i interfejsowaniem pomiedzy command a userem, i chuj

// załóżmy iż obsługujemy tylko tkomendę move, pomijamy tu pierdoły typu przyspieszenia, prędkości itp. (dojdą później)
void RobotController::run(){
	if (!commands.empty()){
		state = RCStates::Run;
		//commandIter = commands.begin();
		if(!(*commandIter)->isRuning){
			(*commandIter)->init(*this);
			logger::log<<"Wielka czarna dupa!"<<std::endl;

		}
	}
}
void RobotController::pause(){
	state = RCStates::Pause;
}
void RobotController::stop(){
	state = RCStates::Stop;
	commandIter = commands.begin();
	pause();
}
void RobotController::next(){
	//if (std::next(commandIter) != commands.end() && commandIter != commands.end())
		commandIter++;
		if (commandIter == commands.end())
			stop();
}
void RobotController::prev(){
	if (commandIter != commands.begin())
		commandIter--;
}
MoveCommand& RobotController::move(IInterpolator *interpolator, const std::string &name){
	MoveCommand *newCommand = new MoveCommand(interpolator);
	newCommand->name = name;
	newCommand->isRuning = false;
	commands.push_back(std::unique_ptr<ICommand>(newCommand));

	if (commandIter == commands.end())
		commandIter = commands.begin();

	newCommand->velocity = 0.1;
	newCommand->solver = new  JT1();

	return *newCommand;
}
WaitCommand& RobotController::wait(float time){
	WaitCommand *newCommand = new WaitCommand(time);
	newCommand->name = "wait: " + std::to_string((int)time/1000)+"s";
	newCommand->isRuning = false;
	commands.push_back(std::unique_ptr<ICommand>(newCommand));

	if (commandIter == commands.end())
		commandIter = commands.begin();

	return *newCommand;
}

bool WaitCommand::update(RobotController &rc, float dt){
	timeLeft -= dt;
	return timeLeft < 0.f;
}

bool RobotController::update(float dt){

	if(commandIter == commands.end()){
		stop();
	}

	if(state == RCStates::Pause)
		return false;

	if((*commandIter)->update(*this, dt)){
		++commandIter;
		(*commandIter)->init(*this);
		if (commandIter == commands.end()){
			stop();
		}
		return true;
	}
	return false;
}


void MoveCommand::init(RobotController &rc){
	isRuning = true;
	solver->solve(Point{ interpolator->firstPoint(), glm::quat(0, 0, 0, 1) }, *rc.robot);
	targetJointPosition = solver->result;
	rc.robot->isReady = false;
	logger::log<<"command "+name+" start"<<std::endl;
	std::cout<<targetJointPosition.size()<<std::endl;
	for(auto &it : targetJointPosition){
		std::cout<<"* "<<it<<" * "<<std::endl;
	}
}
double MoveCommand::calculateRequiredDistance(float dt){
	return dt*velocity;
}
glm::vec4 MoveCommand::calculateNextPoint(float dt){
	requiredDistance += calculateRequiredDistance(dt);
	glm::vec4 newTarget;
	std::cout<<requiredDistance<<std::endl;
	while(requiredDistance > 0.0 && (not interpolator->finished)){
		newTarget = interpolator->nextPoint();
		requiredDistance -= glm::distance(previousPoint, newTarget);
	}

	logger::log<<"Calculating new point"+glm::to_string(newTarget)<<std::endl;
	return newTarget;
}
bool MoveCommand::update(RobotController &rc, float dt){
	if(not rc.robot->isReady){
		rc.robot->goTo(targetJointPosition, dt);
		previousPoint = rc.robot->endEffector.position;
		return false;
	}
	if(rc.robot->isReady && interpolator->finished){
		interpolator->reset();
		return true;
	}

	glm::vec4 newTarget = calculateNextPoint(dt);

	solver->solve(Point{ newTarget, glm::quat(0, 0, 0, 1) }, *rc.robot);

	targetJointPosition = solver->result;
	rc.robot->isReady = false;

	return false;
}



