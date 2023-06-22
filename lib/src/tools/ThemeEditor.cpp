#include <oclero/qlementine/tools/ThemeEditor.hpp>

#include <oclero/qlementine/widgets/ColorEditor.hpp>
#include <oclero/qlementine/widgets/Label.hpp>
#include <oclero/qlementine/widgets/LineEdit.hpp>

#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QStyle>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QStandardPaths>

#define ADD_TITLE(TEXT) \
  formLayout->addItem(new QSpacerItem(0, vSpacing * 3, QSizePolicy::Ignored, QSizePolicy::Fixed)); \
  formLayout->addRow(new Label(TEXT, TextRole::H3, &owner));

#define ADD_SUBTITLE(TEXT) \
  formLayout->addItem(new QSpacerItem(0, vSpacing / 2, QSizePolicy::Ignored, QSizePolicy::Fixed)); \
  formLayout->addRow(new Label(TEXT, TextRole::H4, &owner));

#define ADD_DESCRIPTION(TEXT) formLayout->addRow(new Label(TEXT, TextRole::Caption, &owner));

#define ADD_COLOR_EDITOR(NAME, DESCRIPTION) \
  { \
    const auto pair = makeColorEditorAndLabel(#NAME, DESCRIPTION, &owner, theme.NAME, [this](const QColor& c) { \
      theme.NAME = c; \
      emit owner.themeChanged(theme); \
    }); \
    this->NAME##Editor = pair.second; \
    formLayout->addRow(pair.first, pair.second); \
  }

#define ADD_METADATA_TEXT_EDITOR(NAME, DESCRIPTION) \
  { \
    const auto pair = makeTextEditorAndLabel(#NAME, DESCRIPTION, &owner, theme.meta.NAME, [this](const QString& s) { \
      theme.meta.NAME = s; \
      emit owner.themeChanged(theme); \
    }); \
    this->NAME##Editor = pair.second; \
    formLayout->addRow(pair.first, pair.second); \
  }

#define UPDATE_COLOR_EDITOR(NAME) \
  this->NAME##Editor->blockSignals(true); \
  this->NAME##Editor->setColor(theme.NAME); \
  this->NAME##Editor->blockSignals(false);

#define UPDATE_METADATA_TEXT_EDITOR(NAME) \
  this->NAME##Editor->blockSignals(true); \
  this->NAME##Editor->setText(theme.meta.NAME); \
  this->NAME##Editor->blockSignals(false);

namespace oclero::qlementine {
const QString PREVIOUS_PATH_SETTINGS_KEY{ "previousPath" };
const QString DEFAULT_FILE_NAME{ "theme.json" };

std::pair<QWidget*, ColorEditor*> makeColorEditorAndLabel(const QString& label, const QString& description,
  QWidget* parent, const QColor& initialValue, const std::function<void(const QColor&)>& onChanged) {
  auto* colorEditor = new ColorEditor(initialValue, parent);
  QObject::connect(colorEditor, &ColorEditor::colorChanged, parent, [colorEditor, onChanged]() {
    onChanged(colorEditor->color());
  });

  auto* leftColumn = new QWidget(parent);
  auto* leftColumnLayout = new QVBoxLayout(leftColumn);
  leftColumnLayout->setContentsMargins(0, 0, 0, 0);
  const auto vSpacing = leftColumn->style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing);
  leftColumnLayout->setSpacing(vSpacing / 4);
  auto* nameLabel = new Label(label, TextRole::Default, leftColumn);
  nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  leftColumnLayout->addWidget(nameLabel);
  if (!description.isEmpty()) {
    auto* descriptionLabel = new Label(description, TextRole::Caption, leftColumn);
    leftColumnLayout->addWidget(descriptionLabel);
  }

  return { leftColumn, colorEditor };
}

std::pair<QWidget*, LineEdit*> makeTextEditorAndLabel(const QString& label, const QString& description, QWidget* parent,
  const QString& initialValue, const std::function<void(const QString&)>& onChanged) {
  auto* lineEdit = new LineEdit(parent);
  lineEdit->setPlaceholderText(label);
  QObject::connect(lineEdit, &QLineEdit::editingFinished, parent, [lineEdit, onChanged] {
    onChanged(lineEdit->text().trimmed());
  });

  auto* leftColumn = new QWidget(parent);
  auto* leftColumnLayout = new QVBoxLayout(leftColumn);
  leftColumnLayout->setContentsMargins(0, 0, 0, 0);
  const auto vSpacing = leftColumn->style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing);
  leftColumnLayout->setSpacing(vSpacing / 4);
  auto* nameLabel = new Label(label, TextRole::Default, leftColumn);
  nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  leftColumnLayout->addWidget(nameLabel);
  if (!description.isEmpty()) {
    auto* descriptionLabel = new Label(description, TextRole::Caption, leftColumn);
    leftColumnLayout->addWidget(descriptionLabel);
  }

  return { leftColumn, lineEdit };
}

