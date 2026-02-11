#ifndef CUSTOM_WINDOW_H
#define CUSTOM_WINDOW_H

#include <QMainWindow>
#include <QLabel>

// Forward declarations
class MainWindow; // Groot's MainWindow

class CustomWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CustomWindow(QWidget *parent = nullptr);
    virtual ~CustomWindow();

private:
    MainWindow* m_groot_window;
    QLabel* m_label;
};

#endif // CUSTOM_WINDOW_H
