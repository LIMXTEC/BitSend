// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>
#include <validation.h>

// Bitsend: Select retargeting
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // current difficulty formula, ERC3 - DUAL_KGW3, written by Christian Knoepke - apfelbaum@email.de
	// BitSend and Eropecoin Developer
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
	bool kgwdebug=false;
    uint64_t PastBlocksMass = 0;
    int64_t PastRateActualSeconds = 0;
    int64_t PastRateTargetSeconds = 0;
    double PastRateAdjustmentRatio = double(1);
    arith_uint256 PastDifficultyAverage;
    arith_uint256 PastDifficultyAveragePrev;
    double EventHorizonDeviation;
    double EventHorizonDeviationFast;
    double EventHorizonDeviationSlow;
	//DUAL_KGW3 SETUP
	static const uint64_t Blocktime = 3 * 60; 
	static const unsigned int timeDaySeconds = 60 * 60 * 24;
    uint64_t pastSecondsMin = timeDaySeconds * 0.025;
    uint64_t pastSecondsMax = timeDaySeconds * 7;
    uint64_t PastBlocksMin = pastSecondsMin / Blocktime;
    uint64_t PastBlocksMax = pastSecondsMax / Blocktime;
	
	 const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
	
    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || (uint64_t)BlockLastSolved->nHeight < PastBlocksMin) {  return bnPowLimit.GetCompact(); }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        PastBlocksMass++;
        PastDifficultyAverage.SetCompact(BlockReading->nBits);
        if (i > 1) {
            if(PastDifficultyAverage >= PastDifficultyAveragePrev)
                PastDifficultyAverage = ((PastDifficultyAverage - PastDifficultyAveragePrev) / i) + PastDifficultyAveragePrev;
            else
                PastDifficultyAverage = PastDifficultyAveragePrev - ((PastDifficultyAveragePrev - PastDifficultyAverage) / i);
        }
        PastDifficultyAveragePrev = PastDifficultyAverage;
        PastRateActualSeconds = BlockLastSolved->GetBlockTime() - BlockReading->GetBlockTime();
        PastRateTargetSeconds = Blocktime * PastBlocksMass;
        PastRateAdjustmentRatio = double(1);
        if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(28.2)), -1.228));//72 =>28.2 //28.2 and 144 possible
        EventHorizonDeviationFast = EventHorizonDeviation;
        EventHorizonDeviationSlow = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
                if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast))
                { assert(BlockReading); break; }
        }
        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }
	
	//KGW Original
    arith_uint256 kgw_dual1(PastDifficultyAverage);
	arith_uint256 kgw_dual2;
	kgw_dual2.SetCompact(pindexLast->nBits);
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
         kgw_dual1 *= PastRateActualSeconds;
         kgw_dual1 /= PastRateTargetSeconds;
    }
	
	int64_t nActualTime1 = pindexLast->GetBlockTime() - pindexLast->pprev->GetBlockTime();
	int64_t nActualTimespanshort = nActualTime1;	
	
	// Retarget BSD Original ...not exactly
	// Small Fix
	if(nActualTime1 < 0) nActualTime1 = Blocktime;

    if (nActualTime1 < Blocktime / 3)
        nActualTime1 = Blocktime / 3;
    if (nActualTime1 > Blocktime * 3)
        nActualTime1 = Blocktime * 3;
		
    kgw_dual2 *= nActualTime1;
    kgw_dual2 /= Blocktime;
	
	//Fusion from Retarget and Classic KGW3 (BitSend=)
	
	arith_uint256 bnNew;
	bnNew = ((kgw_dual2 + kgw_dual1)/2);
	// DUAL KGW3 increased rapidly the Diff if Blocktime to last block under Blocktime/6 sec.
	
	if(nActualTimespanshort < 0){
		
		//LogPrintf("nActualTimespanshort was = %d \n", nActualTimespanshort );
		nActualTimespanshort = 0;
	}
	if(kgwdebug)LogPrintf("nActualTimespanshort = %d \n", nActualTimespanshort );
	if( nActualTimespanshort < Blocktime/6 )
		{
		if(kgwdebug)LogPrintf("Vordiff:%08x %s bnNew first  \n", bnNew.GetCompact(), bnNew.ToString().c_str());
		const int nLongShortNew1   = 85; const int nLongShortNew2   = 100;
		bnNew = bnNew * nLongShortNew1;	bnNew = bnNew / nLongShortNew2;	
		if(kgwdebug)LogPrintf("Erhöhte Diff:\n %08x %s bnNew second \n", bnNew.GetCompact(), bnNew.ToString().c_str() );
		}

	
	//BitBreak BitSend
	// Reduce difficulty if current block generation time has already exceeded maximum time limit.
	const int nLongTimeLimit   = 6 * 60 * 60; 
    if(kgwdebug)
	{		
	LogPrintf("Prediff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
	LogPrintf("Vordiff %d \n", nLongTimeLimit);
	LogPrintf(" %d Block", BlockReading->nHeight );
	}
	
	if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit)  //block.nTime 
	{
		bnNew = bnPowLimit;//bnNew = bnPowLimit/15;
       	if(kgwdebug)LogPrintf("<BSD> Maximum block time hit - cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str()); 
	}

    if (bnNew > bnPowLimit) {
        bnNew = bnPowLimit;
    }
    return bnNew.GetCompact();
}

