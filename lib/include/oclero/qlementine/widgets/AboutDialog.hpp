// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <QDialog>

class QLabel;
class QHBoxLayout;

namespace oclero::qlementine {
class Label;

class AboutDialog : public QDialog {
  Q_OBJECT

public:
  explicit AboutDialog(QWidget* parent = nullptr);
  virtual ~AboutDialog() = default;

  void setDescription(const QString& description);
  void setWebsiteUrl(const QString& url);
  void setLicense(const QString& license);
  void setCopyright(const QString& copyright);
  void addSocialMediaLink(const QString& name, const QString& url, const QIcon& icon = QIcon());

private:
  void setupUi();

private:
  QLabel* _appDescriptionLabel{ nullptr };
  QLabel* _websiteLabel{ nullptr };
  Label* _licenseLabel{ nullptr };
  Label* _copyrightLabel{ nullptr };
  QHBoxLayout* _buttonsLayout{ nullptr };
};
} // namespace oclero::qlementine
