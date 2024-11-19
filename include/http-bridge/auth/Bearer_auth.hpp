#pragma once

// client uses an ECDSA signature to get a limited time bearer token

// server generates bearer token with limits (client, time) and signs it

// server returns bearer token to client

class Bearer_key
{
	std::string key_id;     // UUID
	std::string nonce;      // 256b nonce
	std::string pubkey_b64; // b64(X509::BER(key))
	std::string t_issue;    // string of issue timestamp
	std::string signature   // b64(EMSA1-SHA-256(key_id + nonce + tai_issue_time)
};

class Bearer_key_record
{
	std::string                 key_id;     // UUID
	std::string                 nonce;      // 256b nonce
	Botan::ECDSA_PrivateKey     key;
	date::tai_clock::time_point t_issue;    // string of issue timestamp
	std::string                 signature   // b64(EMSA1-SHA-256(key_id + nonce + tai_issue_time)
};

class Bearer_auth
{
public:

	// invalidate all client keys and generate new session key
	void create_new_session_key();

	bool validate_client_key(const Bearer_key& key);

	Bearer_key generate_client_key();

	void remove_old_client_key();

	static std::string time_to_str(date::tai_clock::time_point& t);
	static bool time_from_str(std::string& str, const date::tai_clock::time_point* const out_t);

protected:

	std::mutex m_mutex;

	Botan::AutoSeeded_RNG m_rng;

	Botan::ECDSA_PrivateKey m_session_key;

	std::map<std::string, Botan::ECDSA_PrivateKey > m_auth_client_key;
};