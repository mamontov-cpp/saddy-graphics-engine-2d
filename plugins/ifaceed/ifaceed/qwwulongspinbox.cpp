//
// C++ Implementation: "unsigned long long" value spin box widget
//
// Description:
//
//
// Author: Witold Wysota <wwwidgets@wysota.eu.org>, (C) 2007-2010
// Remade by HiddenSeeker
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qwwulongspinbox.h"

#if !defined(WW_NO_LONGSPINBOX) && !defined(QT_NO_SPINBOX)

#include <QLineEdit>
#include <QMessageBox>
#include <QStyleOptionSpinBox>
#include <QLocale>
#include <QValidator>
#include "wwglobal_p.h"

void QwwULongSpinBoxPrivate::updateEdit() {
    Q_Q(QwwULongSpinBox);
    QLineEdit *edit = q->lineEdit();
    QString textval = prefix + q->textFromValue(value) + suffix;
    const bool empty = edit->text().isEmpty();
    int cursor = edit->cursorPosition();
    const bool sb = edit->blockSignals(true);
    edit->setText(textval);
    cursor = qBound(prefix.size(), cursor, edit->displayText().size() - suffix.size());
    edit->setCursorPosition(empty ? prefix.size() : cursor);
    edit->blockSignals(sb);
    updateButtons();
}


void QwwULongSpinBoxPrivate::updateButtons() {
    Q_Q(QwwULongSpinBox);
    QStyleOptionSpinBox opt;
    opt.init(q);
    opt.activeSubControls = 0;
    opt.buttonSymbols = q->buttonSymbols();
    opt.subControls = QStyle::SC_SpinBoxUp | QStyle::SC_SpinBoxDown | QStyle::SC_SpinBoxFrame;
    opt.stepEnabled = q->stepEnabled();
    opt.frame = q->hasFrame();
    q->update(q->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxUp, q));
    q->update(q->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxDown, q));
}

void QwwULongSpinBoxPrivate::_q_editorTextChanged(const QString & t) {
    Q_Q(QwwULongSpinBox);
    QString tmp = t;
    int pos = q->lineEdit()->cursorPosition();
    QValidator::State state = q->validate(tmp, pos);
    if (state==QValidator::Acceptable) {
        q->setValue(valueFromText(tmp));
    }
}

class QwwULongSpinBoxValidator : public QValidator {
public:
    QwwULongSpinBoxValidator(QwwULongSpinBox *sb) : QValidator(sb), m_spinbox(sb) {}
    void fixup ( QString & input ) const {
        m_spinbox->fixup(input);
    }
    State validate ( QString & input, int & pos ) const {
        if (!m_spinbox->prefix().isEmpty() && !input.startsWith(m_spinbox->prefix())) {
            input.prepend(m_spinbox->prefix());
        }
        if (!m_spinbox->suffix().isEmpty() && !input.endsWith(m_spinbox->suffix())) {
            input.append(m_spinbox->suffix());
        }
        return m_spinbox->validate(input, pos);
    }
private:
    QwwULongSpinBox *m_spinbox;
};


/*!
 *  \class  QwwLongSpinBox
 *  \brief  The QwwLongSpinBox class provides a spinbox taking long long values.
 *  \inmodule wwWidgets
 *
 *  \image qwwlongspinbox.png QwwLongSpinBox
 */
/*!
 * \fn      void QwwLongSpinBox::valueChanged( qlonglong i )
 * \brief   This signal is emitted whenever value of the spin box is set to \a i
 */
/*!
 * \fn      void QwwLongSpinBox::valueChanged( const QString & text )
 * \brief   This signal is emitted whenever the textual representation of the value changes to \a text.
 */

/*!
 * Constructs a long spin box widget with a given \a parent.
 *
 */
QwwULongSpinBox::QwwULongSpinBox(QWidget *parent)
        : QAbstractSpinBox(parent), QwwPrivatable(new QwwULongSpinBoxPrivate(this)) {
    Q_D(QwwULongSpinBox);
    d->updateEdit();

    lineEdit()->setValidator(new QwwULongSpinBoxValidator(this));
    connect(lineEdit(), SIGNAL(textChanged(const QString &)), SLOT(_q_editorTextChanged(const QString &)));
}



/*!
 * \property QwwLongSpinBox::value
 * \brief Property holds the spinbox value.
 */
qulonglong QwwULongSpinBox::value() const {
    Q_D(const QwwULongSpinBox);
    return d->value;
}

/*!
 * \property QwwLongSpinBox::minimum
 * \brief Property holds the minimum value allowed for the spinbox.
 */
qulonglong QwwULongSpinBox::minimum() const {
    Q_D(const QwwULongSpinBox);
    return d->minimum;
}

/*!
 * \property QwwLongSpinBox::maximum
 * \brief Property holds the maximum value allowed for the spinbox.
 */
qulonglong QwwULongSpinBox::maximum() const {
    Q_D(const QwwULongSpinBox);
    return d->maximum;
}

/*!
 * \property QwwLongSpinBox::prefix
 * \brief Property holds the spinbox's prefix.
 */
