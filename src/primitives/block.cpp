// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The BitSend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>

uint256 CBlockHeader::GetHash() const
{
    // BitSend PoW Hardfork, https://chainz.cryptoid.info/bsd/block.dws?229114.htm
    const int FORKX17_Main_Net2 = 1477958400;
    if(GetBlockTime() >= FORKX17_Main_Net2)
        // return XEVAN(BEGIN(nVersion), END(nNonce));
		return XEVAN((char*)&(nVersion), (char*)&((&(nNonce))[1]));
    else
        return SerializeHash(*this);
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
