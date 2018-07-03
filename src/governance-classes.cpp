// Copyright (c) 2014-2018 The Dash Core developers
// Copyright (c) 2014-2018 The Bitsend Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <core_io.h>
#include <governance-classes.h>
#include <init.h>
#include <validation.h>
#include <utilstrencodings.h>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <univalue.h>

// DECLARE GLOBAL VARIABLES FOR GOVERNANCE CLASSES
CGovernanceTriggerManager triggerman;

// SPLIT UP STRING BY DELIMITER
// http://www.boost.org/doc/libs/1_58_0/doc/html/boost/algorithm/split_idp202406848.html
std::vector<std::string> SplitBy(std::string strCommand, std::string strDelimit)
{
    std::vector<std::string> vParts;
    boost::split(vParts, strCommand, boost::is_any_of(strDelimit));

    for(int q=0; q<(int)vParts.size(); q++) {
        if(strDelimit.find(vParts[q]) != std::string::npos) {
            vParts.erase(vParts.begin()+q);
            --q;
        }
    }

   return vParts;
}

CAmount ParsePaymentAmount(const std::string& strAmount)
{
    CAmount nAmount = 0;
    if (strAmount.empty()) {
        std::ostringstream ostr;
        ostr << "ParsePaymentAmount: Amount is empty";
        throw std::runtime_error(ostr.str());
    }
    if(strAmount.size() > 20) {
        // String is much too long, the functions below impose stricter
        // requirements
        std::ostringstream ostr;
        ostr << "ParsePaymentAmount: Amount string too long";
        throw std::runtime_error(ostr.str());
    }
    // Make sure the string makes sense as an amount
    // Note: No spaces allowed
    // Also note: No scientific notation
    size_t pos = strAmount.find_first_not_of("0123456789.");
    if (pos != std::string::npos) {
        std::ostringstream ostr;
        ostr << "ParsePaymentAmount: Amount string contains invalid character";
        throw std::runtime_error(ostr.str());
    }

    pos = strAmount.find(".");
    if (pos == 0) {
        // JSON doesn't allow values to start with a decimal point
        std::ostringstream ostr;
        ostr << "ParsePaymentAmount: Invalid amount string, leading decimal point not allowed";
        throw std::runtime_error(ostr.str());
    }

    // Make sure there's no more than 1 decimal point
    if ((pos != std::string::npos) && (strAmount.find(".", pos+1) != std::string::npos)) {
        std::ostringstream ostr;
        ostr << "ParsePaymentAmount: Invalid amount string, too many decimal points";
        throw std::runtime_error(ostr.str());
    }

    // Note this code is taken from AmountFromValue in rpcserver.cpp
    // which is used for parsing the amounts in createrawtransaction.
    if (!ParseFixedPoint(strAmount, 8, &nAmount)) {
        nAmount = 0;
        std::ostringstream ostr;
        ostr << "ParsePaymentAmount: ParseFixedPoint failed for string: " << strAmount;
        throw std::runtime_error(ostr.str());
    }
    if (!MoneyRange(nAmount)) {
        nAmount = 0;
        std::ostringstream ostr;
        ostr << "ParsePaymentAmount: Invalid amount string, value outside of valid money range";
        throw std::runtime_error(ostr.str());
    }

    return nAmount;
}

/**
*   Add Governance Object
*/

bool CGovernanceTriggerManager::AddNewTrigger(uint256 nHash)
{
    AssertLockHeld(governance.cs);

    // IF WE ALREADY HAVE THIS HASH, RETURN
    if(mapTrigger.count(nHash)) {
        return false;
    }

    CSuperblock_sptr pSuperblock;
    try  {
        CSuperblock_sptr pSuperblockTmp(new CSuperblock(nHash));
        pSuperblock = pSuperblockTmp;
    }
    catch(std::exception& e) {
        LogPrintf("CGovernanceTriggerManager::AddNewTrigger -- Error creating superblock: %s\n", e.what());
        return false;
    }
    catch(...) {
        LogPrintf("CGovernanceTriggerManager::AddNewTrigger: Unknown Error creating superblock\n");
        return false;
    }

    pSuperblock->SetStatus(SEEN_OBJECT_IS_VALID);

    mapTrigger.insert(std::make_pair(nHash, pSuperblock));

    return true;
}

