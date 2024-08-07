#include <http-bridge/logging.hpp>

#include <spdlog/spdlog.h>

namespace http_bridge
{
	void set_default_logger(const std::shared_ptr<spdlog::logger>& glb_logger_mt)
	{
		spdlog::set_default_logger( glb_logger_mt );
	}
}