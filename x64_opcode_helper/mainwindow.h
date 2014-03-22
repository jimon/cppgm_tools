#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget * parent = 0);
	~MainWindow();

private slots:
	#define ON_CB(__name) void on_##__name##_stateChanged(int arg1) {Q_UNUSED(arg1); build(); }
	#define ON_ComB(__name) void on_##__name##_currentIndexChanged(int arg1) {Q_UNUSED(arg1); build(); }
	#define ON_LE(__name) void on_##__name##_textChanged(const QString & arg1) {Q_UNUSED(arg1); build(); }

	ON_CB(prefix_0xf0)
	ON_CB(prefix_0xf2)
	ON_CB(prefix_0xf3)
	ON_CB(prefix_0x66)
	ON_CB(prefix_0x67)
	ON_CB(rex)
	ON_CB(rex_w)
	ON_CB(rex_r)
	ON_CB(rex_x)
	ON_CB(rex_b)
	ON_CB(opcode_0x0f)
	ON_CB(opcode_0x38)
	ON_CB(opcode_0x3a)
	ON_CB(modrm)
	ON_CB(sib)
	ON_ComB(modrm_mod)
	ON_ComB(modrm_reg)
	ON_ComB(modrm_rm)
	ON_ComB(sib_scale)
	ON_ComB(sib_index)
	ON_ComB(sib_base)
	ON_ComB(imm_size)
	ON_LE(opcode)
	ON_LE(imm)

	void on_instuction_textChanged(const QString & arg1);

private:
	Ui::MainWindow * ui;
	void build();
	void disasm(const QString & opcode);
};

#endif // MAINWINDOW_H
