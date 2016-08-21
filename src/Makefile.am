include Makefile.include

AM_CPPFLAGS += -I$(builddir)

noinst_LIBRARIES = \
  libbitsend_server.a \
  libbitsend_common.a \
  libbitsend_cli.a
if ENABLE_WALLET
noinst_LIBRARIES += libbitsend_wallet.a
endif

bin_PROGRAMS =

if BUILD_BITCOIND
  bin_PROGRAMS += bitsendd
endif

if BUILD_BITCOIN_CLI
  bin_PROGRAMS += bitsend-cli
endif

SUBDIRS = . $(BUILD_QT) $(BUILD_TEST)
DIST_SUBDIRS = . qt test
.PHONY: FORCE
# bitsend core #
BITCOIN_CORE_H = \
  activemasternode.h \
  addrman.h \
  alert.h \
  allocators.h \
  base58.h bignum.h \
  bloom.h \
  chainparams.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  core.h \
  crypter.h \
  darksend.h \
  darksend-relay.h \
  db.h \
  hash/bitsend11hash.h \
  hash/bitsend17hash.h \
  init.h \
  instantx.h \
  key.h \
  keepass.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  masternode.h \
  masternode-pos.h \
  masternodeman.h \
  masternodeconfig.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  protocol.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  script.h \
  serialize.h \
  hash/sph_blake.h \
  hash/sph_bmw.h \
  hash/sph_cubehash.h \
  hash/sph_echo.h \
  hash/sph_groestl.h \
  hash/sph_jh.h \
  hash/sph_keccak.h \
  hash/sph_luffa.h \
  hash/sph_shavite.h \
  hash/sph_simd.h \
  hash/sph_skein.h \
  hash/sph_types.h \
  hash/sph_fugue.h \
  hash/sph_hamsi.h \
  hash/sph_haval.h \
  hash/sph_shabal.h \
  hash/sph_sha2.h \
  hash/sph_whirlpool.h \
  hash/spork.h \
  sync.h \
  threadsafety.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  util.h \
  version.h \
  walletdb.h \
  wallet.h

JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

obj/build.h: FORCE
	@$(MKDIR_P) $(abs_top_builddir)/src/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
version.o: obj/build.h

libbitsend_server_a_SOURCES = \
  activemasternode.cpp \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  checkpoints.cpp \
  coins.cpp \
  init.cpp \
  keystore.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  rpcblockchain.cpp \
  rpcdarksend.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  txdb.cpp \
  txmempool.cpp \
  $(JSON_H) \
  $(BITCOIN_CORE_H)

libbitsend_wallet_a_SOURCES = \
  activemasternode.cpp \
  db.cpp \
  crypter.cpp \
  rpcdump.cpp \
  rpcwallet.cpp \
  wallet.cpp \
  walletdb.cpp \
  keepass.cpp \
  $(BITCOIN_CORE_H)

libbitsend_common_a_SOURCES = \
  activemasternode.cpp \
  base58.cpp \
  allocators.cpp \
  chainparams.cpp \
  core.cpp \
  darksend.cpp \
  darksend-relay.cpp \
  masternode.cpp \
  masternode-pos.cpp \
  masternodeman.cpp \
  masternodeconfig.cpp \
  instantx.cpp \
  hash/hash.cpp \
  key.cpp \
  netbase.cpp \
  protocol.cpp \
  rpcprotocol.cpp \
  script.cpp \
  sync.cpp \
  util.cpp \
  version.cpp \
  hash/aes_helper.c \
  hash/luffa.c \
  hash/groestl.c \
  hash/jh.c \
  hash/echo.c \
  hash/shavite.c \
  hash/keccak.c \
  hash/skein.c \
  hash/fugue.c \
  hash/hamsi.c \
  hash/haval.c \
  hash/sha2.c \
  hash/shabal.c \
  hash/whirlpool.c \
  hash/sph_sha2big.c \
  spork.cpp \
  hash/bmw.c \
  hash/simd.c \
  hash/cubehash.c \
  hash/blake.c \
  $(BITCOIN_CORE_H)

if GLIBC_BACK_COMPAT
libbitsend_common_a_SOURCES += compat/glibc_compat.cpp
libbitsend_common_a_SOURCES += compat/glibcxx_compat.cpp
endif

libbitsend_cli_a_SOURCES = \
  rpcclient.cpp \
  $(BITCOIN_CORE_H)

nodist_libbitsend_common_a_SOURCES = $(top_srcdir)/src/obj/build.h
#

# bitsendd binary #
bitsendd_LDADD = \
  libbitsend_server.a \
  libbitsend_cli.a \
  libbitsend_common.a \
  $(LIBLEVELDB) \
  $(LIBMEMENV)
if ENABLE_WALLET
bitsendd_LDADD += libbitsend_wallet.a
endif
bitsendd_SOURCES = bitsendd.cpp
#

if TARGET_WINDOWS
bitsendd_SOURCES += bitsendd-res.rc
endif

AM_CPPFLAGS += $(BDB_CPPFLAGS)
bitsendd_LDADD += $(BOOST_LIBS) $(BDB_LIBS)

# bitsend-cli binary #
bitsend_cli_LDADD = \
  libbitsend_cli.a \
  libbitsend_common.a \
  $(BOOST_LIBS)
bitsend_cli_SOURCES = bitsend-cli.cpp
#

if TARGET_WINDOWS
bitsend_cli_SOURCES += bitsend-cli-res.rc
endif

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
leveldb/libleveldb.a: leveldb/libmemenv.a

leveldb/%.a:
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
	  OPT="$(CXXFLAGS) $(CPPFLAGS)"

qt/bitsendstrings.cpp: $(libbitsend_server_a_SOURCES) $(libbitsend_common_a_SOURCES) $(libbitsend_cli_a_SOURCES)
	@test -n $(XGETTEXT) || echo "xgettext is required for updating translations"
	@cd $(top_srcdir); XGETTEXT=$(XGETTEXT) share/qt/extract_strings_qt.py

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb Makefile.include

clean-local:
	-$(MAKE) -C leveldb clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
