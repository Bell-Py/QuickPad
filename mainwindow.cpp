#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusBar()->showMessage("Готово");

    connect(ui->editor, &QPlainTextEdit::cursorPositionChanged, this, [=]() {
        QTextCursor cursor = ui->editor->textCursor();
        int line = cursor.blockNumber() + 1;
        int col = cursor.columnNumber() + 1;

        statusBar()->showMessage(
            "Line " + QString::number(line) +
            ", Col " + QString::number(col)
            );
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,
                             "About",
                             "QuickPad\nПростий текстовий редактор\nВерсія 1.0");
}

void MainWindow::on_actionNew_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         "New Document",
                                         "Введіть назву документа:",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);

    if (ok && !name.isEmpty()) {
        QMessageBox::information(this,
                                 "Створено",
                                 "Новий документ: " + name);
    }
}

void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Exit",
                                  "Ви впевнені, що хочете вийти?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::on_actionCut_triggered()
{
    ui->editor->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->editor->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->editor->paste();
}
