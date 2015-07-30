// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Limecoinx developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

//
// Main network
//

unsigned int pnSeed[] =
{
  0x2E659772, 0x6C3DB332, 0x68CF9F6E, 0x2D3F011B,
  0x0538D1FF, 0x95ACA665, 0x1BC0903A, 0xC6C766A5, 0xB924485A, 0x7526EDE1, 0xB924485A, 0x5519C5BA, 0x3EDD537F, 0x6C3DB332,
  0x904CEF42, 0x0587BF16, 0xA2F3CEE2, 0xCDD18F2A, 0x2F115487, 0xD4E35BBE, 0x5D989674, 0xBCA61AD1, 0x94FB46C2, 0x904CEF42,
  0x2E69D968, 0x05092709,
};
// //limxdev 04-2015   New Seeds

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xa3;    //limxdev 04-2015   mainc.pp 3072    ggf testnet wert = falshc
        pchMessageStart[1] = 0xd5;    ///limxdev 04-2015
        pchMessageStart[2] = 0xc2;    //limxdev 04-2015
        pchMessageStart[3] = 0xf9;    //limxdev 04-2015
        
  //    name, networkVersion, privateKeyPrefix, WIF_Start, CWIF_Start, donate
  // ("LimecoinX",  0x66, 0xcc, "7",    "X"    , "  ")
  
        vAlertPubKey = ParseHex("048240a8748a80a286b270ba126705ced4f2ce5a7847b3610ea3c06513150dade2a8512ed5ea86320824683fc0818f0ac019214973e677acd1244f6d0571fc5103"); //limxdev 04-2015   ist bei allen gleich
        nDefaultPort = 8886;
        nRPCPort = 8800;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);  // Limecoinx starting difficulty is 1 / 2^12    //limxdev 04-2015   main.cpp 36
        nSubsidyHalvingInterval = 210000; //limxdev 04-2015   UNBEKANNT FUNKTION

        // Genesis block            Sprungmarke AAAAB
        const char* pszTimestamp = "limecoinX ofrece amplia gama de servicios y mejoras para ti";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 100 * COIN; //limxdev 04-2015
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;   //limx
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1402097469;   //limxdev 04-2015
        genesis.nBits    = 0x1e0ffff0;  //limxdev 04-2015
        genesis.nNonce   = 34784351;  //limxdev 04-2015

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000012e1b8843ac9ce8c18603658eaf8895f99d3f5e7e1b7b1686f35e3c087a"));   //limx   
        assert(genesis.hashMerkleRoot == uint256("0xc0c4f68994b78c31b6c7d7a8a895b5a085857adfbb73699bcfe5a247a357df57"));   //checkpoint 38

     //   vSeeds.push_back(CDNSSeedData("", ""));  //Limxian
     //   vSeeds.push_back(CDNSSeedData("", "")); //coinwall
        vSeeds.push_back(CDNSSeedData("85.214.68.75", "85.214.68.75"));
        vSeeds.push_back(CDNSSeedData("108.61.179.50", "108.61.179.50")); // Aika
        vSeeds.push_back(CDNSSeedData("104.207.159.110", "104.207.159.110"));  // Rapta Coingahter
    //    vSeeds.push_back(CDNSSeedData("27.192.144.58", "27.192.144.58"));
    //    vSeeds.push_back(CDNSSeedData("212.227.91.190", "212.227.91.190"));  //p2pool.palim.eu
    //    vSeeds.push_back(CDNSSeedData("5.9.39.9", "5.9.39.9"));       //Bchain

        base58Prefixes[PUBKEY_ADDRESS] = list_of( 102);                    //limxdev 04-2015   addresses start with 'i'
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 5);                    //limxdev 04-2015   script addresses start with '7'
        base58Prefixes[SECRET_KEY] =     list_of(204);                    //limxdev 04-2015  Pubkey +128 limxdev
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E); //limxdev 04-2015  'xpub '
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4); //limxdev 04-2015  'xpriv'
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000005);             // Limecoinx BIP44 coin type is '5'

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 7*24*60*60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x4c;   //limxdev 04-2015
        pchMessageStart[1] = 0x1a;   //limxdev 04-2015
        pchMessageStart[2] = 0x2c;   //limxdev 04-2015
        pchMessageStart[3] = 0xaf;   //limxdev 04-2015   Testnet

        vAlertPubKey = ParseHex("04517d8a699cb43d3938d7b24faaff7cda448ca4ea267723ba614784de661949bf632d6304316b244646dea079735b9a6fc4af804efb4752075b9fe2245e14e412");
        nDefaultPort = 8885;
        nRPCPort = 8884;
        strDataDir = "testnet3";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1402095180;   //limxdev 04-2015 main.cpp 3127
        genesis.nNonce = 386986707;   //limxdev 04-2015 main.cpp 3128

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000008030a1e9a647ecc6119e0782166552e49dadfa8353afa26f3a6c2179845"));  //limxdev 04-2015  Fehler

        vFixedSeeds.clear();
        vSeeds.clear();
        /*vSeeds.push_back(CDNSSeedData("5.56.209.255", "5.56.209.255"));   //limxdev 04-2015
        vSeeds.push_back(CDNSSeedData("", ""));       //limxdev 04-2015 vSeeds.push_back(CDNSSeedData("limecoinx.qa", "testnet-seed.limecoinx.qa"));
        *///legacy seeders
        vSeeds.push_back(CDNSSeedData("85.214.68.75", "85.214.68.75"));      //limxdev 04-2015
        vSeeds.push_back(CDNSSeedData("", ""));      //limxdev 04-2015
        vSeeds.push_back(CDNSSeedData("", ""));       //limxdev 04-2015

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);                    //limxdev 04-2015   Testnet vorher 139 limecoinx addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 196);                    //limxdev 04-2015   Testnet vorher 19 limecoinx script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY]     = list_of(211);                    ///limxdev 04-2015   Testnet vorher 239  private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF); //limxdev 04-2015   tpub
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94); //limxdev 04-2015  tprv
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000001);             // Testnet limecoinx BIP44 coin type is '5' (All coin's testnet default)
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
      // Alle Parameter hier unverändert
        pchMessageStart[0] = 0xfc;          // Limxdev 04-2015 old    0xfc
        pchMessageStart[1] = 0xc1;          // Limxdev 04-2015 old    0xc1
        pchMessageStart[2] = 0xb7;         // Limxdev 04-2015 old     0xb7
        pchMessageStart[3] = 0xdc;          // Limxdev 04-2015 old    0xdc
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1402095180;       // Limxdev 04-2015 old   1417713337  Testnet
        genesis.nBits = 0x1e0ffff0;        // Limxdev 04-2015 old    0x207fffff  Zeile 60
        genesis.nNonce = 386986707;         // Limxdev 04-2015 old   1096447    Testnet
        nDefaultPort = 19994;             // Limxdev 04-2015 old   19994
        strDataDir = "regtest";

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000008030a1e9a647ecc6119e0782166552e49dadfa8353afa26f3a6c2179845"));   // Limxdev 04-2015 old      0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
