#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget * parent)
	:QMainWindow(parent), ui(new Ui::MainWindow), custom(NULL)
{
	ui->setupUi(this);
	ui->progressBar->setValue(0);

	QFile settings_file(QString("%1/config.ini").arg(QCoreApplication::applicationDirPath()));
	if(!settings_file.exists())
	{
		on_save_settings();
	}
	else
	{
		QSettings settings(QString("%1/config.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
		ui->path_exe->setText(settings.value("exe").toString());
		ui->path_tests->setText(settings.value("tests").toString());
		ui->path_diff->setText(settings.value("diff").toString());

		//ui->path_exe->setText("C:/Work/projects/cppgm/code/pa1/pa1-build-Qt_Mingw-Debug/debug/pa1.exe");
		//ui->path_tests->setText("C:/Work/projects/cppgm/tasks/pa1/tests");
	}

	on_analyze_new_options();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionExit_triggered()
{
	close();
}

void MainWindow::on_select_exe_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select Executable", QString(),  "Executable (*.exe)");

	if(!fileName.isNull())
		ui->path_exe->setText(fileName);

	on_analyze_new_options();

	on_save_settings();
}

void MainWindow::on_select_tests_dir_clicked()
{
	QString fileName = QFileDialog::getExistingDirectory(this, "Select CPPGM tests cases dir");

	if(!fileName.isNull())
		ui->path_tests->setText(fileName);

	on_analyze_new_options();

	on_save_settings();
}

void MainWindow::on_pushButton_3_clicked()
{
	on_actionRun_triggered();
}

void MainWindow::on_actionRun_triggered()
{
	if(ui->tabWidget->currentIndex() == 0)
		return;
	else if(ui->tabWidget->currentIndex() == 1)
	{
		executed_index = 0;
		ui->progressBar->setValue(0);

		if(process.count() == 0)
		{
			for(int i = 0; i < code_org.count(); ++i)
			{
				process.append(new QProcess(this));
				connect(process.at(i), SIGNAL(finished(int)), this, SLOT(on_processFinished(int)));
			}
		}
		else
		{
			for(int i = 0; i < code_org.count(); ++i)
			{
				process.at(i)->close();
			}
		}

		for(int i = 0; i < code_org.count(); ++i)
		{
			ui->cases_table->item(i, 1)->setBackgroundColor(QColor(255, 128, 128));
			ui->cases_table->item(i, 1)->setText("?");
		}

		if(code_org.count() > 0)
		{
			process.at(executed_index)->setStandardInputFile(code_org_filenames[executed_index]);
			process.at(executed_index)->start(ui->path_exe->text());
			//process.at(executed_index)->waitForStarted();
			//process.at(executed_index)->write(code_org.at(executed_index).toUtf8());

		}
		else
			ui->progressBar->setValue(100);
	}
	else if(ui->tabWidget->currentIndex() == 2)
	{
		if(!custom)
		{
			custom = new QProcess(this);
			connect(custom, SIGNAL(finished(int)), this, SLOT(on_process_custom_Finished(int)));
		}

		custom->start(ui->path_exe->text());
		custom->waitForStarted();
		custom->write(ui->custom_src->document()->toPlainText().toUtf8());
		custom->waitForBytesWritten();
		custom->closeWriteChannel();
	}
}

void MainWindow::on_processFinished(int res)
{
	Q_UNUSED(res);
	QString stok = process.at(executed_index)->readAllStandardOutput();
	QString sterr = process.at(executed_index)->readAllStandardError();
	code_test_res[executed_index] = (sterr.size() > 0) ? sterr : stok;

	process.at(executed_index)->close();

	ui->progressBar->setValue((float)executed_index / (float)code_org.count());

	QString a = code_org_res[executed_index];
	QString b = code_test_res[executed_index];

	b = b.remove('\r');

	bool failed = a != b;

	if(failed)
	{
		ui->cases_table->item(executed_index, 1)->setBackgroundColor(QColor(255, 128, 128));
		ui->cases_table->item(executed_index, 1)->setText("fail");
	}
	else
	{
		ui->cases_table->item(executed_index, 1)->setBackgroundColor(QColor(128, 255, 128));
		ui->cases_table->item(executed_index, 1)->setText("ok");
	}

	if(executed_index < code_org.count() - 1)
	{
		executed_index++;
		process.at(executed_index)->setStandardInputFile(code_org_filenames[executed_index]);
		process.at(executed_index)->start(ui->path_exe->text());
	}
	else
	{
		ui->progressBar->setValue(100);
	}
}

void MainWindow::on_cases_table_itemSelectionChanged()
{
	if(ui->cases_table->selectedItems().count() >= 1)
	{
		int row = ui->cases_table->selectedItems().first()->row();
		int column = ui->cases_table->selectedItems().first()->column();

		if(column == 0)
		{
			ui->text_original->setText(code_org.at(row));
			ui->text_current->setText("for results press on test column item");
		}
		else
		{
			ui->text_original->setText(code_org_res.at(row));
			ui->text_current->setText(code_test_res.at(row));
		}
	}
	else
	{
		ui->text_original->setText("");
		ui->text_current->setText("");
	}
}

void MainWindow::on_analyze_new_options()
{
	ui->cases_table->clear();

	code_org.clear();
	code_org_res.clear();
	code_test_res.clear();
	code_org_filenames.clear();

	QDir dir(ui->path_tests->text());
	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Name);

	QFileInfoList list = dir.entryInfoList();

	int count = 0, current = 0;

	for(int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		if(fileInfo.suffix() == "t")
		{
			count++;
		}
	}

	ui->cases_table->setColumnCount(2);
	ui->cases_table->setRowCount(count);

	for(int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		if(fileInfo.suffix() == "t")
		{
			ui->cases_table->setItem(current, 0, new QTableWidgetItem(fileInfo.baseName()));
			ui->cases_table->setItem(current, 1, new QTableWidgetItem("?"));

			ui->cases_table->item(current, 1)->setBackgroundColor(QColor(255, 128, 128));
			ui->cases_table->item(current, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

			current++;

			code_org_filenames.append(fileInfo.absoluteFilePath());

			QFile file(fileInfo.absoluteFilePath());
			if(file.open(QIODevice::ReadOnly | QIODevice::Text))
				code_org.append(QString(file.readAll()));
			else
				code_org.append(QString("cant open %1").arg(fileInfo.absoluteFilePath()));

			QString resFile = QString("%1/%2%3").arg(fileInfo.absolutePath()).arg(fileInfo.completeBaseName()).arg(".ref");
			QFile file2(resFile);
			if(file2.open(QIODevice::ReadOnly | QIODevice::Text))
				code_org_res.append(QString(file2.readAll()));
			else
				code_org_res.append(QString("cant open %1").arg(resFile));

			code_test_res.append("not executed yet");
		}
	}

	ui->cases_table->setColumnWidth(0, 250);
	ui->cases_table->setColumnWidth(1, 50);

	ui->progressBar->setValue(0);
}


void MainWindow::on_path_exe_editingFinished()
{
	on_save_settings();
}

void MainWindow::on_path_tests_editingFinished()
{
	on_save_settings();
	on_analyze_new_options();
}

void MainWindow::on_path_diff_editingFinished()
{
	on_save_settings();
}

void MainWindow::on_cases_table_clicked(const QModelIndex & index)
{
	Q_UNUSED(index);
}

void MainWindow::on_diffbutton_clicked()
{
	on_actionDiff_triggered();
}

void MainWindow::on_actionDiff_triggered()
{
	if(ui->cases_table->selectedItems().count() >= 1)
	{
		int row = ui->cases_table->selectedItems().first()->row();

		QString app = QCoreApplication::applicationDirPath();

		QFile file_res(QString("%1/diff_res.txt").arg(app));

		if(file_res.open(QIODevice::WriteOnly))
		{
			file_res.write(code_test_res.at(row).toUtf8());
			file_res.close();
		}
		else
			return;

		QFileInfo fileInfo(code_org_filenames.at(row));

		QString org_path = QString("%1/%2%3").arg(fileInfo.absolutePath()).arg(fileInfo.completeBaseName()).arg(".ref");

		QProcess * diff = new QProcess(this);
		diff->setWorkingDirectory(app);
		diff->start(QString("%1 %2 diff_res.txt").arg(ui->path_diff->text()).arg(org_path));
	}
}

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox::about(this, "CPPGM", "by jimon, 2013\njimon.j1m0n@gmail.com");
}

void MainWindow::on_custom_run_clicked()
{
	on_actionRun_triggered();
}

void MainWindow::on_process_custom_Finished(int res)
{
	Q_UNUSED(res);
	QString stok = custom->readAllStandardOutput();
	QString sterr = custom->readAllStandardError();
	QString result = (sterr.size() > 0) ? sterr : stok;
	ui->custom_res->setText(result);
}

void MainWindow::on_save_settings()
{
	QSettings settings(QString("%1/config.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
	settings.setValue("exe", ui->path_exe->text());
	settings.setValue("tests", ui->path_tests->text());
	settings.setValue("diff", ui->path_diff->text());
	settings.sync();
}

void MainWindow::on_select_tests_dir_2_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select Diff Executable", QString(),  "Executable (*.exe)");

	if(!fileName.isNull())
		ui->path_diff->setText(fileName);

	on_save_settings();
}
