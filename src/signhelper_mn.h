

#include "validation.h"

#include "sync.h"
#include "activemasternode.h"




class CMNSignHelper{

	public:
    /// Is the inputs associated with this public key? (and there is 14000 BSD - checking if valid masternode)
    /*bool IsVinAssociatedWithPubkey(CTxIn& vin, CPubKey& pubkey){
		CScript payee2;
		payee2=GetScriptForDestination(pubkey.GetID());

		CTransactionRef txVin;
		uint256 hash;
		if(GetTransaction(vin.prevout.hash, txVin, Params().GetConsensus(),hash, true)){
			BOOST_FOREACH(CTxOut out, txVin->vout){
				if(out.nValue == MASTERNODEAMOUNT*COIN){
					if(out.scriptPubKey == payee2) return true;
				}
			}
		}

		return false;
	}*/
    /// Set the private/public key values, returns true if successful
    bool SetKey(std::string strSecret, std::string& errorMessage, CKey& key, CPubKey& pubkey){
		CBitcoinSecret vchSecret;
		bool fGood = vchSecret.SetString(strSecret);

		if (!fGood) {
			errorMessage = _("Invalid private key.");
			return false;
		}

		key = vchSecret.GetKey();
		pubkey = key.GetPubKey();

		return true;
	}

    /// Sign the message, returns true if successful
    bool SignMessage(std::string strMessage, std::string& errorMessage, vector<unsigned char>& vchSig, CKey key)
	{
		CHashWriter ss(SER_GETHASH, 0);
		ss << strMessageMagic;
		ss << strMessage;

		if (!key.SignCompact(ss.GetHash(), vchSig)) {
			errorMessage = _("Signing failed.");
			return false;
		}

		return true;
	}
    /// Verify the message, returns true if succcessful
    bool VerifyMessage(CPubKey pubkey, vector<unsigned char>& vchSig, std::string strMessage, std::string& errorMessage)
	{
		CHashWriter ss(SER_GETHASH, 0);
		ss << strMessageMagic;
		ss << strMessage;

		CPubKey pubkey2;
		if (!pubkey2.RecoverCompact(ss.GetHash(), vchSig)) {
			errorMessage = _("Error recovering public key.");
			return false;
		}

		if (fDebug && pubkey2.GetID() != pubkey.GetID())
			LogPrintf("CDarkSendSigner::VerifyMessage -- keys don't match: %s %s", pubkey2.GetID().ToString(), pubkey.GetID().ToString());

		return (pubkey2.GetID() == pubkey.GetID());
	}
};



extern CMNSignHelper darkSendSigner;
extern std::string strMasterNodePrivKey;

