#pragma once

#include <oclero/qlementine/style/QlementineStyle.hpp>

#include <QItemDelegate>

namespace oclero::qlementine {
class ComboBoxDelegate : public QItemDelegate {
public:
	ComboBoxDelegate(QWidget* widget);

protected:
	void paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const override;
	QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& idx) const override;

private:
	const QWidget* _widget{ nullptr };
};
} // namespace oclero::qlementine
