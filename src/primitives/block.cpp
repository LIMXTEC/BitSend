// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>
#include "chain.h"
#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>
#include <crypto/scrypt.h> // Bitsend Scrypt
#include <crypto/hashblock.h> // Bitsend TimeTravel

uint256 CBlockHeader::GetHash() const
{
    if(GetBlockTime() >= 1477958400) // Bitsend PoW Hardfork, Friday, 09-Sep-16 18:00:00 UTC
		{
		return XEVAN(BEGIN(nVersion), END(nNonce));
		strprintf("XEVAN_Hash is on.");
		}
		else
		{
			return HashX11(BEGIN(nVersion), END(nNonce));
			strprintf("X11_Hash is on.");
		}
}

uint256 CBlockHeader::GetPoWHash() const
{
		if(GetBlockTime() >= 1477958400) // Bitsend PoW Hardfork, Friday, 09-Sep-16 18:00:00 UTC
		{
		return XEVAN(BEGIN(nVersion), END(nNonce));
		strprintf("XEVAN_Hash is on.");
		}
		else
		{
			return HashX11(BEGIN(nVersion), END(nNonce));
			strprintf("X11_Hash is on.");
		}
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
