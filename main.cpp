#include <iostream>
#include "QTable.h"

int main(){
	QTable Q;
	for(int i = 0; i<100; i++){
		std::cout << Q.getMaxQ(3) << "\n";
	}
	return 0;
}
