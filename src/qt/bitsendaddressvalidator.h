// Copyright (c) 2011-2014 The Bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITSEND_QT_BITSENDADDRESSVALIDATOR_H
#define BITSEND_QT_BITSENDADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BitsendAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitsendAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Bitsend address widget validator, checks for a valid bitsend address.
 */
class BitsendAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitsendAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BITSEND_QT_BITSENDADDRESSVALIDATOR_H
