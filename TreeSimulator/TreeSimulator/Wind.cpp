#include "Wind.h"

#include <sstream>

Wind::Wind()
{
}

Wind::~Wind()
{
}

std::string Wind::printWindProfile(TransformationMatrix & originOfProfile, double tTotal)
{
	std::stringstream profilStr;

	double minX = getMinX(0, tTotal);
	double maxX = getMaxX(0, tTotal);
	double minY = getMinY(0, tTotal);
	double maxY = getMaxY(0, tTotal);

	// Creation of the axis for speed of wind in x and y over t
	// Base size X=484 pixels, Y=291 pixels
	// Will assume a res of 1000x200 to accumodate 1366x768 screens

	// Positions of label on the vertical axis
	Vector posV(25, 0);

	// Positions for the vertical axis
	Vector tailVertAxis(15, 95);
	Vector tipVertAxis(15, 5);
	Vector arrowLeft(10, 10);
	Vector arrowRight(20, 10);

	// Positions for the time axis of the x component
	int heightXAxis;

	if (minX > 0) {
		heightXAxis = 90;
	}
	else if (maxX < 0) {
		heightXAxis = 15;
	}
	else {
		heightXAxis = 50;
	}

	// Position of labels on x horizontal axis
	Vector posTx(980, heightXAxis);
	Vector pos0x(5, heightXAxis);
	// Positions for the x horizontal axix
	Vector tailHoriAxisX(5, heightXAxis);
	Vector tipHoriAxisX(995, heightXAxis);
	Vector arrowTopX(990, heightXAxis - 5);
	Vector arrowBottomX(990, heightXAxis + 5);

	// Positions for the time axis of the y component
	int heightYAxis;

	if (minY > 0) {
		heightYAxis = 90;
	}
	else if (maxY < 0) {
		heightYAxis = 15;
	}
	else {
		heightYAxis = 50;
	}

	// Scaling ratios to fit the whole 
	double scaleX;
	double scaleY;
	// Scaling of the X axis
	if (maxX == 0 || minX == 0) {
		scaleX = 1;
	}
	else if (abs(maxX) < abs(minX)) {
		scaleX = abs(45.0 / minX);
	}
	else {
		scaleX = abs(45.0 / maxX);
	}
	// Scaling of the Y axix
	if (maxY == 0 || minY == 0) {
		scaleY = 1;
	}
	else if (abs(maxY) < abs(minY)) {
		scaleY = abs(45.0 / minY);
	}
	else {
		scaleY = abs(45.0 / maxY);
	}

	// Position of labels on y horizontal axis
	Vector posTy(980, heightXAxis);
	Vector pos0y(5, heightXAxis);
	// Positions for the y horizontal axix
	Vector tailHoriAxisY(5, heightYAxis);
	Vector tipHoriAxisY(995, heightYAxis);
	Vector arrowTopY(990, heightYAxis - 5);
	Vector arrowBottomY(990, heightYAxis + 5);

	// Translation matrix to place the axes of the axes
	Translation transYComp(0, 100);

	// Start of writhing the directives for the axes
	// Labels of axes for x component
	profilStr << "dtxt " << originOfProfile * pos0x << " " << 0 << '\n';
	profilStr << "dtxt " << originOfProfile * posV << " vx(m/s)" << '\n';
	profilStr << "dtxt " << originOfProfile * posTx << "t(s)" << '\n';
	// Vertical axis for x component
	profilStr << "dlin " << originOfProfile * tipVertAxis << " " << originOfProfile * tailVertAxis << '\n';
	profilStr << "dlin " << originOfProfile * tipVertAxis << " " << originOfProfile * arrowLeft << '\n';
	profilStr << "dlin " << originOfProfile * tipVertAxis << " " << originOfProfile * arrowRight << '\n';
	// Horizontal axis for x component
	profilStr << "dlin " << originOfProfile * tipHoriAxisX << " " << originOfProfile * tailHoriAxisX << '\n';
	profilStr << "dlin " << originOfProfile * tipHoriAxisX << " " << originOfProfile * arrowTopX << '\n';
	profilStr << "dlin " << originOfProfile * tipHoriAxisX << " " << originOfProfile * arrowBottomX << '\n';

	//Labels of axes for y component
	profilStr << "dtxt " << originOfProfile * transYComp*pos0y << " " << 0 << '\n';
	profilStr << "dtxt " << originOfProfile * transYComp*posV << " " << "vy(m/s)" << '\n';
	profilStr << "dtxt " << originOfProfile * transYComp* posTy << "t(s)" << '\n';
	// Vertical axis for y component
	profilStr << "dlin " << originOfProfile * transYComp * tipVertAxis << " " << originOfProfile * transYComp * tailVertAxis << '\n';
	profilStr << "dlin " << originOfProfile * transYComp * tipVertAxis << " " << originOfProfile * transYComp * arrowLeft << '\n';
	profilStr << "dlin " << originOfProfile * transYComp * tipVertAxis << " " << originOfProfile * transYComp * arrowRight << '\n';
	// Horizontal axis for y component
	profilStr << "dlin " << originOfProfile *transYComp * tipHoriAxisY << " " << originOfProfile * transYComp * tailHoriAxisY << '\n';
	profilStr << "dlin " << originOfProfile * transYComp * tipHoriAxisY << " " << originOfProfile * transYComp * arrowTopY << '\n';
	profilStr << "dlin " << originOfProfile * transYComp * tipHoriAxisY << " " << originOfProfile * transYComp * arrowBottomY << '\n';

	// Wind profile as points;
	for (int i = 0; i < 980; ++i) {
		profilStr << "dpnt " << originOfProfile * Vector(i + 15, -scaleX * (*this)(i*tTotal / 980.0).getX() + heightXAxis) << '\n';
		profilStr << "dpnt " << originOfProfile * transYComp * Vector(i + 15, -scaleY * (*this)(i*tTotal*980.0).getY() + heightYAxis) << '\n';
	}

	return profilStr.str();
}

std::string Wind::printCursor(TransformationMatrix & originOfProfile, double t, double tTotal)
{
	std::stringstream cursorStr;

	Vector top(t * 980 / tTotal + 15, 0);
	Vector buttom(t * 980 / tTotal + 15, 200);

	cursorStr << "dlin " << originOfProfile * top << " " << originOfProfile * buttom << '\n';

	return cursorStr.str();
}

double Wind::getMaxX(double begin, double end)
{
	double max = 0;
	double cur;

	for (double t = begin; t <= end; t = t + 0.001) {

		cur = (*this)(t).getX();

		if (cur > max) {
			max = cur;
		}
	}

	return max;
}

double Wind::getMinX(double begin, double end)
{
	double min = DBL_MAX;
	double cur;

	for (double t = begin; t <= end; t = t + 0.001) {
		
		cur = (*this)(t).getX();

		if (cur < min) {
			min = cur;
		}
	}

	return min;
}

double Wind::getMaxY(double begin, double end)
{
	double max = 0;
	double cur;

	for (double t = begin; t <= end; t = t + 0.001) {

		cur = (*this)(t).getY();

		if (cur > max) {
			max = cur;
		}
	}

	return max;
}

double Wind::getMinY(double begin, double end)
{
	double min = DBL_MAX;
	double cur;

	for (double t = begin; t <= end; t = t + 0.001) {

		cur = (*this)(t).getY();

		if (cur < min) {
			min = cur;
		}
	}

	return min;
}
