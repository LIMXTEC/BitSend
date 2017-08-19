## BSD - Bitsend Core 1.4.0
#---WARNING----This Branch is under development----#
If you want to build or use source, please relay on release tags. This branch is highly experimental.

first Mile Stone 
-libsecp256k1
- pay to child
- Segwit 

Next Milestone 
- masternode 
- instant x
- difficulty rtetarget

----------------
![alt tag](https://dl.dropboxusercontent.com/u/21000833/Bitsend/Website/Splash.png)

 http://www.bitsend.info 
 Bitsendtalk https://bitsendtalk.org/index.php?topic=1370307
 

Release 21.03.2017
----------------
Update BitSend 1.2.7.1
- add the command `shrinkdebuginterval` (new feature which allows shrinking the debug file periodically)
- getblocktemplate fix
- add new Help File "Create build-ubuntu-1404.md"


What is Bitsend?
----------------

Bitsend is an experimental and new digital currency that enables anonymous, instant
payments to anyone, anywhere in the world. Bitsend uses peer-to-peer technology
to operate without a central authority: transaction management and money subsidy
are carried out collectively by the network. Bitsend Core is the name of open
source software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the Bitsend Core software, see http://www.bitsend.info

License
-------

Bitsend Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is meant to be stable. Development is normally done in separate branches.
[Tags](https://github.com/LIMXTEC/BitSend/tags) are created to indicate new official,
stable release versions of BitSend Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).


Building process
-----------------

**compiling Bitsend from git**

Use the autogen script to prepare the build environment.

    ./autogen.sh
    ./configure
    make

**precompiled binaries**

Precompiled binaries are available at GitHub, see
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
