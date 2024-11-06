/**
 * This file is part of emb-lin-util, a collection of utility code for embedded linux.
 * 
 * This software is distrubuted in the hope it will be useful, but without any warranty, including the implied warrranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See LICENSE.txt for details.
 * 
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2024 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the LGPL-3.0 license. See LICENSE.txt for details.
 * SPDX-License-Identifier: LGPL-3.0-only
*/

#pragma once

#include <spdlog/logger.h>

namespace http_bridge
{
	// not thread safe wrt self or library's usage of logger
	// call early
	void set_default_logger(const std::shared_ptr<spdlog::logger>& glb_logger_mt);
}