/**
*
*   Clean And Remove
*
*/

void CGovernanceTriggerManager::CleanAndRemove()
{
    LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- Start\n");
    AssertLockHeld(governance.cs);

    // LOOK AT THESE OBJECTS AND COMPILE A VALID LIST OF TRIGGERS
    for(trigger_m_it it = mapTrigger.begin(); it != mapTrigger.end(); ++it) {
        //int nNewStatus = -1;
        CGovernanceObject* pObj = governance.FindGovernanceObject((*it).first);
        if(!pObj) {
            continue;
        }
        CSuperblock_sptr& pSuperblock = it->second;
        if(!pSuperblock) {
            continue;
        }
        // IF THIS ISN'T A TRIGGER, WHY ARE WE HERE?
        if(pObj->GetObjectType() != GOVERNANCE_OBJECT_TRIGGER) {
            pSuperblock->SetStatus(SEEN_OBJECT_ERROR_INVALID);
        }
    }

    // Remove triggers that are invalid or already executed
    LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- mapTrigger.size() = %d\n", mapTrigger.size());
    trigger_m_it it = mapTrigger.begin();
    while(it != mapTrigger.end()) {
        bool remove = false;
        CSuperblock_sptr& pSuperblock = it->second;
        if(!pSuperblock) {
            LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- NULL superblock marked for removal\n");
            remove = true;
        } else {
            LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- superblock status = %d\n", pSuperblock->GetStatus());
            switch(pSuperblock->GetStatus()) {
            case SEEN_OBJECT_ERROR_INVALID:
            case SEEN_OBJECT_UNKNOWN:
                LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- Unknown or invalid trigger found\n");
                remove = true;
                break;
            case SEEN_OBJECT_IS_VALID:
            case SEEN_OBJECT_EXECUTED:
                {
                    int nTriggerBlock = pSuperblock->GetBlockStart();
                    // Rough approximation: a cycle of superblock ++
                    int nExpirationBlock = nTriggerBlock + GOVERNANCE_TRIGGER_EXPIRATION_BLOCKS;
                    LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- nTriggerBlock = %d, nExpirationBlock = %d\n", nTriggerBlock, nExpirationBlock);
                    if(governance.GetCachedBlockHeight() > nExpirationBlock) {
                        LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- Outdated trigger found\n");
                        remove = true;
                        CGovernanceObject* pgovobj = pSuperblock->GetGovernanceObject();
                        if(pgovobj) {
                            LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- Expiring outdated object: %s\n", pgovobj->GetHash().ToString());
                            pgovobj->fExpired = true;
                            pgovobj->nDeletionTime = GetAdjustedTime();
                        }
                    }
                }
                break;
            default:
                break;
            }
        }

        if(remove) {
            LogPrint(MCLog::GOV, "CGovernanceTriggerManager::CleanAndRemove -- Removing trigger object\n");
            mapTrigger.erase(it++);
        }
        else  {
            ++it;
        }
    }
}

/**
*   Get Active Triggers
*
*   - Look through triggers and scan for active ones
*   - Return the triggers in a list
*/

std::vector<CSuperblock_sptr> CGovernanceTriggerManager::GetActiveTriggers()
{
    AssertLockHeld(governance.cs);
    std::vector<CSuperblock_sptr> vecResults;

    // LOOK AT THESE OBJECTS AND COMPILE A VALID LIST OF TRIGGERS
    trigger_m_it it = mapTrigger.begin();
    while(it != mapTrigger.end()) {

        CGovernanceObject* pObj = governance.FindGovernanceObject((*it).first);

        if(pObj) {
            vecResults.push_back(it->second);
        }
        ++it;
    }

    return vecResults;
}

/**
*   Is Superblock Triggered
*
*   - Does this block have a non-executed and actived trigger?
*/

