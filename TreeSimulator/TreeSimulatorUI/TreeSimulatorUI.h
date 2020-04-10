#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TreeSimulatorUI.h"

class TreeSimulatorUI : public QMainWindow
{
	Q_OBJECT

public:
	TreeSimulatorUI(QWidget *parent = Q_NULLPTR);

public slots:
	void generateSeed();
	void simulate();
	void selectOutputFile();
	void updateProgress(double progess);

	// slots to prevent min and max from scrossing
	void checkMinNbBranches(int maxNbBranches);
	void checkMaxNbBranches(int minNbBranches);

	void checkMinLengthRatio(double maxLengthRatio);
	void checkMaxLengthRatio(double minLengthRatio);

	void checkMinDiaRatio(double maxDiaRatio);
	void checkMaxDiaRatio(double minDiaRatio);

	void checkMinOpeningAngle(double maxOpeningAngle);
	void checkMaxOpeningAngle(double minOpeningAngle);

	void checkMinTrunkLength(double maxTrunkLength);
	void checkMaxTrunkLength(double minTrunkLength);

	void checkMinTrunkDia(double maxTrunkDia);
	void checkMaxTrunkDia(double minTrunkDia);

private:
	Ui::TreeSimulatorUIClass ui;
	QString filepath;
	QString currentDirectory;
};
