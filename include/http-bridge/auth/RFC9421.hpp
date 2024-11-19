class RFC9421_signature_parameters
{
	std::string created;
	std::string expires;
	std::string nonce;
	std::string alg;
	std::string keyid;
	std::string tag;
}

class RFC9421_request
{
	std::string method;
	std::string target_uri;
	std::string authority;
	std::string scheme;
	std::string request_target;
	std::string path;
	std::string query;
	std::list<std::string> query_param;

	std::list<std::string, std::string> signature_input;
	std::map<std::string, std::string> signature;
};