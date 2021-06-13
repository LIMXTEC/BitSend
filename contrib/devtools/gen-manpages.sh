#!/usr/bin/env bash
# Copyright (c) 2016-2019 The BitSend Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

BITSENDD=${BITSENDD:-$BINDIR/bitsendd}
BITSENDCLI=${BITSENDCLI:-$BINDIR/bitsend-cli}
BITSENDTX=${BITSENDTX:-$BINDIR/bitsend-tx}
WALLET_TOOL=${WALLET_TOOL:-$BINDIR/bitsend-wallet}
BITSENDQT=${BITSENDQT:-$BINDIR/qt/bitsend-qt}

[ ! -x $BITSENDD ] && echo "$BITSENDD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
read -r -a BTCVER <<< "$($BITSENDCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }')"

# Create a footer file with copyright content.
# This gets autodetected fine for bitsendd if --version-string is not set,
# but has different outcomes for bitsend-qt and bitsend-cli.
echo "[COPYRIGHT]" > footer.h2m
$BITSENDD --version | sed -n '1!p' >> footer.h2m

for cmd in $BITSENDD $BITSENDCLI $BITSENDTX $WALLET_TOOL $BITSENDQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
