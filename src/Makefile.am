# Copyright (c) 2013-2018 The Bitsend Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

BITSEND_INCLUDES=-I$(builddir) $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

BITSEND_INCLUDES += -I$(srcdir)/secp256k1/include
BITSEND_INCLUDES += $(UNIVALUE_CFLAGS)

LIBBITSEND_SERVER=libbitsend_server.a
LIBBITSEND_COMMON=libbitsend_common.a
LIBBITSEND_CONSENSUS=libbitsend_consensus.a
LIBBITSEND_CLI=libbitsend_cli.a
LIBBITSEND_UTIL=libbitsend_util.a
LIBBITSEND_CRYPTO=crypto/libbitsend_crypto.a
LIBBITSENDQT=qt/libbitsendqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBBITSEND_ZMQ=libbitsend_zmq.a
endif
if BUILD_BITSEND_LIBS
LIBBITSENDCONSENSUS=libbitsendconsensus.la
endif
if ENABLE_WALLET
LIBBITSEND_WALLET=libbitsend_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBBITSEND_CRYPTO) \
  $(LIBBITSEND_UTIL) \
  $(LIBBITSEND_COMMON) \
  $(LIBBITSEND_CONSENSUS) \
  $(LIBBITSEND_SERVER) \
  $(LIBBITSEND_CLI) \
  $(LIBBITSEND_WALLET) \
  $(LIBBITSEND_ZMQ)

lib_LTLIBRARIES = $(LIBBITSENDCONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_BITSENDD
  bin_PROGRAMS += bitsendd
endif

if BUILD_BITSEND_UTILS
  bin_PROGRAMS += bitsend-cli bitsend-tx
endif

.PHONY: FORCE check-symbols check-security
# bitsend core #
BITSEND_CORE_H = \
  addrdb.h \
  activemasternode.h \
  addrman.h \
  base58.h \
  bech32.h \
  bloom.h \
  blockencodings.h \
  cachemap.h \
  cachemultimap.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/tx_verify.h \
  core_io.h \
  core_memusage.h \
  cuckoocache.h \
  fs.h \
  dsnotificationinterface.h \
  governance.h \
  governance-classes.h \
  governance-exceptions.h \
  governance-object.h \
  governance-validators.h \
  governance-vote.h \
  governance-votedb.h \
  flat-database.h \
  httprpc.h \
  httpserver.h \
  indirectmap.h \
  init.h \
  key.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  masternode.h \
  masternode-payments.h \
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  memusage.h \
  merkleblock.h \
  messagesigner.h \
  miner.h \
  net.h \
  net_processing.h \
  netaddress.h \
  netbase.h \
  netfulfilledman.h \
  netmessagemaker.h \
  noui.h \
  policy/feerate.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  protocol.h \
  random.h \
  reverse_iterator.h \
  reverselock.h \
  rpc/blockchain.h \
  rpc/client.h \
  rpc/mining.h \
  rpc/protocol.h \
  rpc/safemode.h \
  rpc/server.h \
  rpc/register.h \
  rpc/util.h \
  scheduler.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/ismine.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/lockedpool.h \
  sync.h \
  threadsafety.h \
  threadinterrupt.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utiltime.h \
  validation.h \
  validationinterface.h \
  versionbits.h \
  wallet/coincontrol.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/feebumper.h \
  wallet/fees.h \
  wallet/init.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  wallet/walletutil.h \
  warnings.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h \
  crypto/scrypt.h \
  $(BITSEND_TIMETRAVEL_H)


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libbitsend_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between bitsendd and bitsend-qt
libbitsend_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libbitsend_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitsend_server_a_SOURCES = \
  activemasternode.cpp \
  addrdb.cpp \
  addrman.cpp \
  bloom.cpp \
  blockencodings.cpp \
  chain.cpp \
  checkpoints.cpp \
  consensus/tx_verify.cpp \
  dsnotificationinterface.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  governance.cpp \
  governance-classes.cpp \
  governance-object.cpp \
  governance-validators.cpp \
  governance-vote.cpp \
  governance-votedb.cpp \
  masternode.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  merkleblock.cpp \
  messagesigner.cpp \
  miner.cpp \
  net.cpp \
  netfulfilledman.cpp \
  net_processing.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  policy/rbf.cpp \
  pow.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/masternode.cpp \
  rpc/governance.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/safemode.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  script/ismine.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  ui_interface.cpp \
  validation.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(BITSEND_CORE_H)

if ENABLE_ZMQ
libbitsend_zmq_a_CPPFLAGS = $(BITSEND_INCLUDES) $(ZMQ_CFLAGS)
libbitsend_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitsend_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between bitsendd and bitsend-qt, but only linked
# when wallet enabled
libbitsend_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES)
libbitsend_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitsend_wallet_a_SOURCES = \
  rpc/misc.cpp \
  script/ismine.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/feebumper.cpp \
  wallet/fees.cpp \
  wallet/init.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  wallet/walletutil.cpp \
  $(BITSEND_CORE_H)