struct ThemeEditor::Impl {
  ColorEditor* primaryColorEditor;
  ColorEditor* primaryColorHoveredEditor;
  ColorEditor* primaryColorPressedEditor;
  ColorEditor* primaryColorDisabledEditor;

  ColorEditor* primaryColorForegroundEditor;
  ColorEditor* primaryColorForegroundHoveredEditor;
  ColorEditor* primaryColorForegroundPressedEditor;
  ColorEditor* primaryColorForegroundDisabledEditor;

  ColorEditor* neutralColorEditor;
  ColorEditor* neutralColorHoveredEditor;
  ColorEditor* neutralColorPressedEditor;
  ColorEditor* neutralColorDisabledEditor;

  ColorEditor* neutralColorForegroundEditor;
  ColorEditor* neutralColorForegroundHoveredEditor;
  ColorEditor* neutralColorForegroundPressedEditor;
  ColorEditor* neutralColorForegroundDisabledEditor;

  ColorEditor* neutralAlternativeColorEditor;
  ColorEditor* neutralAlternativeColorHoveredEditor;
  ColorEditor* neutralAlternativeColorPressedEditor;
  ColorEditor* neutralAlternativeColorDisabledEditor;

  ColorEditor* adaptativeColor1Editor;
  ColorEditor* adaptativeColor2Editor;
  ColorEditor* adaptativeColor3Editor;
  ColorEditor* adaptativeColor4Editor;
  ColorEditor* adaptativeColor5Editor;

  ColorEditor* backgroundColorMain1Editor;
  ColorEditor* backgroundColorMain2Editor;
  ColorEditor* backgroundColorMain3Editor;

  ColorEditor* borderColor1Editor;
  ColorEditor* borderColor2Editor;
  ColorEditor* borderColor3Editor;

  ColorEditor* focusColorEditor;

  ColorEditor* shadowColor1Editor;
  ColorEditor* shadowColor2Editor;
  ColorEditor* shadowColor3Editor;

  ColorEditor* statusColorErrorEditor;
  ColorEditor* statusColorErrorHoveredEditor;
  ColorEditor* statusColorErrorPressedEditor;
  ColorEditor* statusColorErrorDisabledEditor;

  ColorEditor* statusColorWarningEditor;
  ColorEditor* statusColorWarningHoveredEditor;
  ColorEditor* statusColorWarningPressedEditor;
  ColorEditor* statusColorWarningDisabledEditor;

  ColorEditor* statusColorSuccessEditor;
  ColorEditor* statusColorSuccessHoveredEditor;
  ColorEditor* statusColorSuccessPressedEditor;
  ColorEditor* statusColorSuccessDisabledEditor;

  ColorEditor* statusColorInfoEditor;
  ColorEditor* statusColorInfoHoveredEditor;
  ColorEditor* statusColorInfoPressedEditor;
  ColorEditor* statusColorInfoDisabledEditor;

  ColorEditor* statusColorForegroundEditor;
  ColorEditor* statusColorForegroundHoveredEditor;
  ColorEditor* statusColorForegroundPressedEditor;
  ColorEditor* statusColorForegroundDisabledEditor;

  LineEdit* nameEditor;
  LineEdit* authorEditor;
  LineEdit* versionEditor;

  Impl(ThemeEditor& o)
    : owner(o) {}

  /// Utility to load/save from/to JSON files.
  void setupJSONLoader(QWidget* parent, QFormLayout* formLayout) {
    auto* rowLayout = new QHBoxLayout();
    const auto hSpacing = owner.style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing);
    rowLayout->setSpacing(hSpacing * 2);

