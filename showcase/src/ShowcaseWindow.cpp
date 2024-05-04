#include "ShowcaseWindow.hpp"

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/utils/WidgetUtils.hpp>
#include <oclero/qlementine/widgets/LineEdit.hpp>
#include <oclero/qlementine/widgets/SegmentedControl.hpp>
#include <oclero/qlementine/widgets/NavigationBar.hpp>

#include <QPointer>
#include <QBoxLayout>
#include <QScrollArea>
#include <QToolBar>
#include <QDockWidget>
#include <QTreeView>
#include <QCheckBox>
#include <QMenuBar>
#include <QToolButton>
#include <QTabBar>
#include <QSplitter>
#include <QPainter>
#include <QStatusBar>
#include <QPushButton>

namespace oclero::qlementine::showcase {
class DummyWorkspace : public QWidget {
public:
  using QWidget::QWidget;

protected:
  void paintEvent(QPaintEvent* evt) override {
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
  }
};

struct ShowcaseWindow::Impl {
  ShowcaseWindow& owner;
  QPointer<QlementineStyle> qlementineStyle;

  QVBoxLayout* rootLayout{ nullptr };
  QMenuBar* menuBar{ nullptr };
  QTabBar* tabBar{ nullptr };
  QToolBar* toolBar{ nullptr };
  QSplitter* splitter{ nullptr };
  QWidget* leftPanel{ nullptr };
  QWidget* rightPanel{ nullptr };
  QWidget* workspace{ nullptr };
  QStatusBar* statusBar{ nullptr };

  struct Actions {
    QAction* newFile{ nullptr };
    QAction* openFile{ nullptr };
    QAction* saveFile{ nullptr };
    QAction* closeFile{ nullptr };
    QAction* preferences{ nullptr };
    QAction* quit{ nullptr };

    QAction* undo{ nullptr };
    QAction* redo{ nullptr };
    QAction* cut{ nullptr };
    QAction* copy{ nullptr };
    QAction* paste{ nullptr };
    QAction* delete_{ nullptr };
  } actions;

  Impl(ShowcaseWindow& o)
    : owner(o)
    , actions({}) {}

  void setupUI() {
    setupMenuBar();
    setupTabBar();
    setupToolBar();
    setupLeftPanel();
    setupRightPanel();
    setupWorkspace();
    setupSplitter();
    setupStatusBar();
    setupLayout();
  }

  void setupMenuBar() {
    menuBar = new QMenuBar(nullptr);
    const auto cb = []() {};

    {
      auto* menu = menuBar->addMenu("File");
      {
        // TODO: Use the enum provided by Qt6 instead of strings for icon IDs.
        menu->addAction(QIcon::fromTheme("document-new"), "New", cb, QKeySequence::StandardKey::New);
        menu->addAction(QIcon::fromTheme("document-open"), "Open...", cb, QKeySequence::StandardKey::Open);

        auto* recentFilesMenu = menu->addMenu("Recent Files");
        for (auto i = 0; i < 5; ++i) {
          recentFilesMenu->addAction(
            QIcon::fromTheme("document-open-recent"), QString("Recent File %1").arg(i + 1), cb, QKeySequence{});
        }

        menu->addSeparator();
        menu->addAction(QIcon::fromTheme("document-save"), "Save", cb, QKeySequence::StandardKey::Save);
        menu->addAction(QIcon::fromTheme(""), "Close", cb, QKeySequence::StandardKey::Close);
        menu->addAction(QIcon::fromTheme("document-print"), "Print...", cb, QKeySequence::StandardKey::Print);

        menu->addSeparator();
        menu->addAction(QIcon::fromTheme(""), "Preferences...", cb, QKeySequence::StandardKey::Preferences);

        menu->addSeparator();
        menu->addAction(
          QIcon::fromTheme("application-exit"), "Quit",
          []() {
            qApp->quit();
          },
          QKeySequence::StandardKey::Quit);
      }
    }
    {
      auto* menu = menuBar->addMenu("Edit");
      {
        menu->addAction(QIcon::fromTheme("edit-undo"), "Undo", cb, QKeySequence::StandardKey::Undo);
        menu->addAction(QIcon::fromTheme("edit-redo"), "Redo", cb, QKeySequence::StandardKey::Redo);

        menu->addSeparator();
        menu->addAction(QIcon::fromTheme("edit-cut"), "Cut", cb, QKeySequence::StandardKey::Cut);
        menu->addAction(QIcon::fromTheme("edit-copy"), "Copy", cb, QKeySequence::StandardKey::Copy);
        menu->addAction(QIcon::fromTheme("edit-paste"), "Paste", cb, QKeySequence::StandardKey::Paste);
        menu->addAction(QIcon::fromTheme("edit-delete"), "Delete", cb, QKeySequence::StandardKey::Delete);
      }
    }
    {
      auto* menu = menuBar->addMenu("Help");
      {
        menu->addAction(QIcon::fromTheme("mail-send"), "Contact", cb, QKeySequence{});
        menu->addAction(QIcon::fromTheme(""), "About...", cb, QKeySequence{});
      }
    }
  }

  void setupTabBar() {
    tabBar = new QTabBar(&owner);
    tabBar->setDocumentMode(true);
    tabBar->setFocusPolicy(Qt::NoFocus);
    tabBar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    tabBar->setTabsClosable(true);
    tabBar->setMovable(false);
    tabBar->setExpanding(false);
    tabBar->setChangeCurrentOnDrag(true);
    tabBar->setUsesScrollButtons(true);

    qlementineStyle->setAutoIconColor(tabBar, oclero::qlementine::AutoIconColor::None);

    for (auto i = 0; i < 4; ++i) {
      tabBar->addTab(QIcon(), QString("Document %1").arg(i + 1));
    }

    QObject::connect(tabBar, &QTabBar::tabCloseRequested, tabBar, [this](int index) {
      tabBar->removeTab(index);
    });
  }

