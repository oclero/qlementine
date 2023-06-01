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

    // Basic Widget
    {
       auto* page = new QScrollArea(content);

       auto* root = new QWidget(page);
       page->setWidget(root);
       page->setWidgetResizable(true);
       page->setAlignment(Qt::AlignHCenter);
       auto* layout = new QVBoxLayout();
       root->setLayout(layout);

       // QCheckBox
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

       // QComboBox
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

       // QCommandLinkButton
       {
         auto* widget = new QCommandLinkButton("ClickMe", "A vista style button", root);

         layout->addWidget(addseg("QCommandLinkButton", root));
         layout->addWidget(widget);
       }

       // QDateEdit
       {
         auto* widget = new QDateEdit(root);

         layout->addWidget(addseg("QDateEdit", root));
         layout->addWidget(widget);
       }

       // QDateTimeEdit
       {
         auto* widget = new QDateTimeEdit(root);

         layout->addWidget(addseg("QDateTimeEdit", root));
         layout->addWidget(widget);
       }

       // QDial
       {
         auto* widget = new QDial(root);

         layout->addWidget(addseg("QDial", root));
         layout->addWidget(widget);
       }

       // QDoubleSpinBox
       {
         auto* widget = new QDoubleSpinBox(root);

         layout->addWidget(addseg("QDoubleSpinBox", root));
         layout->addWidget(widget);

       }

       // QFontComboBox
       {
         auto* widget = new QFontComboBox(root);

         layout->addWidget(addseg("QFontComboBox", root));
         layout->addWidget(widget);
       }

       // QLCDNumber
       {
         auto* widget = new QLCDNumber(root);
         widget->setDigitCount(1000);
         widget->setMaximumWidth(200);

         layout->addWidget(addseg("QLCDNumber", root));
         layout->addWidget(widget);
       }

       // QLabel
       {
         auto* widget = new QLabel(root);
         widget->setText("This is a label");

         layout->addWidget(addseg("QLabel", root));
         layout->addWidget(widget);
       }

       // QLineEdit
       {
         auto* widget = new QWidget(root);
         auto* llayout = new QHBoxLayout;
         widget->setLayout(llayout);

         auto* normal = new QLineEdit(widget);
         llayout->addWidget(normal);
         auto* placeHold = new QLineEdit(widget);
         placeHold->setPlaceholderText("typing...");
         llayout->addWidget(placeHold);

         layout->addWidget(addseg("QLineEdit", root));
         layout->addWidget(widget);
       }

       // QMenu
       {
         auto* widget = new QMenuBar(root);
         auto* menu = new QMenu("Menu", widget);
         menu->addAction(QIcon(QStringLiteral(":/plus_24.svg")), "Item1");


         layout->addWidget(addseg("QMenu", root));
         layout->addWidget(widget);
       }

       // QProgressBar
       {
         auto* widget = new QProgressBar(root);
         widget->setValue(42);

         layout->addWidget(addseg("QProgressBar", root));
         layout->addWidget(widget);
       }

       // QPushButton
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

       // QRadioButton
       {
         auto* widget = new QWidget(root);
         auto* llayout = new QHBoxLayout;
         widget->setLayout(llayout);

         auto* radiobutton = new QRadioButton("Normal", widget);
         llayout->addWidget(radiobutton);

         auto* radiobutton2 = new QRadioButton("Disabled", widget);
         radiobutton2->setDisabled(true);
         llayout->addWidget(radiobutton2);

         auto* radiobutton3 = new QRadioButton("WithIcon", widget);
         radiobutton3->setIcon(QIcon(QStringLiteral(":/plus_24.svg")));
         llayout->addWidget(radiobutton3);

         auto* radiobutton4 = new QRadioButton("NoneCheckable", widget);
         radiobutton4->setCheckable(false);
         llayout->addWidget(radiobutton4);

         layout->addWidget(addseg("QRadioButton", root));
         layout->addWidget(widget);
       }

       // QScrollBar
       {
         auto* widget = new QScrollBar(Qt::Horizontal, root);

         layout->addWidget(addseg("QScrollBar", root));
         layout->addWidget(widget);
       }

       // QSlider
       {
         auto* widget = new QSlider(Qt::Horizontal, root);
         auto* widget2 = new QSlider(Qt::Vertical, root);

         layout->addWidget(addseg("QSlider", root));
         layout->addWidget(widget);
         layout->addWidget(widget2);
       }

       // QSpinBox
       {
         auto* widget = new QSpinBox(root);

         layout->addWidget(addseg("QSpinBox", root));
         layout->addWidget(widget);
       }

       // QTabBar
       {
         auto* widget = new QTabBar(root);
         widget->addTab("Page1");
         widget->addTab(QIcon(QStringLiteral(":/plus_24.svg")), "Page2");
         widget->addTab(QIcon(QStringLiteral(":/plus_24.svg")), "Page3");
         widget->setExpanding(false);

         layout->addWidget(addseg("QTabBar", root));
         layout->addWidget(widget);
       }

       // QTimeEdit
       {
         auto* widget = new QTimeEdit(root);

         layout->addWidget(addseg("QTimeEdit", root));
         layout->addWidget(widget);
       }

       // QToolBox
       {
         auto* widget = new QToolBox(root);
         widget->addItem(new QWidget(), QIcon(QStringLiteral(":/plus_24.svg")), "Item1");
         widget->addItem(new QWidget(), QIcon(QStringLiteral(":/plus_24.svg")), "Item2");

         layout->addWidget(addseg("QToolBox", root));
         layout->addWidget(widget);
       }

       // QToolButton
       {
         auto* widget = new QToolBar(root);

         auto icon = QIcon(QStringLiteral(":/plus_24.svg"));

         auto* toolbutton1 = new QToolButton(widget);
         toolbutton1->setIcon(icon);
         toolbutton1->setToolTip("with actions");
         toolbutton1->addAction(new QAction(icon, "Item1"));
         toolbutton1->addAction(new QAction(icon, "Item2"));
         widget->addWidget(toolbutton1);

         auto* toolbutton2 = new QToolButton(widget);
         toolbutton2->setIcon(icon);
         toolbutton2->setToolTip("QToolButton::DelayedPopup");
         toolbutton2->setPopupMode(QToolButton::DelayedPopup);
         widget->addWidget(toolbutton2);

         auto* toolbutton3 = new QToolButton(widget);
         toolbutton3->setIcon(icon);
         toolbutton3->setToolTip("QToolButton::MenuButtonPopup");
         toolbutton3->setPopupMode(QToolButton::MenuButtonPopup);
         widget->addWidget(toolbutton3);

         auto* toolbutton4 = new QToolButton(widget);
         toolbutton4->setIcon(icon);
         toolbutton4->setToolTip("QToolButton::InstantPopup");
         toolbutton4->setPopupMode(QToolButton::InstantPopup);
         widget->addWidget(toolbutton4);

         auto* toolbutton5 = new QToolButton(widget);
         toolbutton5->setIcon(icon);
         toolbutton5->setToolTip("Qt::UpArrow");
         toolbutton5->setArrowType(Qt::UpArrow);
         widget->addWidget(toolbutton5);

         auto* toolbutton6 = new QToolButton(widget);
         toolbutton6->setIcon(icon);
         toolbutton6->setToolTip("Qt::DownArrow");
         toolbutton6->setArrowType(Qt::DownArrow);
         widget->addWidget(toolbutton6);

         auto* toolbutton7 = new QToolButton(widget);
         toolbutton7->setIcon(icon);
         toolbutton7->setToolTip("Qt::LeftArrow");
         toolbutton7->setArrowType(Qt::LeftArrow);
         widget->addWidget(toolbutton7);

         auto* toolbutton8 = new QToolButton(widget);
         toolbutton8->setIcon(icon);
         toolbutton8->setToolTip("Qt::RightArrow");
         toolbutton8->setArrowType(Qt::RightArrow);
         widget->addWidget(toolbutton8);

         auto* toolbutton9 = new QToolButton(widget);
         toolbutton9->setIcon(icon);
         toolbutton9->setToolTip("AutoRaise enabled");
         toolbutton9->setAutoRaise(true);
         widget->addWidget(toolbutton9);

         layout->addWidget(addseg("QToolBox", root));
         layout->addWidget(widget);
       }

       layout->addStretch();
       content->addTab(page, "Basic Widget");
    }


    // Advanced Widget
    {
      auto* page = new QScrollArea(content);

      auto* root = new QWidget(page);
      page->setWidget(root);
      page->setWidgetResizable(true);
      page->setAlignment(Qt::AlignHCenter);
      auto* layout = new QVBoxLayout();
      root->setLayout(layout);

      // QCalendarWidget
      {
        auto* calendar = new QCalendarWidget(root);
        layout->addWidget(addseg("QCalendarWidget", root));
        layout->addWidget(calendar);
      }

       // QDialogButtonBox
      {
        auto* widget = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, root);
        layout->addWidget(addseg("QDialogButtonBox", root));
        layout->addWidget(widget);
      }


      // QDialog
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

        auto* fileDialog = new QPushButton("QFileDialog", root);
        connect(fileDialog, &QPushButton::clicked, []() {
          QFileDialog dialog;
          dialog.exec();
        });
        llayout->addWidget(fileDialog);

        auto* fontDialog = new QPushButton("QFontDialog", root);
        connect(fontDialog, &QPushButton::clicked, []() {
          QFontDialog dialog;
          dialog.exec();
        });
        llayout->addWidget(fontDialog);

        layout->addWidget(addseg("QDialog(s)", root));
        layout->addWidget(widget);
      }

      layout->addStretch();
      content->addTab(page, "Advanced Widget");
    }

    // Organizer Widget
    {
      auto* page = new QScrollArea(content);

      auto* root = new QWidget(page);
      page->setWidget(root);
      page->setWidgetResizable(true);
      page->setAlignment(Qt::AlignHCenter);
      auto* layout = new QVBoxLayout();
      root->setLayout(layout);

      // QButtonGroup
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

      // QGroupBox
      {
        auto* widget = new QGroupBox("Group", root);

        auto* llayout = new QHBoxLayout;
        widget->setLayout(llayout);

        llayout->addWidget(new QPushButton("Button1", widget));
        llayout->addWidget(new QPushButton("Button2", widget));

        layout->addWidget(addseg("QGroupBox", root));
        layout->addWidget(widget);
      }

      // QSplitter
      {
        auto* widget = new QSplitter(root);

        layout->addWidget(addseg("QSplitter", root));
        layout->addWidget(widget);
      }

       // QTabWidget
      {
        auto* widget = new QTabWidget(root);
        widget->addTab(new QLabel("Page1"), "Page1");
        widget->addTab(new QLabel("Page2"), QIcon(QStringLiteral(":/plus_24.svg")), "Page2");
        widget->addTab(new QLabel("Page3"), QIcon(QStringLiteral(":/plus_24.svg")), "Page3");

        layout->addWidget(addseg("QTabWidget", root));
        layout->addWidget(widget);
      }

      layout->addStretch();
      content->addTab(page, "Organizer Widget");
    }

    // Model/View
    {
      auto* page = new QScrollArea(content);

      auto* root = new QWidget(page);
      page->setWidget(root);
      page->setWidgetResizable(true);
      page->setAlignment(Qt::AlignHCenter);
      auto* layout = new QVBoxLayout();
      root->setLayout(layout);

      // QListWidget
      {
        auto* widget = new QListWidget(root);

        widget->addItems({ "Item1", "Item2", "Item3" });

        layout->addWidget(addseg("QListWidget", root));
        layout->addWidget(widget);
      }

      // QTableWidget
      {

        auto* widget = new QTableWidget(root);

        constexpr int rows = 100;
        constexpr int cols = 10;

        widget->setColumnCount(cols);
        widget->setRowCount(rows);

        for (int row = 0; row < rows; row++) {
          for (int col = 0; col < cols; col++) {
            widget->setItem(row, col, new QTableWidgetItem(QString::number(row) + ":" + QString::number(col)));
          }
        }

        layout->addWidget(addseg("QTableWidget", root));
        layout->addWidget(widget);
      }

      // QTreeView
      {
        auto* widget = new QTreeView(root);
        auto* model = new QFileSystemModel(root);
        widget->setModel(model);
        model->setRootPath(QCoreApplication::applicationDirPath());

        layout->addWidget(addseg("QTreeView", root));
        layout->addWidget(widget);
      }


      layout->addStretch();
      content->addTab(page, "Model/View");
    }

    // Main Window
    {
      auto* page = new QScrollArea(content);

      auto* root = new QWidget(page);
      page->setWidget(root);
      page->setWidgetResizable(true);
      page->setAlignment(Qt::AlignHCenter);
      auto* layout = new QVBoxLayout();
      root->setLayout(layout);


      // MainWindow
      {
        auto icon = QIcon(QStringLiteral(":/plus_24.svg"));

        auto* widget = new QMainWindow();
        auto* focus = new QFocusFrame();
        focus->setWidget(widget);

        auto* status = new QStatusBar(widget);
        status->addWidget(new QLabel("Status1"));
        status->addWidget(new QLabel("Status2"),100);
        status->addWidget(new QLabel("Status3"));
        widget->setStatusBar(status);

        auto* menu = new QMenuBar(widget);
        auto* fmenu = menu->addMenu("File");
        auto* emenu = menu->addMenu("Edit");
        auto* vmenu = menu->addMenu("View");
        menu->addMenu("Window");
        auto* hmenu = menu->addMenu("Help");
        widget->setMenuBar(menu);
        auto* anew = fmenu->addAction(icon, "New");
        auto* aopen = fmenu->addAction(icon, "Open");
        auto* asave = fmenu->addAction(icon, "Save");
        auto* asaveas = fmenu->addAction(icon, "Save As");
        fmenu->addSeparator();
        auto* aclose = fmenu->addAction(icon, "Close");
        auto* acopy = emenu->addAction(icon, "Copy");
        auto* acut = emenu->addAction(icon, "Cut");
        auto* apaste = emenu->addAction(icon, "Paste");
        auto* ahelp = hmenu->addAction(icon, "Help");
        auto* aabout= hmenu->addAction(icon, "About");

        auto* ftoolbar = widget->addToolBar("File");
        ftoolbar->setAllowedAreas(Qt::ToolBarArea::AllToolBarAreas);
        ftoolbar->setMovable(true);
        ftoolbar->setIconSize({ 24, 24 });
        ftoolbar->addActions({ anew, aopen, asave, asaveas, aclose });

        auto* etoolbar = widget->addToolBar("Edit");
        etoolbar->setAllowedAreas(Qt::ToolBarArea::AllToolBarAreas);
        etoolbar->setMovable(true);
        etoolbar->setIconSize({ 24, 24 });
        etoolbar->addActions({acopy,acut, apaste});

        auto* htoolbar = widget->addToolBar("Help");
        htoolbar->setAllowedAreas(Qt::ToolBarArea::AllToolBarAreas);
        htoolbar->setMovable(true);
        htoolbar->setIconSize({ 24, 24 });
        htoolbar->addActions({ ahelp, aabout });

        widget->setDockOptions(QMainWindow::DockOption::AllowTabbedDocks);
        auto* treeview = new QTreeView();
        auto* treemodel = new QFileSystemModel();
        treeview->setModel(treemodel);
        treemodel->setRootPath(QCoreApplication::applicationDirPath());
        auto* dock1 = new QDockWidget("Browser", widget);
        dock1->setWidget(treeview);
        dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        vmenu->addAction(dock1->toggleViewAction());
        widget->addDockWidget(Qt::RightDockWidgetArea, dock1);

        auto* docs = new QListWidget();
        docs->addItems(QStringList()
          << "A custom QStyle named QlementineStyle, that implements all the necessary API to give a modern look and feel to your Qt application. It's a drop-in replacement for the default QStyle."
          << "An actual way to have client-side decoration (CSD) on your Qt window, with actual OS window animations and effects. (Windows only, at the moment)"
          << "Lots of utilities to help you write beautiful Qt widgets.");
        docs->setWordWrap(true);
        auto* dock2 = new QDockWidget(tr("Features"), this);
        dock2->setWidget(docs);
        dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        vmenu->addAction(dock2->toggleViewAction());
        widget->addDockWidget(Qt::RightDockWidgetArea, dock2);

        
        auto* mdiarea = new QMdiArea(widget);
        widget->setCentralWidget(mdiarea);

        auto* subwnd1 = mdiarea->addSubWindow(new QTextEdit(), Qt::Window);
        subwnd1->setWindowTitle("Window1");
        auto* subwnd2 = mdiarea->addSubWindow(new QTextEdit(), Qt::Window);
        subwnd2->setWindowTitle("Window2");

        layout->addWidget(widget);
      }

      layout->addStretch();
      content->addTab(page, "Main Window ");
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
