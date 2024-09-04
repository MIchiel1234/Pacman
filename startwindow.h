#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

namespace Ui {
class StartWindow; // Forward declaration of Ui::StartWindow
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private slots:
    void on_startButton_clicked();
    void on_hostButton_clicked();
    void on_joinButton_clicked();

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
