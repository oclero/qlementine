#include "CsdWindow.hpp"

#include <QApplication>
#include <QMenuBar>
#include <QPainter>
#include <QScreen>
#include <QSlider>
#include <QTimer>
#include <QWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <QMessageBox>
#include <QCheckBox>

CsdWindow::CsdWindow(QWidget* parent)
	: oclero::qlementine::FramelessWindow(parent) {
	setWindowIcon(QIcon(":/qlementine_icon.ico"));
	setupUi();
	resize(600, 400);
	setWindowTitle("Custom native window");
	populateMenuBar(menuBar());
}

void CsdWindow::paintEvent(QPaintEvent* event) {
	if (_useDefaultColor) {
		FramelessWindow::paintEvent(event);
	} else {
		QPainter painter(this);
		painter.fillRect(rect(), _backgroundColor);
	}
}

void CsdWindow::setupUi() {
	auto* content = new QWidget(this);
	auto* verticalLayout = new QVBoxLayout(content);

	// Slider to modify the window background color.
	auto* slider = new QSlider(content);
	slider->setRange(0, 255);
	slider->setValue(_backgroundColor.red());
	QObject::connect(slider, &QSlider::valueChanged, this, [this](int value) {
		_backgroundColor.setRed(value);
		if (!_useDefaultColor) {
			update();
		}
	});
	slider->setMinimumWidth(255);
	slider->setMaximumWidth(350);
	verticalLayout->addWidget(slider, 0, Qt::AlignmentFlag::AlignCenter);

	// Checkbox to use or not the default background color.
	auto* checkbox = new QCheckBox("Use default window color", content);
	checkbox->setChecked(false);

	QObject::connect(checkbox, &QCheckBox::toggled, this, [this](bool checked) {
		_useDefaultColor = checked;
		update();
	});
	verticalLayout->addWidget(checkbox, 0, Qt::AlignmentFlag::AlignCenter);

	setContentWidget(content);
}

void CsdWindow::populateMenuBar(QMenuBar* menuBar) {
	auto* fileMenu = new QMenu("&File", menuBar);
	{
		auto* quitAction = new QAction("&Quit", fileMenu);
		quitAction->setMenuRole(QAction::MenuRole::QuitRole);
		quitAction->setShortcut(Qt::CTRL + Qt::Key_Q);
		quitAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
		QObject::connect(quitAction, &QAction::triggered, this, []() { QApplication::quit(); });
		fileMenu->addAction(quitAction);
	}

	menuBar->addMenu(fileMenu);

	auto* windowMenu = new QMenu("&Window", menuBar);
	{
		auto minimizeAction = new QAction("Minimize", windowMenu);
		QObject::connect(minimizeAction, &QAction::triggered, this, [this]() { windowHandle()->showMinimized(); });
		windowMenu->addAction(minimizeAction);

		auto* maximizeAction = new QAction("Maximize", windowMenu);
		QObject::connect(maximizeAction, &QAction::triggered, this, [this]() {
			if (auto window = this->window()) {
				if (window->windowState() & Qt::WindowMaximized) {
					window->showNormal();
				} else {
					window->showMaximized();
				}
			}
		});
		windowMenu->addAction(maximizeAction);

		auto* closeAction = new QAction("&Close", windowMenu);
		QObject::connect(closeAction, &QAction::triggered, this, [this]() { windowHandle()->close(); });
		windowMenu->addAction(closeAction);
	}
	menuBar->addMenu(windowMenu);

	auto* helpMenu = new QMenu("&Help", menuBar);
	{
		auto* aboutAction = new QAction("&About", windowMenu);
		aboutAction->setMenuRole(QAction::AboutRole);
		QObject::connect(aboutAction, &QAction::triggered, this, [this]() {
			QMessageBox msgBox(QMessageBox::Icon::Information, "About", "Example of frameless window", QMessageBox::NoButton, this);
			msgBox.exec();
		});
		helpMenu->addAction(aboutAction);
	}
	menuBar->addMenu(helpMenu);
}
