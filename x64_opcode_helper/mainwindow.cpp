#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "beaengine/BeaEngine.h"

MainWindow::MainWindow(QWidget * parent)
	:QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	build();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_instuction_textChanged(const QString & arg1)
{
	disasm(arg1);
}

void MainWindow::build()
{
	ui->prefix_0xf0->setEnabled(!(ui->prefix_0xf2->isChecked() || ui->prefix_0xf3->isChecked()));
	ui->prefix_0xf2->setEnabled(!(ui->prefix_0xf0->isChecked() || ui->prefix_0xf3->isChecked()));
	ui->prefix_0xf3->setEnabled(!(ui->prefix_0xf0->isChecked() || ui->prefix_0xf2->isChecked()));

	ui->opcode_0x38->setEnabled(ui->opcode_0x0f->isChecked() && (!ui->opcode_0x3a->isChecked()));
	ui->opcode_0x3a->setEnabled(ui->opcode_0x0f->isChecked() && (!ui->opcode_0x38->isChecked()));

	ui->rex_b->setEnabled(ui->rex->isChecked());
	ui->rex_w->setEnabled(ui->rex->isChecked());
	ui->rex_r->setEnabled(ui->rex->isChecked());
	ui->rex_x->setEnabled(ui->rex->isChecked());

	ui->modrm_mod->setEnabled(ui->modrm->isChecked());
	ui->modrm_reg->setEnabled(ui->modrm->isChecked());
	ui->modrm_rm->setEnabled(ui->modrm->isChecked());

	ui->sib_scale->setEnabled(ui->sib->isChecked());
	ui->sib_index->setEnabled(ui->sib->isChecked());
	ui->sib_base->setEnabled(ui->sib->isChecked());

	QString out;

	if(ui->prefix_0xf0->isChecked()) out += "F0 ";
	if(ui->prefix_0xf2->isChecked()) out += "F2 ";
	if(ui->prefix_0xf3->isChecked()) out += "F3 ";
	if(ui->prefix_0x66->isChecked()) out += "66 ";
	if(ui->prefix_0x67->isChecked()) out += "67 ";

	if(ui->rex->isChecked())
	{
		uint8_t byte = 0x40;
		if(ui->rex_w->isChecked()) byte += 0x8;
		if(ui->rex_r->isChecked()) byte += 0x4;
		if(ui->rex_x->isChecked()) byte += 0x2;
		if(ui->rex_b->isChecked()) byte += 0x1;
		out += QString().sprintf("%02X", byte).toUpper() + " ";
	}

	if(ui->opcode_0x0f->isChecked()) out += "0F ";
	if(ui->opcode_0x38->isChecked()) out += "38 ";
	if(ui->opcode_0x3a->isChecked()) out += "3A ";

	{
		uint8_t byte = ui->opcode->text().toInt(0, 16);
		out += QString().sprintf("%02X", byte) + " ";
	}

	if(ui->modrm->isChecked())
	{
		uint8_t byte = 0x0;
		byte += ui->modrm_mod->currentText().toInt(0, 2) * 64;
		byte += ui->modrm_reg->currentText().toInt(0, 2) * 8;
		byte += ui->modrm_rm->currentText().toInt(0, 2);
		out += QString().sprintf("%02X", byte) + " ";
	}

	if(ui->sib->isChecked())
	{
		uint8_t byte = 0x0;
		byte += ui->sib_scale->currentText().toInt(0, 2) * 64;
		byte += ui->sib_index->currentText().toInt(0, 2) * 8;
		byte += ui->sib_base->currentText().toInt(0, 2);
		out += QString().sprintf("%02X", byte) + " ";
	}

	{
		qulonglong byte8 = ui->imm->text().replace(" ", "").toULongLong(0, 16);

		for(uint8_t i = 0; i < ui->imm_size->currentText().mid(0, 1).toInt(); ++i)
			out += QString().sprintf("%02X", ((uint8_t*)&byte8)[i]) + " ";
	}

	ui->instuction->setText(out);

	disasm(ui->instuction->text());
}

void MainWindow::disasm(const QString & opcode)
{
	QVector<uint8_t> bytes;

	for(size_t i = 0; i < (size_t)opcode.size();)
	{
		uint8_t byte = 0;
		for(size_t j = 0; j < 2; ++j, ++i)
		{
			if(i >= (size_t)opcode.size())
				break;
			if(*(opcode.data() + i) == ' ')
				break;
			uint8_t v = opcode.mid(i, 1).toInt(0, 16);
			if(j == 0)
				byte = v;
			else
				byte = byte * 16 + v;
		}
		while((*(opcode.data() + i) == ' ') && (i < (size_t)opcode.size()))
			++i;
		bytes.push_back(byte);
	}

	QString output;

	output += "instuction bytes :<br>";
	for(int i = 0; i < bytes.size(); ++i)
		output += QString().number(bytes[i], 16) + ((i + 1 < bytes.size()) ? " " : "<br>");

	output += "decoded instuction :<br>";

	DISASM m;
	memset(&m, 0, sizeof(m));
	m.EIP = (uintptr_t)bytes.data();
	m.Archi = 64;
	Disasm(&m);
	output += m.CompleteInstr;


	ui->plainTextEdit->document()->setHtml(output);
}


