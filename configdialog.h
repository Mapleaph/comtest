#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

signals:
    void sendConfigData(QStringList configData);

private slots:
    void on_btn_ok_clicked();

    void on_btn_auto_config_clicked();

private:
    Ui::ConfigDialog *ui;

};

#endif // CONFIGDIALOG_H
