# Declare lib.
declare_static_library(qlementine NAMESPACE oclero
	FOLDER libs
	SOURCES
		src/animation/WidgetAnimator.cpp
		src/animation/WidgetAnimationManager.cpp
		src/csd/FramelessWindowBehavior.cpp
		src/csd/WindowsTitleBar.cpp
		src/csd/WindowsTitleBarButton.cpp
		src/csd/WindowsTitleMenuBarProxyStyle.cpp
		src/resources/ResourceInitialization.cpp
		src/style/Delegates.cpp
		src/style/EventFilters.cpp
		src/style/EventFilters.hpp
		src/style/QlementineStyle.cpp
		src/style/Theme.cpp
		src/style/ThemeManager.cpp
		src/utils/BadgeUtils.cpp
		src/utils/FontUtils.cpp
		src/utils/ImageUtils.cpp
		src/utils/PrimitiveUtils.cpp
		src/utils/RadiusesF.cpp
		src/utils/StateUtils.cpp
		src/utils/StyleUtils.cpp
		src/utils/WidgetUtils.cpp
		src/widgets/AbstractItemListWidget.cpp
		src/widgets/Action.cpp
		src/widgets/ActionButton.cpp
		src/widgets/CommandLinkButton.cpp
		src/widgets/Expander.cpp
		src/widgets/FramelessWindow.cpp
		src/widgets/IconWidget.cpp
		src/widgets/Label.cpp
		src/widgets/LineEdit.cpp
		src/widgets/Menu.cpp
		src/widgets/NavigationBar.cpp
		src/widgets/Popover.cpp
		src/widgets/PopoverButton.cpp
		src/widgets/RoundedFocusFrame.cpp
		src/widgets/SegmentedControl.cpp
		src/widgets/StatusBadgeWidget.cpp
		src/widgets/Switch.cpp
	PUBLIC_HEADERS
		include/oclero/qlementine/pch.hpp
		include/oclero/qlementine/Common.hpp
		include/oclero/qlementine/animation/WidgetAnimation.hpp
		include/oclero/qlementine/animation/WidgetAnimator.hpp
		include/oclero/qlementine/animation/WidgetAnimationManager.hpp
		include/oclero/qlementine/csd/FramelessWindowBehavior.hpp
		include/oclero/qlementine/csd/WindowsTitleBar.hpp
		include/oclero/qlementine/csd/WindowsTitleBarButton.hpp
		include/oclero/qlementine/csd/WindowsTitleBarCommon.hpp
		include/oclero/qlementine/csd/WindowsTitleMenuBarProxyStyle.hpp
		include/oclero/qlementine/resources/ResourceInitialization.hpp
		include/oclero/qlementine/style/Delegates.hpp
		include/oclero/qlementine/style/QlementineStyle.hpp
		include/oclero/qlementine/style/QlementineStyleOption.hpp
		include/oclero/qlementine/style/Theme.hpp
		include/oclero/qlementine/style/ThemeManager.hpp
		include/oclero/qlementine/utils/BlurUtils.hpp
		include/oclero/qlementine/utils/BadgeUtils.hpp
		include/oclero/qlementine/utils/FontUtils.hpp
		include/oclero/qlementine/utils/ImageUtils.hpp
		include/oclero/qlementine/utils/PrimitiveUtils.hpp
		include/oclero/qlementine/utils/RadiusesF.hpp
		include/oclero/qlementine/utils/StateUtils.hpp
		include/oclero/qlementine/utils/StyleUtils.hpp
		include/oclero/qlementine/utils/WidgetUtils.hpp
		include/oclero/qlementine/widgets/AbstractItemListWidget.hpp
		include/oclero/qlementine/widgets/Action.hpp
		include/oclero/qlementine/widgets/ActionButton.hpp
		include/oclero/qlementine/widgets/CommandLinkButton.hpp
		include/oclero/qlementine/widgets/Expander.hpp
		include/oclero/qlementine/widgets/FramelessWindow.hpp
		include/oclero/qlementine/widgets/IconWidget.hpp
		include/oclero/qlementine/widgets/Label.hpp
		include/oclero/qlementine/widgets/LineEdit.hpp
		include/oclero/qlementine/widgets/Menu.hpp
		include/oclero/qlementine/widgets/NavigationBar.hpp
		include/oclero/qlementine/widgets/Popover.hpp
		include/oclero/qlementine/widgets/PopoverButton.hpp
		include/oclero/qlementine/widgets/RoundedFocusFrame.hpp
		include/oclero/qlementine/widgets/SegmentedControl.hpp
		include/oclero/qlementine/widgets/StatusBadgeWidget.hpp
		include/oclero/qlementine/widgets/Switch.hpp
	RESOURCES
		resources/qlementine.qrc
		$<$<PLATFORM_ID:Darwin>:resources/qlementine_fonts.qrc>
		$<$<PLATFORM_ID:Windows>:resources/qlementine_fonts_windows.qrc>
	PUBLIC_LINK_LIBRARIES
		Qt${QT_VERSION_MAJOR}::Widgets
		Qt${QT_VERSION_MAJOR}::Svg
)

if(WIN32)
	target_link_libraries(qlementine PRIVATE Qt${QT_VERSION_MAJOR}::WinExtras)
endif()

target_precompile_headers(qlementine PUBLIC include/oclero/qlementine/pch.hpp)

set_target_properties(qlementine PROPERTIES
	AUTOMOC ON
	AUTORCC ON
)

# if(ENABLE_TESTING)
# 	add_subdirectory(tests)
# endif()

if(ENABLE_SANDBOX)
	add_subdirectory(sandbox)
endif()
