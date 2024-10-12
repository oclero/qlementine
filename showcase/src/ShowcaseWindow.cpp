#include "ShowcaseWindow.hpp"

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/utils/WidgetUtils.hpp>
#include <oclero/qlementine/widgets/LineEdit.hpp>
#include <oclero/qlementine/widgets/SegmentedControl.hpp>
#include <oclero/qlementine/widgets/NavigationBar.hpp>
#include <oclero/qlementine/utils/IconUtils.hpp>

#include <oclero/qlementine/icons/Icons16.hpp>

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
#include <QTreeWidget>
#include <QStackedWidget>
#include <QListWidget>
#include <QStyledItemDelegate>

namespace oclero::qlementine::showcase {
using Icons16 = oclero::qlementine::icons::Icons16;

static QIcon makeQIcon(Icons16 id, const QSize& size = QSize(16, 16)) {
  return oclero::qlementine::makeThemedIcon(id, size);
}

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
        menu->addAction(makeQIcon(Icons16::Document_New), "New", QKeySequence::StandardKey::New, cb);
        menu->addAction(makeQIcon(Icons16::Document_Open), "Open...", QKeySequence::StandardKey::Open, cb);

        auto* recentFilesMenu = menu->addMenu(makeQIcon(Icons16::Document_OpenRecent), "Recent Files");
        for (auto i = 0; i < 5; ++i) {
          recentFilesMenu->addAction(
            makeQIcon(Icons16::File_File), QString("Recent File %1").arg(i + 1), QKeySequence{}, cb);
        }

        menu->addSeparator();
        menu->addAction(makeQIcon(Icons16::Action_Save), "Save", QKeySequence::StandardKey::Save, cb);
        menu->addAction(makeQIcon(Icons16::Action_Close), "Close", QKeySequence::StandardKey::Close, cb);
        menu->addAction(makeQIcon(Icons16::Action_Print), "Print...", QKeySequence::StandardKey::Print, cb);
        menu->addAction(makeQIcon(Icons16::Action_PrintPreview), "Print Preview...", QKeySequence{}, cb);

        menu->addSeparator();
        menu->addAction(
          makeQIcon(Icons16::Navigation_Settings), "Preferences...", QKeySequence::StandardKey::Preferences, cb);