# crypto primitives library
crypto_libbitsend_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(SSL_CFLAGS)
crypto_libbitsend_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitsend_crypto_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h \
  crypto/aes_helper.c \
  crypto/luffa.c \
  crypto/groestl.c \
  crypto/jh.c \
  crypto/echo.c \
  crypto/shavite.c \
  crypto/keccak.c \
  crypto/skein.c \
  crypto/fugue.c \
  crypto/hamsi.c \
  crypto/haval.c \
  crypto/shabal.c \
  crypto/whirlpool.c \
  crypto/sph_sha2big.c \
  crypto/bmw.c \
  crypto/simd.c \
  crypto/cubehash.c \
  crypto/blake.c \
  crypto/sph_blake.h \
  crypto/sph_bmw.h \
  crypto/sph_cubehash.h \
  crypto/sph_echo.h \
  crypto/sph_groestl.h \
  crypto/sph_jh.h \
  crypto/sph_keccak.h \
  crypto/sph_luffa.h \
  crypto/sph_shavite.h \
  crypto/sph_simd.h \
  crypto/sph_skein.h \
  crypto/sph_types.h \
  crypto/sph_fugue.h \
  crypto/sph_hamsi.h \
  crypto/sph_haval.h \
  crypto/sph_shabal.h \
  crypto/sph_sha2.h \
  crypto/sph_whirlpool.h

if USE_ASM
crypto_libbitsend_crypto_a_SOURCES += crypto/sha256_sse4.cpp
endif

# consensus: shared between all executables that validate any consensus rules.
libbitsend_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES) $(BOOST_CPPFLAGS)
libbitsend_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitsend_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/bitsendconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  utilstrencodings.cpp \
  utilstrencodings.h \
  version.h \
  crypto/scrypt.cpp \
  crypto/scrypt-sse2.cpp \
  crypto/scrypt.h

# common: shared between bitsendd, and bitsend-qt and non-server tools
libbitsend_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES)
libbitsend_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitsend_common_a_SOURCES = \
  base58.cpp \
  bech32.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  crypto/scrypt.cpp \
  $(BITSEND_TIMETRAVEL) \
  key.cpp \
  keystore.cpp \
  netaddress.cpp \
  netbase.cpp \
  policy/feerate.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/sign.cpp \
  script/standard.cpp \
  warnings.cpp \
  $(BITSEND_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libbitsend_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES)
libbitsend_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitsend_util_a_SOURCES = \
  support/lockedpool.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  fs.cpp \
  random.cpp \
  rpc/protocol.cpp \
  rpc/util.cpp \
  support/cleanse.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(BITSEND_CORE_H)

if GLIBC_BACK_COMPAT
libbitsend_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between bitsend-cli and bitsend-qt
libbitsend_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES)
libbitsend_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitsend_cli_a_SOURCES = \
  rpc/client.cpp \
  $(BITSEND_CORE_H)

nodist_libbitsend_util_a_SOURCES = $(srcdir)/obj/build.h
#

# bitsendd binary #
bitsendd_SOURCES = bitsendd.cpp
bitsendd_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES)
bitsendd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
bitsendd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
bitsendd_SOURCES += bitsendd-res.rc
endif

bitsendd_LDADD = \
  $(LIBBITSEND_SERVER) \
  $(LIBBITSEND_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBBITSEND_UTIL) \
  $(LIBBITSEND_WALLET) \
  $(LIBBITSEND_ZMQ) \
  $(LIBBITSEND_CONSENSUS) \
  $(LIBBITSEND_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

bitsendd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# bitsend-cli binary #
bitsend_cli_SOURCES = bitsend-cli.cpp
bitsend_cli_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES) $(EVENT_CFLAGS)
bitsend_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
bitsend_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
bitsend_cli_SOURCES += bitsend-cli-res.rc
endif

bitsend_cli_LDADD = \
  $(LIBBITSEND_CLI) \
  $(LIBUNIVALUE) \
  $(LIBBITSEND_UTIL) \
  $(LIBBITSEND_CRYPTO)

bitsend_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# bitsend-tx binary #
bitsend_tx_SOURCES = bitsend-tx.cpp
bitsend_tx_CPPFLAGS = $(AM_CPPFLAGS) $(BITSEND_INCLUDES)
bitsend_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
bitsend_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
bitsend_tx_SOURCES += bitsend-tx-res.rc
endif

bitsend_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBBITSEND_COMMON) \
  $(LIBBITSEND_UTIL) \
  $(LIBBITSEND_CONSENSUS) \
  $(LIBBITSEND_CRYPTO) \
  $(LIBBITSEND_TIMETRAVEL) \
  $(LIBSECP256K1)

bitsend_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# bitsendconsensus library #
if BUILD_BITSEND_LIBS
include_HEADERS = script/bitsendconsensus.h
libbitsendconsensus_la_SOURCES = $(crypto_libbitsend_crypto_a_SOURCES) $(libbitsend_consensus_a_SOURCES)

if GLIBC_BACK_COMPAT
  libbitsendconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libbitsendconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libbitsendconsensus_la_LIBADD = $(LIBSECP256K1) $(CRYPTO_LIBS) $(BOOST_LIBS)
libbitsendconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) $(BOOST_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_BITSEND_INTERNAL $(SSL_CFLAGS)
libbitsendconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += obj/build.h

EXTRA_DIST = $(CTAES_DIST)


config/bitsend-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/bitsend-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/bitsend-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/bitsend-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
