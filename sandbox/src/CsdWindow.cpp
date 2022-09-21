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
#include <QTabWidget>
#include <QtWidgets>

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
	auto* content = new QTabWidget(this);

    // input widgets
    {
       auto* page = new QScrollArea(content);

       auto* root = new QWidget(page);
       page->setWidget(root);
       page->setWidgetResizable(true);
       page->setAlignment(Qt::AlignHCenter);
       auto* layout = new QVBoxLayout();
       root->setLayout(layout);

       auto addseg = [](const QString& caption, QWidget* parent) {
         auto* segTitle = new QWidget(parent);
         auto layout = new QHBoxLayout;
         segTitle->setLayout(layout);
         layout->addWidget(new QLabel(caption, segTitle));
         auto* hline = new QFrame(segTitle);
         hline->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
         hline->setFrameShape(QFrame::HLine);
         layout->addWidget(hline);
         return segTitle;
       };

       // push button
       {
         auto* widget = new QWidget(root);
         auto* llayout = new QHBoxLayout;
         widget->setLayout(llayout);

         auto* pushButton = new QPushButton("Normal", widget);
         llayout->addWidget(pushButton);

         auto* pushButton2 = new QPushButton("Disabled", widget);
         pushButton2->setDisabled(true);
         llayout->addWidget(pushButton2);

         auto* pushButton3 = new QPushButton("WithIcon", widget);
         pushButton3->setIcon(QIcon(QStringLiteral(":/plus_24.svg")));
         llayout->addWidget(pushButton3);

         auto* pushButton4 = new QPushButton("Flat", widget);
         pushButton4->setFlat(true);
         llayout->addWidget(pushButton4);

         layout->addWidget(addseg("QPushButton", root));
         layout->addWidget(widget);
       }

       // button group
       {
         auto* buttonGroup = new QButtonGroup(root);
         buttonGroup->addButton(new QCheckBox("Button1"));
         buttonGroup->addButton(new QCheckBox("Button2"));
         buttonGroup->setExclusive(true);
         layout->addWidget(addseg("QButtonGroup(checkbox list)", root));
         for (auto* button : buttonGroup->buttons()) {
           layout->addWidget(button);
         }
       }

       // calendar
       {
         auto* calendar = new QCalendarWidget(root);
         layout->addWidget(addseg("QCalendarWidget", root));
         layout->addWidget(calendar);
       }

       // checkbox
       {
         auto* widget = new QWidget(root);
         auto* llayout = new QHBoxLayout;
         widget->setLayout(llayout);

         auto* checkbox = new QCheckBox("Normal", widget);
         llayout->addWidget(checkbox);

         auto* checkbox2 = new QCheckBox("Disabled", widget);
         checkbox2->setDisabled(true);
         llayout->addWidget(checkbox2);

         auto* checkbox3 = new QCheckBox("WithIcon", widget);
         checkbox3->setIcon(QIcon(QStringLiteral(":/plus_24.svg")));
         llayout->addWidget(checkbox3);

         auto* checkbox4 = new QCheckBox("NoneCheckable", widget);
         checkbox4->setCheckable(false);
         llayout->addWidget(checkbox4);

         auto* checkbox5 = new QCheckBox("PartiallyChecked", widget);
         checkbox5->setCheckState(Qt::CheckState::PartiallyChecked);
         llayout->addWidget(checkbox5);

         layout->addWidget(addseg("QCheckBox", root));
         layout->addWidget(widget);
       }

       // QColorDialog
       {
         auto* widget = new QWidget(root);
         auto* llayout = new QHBoxLayout;
         widget->setLayout(llayout);

         auto* colorDialog = new QPushButton("QColorDialog", root);
         connect(colorDialog, &QPushButton::clicked, []() {
           QColorDialog dialog;
           dialog.exec();
           });
         llayout->addWidget(colorDialog);

         layout->addWidget(addseg("QDialog", root));
         layout->addWidget(widget);
       }

       {
         auto createCombo = [](QWidget* parent) {
           auto* widget = new QComboBox(parent);
           widget->addItems({ "North", "South", "West", "East" });
           widget->addItem(QIcon(QStringLiteral(":/plus_24.svg")), "Directions");
           return widget;
         };

         auto* widget = new QWidget(root);
         auto* llayout = new QHBoxLayout;
         widget->setLayout(llayout);

         auto* normal = createCombo(widget);
         llayout->addWidget(normal);
         auto* editable = createCombo(widget);
         editable->setEditable(true);
         llayout->addWidget(editable);

         layout->addWidget(addseg("QComboBox", root));
         layout->addWidget(widget);
       }

       {
         auto* widget = new QCommandLinkButton("ClickMe", "A vista style button", root);

         layout->addWidget(addseg("QCommandLinkButton", root));
         layout->addWidget(widget);
       }

       
       {
         auto* widget = new QDateEdit(root);

         layout->addWidget(addseg("QDateEdit", root));
         layout->addWidget(widget);
       }

       {
         auto* widget = new QDateTimeEdit(root);

         layout->addWidget(addseg("QDateTimeEdit", root));
         layout->addWidget(widget);
       }

       {
         auto* widget = new QDial(root);

         layout->addWidget(addseg("QDial", root));
         layout->addWidget(widget);
       }

       {
         auto* widget = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,root);
         layout->addWidget(addseg("QDialogButtonBox", root));
         layout->addWidget(widget);
       }

       layout->addStretch();
       content->addTab(page, "QtBuildIn");
    }

    // window color
    {
        auto* page = new QWidget(content);
        auto* verticalLayout = new QVBoxLayout(page);

        // Slider to modify the window background color.
        auto* slider = new QSlider(page);
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
        auto* checkbox = new QCheckBox("Use default window color", page);
        checkbox->setChecked(false);

        QObject::connect(checkbox, &QCheckBox::toggled, this, [this](bool checked) {
          _useDefaultColor = checked;
          update();
        });
        verticalLayout->addWidget(checkbox, 0, Qt::AlignmentFlag::AlignCenter);

        content->addTab(page, QStringLiteral("WindowColor"));
    }
  
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
