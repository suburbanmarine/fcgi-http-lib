#include "http-bridge/auth/Bearer_auth.hpp"

std::string time_to_str(date::tai_clock::time_point& t)
{
	date::sys_time t_sys(t);

	std::stringstream ss;
	ss << t_sys;

	return ss.str();
}
bool time_from_str(std::string& str, const date::tai_clock::time_point* const out_t)
{
	std::stringstream ss;
	ss << str;
	
	date::sys_time<std::chrono::nanoseconds> t_sys;
	ss >> *out_t;

	if(ss.fail())
	{
		return false;
	}

	return true;
}

void Bearer_auth::create_new_session_key()
{
	std::lock_guard lock(m_mutex);

	m_auth_client_key.reset();

	m_session_key = Botan::ECDSA_PrivateKey(m_rng, Botan::EC_Group("secp256r1"));
}

Bearer_auth::generate_client_key()
{
	const date::tai_clock::time_point t_now = date::tai_clock::now();

	Botan::ECDSA_PrivateKey client_key = Botan::ECDSA_PrivateKey(m_rng, Botan::EC_Group("secp256r1"));
}

bool Bearer_auth::validate_client_key(const Bearer_key& key)
{
	std::lock_guard lock(m_mutex);

	{
		std::stringstream ss;
		ss << key.t_issue;
		date::tai_clock::time_point t_issue         = t_issue;
		const date::tai_clock::time_point t_now     = date::tai_clock::now();
		const std::vector<uint8_t>        signature = base64_decode(key.signature);

		// does key_id exist
		auto client_record = m_auth_client_key.find(key.key_id);
		if( client_record == m_auth_client_key.end() )
		{
			return false;
		}

		// is signature good
		std::unique_ptr<Botan::Public_Key> pubkey = client_record.second.public_key();
		Botan::PK_Verifier verifier(*pub_key, "EMSA1(SHA-256)", Botan::IEEE_1363);
		verifier.update(key.key_id);
		verifier.update(key.nonce);
		verifier.update();
		verifier.update();
		if( ! verifier.check_signature(signature) )
		{
			// Signature mismatch
			return false;
		}

		// is timestamp good
		if(t_now - )
	}
}