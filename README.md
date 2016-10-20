# BSD - Bitsend Core 1.1.0.3
----------------
http://www.bitsend.info Bitcointalk https://bitcointalk.org/index.php?topic=895425.msg13884353#msg13884353

Copyright (c) 2009-2015 Bitcoin Core Developers
Copyright (c) 2014-2015 Dash Core Developers
Copyright (c) 2014 Doge Core Developers
Copyright (c) 2013 Dr Kimoto Chan
Copyright (c) 2014-2015 LIMX Core Developers
Copyright (c) 2016 BitSend Core Developers


Release 15.10.2016
----------------
Update BitSend 1.2.0.0
- Algos switch improvment
- new protocol version
- added Xevan Algo -Xevan is ultra asic resistant
- -New Switchtime is 11/01/2016 @ 12:00am (UTC) 
- New default maintheme



Release 26.08.2016
----------------
Update BitSend 1.1.0.3
Softfork
- X17+ Settings

Release 26.08.2016
----------------
Update BitSend 1.1.0.2

Hardfork
- Change Masternodes from 5000 BSD to 25.000 BSD
- Change Block size up to 10 MB KB (4 MB)
- Change TX size up to 250 KB (100 KB) + 1800 TXs

Softfork
Block 239.000
- Add the new Dual KGW3 with Diffbreak
- Change Blocktime to 3 min (6min)
- Change Reward to 25 BSD(50 BSD)
- change COINBASE_MATURITY to 288 (25)
- new protocol version
Block 240.000
- Change Algo Hash to X17+ (not compatible to X17)

Note: The Algoswitch reduce the difficult after 6 hours. This rewuid a wallet restart. Then enter setgenerate true.

Release 14.02.2016
----------------
The Version 1.0.0.1 adds new functions. 
- BitSend Diffbreak - This function protects the chain against huge miners. After 6 hours every CPU-Miner is able to create new blocks.
- KGW3 - The new KGW setup is now smarter and has a new hard retarget after 60 Blocks (Backup)
- The new Version supports 4 MB blocks
- We increased the TX fee's for more security (Blockspaming)

Rebrand from LimecoinX to BitSend
----------------
- New GFX and Name
- Limecoinx ->BitSend
- LIMX ->BSD
- new protocol 70077

Release 21.04.2015
----------------
The Version 0.11.X adds new functions. 

- 0.11.2.17 Release notes
- Bitsend Core 0.11.2.17 is forked off the Dash Core 0.11.2.17. Please feel free and compare against forks https://github.com/dashpay
- The old bitsend version 0.9.1.4 was forked from the Darkcoin 0.9.1.0 tree, done by iparn https://github.com/IParn/Bitsend
- The first limecoin currency is full implemented in the BSD chain, done by iparn https://github.com/IParn/limecoin
- The following changes are introduced in this major release. 

Bitsend Core:

- Rebrand to Bitsend Core
- Version bumped to 0.11.2.17 to indicate a new major release
- Renamed client to identify with network from Satoshi to Core
- Bumped protocol version to 70075
- Masternode with 5000 BSD
- NEW Reward = 40 BSD per Miner/Pool & 10 BSD per Masternode
- NEW RPC PORT 8800 and NEW P2P Port Default 8886
- Changed testnet address versions 
- Defined BIP32 (HD) address versions
- Adapted BIP44 coin type 5 for Bitsend (0x80000005) as defined in SLIP-0044
- Added new units: limes (1 / 100.000.000 BSD)
- Added units for testnet: tBSD, mtBSD, uBSD, tlimes
- Fixed wallet locking after sending coins
- Add -regtest mode, similar to testnet but private with instant block generation with setgenerate RPC.
- Add separate bitsend-cli client
- - Implemented KeyPass integration for CLI, RPC and Qt: keepass, keepassport, keepasskey, keepassid, keepassname
- Masternodes:

Masternodes:

- Improve support for start-many with multi masternode config
- New masternode rpc commands: stop-many, start-alias, stop-alias, list-conf
- Fixed possible masternode payments exploit
- Better support for non-specific masternode payments
- Added masternode support for regtest
- Randomly sort masternodes before picking next winner
- Show number of masternodes in debug window


Darksend:

