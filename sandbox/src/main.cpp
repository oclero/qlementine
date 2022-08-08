#include <QApplication>
#include <QWidget>

#include <oclero/qlementine/style/QlementineStyle.hpp>

#include "SandboxWindow.hpp"
#include "CsdWindow.hpp"

#define USE_CUSTOM_STYLE 1
#define CSD_WINDOW 1

int main(int argc, char* argv[]) {
	QGuiApplication::setApplicationDisplayName("sandbox");
	QCoreApplication::setApplicationName("sandbox");
	QGuiApplication::setDesktopFileName("sandbox");
	QCoreApplication::setOrganizationName("oclero");
	QCoreApplication::setOrganizationDomain("olivierclero.com");
	QCoreApplication::setApplicationVersion("1.0.0");

	QApplication qApplication(argc, argv);

	// Set custom QStyle.
#if USE_CUSTOM_STYLE
	auto* const style = new oclero::qlementine::QlementineStyle(&qApplication);
	style->setAnimationsEnabled(true);
	style->setUseMenuForComboBoxPopup(false);
	style->setThemeJsonPath(QStringLiteral(":/light.json"));
	qApplication.setStyle(style);
#endif

#if 0
	auto window = std::make_unique<CsdWindow>();
#else
	// Show window.
	auto window = std::make_unique<oclero::qlementine::sandbox::SandboxWindow>();
#	if USE_CUSTOM_STYLE
	window->setCustomStyle(style);
#	endif
#endif
	window->show();

	return qApplication.exec();
}
