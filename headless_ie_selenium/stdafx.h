// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <VersionHelpers.h>
#include <RestartManager.h>

#include <boost/smart_ptr.hpp>
#include <boost/move/unique_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/random/random_device.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>


#include "../desktop_utils/desktop.h"
#include "../desktop_utils/environment.h"

#define LOGT BOOST_LOG_TRIVIAL(trace)
#define LOGD BOOST_LOG_TRIVIAL(debug)
#define LOGI BOOST_LOG_TRIVIAL(info)
#define LOGW BOOST_LOG_TRIVIAL(warning)
#define LOGF BOOST_LOG_TRIVIAL(fatal)



// TODO: reference additional headers your program requires here
