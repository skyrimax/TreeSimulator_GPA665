#include "TreeSimulatorUI.h"

#include <QFileDialog>

#include <immintrin.h>
#include <fstream>
#include <math.h>

#include "Tree.h"
#include "InterestingWind.h"

TreeSimulatorUI::TreeSimulatorUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.randomSeedPushButton, SIGNAL(clicked()), this, SLOT(generateSeed()));
	connect(ui.simulatePushButton, SIGNAL(clicked()), this, SLOT(simulate()));
	connect(ui.fileSelectorButtonOutput, SIGNAL(clicked()), this, SLOT(selectOutputFile()));

	connect(ui.maxNbBranchesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkMinNbBranches(int)));
	connect(ui.minNbBranchesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkMaxNbBranches(int)));

	connect(ui.maxLengthRatioDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMinLengthRatio(double)));
	connect(ui.minLengthRatioDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMaxLengthRatio(double)));

	connect(ui.maxDiaRatioDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMinDiaRatio(double)));
	connect(ui.minDiaRatioDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMaxDiaRatio(double)));

	connect(ui.maxOpeningAngleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMinOpeningAngle(double)));
	connect(ui.minOpeningAngleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMaxOpeningAngle(double)));

	connect(ui.maxTrunkLengthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMinTrunkLength(double)));
	connect(ui.minTrunkLengthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMaxTrunkLength(double)));

	connect(ui.maxTrunkDiaDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMinTrunkDia(double)));
	connect(ui.minTrunkDiaDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(checkMaxTrunkDia(double)));

	ui.statusBar->showMessage(QString(tr("Specify an output file")));
}

void TreeSimulatorUI::generateSeed()
{
	unsigned long long  seed;
	_rdseed64_step(&seed);

	ui.seedSpinBox->setValue(abs((int)seed));
}

void TreeSimulatorUI::simulate()
{
	ui.statusBar->showMessage(QString(tr("Creating tree...")));
	QStringList resolution(ui.resolutionComboBox->currentText().split(" ", QString::SplitBehavior::SkipEmptyParts));

	Tree* tree = new Tree(ui.maxNbLvlsSpinBox->value(), ui.minLengthRatioDoubleSpinBox->value(), ui.maxLengthRatioDoubleSpinBox->value(),
		ui.minBranchLengthDoubleSpinBox->value(), ui.minTrunkLengthDoubleSpinBox->value(), ui.maxTrunkLengthDoubleSpinBox->value(),
		ui.minDiaRatioDoubleSpinBox->value(), ui.maxDiaRatioDoubleSpinBox->value(), ui.minBranchDiaDoubleSpinBox->value(),
		ui.minTrunkDiaDoubleSpinBox->value(), ui.maxTrunkDiaDoubleSpinBox->value(), ui.minNbBranchesSpinBox->value(),
		ui.maxNbBranchesSpinBox->value(), ui.minOpeningAngleDoubleSpinBox->value()*M_PI / 180.0, ui.maxOpeningAngleDoubleSpinBox->value()*M_PI / 180.0,
		ui.youngModulusDoubleSpinBox->value()*1000000000.0, ui.ultimateTensileStrengthDoubleSpinBox->value()*1000000.0,
		ui.shearModulusDoubleSpinBox->value()*1000000000.0, ui.ultimateShearStrengthDoubleSpinBox->value()*1000000.0,
		ui.densityDoubleSpinBox->value(), (unsigned long long)ui.seedSpinBox->value());

	connect(tree, SIGNAL(finishedThick(double)), this, SLOT(updateProgress(double)));

	int width = resolution.at(0).toInt();
	int height = resolution.at(resolution.size() - 1).toInt() - 133;

	std::fstream file(filepath.toStdString(), std::ios::out | std::ios::trunc);

	ui.statusBar->showMessage(QString("Simulation in progress..."));

	file << tree->simulate(width, height, ui.pixPerMeterSpinBox->value(), InterestingWind(),
		ui.simTimeDoubleSpinBox->value(), ui.fpsSpinBox->value(), ui.tickRateSpinBox->value());

	delete tree;

	ui.statusBar->showMessage(QString("Simulation done"));
}