bool CSuperblockManager::IsSuperblockTriggered(int nBlockHeight)
{
    LogPrint(MCLog::GOV, "CSuperblockManager::IsSuperblockTriggered -- Start nBlockHeight = %d\n", nBlockHeight);
    if (!CSuperblock::IsValidBlockHeight(nBlockHeight)) {
        return false;
    }

    LOCK(governance.cs);
    // GET ALL ACTIVE TRIGGERS
    std::vector<CSuperblock_sptr> vecTriggers = triggerman.GetActiveTriggers();

    LogPrint(MCLog::GOV, "CSuperblockManager::IsSuperblockTriggered -- vecTriggers.size() = %d\n", vecTriggers.size());

    for (CSuperblock_sptr pSuperblock : vecTriggers)
    {
        if(!pSuperblock) {
            LogPrintf("CSuperblockManager::IsSuperblockTriggered -- Non-superblock found, continuing\n");
            continue;
        }

        CGovernanceObject* pObj = pSuperblock->GetGovernanceObject();

        if(!pObj) {
            LogPrintf("CSuperblockManager::IsSuperblockTriggered -- pObj == NULL, continuing\n");
            continue;
        }

        LogPrint(MCLog::GOV, "CSuperblockManager::IsSuperblockTriggered -- data = %s\n", pObj->GetDataAsString());

        // note : 12.1 - is epoch calculation correct?

        if(nBlockHeight != pSuperblock->GetBlockStart()) {
            LogPrint(MCLog::GOV, "CSuperblockManager::IsSuperblockTriggered -- block height doesn't match nBlockHeight = %d, blockStart = %d, continuing\n",
                     nBlockHeight,
                     pSuperblock->GetBlockStart());
            continue;
        }

        // MAKE SURE THIS TRIGGER IS ACTIVE VIA FUNDING CACHE FLAG

        pObj->UpdateSentinelVariables();

        if(pObj->IsSetCachedFunding()) {
            LogPrint(MCLog::GOV, "CSuperblockManager::IsSuperblockTriggered -- fCacheFunding = true, returning true\n");
            return true;
        }
        else  {
            LogPrint(MCLog::GOV, "CSuperblockManager::IsSuperblockTriggered -- fCacheFunding = false, continuing\n");
        }
    }

    return false;
}


bool CSuperblockManager::GetBestSuperblock(CSuperblock_sptr& pSuperblockRet, int nBlockHeight)
{
    if(!CSuperblock::IsValidBlockHeight(nBlockHeight)) {
        return false;
    }

    AssertLockHeld(governance.cs);
    std::vector<CSuperblock_sptr> vecTriggers = triggerman.GetActiveTriggers();
    int nYesCount = 0;

    for (CSuperblock_sptr pSuperblock : vecTriggers) {
        if(!pSuperblock) {
            continue;
        }

        CGovernanceObject* pObj = pSuperblock->GetGovernanceObject();

        if(!pObj) {
            continue;
        }

        if(nBlockHeight != pSuperblock->GetBlockStart()) {
            continue;
        }

        // DO WE HAVE A NEW WINNER?

        int nTempYesCount = pObj->GetAbsoluteYesCount(VOTE_SIGNAL_FUNDING);
        if(nTempYesCount > nYesCount) {
            nYesCount = nTempYesCount;
            pSuperblockRet = pSuperblock;
        }
    }

    return nYesCount > 0;
}

/**
*   Create Superblock Payments
*
*   - Create the correct payment structure for a given superblock
*/