const QString & QwwULongSpinBox::prefix() const {
    Q_D(const QwwULongSpinBox);
    return d->prefix;
}

/*!
 * \property QwwLongSpinBox::suffix
 * \brief Property holds the spinbox's suffix.
 */
const QString & QwwULongSpinBox::suffix() const {
    Q_D(const QwwULongSpinBox);
    return d->suffix;
}

/*!
 *  \brief  Returns textual representation of the \a value.
 */
QString QwwULongSpinBox::textFromValue(qulonglong value) const {
    Q_D(const QwwULongSpinBox);
    return d->textFromValue(value);
}

/*!
 *  \brief  Returns numerical representation of the \a text.
 */
qulonglong QwwULongSpinBox::valueFromText(const QString & text) const {
    Q_D(const QwwULongSpinBox);
    return d->valueFromText(text);
}

/*!
 *
 */
void QwwULongSpinBox::setMaximum(qulonglong v) {
    if (maximum()==v)
        return;
    Q_D(QwwULongSpinBox);
    setRange(qMin(d->minimum, v), v);
}

/**
 *
 * @param v
 */
void QwwULongSpinBox::setMinimum(qulonglong v) {
    if (minimum()==v)
        return;
    Q_D(QwwULongSpinBox);
    setRange(v, qMax(d->maximum, v));
}

/*!
 * \brief Sets the spinbox value to \a v
 * \param v
 */
void QwwULongSpinBox::setValue(qulonglong v) {
    v = qBound(minimum(), v, maximum());
    if (value()==v)
        return;
    Q_D(QwwULongSpinBox);
    d->value = v;
    QString textval = d->prefix + textFromValue(v) + d->suffix;
    d->updateEdit();
    emit valueChanged(v);
    emit valueChanged(textval);
}

/**
 * @brief Sets the spinbox prefix
 * @param p
 */
void QwwULongSpinBox::setPrefix(const QString &p) {
    Q_D(QwwULongSpinBox);
    d->prefix = p;
    d->updateEdit();
}

/**
 * @brief Sets the spinbox suffix
 * @param s
 */
void QwwULongSpinBox::setSuffix(const QString &s) {
    Q_D(QwwULongSpinBox);
    d->suffix = s;
    d->updateEdit();
}

/*!
 * \property QwwLongSpinBox::singleStep
 * \brief This property holds the step for the spinbox
 */
qulonglong QwwULongSpinBox::singleStep() const {
    Q_D(const QwwULongSpinBox);
    return d->singlestep;
}

/**
 * @brief   Sets the step size for the spinbox
 */
void QwwULongSpinBox::setSingleStep(qulonglong s) {
    if (s==0)
        return;
    Q_D(QwwULongSpinBox);
    d->singlestep = s;
}

/*!
 * \brief   Sets the range of values allowed for the spinbox to \a min and \a max
 */
void QwwULongSpinBox::setRange(qulonglong min, qulonglong max) {
    Q_D(QwwULongSpinBox);
    d->minimum = min;
    d->maximum = max;
    if (value()<d->minimum) {
        setValue(d->minimum);
    } else if (value()>d->maximum) {
        setValue(d->maximum);
    }
}


/*!
 * \brief   Changes the spinbox value by a number of \a steps
 * \param steps
 */
void QwwULongSpinBox::stepBy(int steps) {
    if (steps==0)
        return;
    qulonglong nvalue = value()+steps*singleStep();
    if (wrapping()) {
        qlonglong d = nvalue-maximum();
        qlonglong nd = nvalue - minimum();
        if (nvalue>value() && d>0) {
            nvalue = minimum()+d-1;
        } else if (nvalue < value() && nd<0) {
            nvalue = maximum()+nd+1;
        }
    } else {
        nvalue = qBound(minimum(), nvalue, maximum());
    }
    setValue(nvalue);
    selectAll();
}


/*!
 * \brief   Validates the spinbox \a input with cursor at position \a pos
 */
QValidator::State QwwULongSpinBox::validate(QString & input, int & pos) const {
    QString tinput = input;
    if (!prefix().isEmpty() && tinput.startsWith(prefix()))
        tinput.remove(0, prefix().size());
    if (!suffix().isEmpty() && tinput.endsWith(suffix()))
        tinput.chop(suffix().size());
    bool ok;
    qlonglong v = tinput.toLongLong(&ok);
    if (!ok)
        return QValidator::Invalid;
    if (v<minimum() || v>maximum())
        return QValidator::Invalid;
    return QValidator::Acceptable;
}

/*!
 *  \brief  Attempts to correct the \a input.
 */
void QwwULongSpinBox::fixup(QString & input) const {
    input.remove(QLocale().groupSeparator());
}


/*!
 * \brief Returns set of enabled steps.
 */
QAbstractSpinBox::StepEnabled QwwULongSpinBox::stepEnabled() const {
    if (wrapping())
        return StepUpEnabled|StepDownEnabled;
    StepEnabled se = 0;
    if (value()<maximum()) se |= StepUpEnabled;
    if (value()>minimum()) se |= StepDownEnabled;
    return se;
}


#include "moc_qwwulongspinbox.cpp"
#endif // WW_NO_SPINBOX
