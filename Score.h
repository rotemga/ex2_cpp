#pragma once
#ifndef SCORE_H_
#define SCORE_H_

class Score {
	int position;
	int winnerNumSteps;
	int numSteps;
	int dirtCollected;
	int sumDirtInHouse;
	bool isBackInDocking;
public:
	Score();
	virtual ~Score();
	int calcResult();
	// should add operator overloading for <

	int getDirtCollected() const {
		return dirtCollected;
	}

	void setDirtCollected(int dirtCollected) {
		this->dirtCollected = dirtCollected;
	}

	bool isIsBackInDocking() const {
		return isBackInDocking;
	}

	void setIsBackInDocking(bool isBackInDocking) {
		this->isBackInDocking = isBackInDocking;
	}

	int getNumSteps() const {
		return numSteps;
	}

	void setNumSteps(int numSteps) {
		this->numSteps = numSteps;
	}

	int getPosition() const {
		return position;
	}

	void setPosition(int position) {
		this->position = position;
	}

	int getSumDirtInHouse() const {
		return sumDirtInHouse;
	}

	void setSumDirtInHouse(int sumDirtInHouse) {
		this->sumDirtInHouse = sumDirtInHouse;
	}

	int getWinnerNumSteps() const {
		return winnerNumSteps;
	}

	void setWinnerNumSteps(int winnerNumSteps) {
		this->winnerNumSteps = winnerNumSteps;
	}
};

#endif /* SCORE_H_ */

