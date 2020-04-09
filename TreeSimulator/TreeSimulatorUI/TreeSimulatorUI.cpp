#include "TreeSimulatorUI.h"

#include <QFileDialog>

#include <immintrin.h>
#include <fstream>

#include "Tree.h"
#include "InterestingWind.h"

TreeSimulatorUI::TreeSimulatorUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.randomSeedPushButton, SIGNAL(clicked()), this, SLOT(generateSeed()));
	connect(ui.simulatePushButton, SIGNAL(clicked()), this, SLOT(simulate()));
	connect(ui.fileSelectorButtonOutput, SIGNAL(clicked()), this, SLOT(selectOutputFile()));

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

	Tree tree(ui.maxNbLvlsSpinBox->value(), ui.minLengthRatioDoubleSpinBox->value(), ui.maxLengthRatioDoubleSpinBox->value(),
		ui.minLengthRatioDoubleSpinBox->value(), ui.minTrunkLengthDoubleSpinBox->value(), ui.maxTrunkLengthDoubleSpinBox->value(),
		ui.minDiaRatioDoubleSpinBox->value(), ui.maxDiaRatioDoubleSpinBox->value(), ui.minBranchDiaDoubleSpinBox->value(),
		ui.minTrunkDiaDoubleSpinBox->value(), ui.minTrunkDiaDoubleSpinBox->value(), ui.minNbBranchesSpinBox->value(),
		ui.maxNbBranchesSpinBox->value(), ui.minOpeningAngleDoubleSpinBox->value(), ui.maxOpeningAngleDoubleSpinBox->value(),
		ui.youngModulusDoubleSpinBox->value(), ui.ultimateTensileStrengthDoubleSpinBox->value(),
		ui.shearModulusDoubleSpinBox->value(), ui.ultimateShearStrengthDoubleSpinBox->value(),
		ui.densityDoubleSpinBox->value(), (unsigned long long)ui.seedSpinBox->value());

	int width = resolution.at(0).toInt();
	int height = resolution.at(resolution.size() - 1).toInt();

	std::fstream file(filepath.toStdString(), std::ios::out | std::ios::trunc);

	ui.statusBar->showMessage(QString("Simulation in progress..."));

	file << tree.simulate(width, height, ui.pixPerMeterSpinBox->value(), InterestingWind(),
		ui.simTimeDoubleSpinBox->value(), ui.fpsSpinBox->value(), ui.tickRateSpinBox->value());

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
