#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTextCodec>

MainWindow::MainWindow(QWidget * parent)
	:QMainWindow(parent), ui(new Ui::MainWindow), custom(NULL), block_settings_save(false)
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
		block_settings_save = true;

		QSettings settings(QString("%1/config.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
		ui->path_exe->setText(settings.value("exe").toString());
		ui->pa5style->setChecked(settings.value("pa5style").toBool());
		ui->pa8style->setChecked(settings.value("pa8style").toBool());
		ui->path_tests->setText(settings.value("tests").toString());
		ui->path_diff->setText(settings.value("diff").toString());

		block_settings_save = false;

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
	QString fileName = QFileDialog::getExistingDirectory(this, "Select CPPGM tests cases dir", ui->path_tests->text());

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
			if(ui->pa8style->isChecked())
			{
				QStringList out_files;
				for(size_t i = 0; i < code_org_filecounts[executed_index]; ++i)
				{
					QString file = code_org_filecounts[executed_index] == 1 ? code_org_filenames[executed_index] : QString("%1.%2").arg(code_org_filenames[executed_index]).arg(i + 1);
					out_files << file;
					//qDebug(qPrintable(file));
				}

				process.at(executed_index)->start(ui->path_exe->text(), out_files);
			}
			else if(ui->pa5style->isChecked())
			{
				//process.at(executed_index)->start(ui->path_exe->text(), QStringList() << "-o" << "temp" << code_org_filenames[executed_index]);
				process.at(executed_index)->start(ui->path_exe->text(), QStringList() << code_org_filenames[executed_index]);
			}
			else
			{
				process.at(executed_index)->setStandardInputFile(code_org_filenames[executed_index]);
				process.at(executed_index)->start(ui->path_exe->text());
			}
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

	QString stok = "";
	if(ui->pa5style->isChecked())
	{
		// read file doesnt working ?
		//QFile output("temp");
		//if(output.open(QIODevice::ReadOnly))
		//{
		//	QByteArray text = output.readAll();
		//	stok = QTextCodec::codecForUtfText(text)->toUnicode(text);
		//	stok = "fuuu";
		//	output.close();
		//}
		//if(ui->pa8style->isChecked())
		//	stok = process.at(executed_index)->readAllStandardOutput().toHex();
		//else
			stok = process.at(executed_index)->readAllStandardOutput();
	}
	else
	{
		//if(ui->pa8style->isChecked())
		//	stok = process.at(executed_index)->readAllStandardOutput().toHex();
		//else
			stok = process.at(executed_index)->readAllStandardOutput();
	}
	QString sterr = process.at(executed_index)->readAllStandardError();
	code_test_res[executed_index] = stok;
	code_test_err_res[executed_index] = sterr;

	process.at(executed_index)->close();

	ui->progressBar->setValue((float)executed_index / (float)code_org.count());

	const QString& a = code_org_res[executed_index];
	const QString& b = code_test_res[executed_index];

	bool match = false;

	QString::const_iterator ref = a.constBegin();
	QString::const_iterator ref_end = a.constEnd();
	QString::const_iterator my = b.constBegin();
	QString::const_iterator my_end = b.constEnd();

	if (a.size() == 0 && b.size() == 0) {
		match = true;
	} else {
		while (my != my_end && ref != ref_end) {
			if (*my == '\r') {
				my++;
				if (my == my_end) {
					match = ref == ref_end;
					break;
				}
			}
			match = (*my == *ref);
			if (!match) {
				break;
			}
			my++;
			ref++;
		}

		if((my != my_end) || (ref != ref_end))
			match = false;
	}

	bool pa8errr = (ui->pa8style->isChecked() && code_org_is_err[executed_index]) || (!ui->pa8style->isChecked());

	if(sterr.size() && code_org_err_res[executed_index].size() && pa8errr)
	{
		match = true;
	}

	if(!match)
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

		if(ui->pa8style->isChecked())
		{
			QStringList out_files;
			for(size_t i = 0; i < code_org_filecounts[executed_index]; ++i)
			{
				QString file = code_org_filecounts[executed_index] == 1 ? code_org_filenames[executed_index] : QString("%1.%2").arg(code_org_filenames[executed_index]).arg(i + 1);
				out_files << file;
				//qDebug(qPrintable(file));
			}

			process.at(executed_index)->start(ui->path_exe->text(), out_files);
		}
		else if(ui->pa5style->isChecked())
		{
			//process.at(executed_index)->start(ui->path_exe->text(), QStringList() << "-o" << "temp" << code_org_filenames[executed_index]);
			process.at(executed_index)->start(ui->path_exe->text(), QStringList() << code_org_filenames[executed_index]);
		}
		else
		{
			process.at(executed_index)->setStandardInputFile(code_org_filenames[executed_index]);
			process.at(executed_index)->start(ui->path_exe->text());
		}
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
			ui->text_original->setPlainText(code_org.at(row));
			ui->err_original->setPlainText("");
			ui->text_current->setPlainText("for results press on test column item");
			ui->err_current->setPlainText("for results press on test column item");
		}
		else
		{
			ui->text_original->setPlainText(code_org_res.at(row));
			ui->text_current->setPlainText(code_test_res.at(row));
			ui->err_original->setPlainText(code_org_err_res.at(row));
			ui->err_current->setPlainText(code_test_err_res.at(row));
		}
	}
	else
	{
		ui->text_original->setPlainText("");
		ui->text_current->setPlainText("");
	}
}

