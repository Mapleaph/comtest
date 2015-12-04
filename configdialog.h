#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    void InitBaudrateComboBox(QComboBox* box);

signals:
    void sendConfigData(QStringList configData);

private slots:
    void on_btn_ok_clicked();

private:
    Ui::ConfigDialog *ui;

};

#endif // CONFIGDIALOG_H
