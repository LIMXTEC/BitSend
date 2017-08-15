
Debian
====================
This directory contains files used to package bitsendd/bitsend-qt
for Debian-based Linux systems. If you compile bitsendd/bitsend-qt yourself, there are some useful files here.

## bitsend: URI support ##


bitsend-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bitsend-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bitsend-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitsend128.png` to `/usr/share/pixmaps`

bitsend-qt.protocol (KDE)

