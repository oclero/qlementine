#include <QApplication>
#include <QWidget>
#include <QMessageBox>

#include <oclero/qlementine/style/QlementineStyle.hpp>

#include "SandboxWindow.hpp"
#include "CsdWindow.hpp"

#define USE_CUSTOM_STYLE 1
#define CSD_WINDOW 1

int main(int argc, char* argv[]) {
    QApplication qApplication(argc, argv);

  QGuiApplication::setApplicationDisplayName("sandbox");
  QCoreApplication::setApplicationName("sandbox");
  QGuiApplication::setDesktopFileName("sandbox");
  QCoreApplication::setOrganizationName("oclero");
  QCoreApplication::setOrganizationDomain("olivierclero.com");
  QCoreApplication::setApplicationVersion("1.0.0");
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/qlementine_icon.ico")));

  // Set custom QStyle.
#if USE_CUSTOM_STYLE
  auto* const style = new oclero::qlementine::QlementineStyle(&qApplication);
  style->setAnimationsEnabled(true);
  style->setUseMenuForComboBoxPopup(false);
  style->setThemeJsonPath(QStringLiteral(":/light.json"));
  qApplication.setStyle(style);
#endif

    auto button = QMessageBox::question(nullptr, "Sandbox", "Choose one of the demo window", "CSDWindow", "SandboxWindow");

    std::unique_ptr<QWidget> window = nullptr;
    if (button == 0) {
      window = std::move(std::make_unique<CsdWindow>());
    } else if (button == 1){
    // Show window.
    auto _window = std::make_unique<oclero::qlementine::sandbox::SandboxWindow>();
#    if USE_CUSTOM_STYLE
    _window->setCustomStyle(style);
#    endif
      window = std::move(_window);
    }

    window->show();

  return qApplication.exec();
}
