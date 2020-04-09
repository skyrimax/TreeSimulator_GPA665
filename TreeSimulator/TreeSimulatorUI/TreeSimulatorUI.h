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

private:
	Ui::TreeSimulatorUIClass ui;
	QString filepath;
	QString currentDirectory;
};
