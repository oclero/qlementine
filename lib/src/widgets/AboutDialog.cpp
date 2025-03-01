// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT
#include <oclero/qlementine/widgets/AboutDialog.hpp>

#include <QApplication>
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QDesktopServices>

#include <oclero/qlementine/widgets/Label.hpp>

namespace oclero::qlementine {
AboutDialog::AboutDialog(QWidget* parent)
  : QDialog(parent) {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setWindowModality(Qt::WindowModality::ApplicationModal);
  setWindowFlag(Qt::WindowType::MSWindowsFixedSizeDialogHint, true);
  setWindowFlag(Qt::WindowType::WindowContextHelpButtonHint, false);
  setWindowFlag(Qt::WindowType::WindowMaximizeButtonHint, false);
  setWindowFlag(Qt::WindowType::WindowMinimizeButtonHint, false);
  setWindowFlag(Qt::WindowType::WindowFullscreenButtonHint, false);

  setupUi();
}

void AboutDialog::setDescription(const QString& description) {
  const auto trimmed = description.trimmed();
  _appDescriptionLabel->setText(trimmed);
  _appDescriptionLabel->setVisible(!trimmed.isEmpty());
}

void AboutDialog::setWebsiteUrl(const QString& url) {
  auto prettyUrl = url.trimmed();
  if (prettyUrl.startsWith("https://")) {
    prettyUrl.remove(0, 8);
  }
  if (prettyUrl.isEmpty()) {
    _websiteLabel->setText("");
    _websiteLabel->setVisible(false);
  } else {
    _websiteLabel->setText(QString("<a href=\"%1\">%2</a>").arg(url, prettyUrl));
    _websiteLabel->setVisible(true);
  }
}

void AboutDialog::setLicense(const QString& license) {
  const auto trimmed = license.trimmed();
  _licenseLabel->setText(trimmed);
  _licenseLabel->setVisible(!trimmed.isEmpty());
}

void AboutDialog::setCopyright(const QString& copyright) {
  _copyrightLabel->setText(copyright);
}

void AboutDialog::addSocialMediaLink(const QString& name, const QString& url, const QIcon& icon) {
  auto* button = new QPushButton(this);
  button->setIcon(icon);
  button->setFocusPolicy(Qt::NoFocus);
  button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  button->setToolTip(name);
  button->setFlat(true);
  button->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
  const auto button_url = QUrl(url);
  QObject::connect(button, &QPushButton::clicked, button, [button_url]() {
    QDesktopServices::openUrl(button_url);
  });
  _buttonsLayout->addWidget(button);
}

void AboutDialog::setupUi() {
  auto* rootLayout = new QVBoxLayout(this);
  rootLayout->setContentsMargins(32, 16, 32, 16);
  rootLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
  setLayout(rootLayout);

  // Logo, app name and version.
  {
    auto* iconLabel = new QLabel(this);
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    iconLabel->setFixedSize(64, 64);
    iconLabel->setScaledContents(true);
    iconLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    iconLabel->setPixmap(QApplication::windowIcon().pixmap(iconLabel->height() * 2));

    auto iconLabelLayout = new QHBoxLayout();
    iconLabelLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
    iconLabelLayout->addWidget(iconLabel);
    iconLabelLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
    iconLabelLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

    auto* appInfoLayout = new QVBoxLayout();
    appInfoLayout->setContentsMargins(0, 0, 0, 0);
    appInfoLayout->setSpacing(2);

    auto* appNameLabel = new oclero::qlementine::Label(this);
    appNameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    appNameLabel->setRole(oclero::qlementine::TextRole::H4);
    appNameLabel->setText(QApplication::applicationDisplayName());
    appNameLabel->setAlignment(Qt::AlignCenter);

    auto* appVersionLabel = new QLabel(this);
    appVersionLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    appVersionLabel->setText(QApplication::applicationVersion());
    appVersionLabel->setAlignment(Qt::AlignCenter);

    appInfoLayout->addWidget(appNameLabel);
    appInfoLayout->addWidget(appVersionLabel);

    rootLayout->addLayout(iconLabelLayout);
    rootLayout->addLayout(appInfoLayout);
  }

  // App description.
  {
    _appDescriptionLabel = new QLabel(this);
    _appDescriptionLabel->setAlignment(Qt::AlignCenter);
    _appDescriptionLabel->setWordWrap(true);
    _appDescriptionLabel->setFixedWidth(250);
    _appDescriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    rootLayout->addWidget(_appDescriptionLabel);
    rootLayout->setAlignment(_appDescriptionLabel, Qt::AlignHCenter);
  }

  rootLayout->addSpacerItem(new QSpacerItem(0, 16, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

  // Links to social media.
  {
    _buttonsLayout = new QHBoxLayout();
    _buttonsLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
    _buttonsLayout->setSpacing(4);
    _buttonsLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addLayout(_buttonsLayout);
    rootLayout->setAlignment(_buttonsLayout, Qt::AlignHCenter);

    // Legal information.
    {
      _websiteLabel = new oclero::qlementine::Label(this);
      _websiteLabel->setAlignment(Qt::AlignCenter);
      _websiteLabel->setTextInteractionFlags(
        Qt::TextInteractionFlag::LinksAccessibleByMouse | Qt::TextInteractionFlag::LinksAccessibleByKeyboard);
      QObject::connect(_websiteLabel, &QLabel::linkActivated, this, [](const QString& link) {
        QDesktopServices::openUrl(QUrl(link));
      });

      _licenseLabel = new oclero::qlementine::Label(this);
      _licenseLabel->setRole(oclero::qlementine::TextRole::Caption);
      _licenseLabel->setText(QApplication::applicationDisplayName());
      _licenseLabel->setAlignment(Qt::AlignCenter);

      _copyrightLabel = new oclero::qlementine::Label(this);
      _copyrightLabel->setRole(oclero::qlementine::TextRole::Caption);
      _copyrightLabel->setAlignment(Qt::AlignCenter);

      auto* smallTextsLayout = new QVBoxLayout();
      smallTextsLayout->setContentsMargins(0, 0, 0, 0);
      smallTextsLayout->setSpacing(2);
      smallTextsLayout->addWidget(_licenseLabel);
      smallTextsLayout->setAlignment(_licenseLabel, Qt::AlignHCenter);

      smallTextsLayout->addWidget(_copyrightLabel);
      smallTextsLayout->setAlignment(_copyrightLabel, Qt::AlignHCenter);

      rootLayout->addWidget(_websiteLabel);
      rootLayout->setAlignment(_websiteLabel, Qt::AlignHCenter);

      rootLayout->addLayout(smallTextsLayout);
    }
  }
}
} // namespace oclero::qlementine