  void setupToolBar() {
    toolBar = new QToolBar("App ToolBar", &owner);
    toolBar->setBackgroundRole(QPalette::ColorRole::Window);
    toolBar->setAutoFillBackground(false);
    toolBar->setAllowedAreas(Qt::ToolBarArea::TopToolBarArea);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonFollowStyle);

    {
      auto* toolButton = new QToolButton(toolBar);
      toolButton->setFocusPolicy(Qt::NoFocus);
      toolButton->setIcon(QIcon::fromTheme("edit-undo"));
      toolBar->addWidget(toolButton);
    }

    {
      auto* toolButton = new QToolButton(toolBar);
      toolButton->setFocusPolicy(Qt::NoFocus);
      toolButton->setIcon(QIcon::fromTheme("edit-undo"));
      toolBar->addWidget(toolButton);
    }
  }

  void setupLeftPanel() {
    auto* widget = new QWidget(&owner);
    leftPanel = widget;
    leftPanel->setMinimumWidth(200);
    leftPanel->setMaximumWidth(400);

    auto* layout = new QVBoxLayout(widget);
    layout->setContentsMargins({ 0, 0, 0, 0 });
    layout->setSpacing(0);

    {
      auto* topBar = new QWidget(widget);
      layout->addWidget(topBar);
      auto* topBarLayout = new QHBoxLayout(topBar);
      topBarLayout->setContentsMargins({ 12, 8, 12, 8 });

      auto* lineEdit = new LineEdit(widget);
      lineEdit->setClearButtonEnabled(true);
      lineEdit->setPlaceholderText("Search...");
      topBarLayout->addWidget(lineEdit, 1);

      auto* button = new QPushButton(widget);
      button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      topBarLayout->addWidget(button);
    }

    {
      auto* navBar = new NavigationBar(widget);
      layout->addWidget(navBar);
      navBar->setItemsShouldExpand(true);
      navBar->addItem("Objects", QIcon(), QString("%1").arg(12));
      navBar->addItem("Materials", QIcon(), QString("%1").arg(3));
    }

    layout->addWidget(makeHorizontalLine(widget));

    auto* treeView = new QTreeView(widget);
    layout->addWidget(treeView, 1);
  }

  void setupRightPanel() {
    auto* widget = new QWidget(&owner);
    rightPanel = widget;
    rightPanel->setMinimumWidth(200);
    rightPanel->setMaximumWidth(400);

    auto* layout = new QVBoxLayout(widget);
    layout->setContentsMargins({ 0, 0, 0, 0 });
    layout->setSpacing(0);

    {
      auto* topBar = new QWidget(widget);
      layout->addWidget(topBar);
      auto* topBarLayout = new QHBoxLayout(topBar);
      topBarLayout->setContentsMargins({ 12, 8, 12, 8 });

      {
        auto* segmentedControl = new SegmentedControl(topBar);
        topBarLayout->addWidget(segmentedControl);
        segmentedControl->setItemsShouldExpand(false);
        segmentedControl->addItem("Properties", QIcon(), QString("%1").arg(4));
        segmentedControl->addItem("Scene", QIcon(), QString("%1").arg(2));
      }
    }

    layout->addWidget(makeHorizontalLine(widget));

    {
      auto* scrollArea = new QScrollArea(widget);
      layout->addWidget(scrollArea);
      auto* content = new QWidget(scrollArea);
      auto* contentLayout = new QVBoxLayout(content);
      contentLayout->setContentsMargins({ 16, 12, 16, 12 });
      {
        auto* checkBox = new QCheckBox("Some checkbox", content);
        contentLayout->addWidget(checkBox);
      }
    }
  }

  void setupWorkspace() {
    workspace = new DummyWorkspace(&owner);
    workspace->setFocusPolicy(Qt::StrongFocus);
    workspace->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  }

  void setupSplitter() {
    splitter = new QSplitter(&owner);
    splitter->setHandleWidth(1);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(leftPanel);
    splitter->addWidget(workspace);
    splitter->addWidget(rightPanel);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 6);
    splitter->setStretchFactor(2, 1);
  }

  void setupStatusBar() {
    statusBar = new QStatusBar(&owner);
    statusBar->setSizeGripEnabled(false);
  }

  void setupLayout() {
    rootLayout = new QVBoxLayout(&owner);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->setMenuBar(menuBar);
    rootLayout->addWidget(tabBar);
    rootLayout->addWidget(toolBar);
    rootLayout->addWidget(splitter);
    rootLayout->addWidget(statusBar);
    workspace->setFocus(Qt::NoFocusReason);
  }

  void setupShortcuts() {
    // TODO
  }
};

ShowcaseWindow::ShowcaseWindow(QWidget* parent)
  : QWidget(parent)
  , _impl(new Impl(*this)) {
  setWindowIcon(QIcon(QStringLiteral(":/showcase/qlementine_icon.ico")));
  _impl->setupUI();
  _impl->setupShortcuts();
  setMinimumSize(600, 400);
  resize(800, 600);
  oclero::qlementine::centerWidget(this);
}

ShowcaseWindow::~ShowcaseWindow() = default;

void ShowcaseWindow::setCustomStyle(QlementineStyle* style) {
  _impl->qlementineStyle = style;
}
} // namespace oclero::qlementine::showcase
