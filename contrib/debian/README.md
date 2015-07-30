
Debian
====================
This directory contains files used to package limecoinxd/limecoinx-qt
for Debian-based Linux systems. If you compile limecoinxd/limecoinx-qt yourself, there are some useful files here.

## limecoinx: URI support ##


limecoinx-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install limecoinx-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your limecoinx-qt binary to `/usr/bin`
and the `../../share/pixmaps/limecoinx128.png` to `/usr/share/pixmaps`

limecoinx-qt.protocol (KDE)

