// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "limecoinX ofrece amplia gama de servicios y mejoras para ti";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000; // Bitsend number of blocks until halving on 1st Nov. 2016 09:17:52 PM
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 400000; // Bitsend BIP34 height
        consensus.BIP34Hash = uint256S("000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8"); // Bitsend BIP34 Hash
        consensus.BIP65Height = 400000; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 400000; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000"); // Bitsend PoW Limit
        consensus.nPowTargetTimespan = 6 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 5 * 60;//Bitsend 2.5 minutes
        consensus.nPowTargetTimespanV2 = 6 * 24 * 60 * 60; // Bitsend 2.5 minutes
        consensus.nPowTargetSpacingV2 = 5 * 60; // Bitsend 2.5 minutes
        // consensus.nPowTargetTimespanV3
        // consensus.nPowTargetSpacingV3
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1; // Bitsend
        consensus.nMinerConfirmationWindow = 1728; // Bitsend - max blocks for softfork to activate, default nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1503255158; // 08/20/2017 @ 6:52pm (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1607990400; // December 31, 2008
	    consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nHeight = 400000;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1503255158; // 08/20/2017 @ 6:52pm (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1607990400; // 12/15/2020 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nHeight = 400000;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1503255158; // 08/20/2017 @ 6:52pm (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1607990400; // 12/15/2020 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nHeight = 400000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000006106bead2f4b73");//60988a35d4b6b2

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000000000000013176bf8d7dfeab4e1db31dc93bc311b436e82ab226b90"); //453354

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa3;    //bitsenddev 04-2015   mainc.pp 3072    ggf testnet wert = falshc
        pchMessageStart[1] = 0xd5;    ///bitsenddev 04-2015
        pchMessageStart[2] = 0xc2;    //bitsenddev 04-2015
        pchMessageStart[3] = 0xf9;    //bitsenddev 04-2015
        nDefaultPort = 8886;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1402097469, 34784351, 0x1e0ffff0, 1, 100 * COIN);//Time, Nonce,nBits, version, reward
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000012e1b8843ac9ce8c18603658eaf8895f99d3f5e7e1b7b1686f35e3c087a"));
        assert(genesis.hashMerkleRoot == uint256S("0xc0c4f68994b78c31b6c7d7a8a895b5a085857adfbb73699bcfe5a247a357df57"));
	    
        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they dont support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("188.68.52.172", "188.68.52.172");
        vSeeds.emplace_back("37.120.186.85", "37.120.186.85");
        vSeeds.emplace_back("37.120.190.76", "37.120.190.76");
        vSeeds.emplace_back("213.136.80.93", "213.136.80.93");
        vSeeds.emplace_back("213.136.86.202", "213.136.86.202");
        vSeeds.emplace_back("213.136.86.205", "213.136.86.205");
        vSeeds.emplace_back("213.136.86.207", "213.136.86.207");
		
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,102);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
		base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);
		base58Prefixes[EXT_PUBLIC_KEY] = {0x04,0x88,0xB2,0x1E};
		base58Prefixes[EXT_SECRET_KEY] = {0x04,0x88,0xAD,0xE4};

         // Bitsend 9
        bech32_hrp = "mc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
		//fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        
        checkpointData = {
            {
                { 0, uint256S("0x0000012e1b8843ac9ce8c18603658eaf8895f99d3f5e7e1b7b1686f35e3c087a")},  // Bitsend: checkpoint at 0
                
            }
        };
        chainTxData = ChainTxData{
            // Data as of block c69870ed876b17ebcf582ca8cd6362de13ebd83c98556d6518abfa8b4d71a975 (height 506003).
            //1525454386, // * UNIX timestamp of last known number of transactions
           // 573703,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
           // 500.0       // * estimated number of transactions per second after that timestamp
        };
      
        // Masternodes
        consensus.nMasternodePaymentsStartBlock = 513000;
        consensus.nSuperblockStartBlock = 513000;
        consensus.nSuperblockCycle = 16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8");
        consensus.BIP65Height = 581885; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 330776; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000001f057509eba81aed91");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000000128796ee387cf110ccb9d2f36cffaf7f73079c995377c65ac0dcc"); //1079274

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 18333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1402097469, 34784351, 0x1e0ffff0, 1, 100 * COIN);//Time, Nonce,nBits, version, reward
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000012e1b8843ac9ce8c18603658eaf8895f99d3f5e7e1b7b1686f35e3c087a"));
        assert(genesis.hashMerkleRoot == uint256S("0xc0c4f68994b78c31b6c7d7a8a895b5a085857adfbb73699bcfe5a247a357df57"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnetbitsend.jonasschnelli.ch");
        vSeeds.emplace_back("seed.tbtc.petertodd.org");
        //vSeeds.emplace_back("dnsseed3.bitsend.io");
		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
		base58Prefixes[EXT_PUBLIC_KEY] = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04,0x35,0x83,0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        //fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        checkpointData = {
            {
                { 546, uint256S("000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70")},  // Bitsend Testnet: checkpoint at 0
            }
        };

         chainTxData = ChainTxData{
            // Data as of block 00000000c2872f8f8a8935c8e3c5862be9038c97d4de2cf37ed496991166928a (height 1063660)
            1483546230,
            12834668,
            0.15
        };
        // Masternodes
        consensus.nMasternodePaymentsStartBlock = 120;
        consensus.nSuperblockStartBlock = 120;
        consensus.nSuperblockCycle = 24;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150; // Bitsend number of blocks until halving on 1st Nov. 2016 09:17:52 PM
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256(); // Bitsend BIP34 Hash
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Bitsend PoW Limit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Bitsend 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60; // Bitsend 2.5 minutes
        consensus.nPowTargetTimespanV2 = 2.5 * 60; // Bitsend 2.5 minutes
        consensus.nPowTargetSpacingV2 = 2.5 * 60; // Bitsend 2.5 minutes
        // consensus.nPowTargetTimespanV3
        // consensus.nPowTargetSpacingV3
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108;
        consensus.nMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); //0

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xc6;
        pchMessageStart[3] = 0xab;
        nDefaultPort = 60333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 0, 0x207fffff, 1, 50 * COIN); // Bitsend Regtest 1296688602, 0
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x62214f698865bf3308a4cddec2a18c1e9a98f272189f696e0f99d5d1b137912b"));
        assert(genesis.hashMerkleRoot == uint256S("0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                { 0, uint256S("0x62214f698865bf3308a4cddec2a18c1e9a98f272189f696e0f99d5d1b137912b")},  // Bitsend Regtest: checkpoint at 0
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
      
        // Masternodes
        consensus.nMasternodePaymentsStartBlock = 0;
        consensus.nSuperblockStartBlock = 1;
        consensus.nSuperblockCycle = 24;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,55); // Bitsend N
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5); // Bitsend 3
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,23); // Bitsend A
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178); // Bitsend 2
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        
        bech32_hrp = "rmc";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
