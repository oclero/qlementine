#include <oclero/qlementine/widgets/CommandLinkButton.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/QlementineStyleOption.hpp>

#include <QStylePainter>

namespace oclero::qlementine {
QSize CommandLinkButton::sizeHint() const {
	if (const auto* qlementineStyle = qobject_cast<QlementineStyle*>(style())) {
		ensurePolished();
		QStyleOptionCommandLinkButton opt;
		initStyleOption(&opt);
		const auto ct = static_cast<QStyle::ContentsType>(QlementineStyle::CT_CommandButton);
		const auto minSize = style()->sizeFromContents(ct, &opt, { 0, 0 }, this);
		return minSize;
	} else {
		return QCommandLinkButton::sizeHint();
	}
}

int CommandLinkButton::heightForWidth(int w) const {
	return w;
}

bool CommandLinkButton::hasHeightForWidth() const {
	return false;
}

void CommandLinkButton::paintEvent(QPaintEvent* e) {
	if (const auto* qlementineStyle = qobject_cast<QlementineStyle*>(style())) {
		QStylePainter p(this);
		QStyleOptionCommandLinkButton opt;
		initStyleOption(&opt);
		const auto ce = static_cast<QStyle::ControlElement>(QlementineStyle::CE_CommandButton);
		p.drawControl(ce, opt);
	} else {
		QCommandLinkButton::paintEvent(e);
	}
}

void CommandLinkButton::initStyleOption(QStyleOptionCommandLinkButton* option) const {
	QCommandLinkButton::initStyleOption(option);
	const auto pm = static_cast<QStyle::PixelMetric>(QlementineStyle::PM_MediumIconSize);
	const auto iconExtent = style()->pixelMetric(pm, option, this);
	option->description = description();
	option->iconSize = QSize{ iconExtent, iconExtent };
}
} // namespace oclero::qlementine
