// Copyright (c) 2011-2014 The bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITSEND_QT_BITSENDADDRESSVALIDATOR_H
#define BITSEND_QT_BITSENDADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class bitsendAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit bitsendAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** bitsend address widget validator, checks for a valid bitsend address.
 */
class bitsendAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit bitsendAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BITSEND_QT_BITSENDADDRESSVALIDATOR_H