        menu->addSeparator();
        menu->addAction(makeQIcon(Icons16::Action_Close), "Quit", QKeySequence::StandardKey::Quit, []() {
          qApp->quit();
        });
      }
    }
    {
      auto* menu = menuBar->addMenu("Edit");
      {
        menu->addAction(makeQIcon(Icons16::Action_Undo), "Undo", QKeySequence::StandardKey::Undo, cb);
        menu->addAction(makeQIcon(Icons16::Action_Redo), "Redo", QKeySequence::StandardKey::Redo, cb);

        menu->addSeparator();
        menu->addAction(makeQIcon(Icons16::Action_Cut), "Cut", QKeySequence::StandardKey::Cut, cb);
        menu->addAction(makeQIcon(Icons16::Action_Copy), "Copy", QKeySequence::StandardKey::Copy, cb);
        menu->addAction(makeQIcon(Icons16::Action_Paste), "Paste", QKeySequence::StandardKey::Paste, cb);
        menu->addAction(makeQIcon(Icons16::Action_Trash), "Delete", QKeySequence::StandardKey::Delete, cb);
      }
    }
    {
      auto* menu = menuBar->addMenu("View");
      {
        menu->addAction(makeQIcon(Icons16::Action_ZoomIn), "Zoom In", QKeySequence::StandardKey::ZoomIn, cb);
        menu->addAction(makeQIcon(Icons16::Action_ZoomOut), "Zoom Out", QKeySequence::StandardKey::ZoomOut, cb);
        menu->addAction(makeQIcon(Icons16::Action_ZoomFit), "Fit", QKeySequence{}, cb);

        menu->addSeparator();
        menu->addAction(
          makeQIcon(Icons16::Action_Fullscreen), "Full Screen", QKeySequence::StandardKey::FullScreen, cb);
      }
    }
    {
      auto* menu = menuBar->addMenu("Help");
      {
        menu->addAction(makeQIcon(Icons16::Misc_Mail), "Contact", QKeySequence{}, cb);
        menu->addAction(makeQIcon(Icons16::Misc_Info), "About...", QKeySequence{}, cb);
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

    qlementineStyle->setAutoIconColor(tabBar, oclero::qlementine::AutoIconColor::ForegroundColor);

    for (auto i = 0; i < 4; ++i) {
      tabBar->addTab(makeQIcon(Icons16::File_File), QString("Document %1").arg(i + 1));
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
      toolButton->setIcon(makeQIcon(Icons16::Action_Undo));
      toolButton->setToolTip("Undo");
      toolBar->addWidget(toolButton);
    }

    {
      auto* toolButton = new QToolButton(toolBar);
      toolButton->setFocusPolicy(Qt::NoFocus);
      toolButton->setIcon(makeQIcon(Icons16::Action_Redo));
      toolButton->setToolTip("Redo");
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

      auto* button = new QPushButton(makeQIcon(Icons16::Action_Filter), "", widget);
      button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      topBarLayout->addWidget(button);
    }

    auto* navBar = new NavigationBar(widget);
    {
      layout->addWidget(navBar);
      navBar->setItemsShouldExpand(true);
      navBar->addItem("Objects", QIcon(), QString("%1").arg(12));
      navBar->addItem("Materials", QIcon(), QString("%1").arg(3));
    }

    layout->addWidget(makeHorizontalLine(widget));

    auto* stackedWidget = new QStackedWidget(widget);
    {
      stackedWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

      {
        auto* treeWidget = new QTreeWidget(widget);
        stackedWidget->addWidget(treeWidget);

        qlementineStyle->setAutoIconColor(treeWidget, oclero::qlementine::AutoIconColor::None);

        const auto object_icon = QIcon(":/showcase/icons/scene_object.svg");
        const auto light_icon = QIcon(":/showcase/icons/scene_light.svg");
        const auto material_icon = QIcon(":/showcase/icons/scene_material.svg");

        treeWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
        treeWidget->setAlternatingRowColors(false);
        treeWidget->setColumnCount(1);
        treeWidget->setHeaderHidden(true);
        treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        for (auto i = 0; i < 24; ++i) {
          auto* root = new QTreeWidgetItem(treeWidget);
          root->setText(0, QString("Root %1").arg(i + 1));
          root->setIcon(0, object_icon);
          root->setText(1, QString("Column 2 of Root %1").arg(i + 1));

          for (auto j = 0; j < 4; ++j) {
            auto* child = new QTreeWidgetItem(root);
            child->setText(0, QString("Child %1 of Root %2").arg(j + 1).arg(i + 1));
            child->setIcon(0, light_icon);
            child->setText(1, QString("Column 2 of Child %1 of Root %2").arg(j + 1).arg(i + 1));

            for (auto k = 0; k < 3; ++k) {
              auto* subChild = new QTreeWidgetItem(child);
              subChild->setText(0, QString("Child %1 of Child %2 of Root %3").arg(k + 1).arg(j + 1).arg(i + 1));
              subChild->setIcon(0, material_icon);
              subChild->setText(
                1, QString("Column 2 of Child %1 of Child %2 of Root %3").arg(k + 1).arg(j + 1).arg(i + 1));
            }
          }
        }

        treeWidget->topLevelItem(0)->setSelected(true);
        navBar->setItemBadge(0, QString::number(treeWidget->topLevelItemCount()));
      }

      {
        class CustomDelegate : public QStyledItemDelegate {
          using QStyledItemDelegate::QStyledItemDelegate;

          QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
            const auto result = QStyledItemDelegate::sizeHint(option, index);
            return { 0, result.height() };
          }
        };


        auto* listWidget = new QListWidget(widget);
        stackedWidget->addWidget(listWidget);

        listWidget->setItemDelegate(new CustomDelegate(listWidget));
        listWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
        listWidget->setSizeAdjustPolicy(QListView::SizeAdjustPolicy::AdjustIgnored);
        listWidget->setAlternatingRowColors(true);
        listWidget->setIconSize(QSize(32, 32));
        qlementineStyle->setAutoIconColor(listWidget, AutoIconColor::None);

        const auto materialIcon = QIcon(":/showcase/icons/scene_material.svg");
        for (auto i = 0; i < 16; ++i) {
          const auto itemText = QString("Item #%1 with very long text that can be elided").arg(i);
          auto* item = new QListWidgetItem(materialIcon, itemText, listWidget);
          item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
          item->setCheckState(i % 3 == 0 ? Qt ::CheckState::Checked : Qt::CheckState::Unchecked);
          listWidget->addItem(item);
        }
        listWidget->item(0)->setSelected(true);
        navBar->setItemBadge(1, QString::number(listWidget->count()));
      }

      layout->addWidget(stackedWidget, 1);
    }

    {
      stackedWidget->setCurrentIndex(navBar->currentIndex());
      QObject::connect(navBar, &NavigationBar::currentIndexChanged, stackedWidget, [stackedWidget, navBar]() {
        stackedWidget->setCurrentIndex(navBar->currentIndex());
      });
    }
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
        segmentedControl->addItem("Properties", makeQIcon(Icons16::Navigation_SlidersVertical), QString("%1").arg(4));
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
