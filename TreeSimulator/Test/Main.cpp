#include "InterestingWind.h"
#include <fstream>
#include "Translation.h"
#include "Rotation.h"
#include "Branch.h"
#include "Tree.h"
#define _USE_MATH_DEFINES
#include <math.h>

int main()
{
	InterestingWind wind;
	std::fstream file("test.sfa", std::ios::out | std::ios::trunc);

	//Branch branch(nullptr, 1.0, 0.02, 0, 45000000000.0, 30000000, 15000000000.0, 15000000, 740);
	//Tree tree(6, 0.5, 0.9, 0.1, 1, 1.5, 0.3, 0.95, 0.005, 0.05, 0.3, 3, 7, 90.0*M_PI / 180.0, 120.0*M_PI / 180.0,
		//45000000000.0, 30000000, 15000000000.0, 15000000, 740, 0);

	Tree tree(2, 0.5, 0.5, 0.1, 2, 2, 0.5, 0.5, 0.005, 0.1, 0.1, 2, 2, M_PI_2, M_PI_2,
		8000000000.0, 30000000, 15000000000.0, 15000000, 1000, 0);

	file << tree.simulate(1000, 940, 125, wind, 12.6, 30, 1000000);

	/*file << "TEST" << '\n';
	file << "INIT" << '\n';
	file << "cbru 255 255 255" << '\n';
	file << "cpen 0 0 0" << '\n';
	file << "eras 255 255 255" << '\n';
	file << "FRAME" << '\n';
	file << tree.printFrame(trunk, 125);
	file << wind.printWindProfile(origin, 12.6);
	file << wind.printCursor(origin, 6.23, 12.6);
	file << "END";*/

	return 0;
}