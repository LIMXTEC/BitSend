// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Limecoinx developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double SIGCHECK_VERIFICATION_FACTOR = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64_t nTimeLastCheckpoint;
        int64_t nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    bool fEnabled = true;

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    // limxdev limecoinx 2015-04
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
          (  0, uint256("0000012e1b8843ac9ce8c18603658eaf8895f99d3f5e7e1b7b1686f35e3c087a")) //limxdev 04-2015
	( 10000, uint256("0x0000000006c1a1573ca82ad24789c10a36535cf085f201122f1d112a88841271"))
	( 15790, uint256("0x000000004151f93a4012545309d4bbcac5f2977dea643a178c4bec1310e6c086"))
	( 21190, uint256("0x000000002a798fd88d0ce270cad1c217aceea236b573c68cfd02b086f5745921"))
	( 26000, uint256("0x00000002bace42e673000616ed8dbf16a49e3a7aec6bf59774fed081f6deac5f"))
	( 29999, uint256("0x00000000ede644fcbdf8f8ce8c53bb15de5dfd5c32384c751fa4ef409992aa07"))
	( 36222, uint256("0x0000000047c4338861a6b191570f07a23bd30c75c03a81ac5e5978053d946408"))
	( 40599, uint256("0x00000000c2596d6bd49b08ab9d233cd7de97a01a7cde19d0e1a136a1f3904f3c"))
	( 45512, uint256("0x00000000e2448d27f6360461739bcd25bf41d3767fc7c0e8c5e53a2db90eaf06"))
	( 49478, uint256("0x00000000dbb3d6386ed45e335316a4f018e451bf60b12fdebbef680969a90acb"))
	( 74910, uint256("0x000000002409374bcab8006f171b8c3eb4485220d94ae555b041ee24eb4d8434"))
	( 84579, uint256("0x00000000372eebd8b26d135798ac04549dc32fdfb584710ed9edf2dcb1be6941"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1429705032, // * UNIX timestamp of last checkpoint block
		102428,    // * total number of transactions between genesis and last checkpoint //
                    //   (the tx=... number in the SetBestChain debug.log lines)
        350     // * estimated number of transactions per day after checkpoint //800
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0 , uint256("0x000008030a1e9a647ecc6119e0782166552e49dadfa8353afa26f3a6c2179845"))   //limxdev 04-2015
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1402095180,
        3000,
        30
    };

    static MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e"))
        ;
    static const CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

    const CCheckpointData &Checkpoints() {
        if (Params().NetworkID() == CChainParams::TESTNET)
            return dataTestnet;
        else if (Params().NetworkID() == CChainParams::MAIN)
            return data;
        else
            return dataRegtest;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!fEnabled)
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex, bool fSigchecks) {
        if (pindex==NULL)
            return 0.0;

        int64_t nNow = time(NULL);

        double fSigcheckVerificationFactor = fSigchecks ? SIGCHECK_VERIFICATION_FACTOR : 1.0;
        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkpoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!fEnabled)
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!fEnabled)
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