    // 'Load' button.
    {
      auto* loadJsonButton = new QPushButton(QIcon::fromTheme("document-open"), "Load JSON file…", parent);
      loadJsonButton->setToolTip("Load a JSON file from disk.");
      loadJsonButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      rowLayout->addWidget(loadJsonButton);

      QObject::connect(loadJsonButton, &QPushButton::pressed, &owner, [this]() {
        // Get previous path.
        const auto defaultDirPath =
          QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation);
        const auto defaultPath = defaultDirPath + '/' + DEFAULT_FILE_NAME;
        QSettings settings;
        const auto previousPath = settings.value(PREVIOUS_PATH_SETTINGS_KEY, defaultPath).toString();

        // Get theme from file and set it on the application.
        const auto fileName =
          QFileDialog::getOpenFileName(&owner, "Load JSON theme", previousPath, "JSON Files (*.json)");
        const auto theme = Theme(fileName);
        owner.setTheme(theme);

        // Save path to QSettings.
        settings.setValue(PREVIOUS_PATH_SETTINGS_KEY, fileName);
        settings.sync();
      });
    }

    // 'Save' button.
    {
      auto* saveJsonButton = new QPushButton(QIcon::fromTheme("document-save"), "Save JSON file…", parent);
      saveJsonButton->setToolTip("Save the current theme as JSON file to disk.");
      saveJsonButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      rowLayout->addWidget(saveJsonButton);

      QObject::connect(saveJsonButton, &QPushButton::pressed, &owner, [this]() {
        // Get previous path.
        const auto defaultDirPath =
          QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation);
        const auto defaultPath = defaultDirPath + '/' + DEFAULT_FILE_NAME;
        QSettings settings;
        const auto previousPath = settings.value(PREVIOUS_PATH_SETTINGS_KEY, defaultPath).toString();

        const auto fileName =
          QFileDialog::getSaveFileName(&owner, "Save JSON theme", previousPath, "JSON Files (*.json)");
        if (!fileName.isEmpty()) {
          const auto jsonDoc = theme.toJson();
          const auto data = jsonDoc.toJson(QJsonDocument::JsonFormat::Indented).replace("    ", "  ");
          QFile file(fileName);
          if (file.open(QIODevice::ReadWrite)) {
            // Save to disk.
            file.resize(0);
            file.write(data);
            file.close();

            // Save path to QSettings.
            settings.setValue(PREVIOUS_PATH_SETTINGS_KEY, fileName);
            settings.sync();
          } else {
            QMessageBox::critical(&owner, "Writing Error", QString("Can't write to file:\n%1").arg(fileName),
              QMessageBox::StandardButton::Ok);
          }
        }
      });
    }

    formLayout->addRow(rowLayout);
  }

  void setupColorEditors(QFormLayout* formLayout, int vSpacing) {
    // Primary Color.
    ADD_TITLE("Primary Color");

    //ADD_SUBTITLE("Background");
    //ADD_DESCRIPTION("Highlighting selected elements, checked elements, default buttons, etc.");
    ADD_COLOR_EDITOR(primaryColor, "");
    ADD_COLOR_EDITOR(primaryColorHovered, "");
    ADD_COLOR_EDITOR(primaryColorPressed, "");
    ADD_COLOR_EDITOR(primaryColorDisabled, "");
    ADD_SUBTITLE("Foreground")
    ADD_DESCRIPTION("Text drawn over selected elements, default buttons, etc.");
    ADD_COLOR_EDITOR(primaryColorForeground, "");
    ADD_COLOR_EDITOR(primaryColorForegroundHovered, "");
    ADD_COLOR_EDITOR(primaryColorForegroundPressed, "");
    ADD_COLOR_EDITOR(primaryColorForegroundDisabled, "");

    // Neutral Color.
    ADD_TITLE("Neutral Color");
    ADD_DESCRIPTION("Used for texts.");
    ADD_SUBTITLE("Background");
    ADD_COLOR_EDITOR(neutralColor, "");
    ADD_COLOR_EDITOR(neutralColorHovered, "");
    ADD_COLOR_EDITOR(neutralColorPressed, "");
    ADD_COLOR_EDITOR(neutralColorDisabled, "");
    ADD_SUBTITLE("Foreground")
    ADD_COLOR_EDITOR(neutralColorForeground, "");
    ADD_COLOR_EDITOR(neutralColorForegroundHovered, "");
    ADD_COLOR_EDITOR(neutralColorForegroundPressed, "");
    ADD_COLOR_EDITOR(neutralColorForegroundDisabled, "");

    // Neutral Alternative Color.
    ADD_TITLE("Neutral Alternative Color");
    ADD_DESCRIPTION("Used for buttons.");
    ADD_COLOR_EDITOR(neutralAlternativeColor, "");
    ADD_COLOR_EDITOR(neutralAlternativeColorHovered, "");
    ADD_COLOR_EDITOR(neutralAlternativeColorPressed, "");
    ADD_COLOR_EDITOR(neutralAlternativeColorDisabled, "");

    // Neutral Alternative Color.
    ADD_TITLE("Adaptative Color");
    ADD_DESCRIPTION("Used for ???");
    ADD_COLOR_EDITOR(adaptativeColor1, "");
    ADD_COLOR_EDITOR(adaptativeColor2, "");
    ADD_COLOR_EDITOR(adaptativeColor3, "");
    ADD_COLOR_EDITOR(adaptativeColor4, "");
    ADD_COLOR_EDITOR(adaptativeColor5, "");

    // Background Color.
    ADD_TITLE("Background Color");
    ADD_COLOR_EDITOR(backgroundColorMain1, "Main background color (window, etc).");
    ADD_COLOR_EDITOR(backgroundColorMain2, "");
    ADD_COLOR_EDITOR(backgroundColorMain3, "");

    // Border Color.
    ADD_TITLE("Border Color");
    ADD_DESCRIPTION("Used to draw the borders.");
    ADD_COLOR_EDITOR(borderColor1, "");
    ADD_COLOR_EDITOR(borderColor2, "");
    ADD_COLOR_EDITOR(borderColor3, "");

    // Focus Color.
    ADD_TITLE("Focus Color");
    ADD_COLOR_EDITOR(focusColor, "Color for the focus border.");

    // Shadow Color.
    ADD_TITLE("Shadow Color");
    ADD_COLOR_EDITOR(shadowColor1, "Shadow 1");
    ADD_COLOR_EDITOR(shadowColor2, "Shadow 2");
    ADD_COLOR_EDITOR(shadowColor3, "Shadow 3");

    // Status Color.
    ADD_TITLE("Status Color");
    ADD_DESCRIPTION("Displaying feedback to the user (success, error, etc.)")
    ADD_SUBTITLE("Error");
    ADD_COLOR_EDITOR(statusColorError, "");
    ADD_COLOR_EDITOR(statusColorErrorHovered, "");
    ADD_COLOR_EDITOR(statusColorErrorPressed, "");
    ADD_COLOR_EDITOR(statusColorErrorDisabled, "");
    ADD_SUBTITLE("Warning");
    ADD_COLOR_EDITOR(statusColorWarning, "");
    ADD_COLOR_EDITOR(statusColorWarningHovered, "");
    ADD_COLOR_EDITOR(statusColorWarningPressed, "");
    ADD_COLOR_EDITOR(statusColorWarningDisabled, "");
    ADD_SUBTITLE("Success");
    ADD_COLOR_EDITOR(statusColorSuccess, "");
    ADD_COLOR_EDITOR(statusColorSuccessHovered, "");
    ADD_COLOR_EDITOR(statusColorSuccessPressed, "");
    ADD_COLOR_EDITOR(statusColorSuccessDisabled, "");
    ADD_SUBTITLE("Info");
    ADD_COLOR_EDITOR(statusColorInfo, "");
    ADD_COLOR_EDITOR(statusColorInfoHovered, "");
    ADD_COLOR_EDITOR(statusColorInfoPressed, "");
    ADD_COLOR_EDITOR(statusColorInfoDisabled, "");
    ADD_SUBTITLE("Foreground");
    ADD_DESCRIPTION("Used to draw text over status colors.");
    ADD_COLOR_EDITOR(statusColorForeground, "");
    ADD_COLOR_EDITOR(statusColorForegroundHovered, "");
    ADD_COLOR_EDITOR(statusColorForegroundPressed, "");
    ADD_COLOR_EDITOR(statusColorForegroundDisabled, "");

    // Metadata
    ADD_TITLE("Metadata");
    ADD_METADATA_TEXT_EDITOR(name, "Name of the Qlementine theme");
    ADD_METADATA_TEXT_EDITOR(author, "Author of the Qlementine theme");
    ADD_METADATA_TEXT_EDITOR(version, "Version of the Qlementine theme");
  }

  void setupUi() {
    auto* globalLayout = new QVBoxLayout(&owner);
    owner.setLayout(globalLayout);
    const auto vSpacing = owner.style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing);
    const auto hSpacing = owner.style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing);

    auto* formLayout = new QFormLayout();
    formLayout->setHorizontalSpacing(hSpacing * 8);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::DontWrapRows);

    setupJSONLoader(&owner, formLayout);
    setupColorEditors(formLayout, vSpacing);

    globalLayout->addLayout(formLayout);
  }

  // Update all the widgets.
  void updateUi() {
    UPDATE_COLOR_EDITOR(primaryColor);
    UPDATE_COLOR_EDITOR(primaryColorHovered);
    UPDATE_COLOR_EDITOR(primaryColorPressed);
    UPDATE_COLOR_EDITOR(primaryColorDisabled);

    UPDATE_COLOR_EDITOR(primaryColorForeground);
    UPDATE_COLOR_EDITOR(primaryColorForegroundHovered);
    UPDATE_COLOR_EDITOR(primaryColorForegroundPressed);
    UPDATE_COLOR_EDITOR(primaryColorForegroundDisabled);

    UPDATE_COLOR_EDITOR(neutralColor);
    UPDATE_COLOR_EDITOR(neutralColorHovered);
    UPDATE_COLOR_EDITOR(neutralColorPressed);
    UPDATE_COLOR_EDITOR(neutralColorDisabled);

    UPDATE_COLOR_EDITOR(neutralColorForeground);
    UPDATE_COLOR_EDITOR(neutralColorForegroundHovered);
    UPDATE_COLOR_EDITOR(neutralColorForegroundPressed);
    UPDATE_COLOR_EDITOR(neutralColorForegroundDisabled);

    UPDATE_COLOR_EDITOR(neutralAlternativeColor);
    UPDATE_COLOR_EDITOR(neutralAlternativeColorHovered);
    UPDATE_COLOR_EDITOR(neutralAlternativeColorPressed);
    UPDATE_COLOR_EDITOR(neutralAlternativeColorDisabled);

    UPDATE_COLOR_EDITOR(adaptativeColor1);
    UPDATE_COLOR_EDITOR(adaptativeColor2);
    UPDATE_COLOR_EDITOR(adaptativeColor3);
    UPDATE_COLOR_EDITOR(adaptativeColor4);
    UPDATE_COLOR_EDITOR(adaptativeColor5);

    UPDATE_COLOR_EDITOR(backgroundColorMain1);
    UPDATE_COLOR_EDITOR(backgroundColorMain2);
    UPDATE_COLOR_EDITOR(backgroundColorMain3);

    UPDATE_COLOR_EDITOR(borderColor1);
    UPDATE_COLOR_EDITOR(borderColor2);
    UPDATE_COLOR_EDITOR(borderColor3);

    UPDATE_COLOR_EDITOR(focusColor);

    UPDATE_COLOR_EDITOR(shadowColor1);
    UPDATE_COLOR_EDITOR(shadowColor2);
    UPDATE_COLOR_EDITOR(shadowColor3);

    UPDATE_COLOR_EDITOR(statusColorError);
    UPDATE_COLOR_EDITOR(statusColorErrorHovered);
    UPDATE_COLOR_EDITOR(statusColorErrorPressed);
    UPDATE_COLOR_EDITOR(statusColorErrorDisabled);


    UPDATE_COLOR_EDITOR(statusColorWarning);
    UPDATE_COLOR_EDITOR(statusColorWarningHovered);
    UPDATE_COLOR_EDITOR(statusColorWarningPressed);
    UPDATE_COLOR_EDITOR(statusColorWarningDisabled);

    UPDATE_COLOR_EDITOR(statusColorSuccess);
    UPDATE_COLOR_EDITOR(statusColorSuccessHovered);
    UPDATE_COLOR_EDITOR(statusColorSuccessPressed);
    UPDATE_COLOR_EDITOR(statusColorSuccessDisabled);

    UPDATE_COLOR_EDITOR(statusColorInfo);
    UPDATE_COLOR_EDITOR(statusColorInfoHovered);
    UPDATE_COLOR_EDITOR(statusColorInfoPressed);
    UPDATE_COLOR_EDITOR(statusColorInfoDisabled);

    UPDATE_COLOR_EDITOR(statusColorForeground);
    UPDATE_COLOR_EDITOR(statusColorForegroundHovered);
    UPDATE_COLOR_EDITOR(statusColorForegroundPressed);
    UPDATE_COLOR_EDITOR(statusColorForegroundDisabled);

    UPDATE_METADATA_TEXT_EDITOR(name);
    UPDATE_METADATA_TEXT_EDITOR(author);
    UPDATE_METADATA_TEXT_EDITOR(version);
  }

  ThemeEditor& owner;
  Theme theme;
};

ThemeEditor::ThemeEditor(QWidget* parent)
  : QWidget(parent)
  , _impl(std::make_unique<Impl>(*this)) {
  _impl->setupUi();
}

ThemeEditor::~ThemeEditor() {}

const Theme& ThemeEditor::theme() const {
  return _impl->theme;
}

void ThemeEditor::setTheme(const Theme& theme) {
  if (theme != _impl->theme) {
    _impl->theme = theme;
    _impl->updateUi();
    emit themeChanged(_impl->theme);
  }
}
} // namespace oclero::qlementine
