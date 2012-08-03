#pragma once

namespace delta3
{
    const qint8 CSPYP1_PROTOCOL_ID      = 1;
    const qint8 CSPYP1_PROTOCOL_VERSION = 3;
    const qint8 CSPYP2_PROTOCOL_ID      = 1;
    const qint8 CSPYP2_PROTOCOL_VERSION = 2;
    const qint8 GRAPH_PROTOCOL_VERSION  = 2;

    enum ProtocolMode
    {
        MOD_TELNET   = 1,
		MOD_GRAPHICS = 2,
        MOD_FILE     = 3,
        MOD_MEDIA    = 4,
        MOD_PROXY    = 5,
        MOD_REDIRECT = 6
    };

    enum Cspyp1Command
    {
        CMD1_AUTH       = 1,
        CMD1_ADM        = 2,
        CMD1_LIST       = 3,
        CMD1_PING       = 4,
        CMD1_RLIST      = 5,
        CMD1_TRANSMIT   = 6,
        CMD1_DISCONNECT = 7
    };

    enum Cspyp2Command
    {
        CMD2_LIST       = 1,
        CMD2_MODES      = 2,
        CMD2_ACTIVATE   = 3,
        CMD2_DEACTIVATE = 4,
        CMD2_TRANSMIT   = 5
    };

    enum GraphMode
    {
        GMOD_INFO	 = 1,
        GMOD_IMGFULL = 2,
        GMOD_IMGDIFF = 3,
        GMOD_KEYEV   = 4,
        GMOD_MMOV    = 5,
        GMOD_MCLICK	 = 6
    };

    enum FileMode
    {
        FMOD_INFO       = 1,
        FMOD_CD         = 2,
        FMOD_DIR        = 3,
        FMOD_DOWNINFO   = 4,
        FMOD_DOWNLOAD   = 5,
        FMOD_RENAME     = 6,
        FMOD_DEL        = 7,
        FMOD_COPYTO     = 8,
        FMOD_MOVETO     = 9,
        FMOD_READY      = 101
    };
}
