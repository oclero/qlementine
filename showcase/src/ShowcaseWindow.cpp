#include "ShowcaseWindow.hpp"

#include <oclero/qlementine/style/QlementineStyle.hpp>

#include <oclero/qlementine/utils/IconUtils.hpp>
#include <oclero/qlementine/utils/WidgetUtils.hpp>
#include <oclero/qlementine/utils/LayoutUtils.hpp>

#include <oclero/qlementine/widgets/LineEdit.hpp>
#include <oclero/qlementine/widgets/NavigationBar.hpp>
#include <oclero/qlementine/widgets/SegmentedControl.hpp>
#include <oclero/qlementine/widgets/Switch.hpp>
#include <oclero/qlementine/widgets/StatusBadgeWidget.hpp>
#include <oclero/qlementine/widgets/ColorButton.hpp>

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
#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include <QSlider>
#include <QDial>
#include <QDateTimeEdit>
#include <QGroupBox>
#include <QButtonGroup>
#include <QProgressBar>

#include <random>

namespace oclero::qlementine::showcase {
using Icons16 = oclero::qlementine::icons::Icons16;

static QIcon makeQIcon(Icons16 id, const QSize& size = { 16, 16 }) {
  return oclero::qlementine::makeThemedIcon(id, size);
}

class DummyWorkspace : public QWidget {
public:
  using QWidget::QWidget;

protected:
  void paintEvent(QPaintEvent* evt) override {
    QPainter p(this);

    QColor backgroundColor;
    if (const auto* qlementine_style = qlementine::appStyle()) {
      const auto theme = qlementine_style->theme();
      backgroundColor = theme.backgroundColorMain4.darker(110);
    }
    p.fillRect(rect(), backgroundColor);
  }
};

static QString getDummyText(const unsigned int minWords = 3, const unsigned int maxWords = 4) {
  static const auto loremIpsumWords = std::array<const QString, 69>{ "Lorem", "Ipsum", "Dolor", "Sit", "Amet",
    "Consectetur", "Adipiscing", "Elit", "Sed", "Do", "Eiusmod", "Tempor", "Incididunt", "Ut", "Labore", "Et", "Dolore",
    "Magna", "Aliqua", "Ut", "Enim", "Ad", "Minim", "Veniam", "Quis", "Nostrud", "Exercitation", "Ullamco", "Laboris",
    "Nisi", "Ut", "Aliquip", "Ex", "Ea", "Commodo", "Consequat", "Duis", "Aute", "Irure", "Dolor", "In",
    "Reprehenderit", "In", "Voluptate", "Velit", "Esse", "Cillum", "Dolore", "Eu", "Fugiat", "Nulla", "Pariatur",
    "Excepteur", "Sint", "Occaecat", "Cupidatat", "Non", "Proident", "Sunt", "In", "Culpa", "Qui", "Officia",
    "Deserunt", "Mollit", "Anim", "Id", "Est", "Laborum" };

  auto rd = std::random_device();
  auto gen = std::mt19937(rd());

  auto randomCountDistrib = std::uniform_int_distribution<>(minWords, maxWords);
  const auto random_word_count = randomCountDistrib(gen);

  auto randomIndexDistrib = std::uniform_int_distribution<>(0, loremIpsumWords.size() - 1 - random_word_count);
  const auto randomWordIndex = randomIndexDistrib(gen);

  auto result = loremIpsumWords.at(randomWordIndex);
  for (auto i = 0; i < random_word_count - 1; ++i) {
    result += ' ' + loremIpsumWords.at(randomWordIndex + 1 + i);
  }
  return result;
}

static QIcon getDummyColoredIcon() {
  static const auto icons = std::array<QIcon, 3>{
    QIcon(":/showcase/icons/scene_object.svg"),
    QIcon(":/showcase/icons/scene_light.svg"),
    QIcon(":/showcase/icons/scene_material.svg"),
  };

  auto rd = std::random_device();
  auto gen = std::mt19937(rd());

  auto randomDistrib = std::uniform_int_distribution<>(0, icons.size() - 1);
  const auto randomIndex = randomDistrib(gen);

  return icons.at(randomIndex);
}

static QIcon getDummyMonochromeIcon(const QSize& size = { 16, 16 }) {
  auto rd = std::random_device();
  auto gen = std::mt19937(rd());

  auto randomDistrib =
    std::uniform_int_distribution<std::underlying_type_t<Icons16>>(1, 410 - 1); // TODO use a constexpr variable.
  const auto randomIndex = randomDistrib(gen);
  const auto randomIcon = static_cast<Icons16>(randomIndex);
  return makeQIcon(randomIcon, size);
}

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
      tabBar->addTab(makeQIcon(Icons16::File_File), getDummyText());
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

