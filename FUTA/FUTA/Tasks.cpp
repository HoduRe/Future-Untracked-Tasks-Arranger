#include "Tasks.h"
#include "MathGeoLib (edited)/src/Algorithm/Random/LCG.h"

double GenerateID() {

	LCG generatorID;
	return ((float)generatorID.Int() - 1.0) + generatorID.Float();

}

Tasks::Tasks() : taskID(GenerateID()), importance(0), progressionState(0), effort(0), started(false), completed(false), deadline(false),
name("ANewTask"), description(""), initialDate(""), finalDate(""), subtaskList() {

}

Tasks::~Tasks() {}


