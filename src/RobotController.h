﻿#pragma once
#include "IInterpolator.h"
#include "Robot-Commands.h"
#include "Editor/MoveCommandBuilder.h"
#include "Editor/WaitCommandBuilder.h"
#include "Editor/ExecuteCommandBuilder.h"
#include <stack>

#define NAM_END }
#define NAM_START {

enum class CommandStatus : int {


};

enum class RCStates {
	Run, Pause, Stop,

};

struct RobotPosition
{
	string name;
	glm::vec4 position;
	glm::quat quat;
	vector<double> joints;
};

class RobotController
{
public:
	~RobotController(){
		std::cerr<<"delete RobotController"<<std::endl;
	}
	RobotController() {
		commandIter = commands.end();
	}

	bool update(shared_ptr<RobotController> &rc, shared_ptr<Scene> &scene, float dt);

	void useEffector();
	void releaseEffector();

	void insertCommand(shared_ptr<ICommand> &ptr){
		commands.push_back(ptr);
	}
	void insertCommand(shared_ptr<ICommand> ptr){
		commands.push_back(ptr);
	}
	std::shared_ptr<ICommand>& getCommand(){
		if(commandIter != commands.end())
			return *commandIter;
		// else if(!commands.empty())
		else
            // return std::shared_ptr<ICommand>(nullptr);
            return commands.front();
	}

	void run();
	void pause();
	void stop();
	void next();
	void prev();

	WaitCommandBuilder& wait(){
		return waitCommandBuilder.init();
	}
	MoveCommandBuilder& move(){
		return moveCommandBuilder.init();
	}
	SingleJointMoveCommandBuilder& jointMove(){
		return singleJointMoveCommandBuilder.init();
	}
	ExecuteCommandBuilder& exec(){
		return execBuiilder.init();
	}
	ExecutePythonCommandBuilder& pyExec(){
		return pyExecBuiilder.init();
	}
	FollowObjectBuilder& follow(){
		return followBuiilder.init();
	}

	std::string getComandName() const {
		if(commandIter != commands.end())
			return (*commandIter)->name;
		return "--";
	}
	void clean(){
		commands.clear();
		commandIter = commands.end();
	}

	/// zapisuje aktualną konfigurację robota na stosie
	void savePosition();
	/// zdejmuje konfigurację robota ze stosu, defaultowo odpala komendę na dotarcie tam
	void peekPosition();
	void popPosition();
	std::stack<RobotPosition> positionCache;

	/// ---- UTILS ----
	void grabObject(shared_ptr<Entity> &obj);
	void releaseObject();

	/// ---- UTILS ----
	std::vector<double> getRobotJ(){
		return robot->getVariables();
	}
	Robot& getRobot(){
		return *robot;
	}
	shared_ptr<Robot> robot;
	std::list<std::shared_ptr<ICommand>> commands;
	std::list<std::shared_ptr<ICommand>>::iterator commandIter;
	RCStates state = RCStates::Pause;
	// SystemSettings defaultSettings;

private:
	WaitCommandBuilder waitCommandBuilder;
	MoveCommandBuilder moveCommandBuilder;
	SingleJointMoveCommandBuilder singleJointMoveCommandBuilder;
	ExecuteCommandBuilder execBuiilder;
	ExecutePythonCommandBuilder pyExecBuiilder;
	FollowObjectBuilder followBuiilder;
};

void RCTest(RobotController &rc);

namespace RCUtils NAM_START

void pinObjectToEffector(shared_ptr<Entity> &obj, shared_ptr<Entity> &effector);
shared_ptr<Entity>& releaseObjects();
void update();

NAM_END