- Reduced lower darksend limit to 1.5 BSD
- Fixed progress bar calculation for low amounts
- Improved support for adding BSD after anon has completed
- Added denomination information to Overview tab
- Added more detailed Darksend status information to Overview tab
- Added Darksend high precision matching engine
- Added Darksend balance to `getinfo`
- Changed maximum rounds of mixing to 16


RPC:

- Add `getwalletinfo`, `getblockchaininfo` and `getnetworkinfo` calls
  (will replace hodge-podge `getinfo` at some point)
- Add a `relayfee` field to `getnetworkinfo`
- Always show syncnode in `getpeerinfo`
- `sendrawtransaction`: report the reject code and reason, and make it possible
  to re-send transactions that are already in the mempool
- `getmininginfo` show right genproclimit
- New notion of 'conflicted' transactions, reported as confirmations: -1
- 'listreceivedbyaddress' now provides tx ids
- Add raw transaction hex to 'gettransaction' output
- Updated help and tests for 'getreceivedby(account|address)'
- In 'getblock', accept 2nd 'verbose' parameter, similar to getrawtransaction,
  but defaulting to 1 for backward compatibility
- Add 'verifychain', to verify chain database at runtime
- Add 'dumpwallet' and 'importwallet' RPCs
- 'keypoolrefill' gains optional size parameter
- Add 'getbestblockhash', to return tip of best chain
- Add 'chainwork' (the total work done by all blocks since the genesis block)
  to 'getblock' output
- Make RPC password resistant to timing attacks
- Clarify help messages and add examples
- Add 'getrawchangeaddress' call for raw transaction change destinations
- Reject insanely high fees by default in 'sendrawtransaction'
- Add RPC call 'decodescript' to decode a hex-encoded transaction script
- Make 'validateaddress' provide redeemScript
- Add 'getnetworkhashps' to get the calculated network hashrate
- New RPC 'ping' command to request ping, new 'pingtime' and 'pingwait' fields
  in 'getpeerinfo' output
- Adding new 'addrlocal' field to 'getpeerinfo' output
- Add verbose boolean to 'getrawmempool'
- Add rpc command 'getunconfirmedbalance' to obtain total unconfirmed balance
- Explicitly ensure that wallet is unlocked in `importprivkey`
- Add check for valid keys in `importprivkey`
- Disable SSLv3 (in favor of TLS) for the RPC client and server.


Command-line options:

- Fix `-printblocktree` output
- Show error message if ReadConfigFile fails
- New option: -nospendzeroconfchange to never spend unconfirmed change outputs
- New option: -zapwallettxes to rebuild the wallet's transaction information
- Rename option '-tor' to '-onion' to better reflect what it does
- Add '-disablewallet' mode to let bitsendd run entirely without wallet (when
  built with wallet)
- Update default '-rpcsslciphers' to include TLSv1.2
- make '-logtimestamps' default on and rework help-message
- RPC client option: '-rpcwait', to wait for server start
- Remove '-logtodebugger'
- Allow `-noserver` with bitsendd
- Make -proxy set all network types, avoiding a connect leak.


Block-chain handling and storage:

- Upgrade leveldb to 1.17
- Check for correct genesis (prevent cases where a datadir from the wrong
  network is accidentally loaded)
- Allow txindex to be removed and add a reindex dialog
- Log aborted block database rebuilds
- Store orphan blocks in serialized form, to save memory
- Limit the number of orphan blocks in memory to 750
- Fix non-standard disconnected transactions causing mempool orphans


Protocol and network code:

- Don't poll showmyip.com, it doesn't exist anymore
- Add a way to limit deserialized string lengths and use it
- Increase IsStandard() scriptSig length
- Avoid querying DNS seeds, if we have open connections
- Remove a useless millisleep in socket handler
- Stricter memory limits on CNode
- Better orphan transaction handling
- Add `-maxorphantx=<n>` and `-maxorphanblocks=<n>` options for control over the
  maximum orphan transactions and blocks
- Per-peer block download tracking and stalled download detection
- Prevent socket leak in ThreadSocketHandler and correct some proxy related
  socket leaks
