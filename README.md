## BSD - Bitsend Core 1.2.7.0
----------------
![alt tag](https://dl.dropboxusercontent.com/u/21000833/Bitsend/Website/Splash.png)

 http://www.bitsend.info 
 Bitcointalk https://bitcointalk.org/index.php?topic=895425.msg13884353#msg13884353

Copyright (c) 2009-2017 Bitcoin Core Developers
Copyright (c) 2014-2015 Dash Core Developers
Copyright (c) 2014 Doge Core Developers
Copyright (c) 2013 Dr Kimoto Chan
Copyright (c) 2014-2015 LIMX Core Developers
Copyright (c) 2017 BitSend Core Developers


Release 21.03.2017
----------------
Update BitSend 1.2.7.1
- add the command `shrinkdebuginterval` (new feature which allows shrinking the debug file periodically)
- getblocktemplate fix
- add new Help File "Create build-ubuntu-1404.md"


Release 13.03.2017
----------------
Update BitSend 1.2.7.0
- Add Peerlist Tab (Debug console)
- Small changes Bitsend splash and paperwallet
- Add new Nodes & Checkpoints
- Copyright 2017


What is Bitsend?
----------------

Bitsend is an experimental and new digital currency that enables anonymous, instant
payments to anyone, anywhere in the world. Bitsend uses peer-to-peer technology
to operate without a central authority: transaction management and money subsidy
are carried out collectively by the network. Bitsend Core is the name of open
source software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the Bitsend Core software, see http://www.bitsend.eu



Building process
-----------------

**compiling Bitsend from git**

Use the autogen script to prepare the build environment.

    ./autogen.sh
    ./configure
    make

**precompiled binaries**

Precompiled binaries are available at github, see
http://sourceforge.net/projects/bitsend/files/?source=navbar

Always verify the signatures and checksums.


Development tips and tricks
---------------------------

**compiling for debugging**

Run configure with the --enable-debug option, then make. Or run configure with
CXXFLAGS="-g -ggdb -O0" or whatever debug flags you need.

**debug.log**

If the code is behaving strangely, take a look in the debug.log file in the data directory;
error and debugging message are written there.

The -debug=... command-line option controls debugging; running with just -debug will turn
on all categories (and give you a very large debug.log file).

The Qt code routes qDebug() output to debug.log under category "qt": run with -debug=qt
to see it.

**testnet and regtest modes**

Run with the -testnet option to run with "play bitsends" on the test network, if you
are testing multi-machine code that needs to operate across the internet.

If you are testing something that can run on one machine, run with the -regtest option.
In regression test mode blocks can be created on-demand; see qa/rpc-tests/ for tests
that run in -regtest mode.

**DEBUG_LOCKORDER**

Bitsend Core is a multithreaded application, and deadlocks or other multithreading bugs
can be very difficult to track down. Compiling with -DDEBUG_LOCKORDER (configure
CXXFLAGS="-DDEBUG_LOCKORDER -g") inserts run-time checks to keep track of what locks
are held, and adds warning to the debug.log file if inconsistencies are detected.
