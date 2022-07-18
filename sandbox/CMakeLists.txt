declare_executable(sandbox EXCLUDE_FROM_ALL ADD_METADATA
	FOLDER sandbox
	APP_NAME "Sandbox"
	EXE_NAME "sandbox"
	COMPANY_NAME "oclero"
	COMPANY_DOMAIN "olivierclero.com"
	APP_ICON "resources/qlementine_icon.ico"
	FILE_ICON "resources/qlementine_icon.ico"
	VERSION ${PROJECT_VERSION}
	SOURCES
		resources/sandbox.qrc
		src/main.cpp
		src/SandboxWindow.cpp
		src/SandboxWindow.hpp
		src/CsdWindow.cpp
		src/CsdWindow.hpp
	LINK_LIBRARIES
		oclero::qlementine
)

set_target_properties(sandbox PROPERTIES
	AUTOMOC ON
	AUTORCC ON
)

configure_executable_postbuild(sandbox)
