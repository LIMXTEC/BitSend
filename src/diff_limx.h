// Copyright (c) 2015 The Guldencoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// This file contains Delta, the Guldencoin Difficulty Re-adjustment algorithm developed by Frank (dt_cdog@yahoo.com) with various enhancements by Malcolm MacLeod (mmacleod@webmail.co.za)
// The core algorithm works by taking time measurements of four periods (last block; short window; medium window; long window) and then apply a weighting to them.
// This allows the algorithm to react to short term fluctuations while still taking long term block targets into account, which helps prevent it from overreacting.
//
// In addition to the core algorithm several extra rules are then applied in certain situations (e.g. multiple quick blocks) to enhance the behaviour.


#ifndef GULDENCOIN_DIFF_LIMX_H
#define GULDENCOIN_DIFF_LIMX_H

#define PERCENT_FACTOR 100

//////////////////////////////////////////////////////////////////////////////
//
//  Limxdev DGW/KGW/Delta
//
unsigned int static DarkGravityWave(const CBlockIndex* pindexLast, const CBlockHeader *pblock) {
    /* current difficulty formula, Dash - DarkGravity v3, written by Evan Duffield - evan@limecoinxpay.io */
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    int64_t nActualTimespan = 0;
    int64_t LastBlockTime = 0;
    int64_t PastBlocksMin = 24;  // LIMX 14  - OLD 24
    int64_t PastBlocksMax = 24;  // LIMX 140 - OLD 24
    int64_t CountBlocks = 0;
    CBigNum PastDifficultyAverage;
    CBigNum PastDifficultyAveragePrev;

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMin) {
        return Params().ProofOfWorkLimit().GetCompact();
    }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        CountBlocks++;

        if(CountBlocks <= PastBlocksMin) {
            if (CountBlocks == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
            else { PastDifficultyAverage = ((PastDifficultyAveragePrev * CountBlocks)+(CBigNum().SetCompact(BlockReading->nBits))) / (CountBlocks+1); }
            PastDifficultyAveragePrev = PastDifficultyAverage;
        }

        if(LastBlockTime > 0){
            int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
            nActualTimespan += Diff;
        }
        LastBlockTime = BlockReading->GetBlockTime();

        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }

    CBigNum bnNew(PastDifficultyAverage);

    int64_t _nTargetTimespan = CountBlocks*nTargetSpacing;

    if (nActualTimespan < _nTargetTimespan/3)
        nActualTimespan = _nTargetTimespan/3;
    if (nActualTimespan > _nTargetTimespan*3)
        nActualTimespan = _nTargetTimespan*3;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= _nTargetTimespan;

    if (bnNew > Params().ProofOfWorkLimit()){
        bnNew = Params().ProofOfWorkLimit();
    }
    
    return bnNew.GetCompact();
}

unsigned int static KimotoGravityWell(const CBlockIndex* pindexLast, const CBlockHeader *pblock, uint64_t TargetBlocksSpacingSeconds, uint64_t PastBlocksMin, uint64_t PastBlocksMax) {
        const CBlockIndex *BlockLastSolved = pindexLast;
        const CBlockIndex *BlockReading = pindexLast;
        const CBlockHeader *BlockCreating = pblock;// Limxdev add from old KGW
        BlockCreating = BlockCreating; //Limxdev add from old KGW
        uint64_t PastBlocksMass = 0;
        int64_t PastRateActualSeconds = 0;
        int64_t PastRateTargetSeconds = 0;
        double PastRateAdjustmentRatio = double(1);
        CBigNum PastDifficultyAverage;
        CBigNum PastDifficultyAveragePrev;
        double EventHorizonDeviation;
        double EventHorizonDeviationFast;
        double EventHorizonDeviationSlow;

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || (uint64_t)BlockLastSolved->nHeight < PastBlocksMin) { return Params().ProofOfWorkLimit().GetCompact(); }
	int64_t LatestBlockTime = BlockLastSolved->GetBlockTime();
        for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
                if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
                PastBlocksMass++;

                if (i == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
                else { PastDifficultyAverage = ((CBigNum().SetCompact(BlockReading->nBits) - PastDifficultyAveragePrev) / i) + PastDifficultyAveragePrev; }
                PastDifficultyAveragePrev = PastDifficultyAverage;
                 if (LatestBlockTime < BlockReading->GetBlockTime()) //Fix Limxdev
                 {
                 	LatestBlockTime = BlockReading->GetBlockTime();
                 }

                PastRateActualSeconds = LatestBlockTime - BlockReading->GetBlockTime(); //Fix Limxdev
                PastRateTargetSeconds = TargetBlocksSpacingSeconds * PastBlocksMass;
                PastRateAdjustmentRatio = double(1);
                if (PastRateActualSeconds < 1) { PastRateActualSeconds = 1; } //Fix Limxdev new KGW
                // else { if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }; } //Fix Limxdev old KGW
                if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
                PastRateAdjustmentRatio = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
                }
                // KGW3
                if (BlockReading->nHeight > 102250) {
                        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(72)), -1.228));
                } else {
                        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(144)), -1.228));
                }
                // KGW 3
                // EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(144)), -1.228)); // Fix Limxdev
                EventHorizonDeviationFast = EventHorizonDeviation;
                EventHorizonDeviationSlow = 1 / EventHorizonDeviation;

                if (PastBlocksMass >= PastBlocksMin) {
                        if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast)) { assert(BlockReading); break; }
                }
                if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
                BlockReading = BlockReading->pprev;
        }

        CBigNum bnNew(PastDifficultyAverage);
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
                bnNew *= PastRateActualSeconds;
                bnNew /= PastRateTargetSeconds;
        }

    if (bnNew > Params().ProofOfWorkLimit()) {
        bnNew = Params().ProofOfWorkLimit();
    }
