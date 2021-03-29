/*			Actions
 *			moveOut	getInGoal moveGlobe moveStar goalViaStar winRoad safety kill move noMoves
 *		home	Q(1,1)	
 *		goal
 *		globe
 * States *4	star
 *		winRoad	
 *		satefy
 *		freeSpace
 *		danger	
 * */

/*
 * Maybe create functions that can count how many times a state is visited and how many times
 * an action is taken - for statistics afterward
 */

#include <time.h>
#include <vector>
#include <fstream>

const int states = 32;
const int actions = 10;

class QTable{

	public:
		int getMaxQAction(int state){
			stateVisited[state] +=1;	//Log how many times a state is selected
			int maxVal = -1000;
			int actionIndex = -1;
			std::vector<int> indexes;
			for(int i = 0;i<actions;i++){
				if(QTable[state][i] == maxVal){	//This is an error
					indexes.push_back(i);
				}
				if(QTable[state][i] > maxVal){
					maxVal = QTable[state][i];
					actionIndex = i;
				}
			}
			if(indexes.size() > 1){
				int i = 0;
				srand(time(0));
				i = rand()%indexes.size();
				actionIndex = indexes[i];
			}
			return(actionIndex);
		}
		void setQVal(int state, int action, double val){
			QTable[state][action] = val;
		}
		double getQVal(int state, int action){
			return QTable[state][action];
		}
		void saveQ(){
			//Save Qtable to file
		}
		void restoreQ(){
			//Load Qtable from file
		}
	private:
		double QTable[states][actions] = {0};
		int stateVisited[states] = {0};
};
