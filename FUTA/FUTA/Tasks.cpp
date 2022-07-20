#include "Tasks.h"
#include "MathGeoLib/src/Algorithm/Random/LCG.h"

double GenerateID() {

	LCG generatorID;
	return ((float)generatorID.Int() - 1.0) + generatorID.Float();

}

Tasks::Tasks() : taskID(GenerateID()), importance(0), progressionState(0), effort(0), started(false), completed(false), deadline(false),
name("NewTask"), description(""), initialDate(""), finalDate(""), subtaskList() {

	description.reserve(250);
	name.reserve(75);

}

Tasks::~Tasks() {}


