//
// C++ Interface: qwwulongspinbox
//
// Description:
//
//
// Author: Witold Wysota <wysota@wysota.eu.org>, (C) 2007
// Remade by mamontov-cpp for unsigned long long
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QWWULONGSPINBOX_H
#define QWWULONGSPINBOX_H



#if !defined(WW_NO_ULONGSPINBOX) && !defined(QT_NO_SPINBOX)
#include <wwglobal.h>
#include <QAbstractSpinBox>
#include <wwglobal.h>
#include <wwglobal_p.h>


class QwwULongSpinBoxPrivate;

class Q_WW_EXPORT QwwULongSpinBox : public QAbstractSpinBox, public QwwPrivatable {
    Q_OBJECT
    Q_PROPERTY(qulonglong minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qulonglong maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(qulonglong value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(qulonglong singleStep READ singleStep WRITE setSingleStep)
// Q_PROPERTY(QString cleanText READ cleanText)
public:
    QwwULongSpinBox(QWidget *parent = 0);

    virtual void stepBy ( int steps );

    qulonglong value() const;

    qulonglong minimum() const;
    void setMinimum(qulonglong v);

    qulonglong maximum() const;
    void setMaximum(qulonglong v);

    const QString &prefix() const;
    void setPrefix(const QString &);

    const QString &suffix() const;
    void setSuffix(const QString &);

    qulonglong singleStep() const;
    void setSingleStep(qulonglong s);

    void setRange(qulonglong min, qulonglong max);
    virtual QValidator::State validate ( QString & input, int & pos ) const;
    virtual void fixup ( QString & input ) const;
public Q_SLOTS:
    void setValue(qulonglong v);
Q_SIGNALS:
    void valueChanged ( qulonglong i );
    void valueChanged ( const QString & text );
protected:
    virtual QString textFromValue(qulonglong value) const;
    virtual qulonglong valueFromText(const QString &text) const;
    virtual StepEnabled stepEnabled () const;
private:
    WW_DECLARE_PRIVATE(QwwULongSpinBox);
    Q_DISABLE_COPY(QwwULongSpinBox);
    Q_PRIVATE_SLOT(d_func(), void _q_editorTextChanged(const QString &t));
};

#endif // WW_NO_SPINBOX

/** Mock failed, when this was inside of source code
 * \internal
 */
class QwwULongSpinBoxPrivate : public QwwPrivate {
public:
    WW_DECLARE_PUBLIC(QwwULongSpinBox);
    QwwULongSpinBoxPrivate(QwwULongSpinBox *p) : QwwPrivate(p) {
        value = 0;
        minimum = 0;
        maximum = 99;
        singlestep = 1;
    }
    qulonglong value;
    qulonglong minimum;
    qulonglong maximum;
    qulonglong singlestep;
    QString prefix;
    QString suffix;
//     void _q_editorTextChanged(const QString & t);
    void updateEdit();
    void updateButtons();
    QString textFromValue(qulonglong value) const { //-V688
        QString s = QLocale().toString(value);
        s.remove(QLocale().groupSeparator());
        return s;
    }
    qulonglong valueFromText(const QString &text) const {
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
