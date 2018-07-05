// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>
#include "chain.h"
#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>

uint256 CBlockHeader::GetHash() const
{
    const int FORKX17_Main_Net2 = 1477958400;
	if(GetBlockTime() >= FORKX17_Main_Net2) // BitSend PoW Hardfork, Friday, 09-Sep-16 18:00:00 UTC
	{
		return XEVAN(BEGIN(nVersion), END(nNonce));
		strprintf("XEVAN_Hash is on.");
	}
    else
	{
	 return HashX11(BEGIN(nVersion), END(nNonce));
	 strprintf("X11_Hash is on.");
	}
    //return HashX11(BEGIN(nVersion), END(nNonce));//return SerializeHash(*this);//TODO-- algo xevan
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