void CSuperblockManager::CreateSuperblock(CMutableTransaction& txNewRet, int nBlockHeight, std::vector<CTxOut>& voutSuperblockRet)
{
    LOCK(governance.cs);

    // GET THE BEST SUPERBLOCK FOR THIS BLOCK HEIGHT

    CSuperblock_sptr pSuperblock;
    if(!CSuperblockManager::GetBestSuperblock(pSuperblock, nBlockHeight)) {
        LogPrint(MCLog::GOV, "CSuperblockManager::CreateSuperblock -- Can't find superblock for height %d\n", nBlockHeight);
        return;
    }

    // make sure it's empty, just in case
    voutSuperblockRet.clear();

    // CONFIGURE SUPERBLOCK OUTPUTS

    // TODO: How many payments can we add before things blow up?
    //       Consider at least following limits:
    //          - max coinbase tx size
    //          - max "budget" available
    for(int i = 0; i < pSuperblock->CountPayments(); i++) {
        CGovernancePayment payment;
        if(pSuperblock->GetPayment(i, payment)) {
            // SET COINBASE OUTPUT TO SUPERBLOCK SETTING

            CTxOut txout = CTxOut(payment.nAmount, payment.script);
            txNewRet.vout.push_back(txout);
            voutSuperblockRet.push_back(txout);

            // PRINT NICE LOG OUTPUT FOR SUPERBLOCK PAYMENT

            CTxDestination address1;
            ExtractDestination(payment.script, address1);

            // TODO: PRINT NICE N.N BSD OUTPUT

            LogPrintf("NEW Superblock : output %d (addr %s, amount %d)\n", i, EncodeDestination(address1), payment.nAmount);
        }
    }
}

bool CSuperblockManager::IsValid(const CTransactionRef& txNew, int nBlockHeight, CAmount blockReward)
{
    // GET BEST SUPERBLOCK, SHOULD MATCH
    LOCK(governance.cs);

    CSuperblock_sptr pSuperblock;
    if(CSuperblockManager::GetBestSuperblock(pSuperblock, nBlockHeight)) {
        return pSuperblock->IsValid(txNew, nBlockHeight, blockReward);
    }

    return false;
}

CSuperblock::
CSuperblock()
    : nGovObjHash(),
      nEpochStart(0),
      nStatus(SEEN_OBJECT_UNKNOWN),
      vecPayments()
{}

CSuperblock::
CSuperblock(uint256& nHash)
    : nGovObjHash(nHash),
      nEpochStart(0),
      nStatus(SEEN_OBJECT_UNKNOWN),
      vecPayments()
{
    CGovernanceObject* pGovObj = GetGovernanceObject();

    if(!pGovObj) {
        throw std::runtime_error("CSuperblock: Failed to find Governance Object");
    }

    if (pGovObj->GetObjectType() != GOVERNANCE_OBJECT_TRIGGER) {
        throw std::runtime_error("CSuperblock: Governance Object not a trigger");
    }

    UniValue obj = pGovObj->GetJSONObject();

    // FIRST WE GET THE START EPOCH, THE DATE WHICH THE PAYMENT SHALL OCCUR
    nEpochStart = obj["event_block_height"].get_int();

    // NEXT WE GET THE PAYMENT INFORMATION AND RECONSTRUCT THE PAYMENT VECTOR
    std::string strAddresses = obj["payment_addresses"].get_str();
    std::string strAmounts = obj["payment_amounts"].get_str();
    ParsePaymentSchedule(strAddresses, strAmounts);

    LogPrint(MCLog::GOV, "CSuperblock -- nEpochStart = %d, strAddresses = %s, strAmounts = %s, vecPayments.size() = %d\n",
             nEpochStart, strAddresses, strAmounts, vecPayments.size());
}

/**
 *   Is Valid Superblock Height
 *
 *   - See if a block at this height can be a superblock
 */

bool CSuperblock::IsValidBlockHeight(int nBlockHeight)
{
    // SUPERBLOCKS CAN HAPPEN ONLY after hardfork and only ONCE PER CYCLE
    return nBlockHeight >= Params().GetConsensus().nSuperblockStartBlock &&
            ((nBlockHeight % Params().GetConsensus().nSuperblockCycle) == 0);
}

CAmount CSuperblock::GetPaymentsLimit(int nBlockHeight)
{
    const Consensus::Params& consensusParams = Params().GetConsensus();

    if(!IsValidBlockHeight(nBlockHeight)) {
        return 0;
    }

    // min subsidy for high diff networks and vice versa
    // int nBits = consensusParams.fPowAllowMinDifficultyBlocks ? UintToArith256(consensusParams.powLimit).GetCompact() : 1;
    // some part of all blocks issued during the cycle goes to superblock, see GetBlockSubsidy
    CAmount nSuperblockPartOfSubsidy = GetBlockSubsidy(nBlockHeight - 1, consensusParams, true);
    // CAmount nSuperblockPartOfSubsidy = GetBlockSubsidy(nBlockHeight - 1, consensusParams);
    CAmount nPaymentsLimit = nSuperblockPartOfSubsidy * consensusParams.nSuperblockCycle;
    LogPrint(MCLog::GOV, "CSuperblock::GetPaymentsLimit -- Valid superblock height %d, payments max %lld\n", nBlockHeight, nPaymentsLimit);

    return nPaymentsLimit;
}

