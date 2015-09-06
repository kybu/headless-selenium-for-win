// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <VersionHelpers.h>

#include <boost/program_options.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/format.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "desktop.h"
#include "environment.h"
#include "process.h"


#define LOGT BOOST_LOG_TRIVIAL(trace)
#define LOGD BOOST_LOG_TRIVIAL(debug)
#define LOGI BOOST_LOG_TRIVIAL(info)
#define LOGW BOOST_LOG_TRIVIAL(warning)
#define LOGF BOOST_LOG_TRIVIAL(fatal)

// TODO: reference additional headers your program requires here
