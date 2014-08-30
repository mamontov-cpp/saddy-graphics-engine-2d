//
// C++ Interface: qwwlongspinbox
//
// Description:
//
//
// Author: Witold Wysota <wysota@wysota.eu.org>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QWWLONGSPINBOX_H
#define QWWLONGSPINBOX_H



#if !defined(WW_NO_LONGSPINBOX) && !defined(QT_NO_SPINBOX)
#include <wwglobal.h>
#include <QAbstractSpinBox>
#include <wwglobal.h>
#include <wwglobal_p.h>


class QwwLongSpinBoxPrivate;

class Q_WW_EXPORT QwwLongSpinBox : public QAbstractSpinBox, public QwwPrivatable {
    Q_OBJECT
    Q_PROPERTY(qlonglong minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qlonglong maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(qlonglong value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(qlonglong singleStep READ singleStep WRITE setSingleStep)
// Q_PROPERTY(QString cleanText READ cleanText)
public:
    QwwLongSpinBox(QWidget *parent = 0);

    virtual void stepBy ( int steps );

    qlonglong value() const;

    qlonglong minimum() const;
    void setMinimum(qlonglong v);

    qlonglong maximum() const;
    void setMaximum(qlonglong v);

    const QString &prefix() const;
    void setPrefix(const QString &);

    const QString &suffix() const;
    void setSuffix(const QString &);

    qlonglong singleStep() const;
    void setSingleStep(qlonglong s);

    void setRange(qlonglong min, qlonglong max);
    virtual QValidator::State validate ( QString & input, int & pos ) const;
    virtual void fixup ( QString & input ) const;
public Q_SLOTS:
    void setValue(qlonglong v);
Q_SIGNALS:
    void valueChanged ( qlonglong i );
    void valueChanged ( const QString & text );
protected:
    virtual QString textFromValue(qlonglong value) const;
    virtual qlonglong valueFromText(const QString &text) const;
    virtual StepEnabled stepEnabled () const;
private:
    WW_DECLARE_PRIVATE(QwwLongSpinBox);
    Q_DISABLE_COPY(QwwLongSpinBox);
    Q_PRIVATE_SLOT(d_func(), void _q_editorTextChanged(const QString &t));
};

#endif // WW_NO_SPINBOX

/** Mock failed, when this was inside of source code
 * \internal
 */
class QwwLongSpinBoxPrivate : public QwwPrivate {
public:
    WW_DECLARE_PUBLIC(QwwLongSpinBox);
    QwwLongSpinBoxPrivate(QwwLongSpinBox *p) : QwwPrivate(p) {
        value = 0;
        minimum = 0;
        maximum = 99;
        singlestep = 1;
    }
    qlonglong value;
    qlonglong minimum;
    qlonglong maximum;
    qlonglong singlestep;
    QString prefix;
    QString suffix;
//     void _q_editorTextChanged(const QString & t);
    void updateEdit();
    void updateButtons();
    QString textFromValue(qlonglong value) const {
        QString s = QLocale().toString(value);
        s.remove(QLocale().groupSeparator());
        return s;
    }
    qlonglong valueFromText(const QString &text) const {
        QString tmp = text;
        if (!prefix.isEmpty() && tmp.startsWith(prefix)) {
            tmp.remove(0, prefix.size());
        }
        if (!suffix.isEmpty() && tmp.endsWith(suffix)) {
            tmp.chop(suffix.size());
        }
        return tmp.toLongLong();
    }
    void _q_editorTextChanged(const QString & t);
};

#endif
