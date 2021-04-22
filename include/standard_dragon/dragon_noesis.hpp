//
// Created by Lilith on 2021-01-28.
//

#pragma once

#ifdef USE_NOESIS

#include <pluginshare.h>
#include <sstream>

#ifdef OLOG
#undef OLOG
#define OLOG(out, msg)                                                    \
    do {                                                                  \
        std::stringstream s;                                              \
        s << msg << std::endl;                                            \
        if (g_nfn != nullptr && g_nfn->NPAPI_DebugLogIsOpen())            \
            g_nfn->NPAPI_DebugLogStr(const_cast<char*>(s.str().c_str())); \
        (out << s.str() << std::flush);                                   \
    } while (0)

#ifndef FOURCC_DX10
#define FOURCC_DX10 (MAKEFOURCC('D', 'X', '1', '0'))
#endif

#endif // USE_NOESIS