void MainWindow::on_analyze_new_options()
{
	ui->cases_table->clear();

	code_org.clear();
	code_org_res.clear();
	code_org_err_res.clear();
	code_test_res.clear();
	code_test_err_res.clear();
	code_org_filenames.clear();

	QDir dir(ui->path_tests->text());
	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Name);

	QFileInfoList list = dir.entryInfoList();

	QMap<QString, uint32_t> files;
	QMap<QString, uint32_t> filesId;

	int count = 0, current = 0;

	for(int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		if(is_task_file(fileInfo.completeSuffix()))
		{
			if(files[fileInfo.baseName()] == 0)
				filesId[fileInfo.baseName()] = count++;
			files[fileInfo.baseName()]++;
		}
	}

	count = files.size();

	ui->cases_table->setColumnCount(2);
	ui->cases_table->setRowCount(count);

	for(int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		if(is_task_file(fileInfo.completeSuffix()))
		{
			current = filesId[fileInfo.baseName()];

			if(ui->cases_table->item(current, 0) == nullptr)
			{
				ui->cases_table->setItem(current, 0, new QTableWidgetItem(fileInfo.baseName()));
				ui->cases_table->setItem(current, 1, new QTableWidgetItem("?"));

				ui->cases_table->item(current, 1)->setBackgroundColor(QColor(255, 128, 128));
				ui->cases_table->item(current, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

				if(files[fileInfo.baseName()] > 1)
					code_org_filenames.append(QString("%1/%2").arg(fileInfo.absolutePath()).arg(fileInfo.completeBaseName()));
				else
					code_org_filenames.append(fileInfo.absoluteFilePath());
				code_org_filecounts.append(1);

				QFile file(fileInfo.absoluteFilePath());
				if(file.open(QIODevice::ReadOnly | QIODevice::Text))
					code_org.append(QString(file.readAll()));
				else
					code_org.append(QString("cant open %1").arg(fileInfo.absoluteFilePath()));

				if(files[fileInfo.baseName()] > 1)
					code_org.back() = QString("--- test : %1\n%2").arg(fileInfo.fileName()).arg(code_org.back());

				QString resFile = QString("%1/%2%3").arg(fileInfo.absolutePath()).arg(fileInfo.baseName()).arg(".ref");
				QFile file2(resFile);
				if(ui->pa8style->isChecked())
				{
					if(file2.open(QIODevice::ReadOnly))
					{
						code_org_is_err.append(false);
						code_org_res.append(QString(read_ref(file2.readAll())));
					}
					else
					{
						code_org_is_err.append(true);
						code_org_res.append(QString("cant open %1").arg(resFile));
					}
				}
				else
				{
					if(file2.open(QIODevice::ReadOnly | QIODevice::Text))
					{
						code_org_is_err.append(false);
						code_org_res.append(QString(file2.readAll()));
					}
					else
					{
						code_org_is_err.append(true);
						code_org_res.append(QString("cant open %1").arg(resFile));
					}
				}

				QString errFile = QString("%1/%2%3").arg(fileInfo.absolutePath()).arg(fileInfo.baseName()).arg(".ref.stderr");
				QFile file3(errFile);
				if(file3.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					code_org_err_res.append(QString(file3.readAll()));
				}
				else
				{
					//code_org_is_err.append(false);
					QString errFile = QString("%1/%2%3").arg(fileInfo.absolutePath()).arg(fileInfo.baseName()).arg(".ref.stdout");
					QFile file3_2(errFile);
					if(file3_2.open(QIODevice::ReadOnly | QIODevice::Text))
					{
						code_org_err_res.append(QString(file3_2.readAll()));
					}
					else
						code_org_err_res.append("reference stderr has not been specified");
				}

				code_test_res.append("not executed yet");
				code_test_err_res.append("not executed yet");
			}
			else
			{
				QString task_string;
				QFile file(fileInfo.absoluteFilePath());
				if(file.open(QIODevice::ReadOnly | QIODevice::Text))
					task_string = QString(file.readAll());
				else
					task_string = QString("cant open %1").arg(fileInfo.absoluteFilePath());

				code_org.back() = QString("%1\n--- test : %2\n%3").arg(code_org.back()).arg(fileInfo.fileName()).arg(task_string);

				code_org_filecounts[current]++;
			}
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

		if(ui->pa8style->isChecked())
		{
			QString fileBase = code_org_filecounts[row] == 1 ? code_org_filenames[row] : QString("%1.1").arg(code_org_filenames[row]);
			QFileInfo fileInfo(fileBase);

			QFile file_ref_org(QString("%1/%2%3").arg(fileInfo.absolutePath()).arg(fileInfo.baseName()).arg(".ref"));

			QString ref_org;
			if(file_ref_org.open(QIODevice::ReadOnly))
				ref_org = read_ref(file_ref_org.readAll());
			else
				return;

			QFile file_ref(QString("%1/diff_ref.txt").arg(app));

			if(file_ref.open(QIODevice::WriteOnly))
			{
				file_ref.write(ref_org.toUtf8());
				file_ref.close();
			}
			else
				return;

			QProcess * diff = new QProcess(this);
			diff->setWorkingDirectory(app);
			diff->start(QString("%1 diff_ref.txt diff_res.txt").arg(ui->path_diff->text()));
		}
		else
		{
			QFileInfo fileInfo(code_org_filenames.at(row));

			QString org_path = QString("%1/%2%3").arg(fileInfo.absolutePath()).arg(fileInfo.completeBaseName()).arg(".ref");

			QProcess * diff = new QProcess(this);
			diff->setWorkingDirectory(app);
			diff->start(QString("%1 %2 diff_res.txt").arg(ui->path_diff->text()).arg(org_path));
		}
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

	ui->custom_res->setPlainText(stok);
	ui->custom_err->setPlainText(sterr);
}

void MainWindow::on_save_settings()
{
	if(block_settings_save)
		return;

	QSettings settings(QString("%1/config.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
	settings.setValue("exe", ui->path_exe->text());
	settings.setValue("pa5style", ui->pa5style->isChecked());
	settings.setValue("pa8style", ui->pa8style->isChecked());
	settings.setValue("tests", ui->path_tests->text());
	settings.setValue("diff", ui->path_diff->text());
	settings.sync();
}

void MainWindow::on_select_tests_dir_2_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select Diff Executable", ui->path_diff->text(),  "Executable (*.exe)");

	if(!fileName.isNull())
		ui->path_diff->setText(fileName);

	on_save_settings();
}

void MainWindow::on_pa5style_stateChanged(int arg1)
{
	Q_UNUSED(arg1);
	on_save_settings();
}

void MainWindow::on_pa8style_stateChanged(int arg1)
{
	Q_UNUSED(arg1);
	on_save_settings();
}

bool MainWindow::is_task_file(QString suffix) const
{
	if(ui->pa8style->isChecked())
	{
		QRegExp rx(R"(t\.\d+)");
		return rx.exactMatch(suffix);
	}
	else
	{
		return suffix == "t";
	}
}

QString MainWindow::read_ref(QByteArray data) const
{
	if(!ui->pa8style->isChecked())
		return data;

	QByteArray temp;

	for(size_t i = 0; i < data.size(); ++i)
	{
		temp.append(QByteArray(1, data.at(i)).toHex());
		if(i < data.size() - 1)
			temp.append("\n");
	}

	return temp;
}