void TreeSimulatorUI::selectOutputFile()
{
	QString selectedFile(QFileDialog::getSaveFileName(this, QString(tr("Save file")),
		currentDirectory, QString("Animation Files (*.sfa)")));

	if (selectedFile != filepath) {
		filepath = selectedFile;

		if (filepath.isEmpty()) {
			ui.outputFilePathLabel->setText("-");
			ui.outputFilePathLabel->setToolTip("");

			ui.simulatePushButton->setEnabled(false);
			ui.statusBar->showMessage(QString(tr("Specify an output file")));
		}
		else {
			if (filepath.length() <= 50) {
				ui.outputFilePathLabel->setText(QString("...") + filepath.right(50));
			}
			else {
				ui.outputFilePathLabel->setText(filepath);
			}
			ui.outputFilePathLabel->setToolTip(filepath);

			ui.simulatePushButton->setEnabled(true);
			ui.statusBar->showMessage(QString("Ready"));
		}
	}
}

void TreeSimulatorUI::updateProgress(double progess)
{
	ui.statusBar->showMessage(QString("Simulation in progress...%1%%").arg(progess, 0, 'f', 10));
}

void TreeSimulatorUI::checkMinNbBranches(int maxNbBranches)
{
	if (ui.minNbBranchesSpinBox->value() > maxNbBranches) {
		ui.minNbBranchesSpinBox->setValue(maxNbBranches);
	}
}

void TreeSimulatorUI::checkMaxNbBranches(int minNbBranches)
{
	if (ui.maxNbBranchesSpinBox->value() < minNbBranches) {
		ui.maxNbBranchesSpinBox->setValue(minNbBranches);
	}
}

void TreeSimulatorUI::checkMinLengthRatio(double maxLengthRatio)
{
	if (ui.minLengthRatioDoubleSpinBox->value() > maxLengthRatio) {
		ui.minLengthRatioDoubleSpinBox->setValue(maxLengthRatio);
	}
}

void TreeSimulatorUI::checkMaxLengthRatio(double minLengthRatio)
{
	if (ui.maxLengthRatioDoubleSpinBox->value() < minLengthRatio) {
		ui.maxLengthRatioDoubleSpinBox->setValue(minLengthRatio);
	}
}

void TreeSimulatorUI::checkMinDiaRatio(double maxDiaRatio)
{
	if (ui.minDiaRatioDoubleSpinBox->value() > maxDiaRatio) {
		ui.minDiaRatioDoubleSpinBox->setValue(maxDiaRatio);
	}
}

void TreeSimulatorUI::checkMaxDiaRatio(double minDiaRatio)
{
	if (ui.maxDiaRatioDoubleSpinBox->value() < minDiaRatio) {
		ui.maxDiaRatioDoubleSpinBox->setValue(minDiaRatio);
	}
}

void TreeSimulatorUI::checkMinOpeningAngle(double maxOpeningAngle)
{
	if (ui.minOpeningAngleDoubleSpinBox->value() > maxOpeningAngle) {
		ui.minOpeningAngleDoubleSpinBox->setValue(maxOpeningAngle);
	}
}

void TreeSimulatorUI::checkMaxOpeningAngle(double minOpeningAngle)
{
	if (ui.maxOpeningAngleDoubleSpinBox->value() < minOpeningAngle) {
		ui.maxOpeningAngleDoubleSpinBox->setValue(minOpeningAngle);
	}
}

void TreeSimulatorUI::checkMinTrunkLength(double maxTrunkLength)
{
	if (ui.minTrunkLengthDoubleSpinBox->value() > maxTrunkLength) {
		ui.minTrunkLengthDoubleSpinBox->setValue(maxTrunkLength);
	}
}

void TreeSimulatorUI::checkMaxTrunkLength(double minTrunkLength)
{
	if (ui.maxTrunkLengthDoubleSpinBox->value() < minTrunkLength) {
		ui.maxTrunkLengthDoubleSpinBox->setValue(minTrunkLength);
	}
}

void TreeSimulatorUI::checkMinTrunkDia(double maxTrunkDia)
{
	if (ui.minTrunkDiaDoubleSpinBox->value() > maxTrunkDia) {
		ui.minTrunkDiaDoubleSpinBox->setValue(maxTrunkDia);
	}
}

void TreeSimulatorUI::checkMaxTrunkDia(double minTrunkDia)
{
	if (ui.maxTrunkDiaDoubleSpinBox->value() < minTrunkDia) {
		ui.maxTrunkDiaDoubleSpinBox->setValue(minTrunkDia);
	}
}
