#pragma once
#include "../Robot-Commands.h"

class MoveCommandBuilder
{
public:
	// unique_ptr<ICommand> get(){
		// return moveCommand.release();
	// }
	MoveCommandBuilder& init(){
		moveCommand = make_unique<MoveCommand>();
		return *this;
	}
	MoveCommandBuilder& velocity(double value){
		moveCommand->velocity = value;
		return *this;
	}
	MoveCommandBuilder& acceleration(double value){
		moveCommand->acceleration = value;
		return *this;
	}
	MoveCommandBuilder& time(float value){
		moveCommand->time = value;
		return *this;
	}
	MoveCommandBuilder& interpolator(IInterpolator *value){
		// moveCommand->interpolator = value;
		return *this;
	}
	MoveCommandBuilder& interpolator(Interpolator value){
		// build interpolator from enum
		return *this;
	}
	MoveCommandBuilder& solver(IIK *value){
		// moveCommand->solver = value;
		return *this;
	}

	MoveCommandBuilder(){}
	~MoveCommandBuilder(){}
	unique_ptr<MoveCommand> moveCommand;
private:
	/// i referencj� do RC, �eby m�c doda�, wtedy RC::move zwraca ten builder :D
};

class ICommandBuilderWidget
{
public:
	virtual ~ICommandBuilderWidget(){}
	virtual void run() = 0;
	// virtual void setCommand() = 0;
};

class MoveCommandBuilderWidget : public ICommandBuilderWidget
{
public:
	MoveCommandBuilderWidget(){
		init();
	}
	void init(){
		moveCommandBuilder = make_unique<MoveCommandBuilder>();
		moveCommandBuilder->init();
	}
	void run();
	void editName();
	void editVelocity();
	void editAcceleration();
	void editTime();;
	void editInterpolator();
	void editSolver();

private:
	std::string fieldValue {""};

	unique_ptr<MoveCommandBuilder> moveCommandBuilder;
};

