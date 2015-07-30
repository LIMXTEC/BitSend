
// Copyright (c) 2009-2012 The Limecoinx developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef SPORK_H
#define SPORK_H

#include "bignum.h"
#include "sync.h"
#include "net.h"
#include "key.h"
#include "core.h"
#include "util.h"
#include "script.h"
#include "base58.h"
#include "main.h"

using namespace std;
using namespace boost;

// Don't ever reuse these IDs for other sporks
#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT               10000
#define SPORK_2_INSTANTX                                      10001
#define SPORK_3_INSTANTX_BLOCK_FILTERING                      10002
#define SPORK_4_NOTUSED                                       10003
#define SPORK_5_MAX_VALUE                                     10004
#define SPORK_6_NOTUSED                                       10005
#define SPORK_7_MASTERNODE_SCANNING                           10006

#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT_DEFAULT       1424217600  //2015-2-18
#define SPORK_2_INSTANTX_DEFAULT                              978307200   //2001-1-1
#define SPORK_3_INSTANTX_BLOCK_FILTERING_DEFAULT              1424217600  //2015-2-18
#define SPORK_5_MAX_VALUE_DEFAULT                             10000        //10000 LIMX 01-05-2015   // Sprungmarke BBBBBBBBBB
#define SPORK_7_MASTERNODE_SCANNING_DEFAULT                   978307200   //2001-1-1

class CSporkMessage;
class CSporkManager;

#include "bignum.h"
#include "net.h"
#include "key.h"
#include "util.h"
#include "protocol.h"
#include "darksend.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;

void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
int GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ExecuteSpork(int nSporkID, int nValue);

//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    uint256 GetHash(){
        uint256 n = HashX11(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    IMPLEMENT_SERIALIZE(
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    )
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;

    std::string strMasterPrivKey;
    std::string strTestPubKey;
    std::string strMainPubKey;

public:

    CSporkManager() {
        
    // 100: G=0 101: MK just test
        strMainPubKey = "04521CAE97FEE1CA1F67B4B6D5E4323125D63DBA5291CF2609F71234ED86355A109140B3316366CAD47DCD2D0C04326A3233AC7797231F0AF88C1FE6FE94B7E37C"; // limxdev 04-2015
        strTestPubKey = "04CBC82D432A42A05F9474A5554413A6166767C928DE669C40144DC585FB85F15E28035EADE398A6B8E38C24A001EAB50023124C4D8328C99EC2FDE47ED54B17BF";  // limxdev do not use 04-2015
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CSporkMessage& spork);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg);

};

#endif
