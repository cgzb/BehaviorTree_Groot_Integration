#include "custom_window.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QWidget>
#include "bt_editor/mainwindow.h"
#include "bt_editor/bt_editor_base.h"

CustomWindow::CustomWindow(QWidget *parent) : QMainWindow(parent)
{
    // Set up the custom window's layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    m_label = new QLabel("Custom Application with Embedded Groot", this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    layout->addWidget(m_label);

    // Create Groot's MainWindow and embed it
    // Using GraphicMode::MONITOR to connect to the running tree
    // Default ports: 1666 (publisher), 1667 (server)
    // We pass 'this' as parent, so Qt manages memory.
    m_groot_window = new MainWindow(GraphicMode::MONITOR, "127.0.0.1", "1666", "1667", true, this);

    // Configure Groot window to behave like a widget
    // QMainWindow::setWindowFlags(Qt::Widget) removes window decorations and makes it embeddable
    m_groot_window->setWindowFlags(Qt::Widget);

    // Add to layout
    layout->addWidget(m_groot_window, 1);

    this->setCentralWidget(centralWidget);
    this->resize(1200, 800);
}

CustomWindow::~CustomWindow()
{
    // m_groot_window is a child of this window (or centralWidget's layout item),
    // so it will be deleted automatically by Qt object tree.
}
