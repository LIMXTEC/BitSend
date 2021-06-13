BitSend Core
=============

Setup
---------------------
BitSend Core is the original BitSend client and it builds the backbone of the network. It downloads and, by default, stores the entire history of BitSend transactions, which requires a few hundred gigabytes of disk space. Depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

To download BitSend Core, visit [bitsendcore.org](https://bitsendcore.org/en/download/).

Running
---------------------
The following are some helpful notes on how to run BitSend Core on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/bitsend-qt` (GUI) or
- `bin/bitsendd` (headless)

### Windows

Unpack the files into a directory, and then run bitsend-qt.exe.

### macOS

Drag BitSend Core to your applications folder, and then run BitSend Core.

### Need Help?

* See the documentation at the [BitSend Wiki](https://en.bitsend.it/wiki/Main_Page)
for help and more information.
* Ask for help on [#bitsend](https://webchat.freenode.net/#bitsend) on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#bitsend).
* Ask for help on the [BitSendTalk](https://bitsendtalk.org/) forums, in the [Technical Support board](https://bitsendtalk.org/index.php?board=4.0).

Building
---------------------
The following are developer notes on how to build BitSend Core on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [FreeBSD Build Notes](build-freebsd.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [NetBSD Build Notes](build-netbsd.md)
- [Gitian Building Guide (External Link)](https://github.com/bitsend-core/docs/blob/master/gitian-building.md)

Development
---------------------
The BitSend repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Productivity Notes](productivity.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://doxygen.bitsendcore.org/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [JSON-RPC Interface](JSON-RPC-interface.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Discuss on the [BitSendTalk](https://bitsendtalk.org/) forums, in the [Development & Technical Discussion board](https://bitsendtalk.org/index.php?board=6.0).
* Discuss project-specific development on #bitsend-core-dev on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#bitsend-core-dev).
* Discuss general BitSend development on #bitsend-dev on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#bitsend-dev).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [bitsend.conf Configuration File](bitsend-conf.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [Reduce Memory](reduce-memory.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [ZMQ](zmq.md)
- [PSBT support](psbt.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
