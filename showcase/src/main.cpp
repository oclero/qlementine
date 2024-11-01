#include <QApplication>

#include <oclero/qlementine/style/QlementineStyle.hpp>

#include "ShowcaseWindow.hpp"

#define USE_CUSTOM_STYLE 1

int main(int argc, char* argv[]) {
  // Must be set before creating a QApplication.
  QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

  QApplication qApplication(argc, argv);

  // Must be set after creating a QApplication.
  QGuiApplication::setApplicationDisplayName("Showcase");
  QCoreApplication::setApplicationName("Showcase");
  QGuiApplication::setDesktopFileName("Showcase");
  QCoreApplication::setOrganizationName("oclero");
  QCoreApplication::setOrganizationDomain("olivierclero.com");
  QCoreApplication::setApplicationVersion("1.0.0");
  QApplication::setWindowIcon(QIcon(QStringLiteral(":/showcase/qlementine_icon.ico")));

#if USE_CUSTOM_STYLE
  // Custom QStyle.
  auto* const style = new oclero::qlementine::QlementineStyle(&qApplication);
  style->setAnimationsEnabled(true);
  style->setAutoIconColor(oclero::qlementine::AutoIconColor::TextColor);
  style->setThemeJsonPath(QStringLiteral(":/light.json"));
  qApplication.setStyle(style);
  // Custom icon theme.
  QIcon::setThemeName("qlementine");
#endif

  auto window = std::make_unique<oclero::qlementine::showcase::ShowcaseWindow>();
#if USE_CUSTOM_STYLE
  window->setCustomStyle(style);
#endif

  window->show();

  return qApplication.exec();
}