    const auto addButton = [this](const Icons16 icon, const QString& tooltip, const QString& text = {}) {
      auto* toolButton = new QToolButton(toolBar);
      toolButton->setFocusPolicy(Qt::NoFocus);
      toolButton->setIcon(makeQIcon(icon));
      toolButton->setToolTip(tooltip);
      if (!text.isEmpty()) {
        toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolButton->setText(text);
      }
      toolBar->addWidget(toolButton);
      return toolButton;
    };

    addButton(Icons16::Action_Save, "Save");
    addButton(Icons16::Action_Print, "Print");
    toolBar->addSeparator();
    addButton(Icons16::Action_Undo, "Undo");
    addButton(Icons16::Action_Redo, "Redo");

    auto* resetButton = addButton(Icons16::Action_Reset, "Reset");
    {
      auto* menu = new QMenu(resetButton);
      for (auto i = 0; i < 10; ++i) {
        menu->addAction(new QAction(getDummyMonochromeIcon(), getDummyText(2, 3), menu));
      }
      resetButton->setMenu(menu);
      resetButton->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);
    }

    toolBar->addSeparator();
    addButton(Icons16::Action_Copy, "Copy");
    addButton(Icons16::Action_Paste, "Paste");
    addButton(Icons16::Action_Cut, "Cut");
    toolBar->addSeparator();
    addButton(Icons16::Media_SkipBackward, "Skip Backward");
    addButton(Icons16::Media_Play, "Play");
    addButton(Icons16::Media_SkipForward, "Skip Forward");
    toolBar->addSeparator();
    auto* exportButton = addButton(Icons16::Action_Export, "Export", "Export");
    {
      auto* menu = new QMenu(exportButton);
      menu->addAction(new QAction(makeQIcon(Icons16::File_Movie), "Movie", menu));
      menu->addAction(new QAction(makeQIcon(Icons16::File_Picture), "Picture", menu));
      menu->addSeparator();
      menu->addAction(new QAction(makeQIcon(Icons16::File_Archive), "Archive", menu));

      exportButton->setMenu(menu);
      exportButton->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);
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
      lineEdit->setIcon(makeQIcon(Icons16::Navigation_Search));
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

        treeWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
        treeWidget->setAlternatingRowColors(false);
        treeWidget->setColumnCount(1);
        treeWidget->setHeaderHidden(true);
        treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        for (auto i = 0; i < 24; ++i) {
          auto* root = new QTreeWidgetItem(treeWidget);
          root->setText(0, getDummyText());
          root->setIcon(0, getDummyColoredIcon());

          for (auto j = 0; j < 4; ++j) {
            auto* child = new QTreeWidgetItem(root);
            child->setText(0, getDummyText());
            child->setIcon(0, getDummyColoredIcon());

            for (auto k = 0; k < 3; ++k) {
              auto* subChild = new QTreeWidgetItem(child);
              subChild->setText(0, getDummyText());
              subChild->setIcon(0, getDummyColoredIcon());
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

        for (auto i = 0; i < 15; ++i) {
          const auto itemText = QString("Item #%1 with very long text that can be elided").arg(i);
          auto* item = new QListWidgetItem(getDummyColoredIcon(), itemText, listWidget);
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
        segmentedControl->addItem("Scene", makeQIcon(Icons16::Misc_Globe), QString("%1").arg(2));
      }
    }

    layout->addWidget(makeHorizontalLine(widget));

    {
      auto* scrollArea = new QScrollArea(widget);
      layout->addWidget(scrollArea);

      auto* content = new QWidget(scrollArea);
      content->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
      scrollArea->setWidget(content);
      scrollArea->setWidgetResizable(true);

      const auto margins = getLayoutMargins(content);
      const auto vMargin = static_cast<int>(margins.top() * .75);
      auto* contentLayout = new QFormLayout(content);
      contentLayout->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::ExpandingFieldsGrow);
      contentLayout->setContentsMargins({ margins.left(), vMargin, margins.right(), vMargin });
      {
        {
          auto* groupBox = new QGroupBox(content);
          groupBox->setAlignment(Qt::AlignRight);
          groupBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
          groupBox->setTitle(getDummyText());
          groupBox->setCheckable(true);
          groupBox->setFlat(false);
          auto* groupBoxLayout = new QFormLayout(groupBox);
          groupBoxLayout->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::ExpandingFieldsGrow);

          {
            auto* switchWidget = new qlementine::Switch(groupBox);
            switchWidget->setChecked(true);
            groupBoxLayout->addRow(getDummyText(2, 2) + ":", switchWidget);
          }
          {
            auto* spinBox = new QSpinBox(groupBox);
            spinBox->setRange(0, 1000);
            spinBox->setSuffix("cm");
            groupBoxLayout->addRow(getDummyText(2, 2) + ":", spinBox);
          }
          {
            auto* comboBox = new QComboBox(groupBox);
            for (auto i = 0; i < 5; ++i) {
              comboBox->addItem(getDummyMonochromeIcon(), getDummyText(1, 1));
            }
            comboBox->setCurrentIndex(0);
            groupBoxLayout->addRow(getDummyText(1, 1) + ":", comboBox);
          }
          {
            auto* comboBox = new QComboBox(groupBox);
            comboBox->setEditable(true);
            comboBox->setPlaceholderText("Placeholder");
            comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            for (auto i = 0; i < 5; ++i) {
              comboBox->addItem(getDummyMonochromeIcon(), getDummyText(1, 1));
            }
            comboBox->setCurrentIndex(0);
            groupBoxLayout->addRow(getDummyText(1, 1) + ":", comboBox);
          }
          {
            auto* dateTimeEdit = new QDateTimeEdit(groupBox);
            //dateTimeEdit->setSizePolicy(QSizepol)
            dateTimeEdit->setCalendarPopup(true);
            groupBoxLayout->addRow(getDummyText(1, 1) + ":", dateTimeEdit);
          }
          contentLayout->addRow(groupBox);
        }
        {
          auto* groupBox = new QGroupBox(content);
          groupBox->setAlignment(Qt::AlignRight);
          groupBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
          groupBox->setTitle(getDummyText());
          groupBox->setCheckable(true);
          groupBox->setFlat(false);
          auto* groupBoxLayout = new QVBoxLayout(groupBox);
          {
            auto* radioGroup = new QButtonGroup(groupBox);
            for (auto i = 0; i < 3; ++i) {
              auto* radioButton = new QRadioButton(getDummyText(), groupBox);
              radioButton->setIcon(getDummyMonochromeIcon());
              radioButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
              radioGroup->addButton(radioButton);
              groupBoxLayout->addWidget(radioButton);
            }
          }
          contentLayout->addRow(groupBox);
        }
        {
          auto* groupBox = new QGroupBox(content);
          groupBox->setAlignment(Qt::AlignRight);
          groupBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
          groupBox->setTitle(getDummyText());
          groupBox->setCheckable(true);
          groupBox->setFlat(false);
          auto* groupBoxLayout = new QFormLayout(groupBox);
          {
            auto* slider = new QSlider(groupBox);
            slider->setRange(0, 100);
            slider->setValue(30);
            groupBoxLayout->addRow(getDummyText(1, 1) + ":", slider);
          }
          {
            auto* slider = new QSlider(groupBox);
            slider->setOrientation(Qt::Orientation::Horizontal);
            slider->setRange(0, 10);
            slider->setPageStep(1);
            slider->setSingleStep(1);
            slider->setValue(7);
            slider->setTickPosition(QSlider::TickPosition::TicksAbove);
            slider->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

            groupBoxLayout->addRow(getDummyText(1, 1) + ":", slider);
          }
          contentLayout->addRow(groupBox);
        }
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

    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 6);
    splitter->setStretchFactor(2, 2);
  }

  void setupStatusBar() {
    statusBar = new QStatusBar(&owner);
    statusBar->setSizeGripEnabled(false);

    const auto margins = getLayoutMargins(statusBar);
    statusBar->setContentsMargins(margins.left(), 0, margins.right(), 0);
    {
      auto* progressBar = new QProgressBar(statusBar);
      progressBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      progressBar->setValue(80);
      statusBar->addPermanentWidget(progressBar);
    }
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