void CSuperblock::ParsePaymentSchedule(std::string& strPaymentAddresses, std::string& strPaymentAmounts)
{
    // SPLIT UP ADDR/AMOUNT STRINGS AND PUT IN VECTORS

    std::vector<std::string> vecParsed1;
    std::vector<std::string> vecParsed2;
    vecParsed1 = SplitBy(strPaymentAddresses, "|");
    vecParsed2 = SplitBy(strPaymentAmounts, "|");

    // IF THESE DONT MATCH, SOMETHING IS WRONG

    if (vecParsed1.size() != vecParsed2.size()) {
        std::ostringstream ostr;
        ostr << "CSuperblock::ParsePaymentSchedule -- Mismatched payments and amounts";
        LogPrintf("%s\n", ostr.str());
        throw std::runtime_error(ostr.str());
    }

    if (vecParsed1.size() == 0) {
        std::ostringstream ostr;
        ostr << "CSuperblock::ParsePaymentSchedule -- Error no payments";
        LogPrintf("%s\n", ostr.str());
        throw std::runtime_error(ostr.str());
    }

    // LOOP THROUGH THE ADDRESSES/AMOUNTS AND CREATE PAYMENTS
    /*
      ADDRESSES = [ADDR1|2|3|4|5|6]
      AMOUNTS = [AMOUNT1|2|3|4|5|6]
    */

    for (int i = 0; i < (int)vecParsed1.size(); i++) {
        CTxDestination address = DecodeDestination(vecParsed1[i]);
        if (!IsValidDestinationString(vecParsed1[i])) {
            std::ostringstream ostr;
            ostr << "CSuperblock::ParsePaymentSchedule -- Invalid Bitsend Address : " <<  vecParsed1[i];
            LogPrintf("%s\n", ostr.str());
            throw std::runtime_error(ostr.str());
        }

        CAmount nAmount = ParsePaymentAmount(vecParsed2[i]);

        CGovernancePayment payment(vecParsed1[i], nAmount);
        if(payment.IsValid()) {
            vecPayments.push_back(payment);
        }
        else {
            vecPayments.clear();
            std::ostringstream ostr;
            ostr << "CSuperblock::ParsePaymentSchedule -- Invalid payment found: address = " << vecParsed1[i]
                 << ", amount = " << nAmount;
            LogPrintf("%s\n", ostr.str());
            throw std::runtime_error(ostr.str());
        }
    }
}

bool CSuperblock::GetPayment(int nPaymentIndex, CGovernancePayment& paymentRet)
{
    if((nPaymentIndex<0) || (nPaymentIndex >= (int)vecPayments.size())) {
        return false;
    }

    paymentRet = vecPayments[nPaymentIndex];
    return true;
}

CAmount CSuperblock::GetPaymentsTotalAmount()
{
    CAmount nPaymentsTotalAmount = 0;
    int nPayments = CountPayments();

    for(int i = 0; i < nPayments; i++) {
        nPaymentsTotalAmount += vecPayments[i].nAmount;
    }

    return nPaymentsTotalAmount;
}

/**
*   Is Transaction Valid
*
*   - Does this transaction match the superblock?
*/

