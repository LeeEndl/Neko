// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2018 Intel Corporation

#ifndef OPENCV_GAPI_OWN_TYPES_HPP
#define OPENCV_GAPI_OWN_TYPES_HPP

#   if defined(__OPENCV_BUILD)
#       include <opencv2/core/base.hpp>
#       define GAPI_EXPORTS CV_EXPORTS
		/* special informative macros for wrapper generators */
#       define GAPI_PROP CV_PROP
#       define GAPI_PROP_RW CV_PROP_RW
#       define GAPI_WRAP CV_WRAP
#       define GAPI_EXPORTS_W_SIMPLE CV_EXPORTS_W_SIMPLE
#       define GAPI_EXPORTS_W CV_EXPORTS_W
#   else
#       define GAPI_PROP
#       define GAPI_PROP_RW
#       define GAPI_WRAP
#       define GAPI_EXPORTS
#       define GAPI_EXPORTS_W_SIMPLE
#       define GAPI_EXPORTS_W

#if 0  // Note: the following version currently is not needed for non-OpenCV build
#       if defined _WIN32
#           define GAPI_EXPORTS __declspec(dllexport)
#       elif defined __GNUC__ && __GNUC__ >= 4
#           define GAPI_EXPORTS __attribute__ ((visibility ("default")))
#       endif

#       ifndef GAPI_EXPORTS
#           define GAPI_EXPORTS
#       endif
#endif

#   endif

#endif // OPENCV_GAPI_OWN_TYPES_HPP