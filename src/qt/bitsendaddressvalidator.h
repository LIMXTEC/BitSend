// Copyright (c) 2011-2014 The BitSend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITSEND_QT_BITSENDADDRESSVALIDATOR_H
#define BITSEND_QT_BITSENDADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BitSendAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitSendAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** BitSend address widget validator, checks for a valid bitsend address.
 */
class BitSendAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitSendAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // BITSEND_QT_BITSENDADDRESSVALIDATOR_H