bool CSuperblock::IsValid(const CTransactionRef& txNew, int nBlockHeight, CAmount blockReward)
{
    // TODO : LOCK(cs);
    // No reason for a lock here now since this method only accesses data
    // internal to *this and since CSuperblock's are accessed only through
    // shared pointers there's no way our object can get deleted while this
    // code is running.
    if(!IsValidBlockHeight(nBlockHeight)) {
        LogPrintf("CSuperblock::IsValid -- ERROR: Block invalid, incorrect block height\n");
        return false;
    }

    std::string strPayeesPossible = "";

    // CONFIGURE SUPERBLOCK OUTPUTS

    int nOutputs = txNew->vout.size();
    int nPayments = CountPayments();
    int nMinerPayments = nOutputs - nPayments;

    LogPrint(MCLog::GOV, "CSuperblock::IsValid nOutputs = %d, nPayments = %d, strData = %s\n",
             nOutputs, nPayments, GetGovernanceObject()->GetDataAsHex());

    // We require an exact match (including order) between the expected
    // superblock payments and the payments actually in the block.

    if(nMinerPayments < 0) {
        // This means the block cannot have all the superblock payments
        // so it is not valid.
        // TODO: could that be that we just hit coinbase size limit?
        LogPrintf("CSuperblock::IsValid -- ERROR: Block invalid, too few superblock payments\n");
        return false;
    }

    // payments should not exceed limit
    CAmount nPaymentsTotalAmount = GetPaymentsTotalAmount();
    CAmount nPaymentsLimit = GetPaymentsLimit(nBlockHeight);
    if(nPaymentsTotalAmount > nPaymentsLimit) {
        LogPrintf("CSuperblock::IsValid -- ERROR: Block invalid, payments limit exceeded: payments %lld, limit %lld\n", nPaymentsTotalAmount, nPaymentsLimit);
        return false;
    }

    // miner should not get more than he would usually get
    CAmount nBlockValue = txNew->GetValueOut();
    if(nBlockValue > blockReward + nPaymentsTotalAmount) {
        LogPrintf("CSuperblock::IsValid -- ERROR: Block invalid, block value limit exceeded: block %lld, limit %lld\n", nBlockValue, blockReward + nPaymentsTotalAmount);
        return false;
    }

    int nVoutIndex = 0;
    for(int i = 0; i < nPayments; i++) {
        CGovernancePayment payment;
        if(!GetPayment(i, payment)) {
            // This shouldn't happen so log a warning
            LogPrintf("CSuperblock::IsValid -- WARNING: Failed to find payment: %d of %d total payments\n", i, nPayments);
            continue;
        }

        bool fPaymentMatch = false;

        for (int j = nVoutIndex; j < nOutputs; j++) {
            // Find superblock payment
            fPaymentMatch = ((payment.script == txNew->vout[j].scriptPubKey) &&
                             (payment.nAmount == txNew->vout[j].nValue));

            if (fPaymentMatch) {
                nVoutIndex = j;
                break;
            }
        }

        if(!fPaymentMatch) {
            // Superblock payment not found!

            CTxDestination address1;
            ExtractDestination(payment.script, address1);
            LogPrintf("CSuperblock::IsValid -- ERROR: Block invalid: %d payment %d to %s not found\n", i, payment.nAmount, EncodeDestination(address1));

            return false;
        }
    }

    return true;
}

/**
*   Get Required Payment String
*
*   - Get a string representing the payments required for a given superblock
*/

std::string CSuperblockManager::GetRequiredPaymentsString(int nBlockHeight)
{
    LOCK(governance.cs);
    std::string ret = "Unknown";

    // GET BEST SUPERBLOCK

    CSuperblock_sptr pSuperblock;
    if(!GetBestSuperblock(pSuperblock, nBlockHeight)) {
        LogPrint(MCLog::GOV, "CSuperblockManager::GetRequiredPaymentsString -- Can't find superblock for height %d\n", nBlockHeight);
        return "error";
    }

    // LOOP THROUGH SUPERBLOCK PAYMENTS, CONFIGURE OUTPUT STRING

    for(int i = 0; i < pSuperblock->CountPayments(); i++) {
        CGovernancePayment payment;
        if(pSuperblock->GetPayment(i, payment)) {
            // PRINT NICE LOG OUTPUT FOR SUPERBLOCK PAYMENT

            CTxDestination address1;
            ExtractDestination(payment.script, address1);

            // RETURN NICE OUTPUT FOR CONSOLE

            if(ret != "Unknown") {
                ret += ", " + EncodeDestination(address1);
            }
            else {
                ret = EncodeDestination(address1);
            }
        }
    }

    return ret;
}