- Use pnode->nLastRecv as sync score (was the wrong way around)
- Drop the fee required to relay a transaction to 0.01mDRK per kilobyte
- Send tx relay flag with version
- New 'reject' P2P message (BIP 0061, see
  https://gist.github.com/gavinandresen/7079034 for draft)
- Dump addresses every 15 minutes instead of 10 seconds
- Relay OP_RETURN data TxOut as standard transaction type
- Remove CENT-output free transaction rule when relaying
- Lower maximum size for free transaction creation
- Send multiple inv messages if mempool.size > MAX_INV_SZ
- Split MIN_PROTO_VERSION into INIT_PROTO_VERSION and MIN_PEER_PROTO_VERSION
- Do not treat fFromMe transaction differently when broadcasting
- Process received messages one at a time without sleeping between messages
- Improve logging of failed connections
- Add some additional logging to give extra network insight
- Limit the number of new addressses to accumulate


Wallet:

- Check redeemScript size does not exceed 520 byte limit
- Ignore (and warn about) too-long redeemScripts while loading wallet
- Make GetAvailableCredit run GetHash() only once per transaction (performance
  improvement)
- Lower paytxfee warning threshold
- Fix importwallet nTimeFirstKey (trigger necessary rescans)
- Log BerkeleyDB version at startup
- CWallet init fix
- Bug fixes and new regression tests to correctly compute
  the balance of wallets containing double-spent (or mutated) transactions
- Store key creation time. Calculate whole-wallet birthday.
- Optimize rescan to skip blocks prior to birthday
- Let user select wallet file with -wallet=foo.dat
- Don't count txins for priority to encourage sweeping
- Don't create empty transactions when reading a corrupted wallet
- Fix rescan to start from beginning after importprivkey


Mining:

- Increase default -blockmaxsize/prioritysize to 750K/50K
- 'getblocktemplate' does not require a key to create a block template
- Mining code fee policy now matches relay fee policy


GUI:

- fix 'opens in testnet mode when presented with a BIP-72 link with no fallback'
- AvailableCoins: acquire cs_main mutex
- Fix unicode character display on MacOSX
- Fix various coin control visual issues
- Show number of in/out connections in debug console
- Show weeks as well as years behind for long timespans behind
- Enable and disable the Show and Remove buttons for requested payments history
  based on whether any entry is selected.
- Show also value for options overridden on command line in options dialog
- Fill in label from address book also for URIs
- Fixes feel when resizing the last column on tables
- Fix ESC in disablewallet mode
- Add expert section to wallet tab in optionsdialog
- Do proper boost::path conversion (fixes unicode in datadir)
- Only override -datadir if different from the default (fixes -datadir in config
  file)
- Show rescan progress at start-up
- Show importwallet progress
- Get required locks upfront in polling functions (avoids hanging on locks)
- Catch Windows shutdown events while client is running
- Optionally add third party links to transaction context menu
- Check for !pixmap() before trying to export QR code (avoids crashes when no QR
  code could be generated)
- Fix "Start bitsend on system login"
- Switch to Qt 5.2.0 for Windows build
- Add payment request (BIP 0070) support
- Improve options dialog
- Show transaction fee in new send confirmation dialog
- Add total balance in overview page
- Allow user to choose data directory on first start, when data directory is
  missing, or when the -choosedatadir option is passed
- Save and restore window positions
- Add vout index to transaction id in transactions details dialog
- Add network traffic graph in debug window
- Add open URI dialog
- Improve receive coins workflow: make the 'Receive' tab into a form to request
  payments, and move historical address list functionality to File menu.
- Move initialization/shutdown to a thread. This prevents "Not responding"
  messages during startup. Also show a window during shutdown.
- Don't regenerate autostart link on every client startup
- Show and store message of normal bitsend:URI
- Fix richtext detection hang issue on very old Qt versions
- OS X: Make use of the 10.8+ user notification center to display Growl-like
  notifications
- OS X: Added NSHighResolutionCapable flag to Info.plist for better font
  rendering on Retina displays.
- OS X: Fix bitsend-qt startup crash when clicking dock icon
- Linux: Fix Gnome bitsend: URI handler


Validation:

- Log reason for non-standard transaction rejection
- Prune provably-unspendable outputs, and adapt consistency check for it.
- Detect any sufficiently long fork and add a warning
- Call the -alertnotify script when we see a long or invalid fork
- Fix multi-block reorg transaction resurrection
- Reject non-canonically-encoded serialization sizes
- Reject dust amounts during validation
- Accept nLockTime transactions that finalize in the next block
- consensus: guard against openssl's new strict DER checks
- fail immediately on an empty signature
- Improve robustness of DER recoding code


Build system:

- Add OSX build descriptors to gitian
- Fix explicit --disable-qt-dbus
- Don't require db_cxx.h when compiling with wallet disabled and GUI enabled
- Improve missing boost error reporting
- gitian-linux: --enable-glibc-back-compat for binary compatibility with old
  distributions
- gitian: don't export any symbols from executable
- gitian: build against Qt 4.6
- devtools: add script to check symbols from Linux gitian executables
- Remove build-time no-IPv6 setting
- Add statically built executables to Linux build
- Switch to autotools-based build system
- Build without wallet by passing `--disable-wallet` to configure, this
  removes the BerkeleyDB dependency
- Upgrade gitian dependencies (libpng, libz, libupnpc, boost, openssl) to more
  recent versions
- Windows 64-bit build support
- Solaris compatibility fixes
- Check integrity of gitian input source tarballs
- Enable full GCC Stack-smashing protection for all OSes
- build: Fix OSX build when using Homebrew and qt5
- Keep symlinks when copying into .app bundle
- osx: fix signing to make Gatekeeper happy (again)


Miscellaneous:

- key.cpp: fail with a friendlier message on missing ssl EC support
- Remove bignum dependency for scripts
- Upgrade OpenSSL to 1.0.1i, includes CVE-2014-0224, CVE-2014-0160 and
  CVE-2014-0076 (see https://www.openssl.org/news/secadv_20140806.txt)
- Upgrade miniupnpc to 1.9.20140701
- Fix boost detection in build system on some platforms
- Replace non-threadsafe C functions (gmtime, strerror and setlocale)
- Add missing cs_main and wallet locks
- Avoid exception at startup when system locale not recognized
- devtools: add a script to fetch and postprocess translations
- Refactor -alertnotify code
- doc: Add instructions for consistent Mac OS X build names

Bitsend Core 0.11.1 supports a full implementation of InstantX, Darksend improvements
and a new version of enforcement compatible with the newer Bitcoin architechure.

- Fully implemented IX
- Added support for DSTX messages, as a result DS should be much faster
- Clear vValue in SelectCoinsMinConf - should fix an issue with conflicted txes
- "Debug window" -> "Tools window" renaming
- "Last Darksend message" text added in overview page
- Many new languages are supported, such as German, Vietnamese, Spanish
- Fixed required maturity of coins before sending
- New masternode payments enforcement implementation
- Added support to ignore IX confirmations when needed
- Added --instantxdepth, which will show X confirmations when a transaction lock is present
- fix coin control crash https://github.com/bitcoin/bitcoin/pull/5700
- always get only confirmed coins by AvailableCoins for every DS relative action
- New languages supported Portuguese, German, Russian, Polish, Spanish, Vietnamese, French,
Italian, Catalan, Chinese, Danish, Finnish, Swedish, Czech, Turkish and Bavarian (and many more)
- Full implementation of spork. Currently this includes 4 different sporks, InstantX, InstantX block enforcement, masternode payments enforcement, and reconverge. This uses the inventory system, so it's super efficient and very powerful for future development. Reconverge will put the blockchain into a mode where it will attempt to reprocess any forks without restarting the client, this means if we even had a fork in the future this can be triggered to fix it without any damage to the network.
- Masternode payments now uses the inventory system, which will result in much lower bandwidth usage
- Improved caching
- use wallet db ds flag in rpc
- update tx types in UI / fix tx decomposition / use wallet db ds flag
- use wallet db to save ds flag / debug
- use AvailableCoinsType instead of string in walletmodel
- New inventory system for IX messaging, providing super fast/low bandwidth IX communication


What is Bitsend?
----------------

Bitsend is an experimental new digital currency that enables anonymous, instant
payments to anyone, anywhere in the world. Bitsend uses peer-to-peer technology
to operate with no central authority: managing transactions and issuing money
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
