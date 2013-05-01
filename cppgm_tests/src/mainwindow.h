#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QProcess>

namespace Ui
{
	class MainWindow;
}

class MainWindow:public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget * parent = 0);
	~MainWindow();

private slots:
	void on_actionExit_triggered();
	void on_select_exe_clicked();
	void on_select_tests_dir_clicked();
	void on_pushButton_3_clicked();
	void on_actionRun_triggered();
	void on_processFinished(int res);
	void on_cases_table_itemSelectionChanged();
	void on_analyze_new_options();
	void on_path_exe_editingFinished();
	void on_path_tests_editingFinished();
	void on_path_diff_editingFinished();
	void on_cases_table_clicked(const QModelIndex &index);
	void on_diffbutton_clicked();
	void on_actionDiff_triggered();
	void on_actionAbout_triggered();
	void on_custom_run_clicked();
	void on_process_custom_Finished(int res);
	void on_save_settings();

	void on_select_tests_dir_2_clicked();

private:
	Ui::MainWindow * ui;

	QList<QString> code_org;
	QList<QString> code_org_res;
	QList<QString> code_org_err_res;
	QList<QString> code_test_res;
	QList<QString> code_test_err_res;
	QList<QProcess*> process;
	QList<QString> code_org_filenames;
	QProcess * custom;
	int executed_index;
};

#endif
