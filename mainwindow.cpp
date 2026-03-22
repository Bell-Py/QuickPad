#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isModified(false)
{
    ui->setupUi(this);

    setWindowTitle("Untitled - QuickPad");
    statusBar()->showMessage("Готово");

    connect(ui->editor, &QPlainTextEdit::textChanged, this, [=]() {
        isModified = true;
    });

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


bool MainWindow::maybeSave()
{
    if (!isModified)
        return true;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Зберегти",
                                  "Документ змінено. Зберегти?",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes) {
        on_actionSave_triggered();
        return true;
    }
    else if (reply == QMessageBox::Cancel) {
        return false;
    }

    return true;
}

void MainWindow::saveToFile(const QString &fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << ui->editor->toPlainText();
        file.close();

        currentFile = fileName;
        setWindowTitle(currentFile + " - QuickPad");
        isModified = false;

        statusBar()->showMessage("Файл збережено", 2000);
    }
}


void MainWindow::on_actionNew_triggered()
{
    if (!maybeSave())
        return;

    ui->editor->clear();
    currentFile.clear();
    setWindowTitle("Untitled - QuickPad");
    isModified = false;
}

void MainWindow::on_actionOpen_triggered()
{
    if (!maybeSave())
        return;

    QString fileName = QFileDialog::getOpenFileName(this, "Відкрити файл");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        ui->editor->setPlainText(in.readAll());
        file.close();

        currentFile = fileName;
        setWindowTitle(currentFile + " - QuickPad");
        isModified = false;

        statusBar()->showMessage("Файл відкрито", 2000);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFile.isEmpty()) {
        on_actionSave_As_triggered();
        return;
    }

    saveToFile(currentFile);
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Зберегти як");

    if (fileName.isEmpty())
        return;

    saveToFile(fileName);
}

void MainWindow::on_actionExit_triggered()
{
    if (!maybeSave())
        return;

    close();
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



void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,
                             "About",
                             "QuickPad\nПростий текстовий редактор\nВерсія 1.0");
}