// Limxdev 22-05-2015
   
    if(fDebug){
    printf("Difficulty Retarget - Kimoto Gravity Well\n");
    printf("PastRateAdjustmentRatio = %g\n", PastRateAdjustmentRatio);
    printf("Before: %08x  %s\n", BlockLastSolved->nBits, CBigNum().SetCompact(BlockLastSolved->nBits).getuint256().ToString().c_str());
    printf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.getuint256().ToString().c_str());
    }
    
    return bnNew.GetCompact();
}

unsigned int static GetNextWorkRequired_Delta(const CBlockIndex* pindexLast, const CBlockHeader *pblock, unsigned int nFirstDeltaBlock)
{
    // These two variables are not used in the calculation at all, but only for logging when -debug is set, to prevent logging the same calculation repeatedly.
    static int64_t nPrevHeight     = 0;
    static int64_t nPrevDifficulty = 0;
    
    std::string sLogInfo;

    // The spacing we want our blocks to come in at.
    int64_t nRetargetTimespan = nTargetSpacing;

    // The minimum difficulty that is allowed, this is set on a per algorithm basis.
    //const unsigned int nProofOfWorkLimit = bnProofOfWorkLimit.GetCompact();
  //  const unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();


    // How many blocks to use to calculate each of the four algo specific time windows (last block; short window; middle window; long window)
    const unsigned int nLastBlock           =   1;
    const unsigned int nShortFrame          =   3;
    const unsigned int nMiddleFrame         =  24;
    const unsigned int nLongFrame           = 576;

    // How many blocks to use for the fifth window, fifth window is across all algorithms.
    #ifdef MULTI_ALGO
    const unsigned int nDayFrame            = 576;
    #endif

    // Weighting to use for each of the four algo specific time windows.
    const int64_t nLBWeight        =  64;
    const int64_t nShortWeight     =  8;
    int64_t nMiddleWeight          =  2;
    int64_t nLongWeight            =  1;

    // Minimum and maximum threshold for the last block, if it exceeds these thresholds then favour a larger swing in difficulty.
    const int64_t nLBMinGap        = nRetargetTimespan / 6;
    const int64_t nLBMaxGap        = nRetargetTimespan * 6;
    
    // Minimum threshold for the short window, if it exceeds these thresholds then favour a larger swing in difficulty.
    const int64_t nQBFrame         = nShortFrame + 1;
    const int64_t nQBMinGap        = (nRetargetTimespan / 1.2) * nQBFrame;

    // Any block with a time lower than nBadTimeLimit is considered to have a 'bad' time, the time is replaced with the value of nBadTimeReplace.
    const int64_t nBadTimeLimit    = 0;
    const int64_t nBadTimeReplace  = nRetargetTimespan / 10;
    
    // Used for 'exception 1' (see code below), if block is lower than 'nLowTimeLimit' then prevent the algorithm from decreasing difficulty any further.
    // If block is lower than 'nFloorTimeLimit' then impose a minor increase in difficulty.
    // This helps to prevent the algorithm from generating and giving away too many sudden/easy 'quick blocks' after a long block or two have occured, and instead forces things to be recovered more gently over time without intefering with other desirable properties of the algorithm.
    const int64_t nLowTimeLimit    = nRetargetTimespan * 0.9;
    const int64_t nFloorTimeLimit  = nRetargetTimespan * 0.65;
    
    // Used for 'exception 2' (see code below), if a block has taken longer than nLongTimeLimit we perform a difficulty reduction by taking the original difficulty and dividing by nLongTimeStep
    // NB!!! nLongTimeLimit MUST ALWAYS EXCEED THE THE MAXIMUM DRIFT ALLOWED (IN BOTH THE POSITIVE AND NEGATIVE DIRECTION)
    // SO AT LEAST DRIFT X2 OR MORE - OR ELSE CLIENTS CAN FORCE LOW DIFFICULTY BLOCKS BY MESSING WITH THE BLOCK TIMES.
    const int64_t nLongTimeLimit   = 2 * 16 * 60;
    const int64_t nLongTimeStep    = 15 * 60;

    // Limit adjustment amount to try prevent jumping too far in either direction.
    // The same limits as DIGI difficulty algorithm are used.
    // min 75% of default time; 33.3% difficulty increase
    unsigned int nMinimumAdjustLimit = nRetargetTimespan * 0.75;
    // max 150% of default time; 33.3% difficuly decrease
    unsigned int nMaximumAdjustLimit = nRetargetTimespan * 1.5;
    
    // Variables used in calculation
    int64_t nDeltaTimespan         = 0;
    int64_t nLBTimespan            = 0;
    int64_t nShortTimespan         = 0;
    int64_t nMiddleTimespan        = 0;
    int64_t nLongTimespan          = 0;
    int64_t nQBTimespan            = 0;
    #ifdef MULTI_ALGO
    int64_t nDayTimespan           = 0;
    #endif

    int64_t nWeightedSum           = 0;
    int64_t nWeightedDiv           = 0;
    int64_t nWeightedTimespan      = 0;
    #ifdef MULTI_ALGO
    int64_t nDailyPercentage       = 0;
    #endif

    const CBlockIndex* pindexFirst = pindexLast; //multi algo - last block is selected on a per algo basis.

    // Genesis block
    if (pindexLast == NULL)
        return Params().ProofOfWorkLimit().GetCompact();//return nProofOfWorkLimit;

    // -- Use a fixed difficuly until we have enough blocks to work with (multi algo - this is calculated on a per algo basis)
    if (pindexLast->nHeight <= nQBFrame)
        return Params().ProofOfWorkLimit().GetCompact();//return nProofOfWorkLimit; 

    // -- Calculate timespan for last block window (multi algo - this is calculated on a per algo basis)
    pindexFirst = pindexLast->pprev;
    nLBTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
   
    // Check for very short and long blocks (algo specific)
    // If last block was far too short, let difficulty raise faster
    // by cutting 50% of last block time
    if (nLBTimespan > nBadTimeLimit && nLBTimespan < nLBMinGap)
        nLBTimespan *= 0.5;
    // Prevent bad/negative block times - switch them for a fixed time.
    if (nLBTimespan <= nBadTimeLimit)
        nLBTimespan = nBadTimeReplace;
    // If last block took far too long, let difficulty drop faster
    // by adding 50% of last block time
    if (nLBTimespan > nLBMaxGap)
        nLBTimespan *= 1.5;


    // -- Calculate timespan for short window (multi algo - this is calculated on a per algo basis)
    pindexFirst = pindexLast;
    for (unsigned int i = 1; pindexFirst && i <= nQBFrame; i++)
    {
        nDeltaTimespan = pindexFirst->GetBlockTime() - pindexFirst->pprev->GetBlockTime();
        // Prevent bad/negative block times - switch them for a fixed time.
        if (nDeltaTimespan <= nBadTimeLimit)
            nDeltaTimespan = nBadTimeReplace;

        if (i<= nShortFrame)
            nShortTimespan += nDeltaTimespan;
        nQBTimespan += nDeltaTimespan;
        pindexFirst = pindexFirst->pprev;
    }
       
    // -- Calculate time interval for middle window (multi algo - this is calculated on a per algo basis)
    if (pindexLast->nHeight - nFirstDeltaBlock <= nMiddleFrame)
    {
        nMiddleWeight = nMiddleTimespan = 0;
    }
    else
    {
        pindexFirst = pindexLast;
        for (unsigned int i = 1; pindexFirst && i <= nMiddleFrame; i++)
        {
            nDeltaTimespan = pindexFirst->GetBlockTime() - pindexFirst->pprev->GetBlockTime();
            // Prevent bad/negative block times - switch them for a fixed time.
            if (nDeltaTimespan <= nBadTimeLimit)
                nDeltaTimespan = nBadTimeReplace;

            nMiddleTimespan += nDeltaTimespan;
            pindexFirst = pindexFirst->pprev;
        }
    }


    // -- Calculate timespan for long window (multi algo - this is calculated on a per algo basis)
    // NB! No need to worry about single negative block times as it has no significant influence over this many blocks.
    if (pindexLast->nHeight - nFirstDeltaBlock <= nLongFrame)
    {
        nLongWeight = nLongTimespan = 0;
    }
    else
    {
        pindexFirst = pindexLast;
        for (unsigned int i = 1; pindexFirst && i <= nLongFrame; i++)
            pindexFirst = pindexFirst->pprev;

        nLongTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    }


    // Check for multiple very short blocks (algo specific)
    // If last few blocks were far too short, and current block is still short, then calculate difficulty based on short blocks alone.
    if ( (nQBTimespan > nBadTimeLimit) && (nQBTimespan < nQBMinGap) && (nLBTimespan < nRetargetTimespan * 0.8) )
    {
        if (fDebug && (nPrevHeight != pindexLast->nHeight) )
            sLogInfo += "<DELTA> Multiple fast blocks - ignoring long and medium weightings.\n";
        nMiddleWeight = nMiddleTimespan = nLongWeight = nLongTimespan = 0;
    }
    
    // -- Combine all the timespans and weights to get a weighted timespan
    nWeightedSum      = (nLBTimespan * nLBWeight) + (nShortTimespan * nShortWeight);
    nWeightedSum     += (nMiddleTimespan * nMiddleWeight) + (nLongTimespan * nLongWeight);
    nWeightedDiv      = (nLastBlock * nLBWeight) + (nShortFrame * nShortWeight);
    nWeightedDiv     += (nMiddleFrame * nMiddleWeight) + (nLongFrame * nLongWeight);
    nWeightedTimespan = nWeightedSum / nWeightedDiv;
    

    // Apply the adjustment limits
    // However if we are close to target time then we use smaller limits to smooth things off a little bit more.
    if (std::abs(nLBTimespan - nRetargetTimespan) < nRetargetTimespan * 0.1)
    {
        // 90% of target
        // 11.11111111111111% difficulty increase
        // 10% difficulty decrease.
        nMinimumAdjustLimit = nRetargetTimespan * 0.9;
        nMaximumAdjustLimit = nRetargetTimespan * 1.1;
    }
    else if (std::abs(nLBTimespan - nRetargetTimespan) < nRetargetTimespan * 0.2)
    {
        // 80% of target - 25% difficulty increase/decrease maximum.
        nMinimumAdjustLimit = nRetargetTimespan * 0.8;
        nMaximumAdjustLimit = nRetargetTimespan * 1.2;
    }
    
    // min
    if (nWeightedTimespan < nMinimumAdjustLimit)
        nWeightedTimespan = nMinimumAdjustLimit;
    // max
    if (nWeightedTimespan > nMaximumAdjustLimit)
        nWeightedTimespan = nMaximumAdjustLimit;


    #ifdef MULTI_ALGO
    // -- Day interval (1 day; general over all algos)
    // so other algos can take blocks that 1 algo does not use
    // in case there are still longer gaps (high spikes)
    if (pindexLast->nHeight <= nDayFrame)
    {
        nDayTimespan = nRetargetTimespan * nDayFrame;
    }
    else
    {
        pindexFirst = pindexLast;
        for (int i = 1; pindexFirst && i <= nDayFrame; i++)
            pindexFirst = pindexFirst->pprev;

        nDayTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    }
    nDailyPercentage = (nDayTimespan * PERCENT_FACTOR) / (nRetargetTimespan * nDayFrame);

    // Limit day interval adjustment to 10%
    if (nDailyPercentage > 110)
        nDailyPercentage = 110;
    if (nDailyPercentage <  90)
        nDailyPercentage =  90;


    // Adjust Time based on day interval
    nWeightedTimespan *= nDailyPercentage;
    nWeightedTimespan /= PERCENT_FACTOR;
    #endif

    
    // Finally calculate and set the new difficulty.
    CBigNum bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nWeightedTimespan;
    bnNew /= nRetargetTimespan;

    
    // Now that we have the difficulty we run a last few 'special purpose' exception rules which have the ability to override the calculation.
    // Exception 1 - Never adjust difficulty downward (human view) if previous block generation was faster than what we wanted.
    nLBTimespan = pindexLast->GetBlockTime() - pindexLast->pprev->GetBlockTime();
    if (nLBTimespan > 0 && nLBTimespan < nLowTimeLimit && bnNew > CBigNum().SetCompact(pindexLast->nBits))
    {
        // If it is this low then we actually give it a slight nudge upwards - 5%
        if (nLBTimespan < nFloorTimeLimit)
        {
            bnNew.SetCompact(pindexLast->nBits);
            bnNew *= 95;
            bnNew /= PERCENT_FACTOR;
            if (fDebug && (nPrevHeight != pindexLast->nHeight) )
                sLogInfo +=  strprintf("<DELTA> Last block time [%ld] was far below target but adjustment still downward, forcing difficulty up by 5%% instead\n", (long int)nLBTimespan);
        }
        else
        {
            bnNew.SetCompact(pindexLast->nBits);
            if (fDebug && (nPrevHeight != pindexLast->nHeight) )
                sLogInfo += strprintf("<DELTA> Last block time [%ld] below target but adjustment still downward, blocking downward adjustment\n", (long int)nLBTimespan);
        }
    }

    
    // Exception 2 - Reduce difficulty if current block generation time has already exceeded maximum time limit. (NB! nLongTimeLimit must exceed maximum possible drift in both positive and negative direction)
    if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit)  //block.nTime 
    {
        // Reduce in a linear fashion at first, and then start to ramp up with a gradual exponential curve (to catch massive hash extinction events)
        int64_t nNumMissedSteps = ((pblock-> nTime - pindexLast->GetBlockTime()) / nLongTimeStep);
        if (nNumMissedSteps <= 12)
            bnNew *=  nNumMissedSteps;
        else
            bnNew *=  12 + (int64_t)std::floor(std::pow((float)1.14, (float)nNumMissedSteps - 12) + 0.5);
	
        if (fDebug && (nPrevHeight != pindexLast->nHeight ||  bnNew.GetCompact() != nPrevDifficulty) )
            sLogInfo +=  strprintf("<DELTA> Maximum block time hit - halving difficulty %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
    }

    
    // Exception 3 - Difficulty should never go below (human view) the starting difficulty, so if it has we force it back to the limit.
    if (bnNew > Params().ProofOfWorkLimit())
        bnNew = Params().ProofOfWorkLimit();  //bnProofOfWorkLimit

    
    if (fDebug)
    {
        if (nPrevHeight != pindexLast->nHeight ||  bnNew.GetCompact() != nPrevDifficulty)
        {
            static CCriticalSection logCS;
	    LOCK(logCS);
            printf("<DELTA> Height= %d\n" , pindexLast->nHeight);
	    printf("%s" , sLogInfo.c_str());
            printf("<DELTA> nTargetTimespan = %ld nActualTimespan = %ld nWeightedTimespan = %ld \n", (long int)nRetargetTimespan, (long int)nLBTimespan, (long int)nWeightedTimespan);
            printf("<DELTA> nShortTimespan/nShortFrame = %ld nMiddleTimespan/nMiddleFrame = %ld nLongTimespan/nLongFrame = %ld \n", (long int)(nShortTimespan/nShortFrame), (long int)(nMiddleTimespan/nMiddleFrame), (long int)(nLongTimespan/nLongFrame));
            printf("<DELTA> Before: %08x %s\n", pindexLast->nBits, CBigNum().SetCompact(pindexLast->nBits).ToString().c_str());
            printf("<DELTA> After:  %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
            printf("<DELTA> Rough change percentage (human view): %lf%%\n", -( ( (bnNew.getuint256().getdouble() - CBigNum().SetCompact(pindexLast->nBits).getuint256().getdouble()) / CBigNum().SetCompact(pindexLast->nBits).getuint256().getdouble()) * 100) );
        }
        nPrevHeight = pindexLast->nHeight;
        nPrevDifficulty = bnNew.GetCompact();
    }

    // Difficulty is returned in compact form.
    return bnNew.GetCompact();
}

#endif