/* // Standard retargeting
unsigned int GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 2.5 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Bitsend: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

// DigiShield retargeting
unsigned int GetNextWorkRequired_V2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentIntervalV2() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* nTargetSpacing minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacingV2*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentIntervalV2() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Bitsend: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentIntervalV2()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentIntervalV2())
        blockstogoback = params.DifficultyAdjustmentIntervalV2();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

// Retargeting to support the PoW change phase (V3)
unsigned int GetNextWorkRequired_V3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{ 

    if (pblock->GetBlockTime() - pindexLast->GetBlockTime() >= 300 && pblock->nBits != GetNextWorkRequired_V2(pindexLast, pblock, params)){
        arith_uint256 bnNew;
        bnNew.SetCompact(GetNextWorkRequired_V2(pindexLast, pblock, params));
        bool fShift = false;

        for (int i=0; i <= (pblock->GetBlockTime() -  pindexLast->GetBlockTime()) / 150; i++)
        {
            if (i == 0) continue;
            if (bnNew.bits() > 235)
            {
                fShift = true;
                bnNew >>= 1;
            }
            bnNew *= 190 + i * 3;
            bnNew /= 173;
            if (fShift)
            {
                fShift = false;
                bnNew <<= 1;
            }
        }

        const arith_uint256 nPowLimit  = UintToArith256(params.powLimit);

        if (bnNew > nPowLimit)
            bnNew = nPowLimit;

        return bnNew.GetCompact();
    }
    else return GetNextWorkRequired_V2(pindexLast, pblock, params);
}
*/
// Bitsend: Select retargeting
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

/* // Standard retargeting
unsigned int CalculateNextWorkRequired_V1(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
		  arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
		  bnOld = bnNew;
    // Bitsend: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 235;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// DigiShield retargeting
unsigned int CalculateNextWorkRequired_V2(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    //LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
    if (nActualTimespan < (params.nPowTargetTimespanV2 - (params.nPowTargetTimespanV2/4)))
        nActualTimespan = (params.nPowTargetTimespanV2 - (params.nPowTargetTimespanV2/4));
    if (nActualTimespan > (params.nPowTargetTimespanV2 + (params.nPowTargetTimespanV2/2)))
        nActualTimespan = (params.nPowTargetTimespanV2 + (params.nPowTargetTimespanV2/2));

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    // Bitsend: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 235;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespanV2;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// Retargeting to support the PoW change phase (V3)
unsigned int CalculateNextWorkRequired_V3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int64_t nBlockTimeO, int64_t nBlockTime, const Consensus::Params& params)
 {
   if (nBlockTime - nBlockTimeO >= 300)
   {
        arith_uint256 bnNew;
        bnNew.SetCompact(GetNextWorkRequired_V2(pindexLast, pblock, params));
        bool fShift = false;
        for (int i=0; i <= (nBlockTime - nBlockTimeO) / 150 ; i++)
        {
            if (i == 0) continue;
            if (bnNew.bits() > 235)
            {
                fShift = true;
                bnNew >>= 1; 
            }
            bnNew *= 190 + i * 3;
            bnNew /= 173;
            if (fShift)
            {
                fShift = false;
                bnNew <<= 1;
            }
        }

        const arith_uint256 nPowLimit = UintToArith256(params.powLimit);

        if (bnNew > nPowLimit)
            bnNew = nPowLimit;

        return bnNew.GetCompact();
    }
    else return pblock->nBits;
 }
 */
bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
   // if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        //return false;

    // Check proof of work matches claimed amount
    //if (UintToArith256(hash) > bnTarget)
        //return false;

    return true;
}
