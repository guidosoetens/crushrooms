#include "crPlayerCommManager.h"


#include <iwlib.h>
#include <ifaddrs.h>

crPlayerCommManager::crPlayerCommManager() {
    mListener = NULL;
    mNextPlayerId = 0;
}

crPlayerCommManager::~crPlayerCommManager() {

}

string crPlayerCommManager::getIpAddress() {

    static char ip[1024];
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    bool foundIp = false;

    if( getifaddrs(&ifaddr) != -1 ) {

        for( ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next ) {
            if( ((ifa->ifa_flags & (IFF_UP | IFF_RUNNING)) == (IFF_UP | IFF_RUNNING)) && ((ifa->ifa_flags & IFF_LOOPBACK) != IFF_LOOPBACK)) {
                if( ifa->ifa_addr == NULL ) {
                    continue;
                }
                family = ifa->ifa_addr->sa_family;
                if( family == AF_INET || family == AF_INET6) {
                    s = getnameinfo(ifa->ifa_addr,
                        (family == AF_INET) ? sizeof(struct sockaddr_in) :
                        sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                    if( s == 0 ) {
                        if( strcmp(ifa->ifa_name, "tun0") != 0 ) {
                            strcpy(ip,host);
                            foundIp = true;
                            break;
                        }
                    }
                }
            }
        }

        freeifaddrs(ifaddr);
    }

    if(foundIp)
        return ip;
    return "UNKNOWN";
}


bool crPlayerCommManager::initialize(IPlayerConnectionListener * listener) {

    mListener = listener;

    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9093;
    bool connected = mServer.setup( options );

    mServer.addListener(this);

    cout << getIpAddress() << endl;

    return connected;
}

void crPlayerCommManager::sendInputCommand(int activePlayerId) {
    for(int i=0; i<mPlayers.size(); ++i) {
        if(mPlayers[i].playerId == activePlayerId)
            mPlayers[i].connectionReference->send("start-cmd");
    }
}

void crPlayerCommManager::sendEndInput(int playerId) {
    for(int i=0; i<mPlayers.size(); ++i) {
        if(mPlayers[i].playerId == playerId) {
            mPlayers[i].connectionReference->send("end-msg");
        }
    }
}

void crPlayerCommManager::onConnect( ofxLibwebsockets::Event& args ) {
    //...
}

void crPlayerCommManager::onOpen( ofxLibwebsockets::Event& args ) {
    crPlayerToken token = { mNextPlayerId++, &args.conn };
    mPlayers.push_back(token);
    if(mListener != NULL)
        mListener->onPlayerJoined(token.playerId);
}

void crPlayerCommManager::onClose( ofxLibwebsockets::Event& args ) {
    for(int i=0; i<mPlayers.size(); ++i) {
        if(mPlayers[i].connectionReference == &args.conn) {
            if(mListener != NULL)
                mListener->onPlayerLeft(mPlayers[i].playerId);
            mPlayers.erase(mPlayers.begin() + i);
            break;
        }
    }
}

void crPlayerCommManager::onIdle( ofxLibwebsockets::Event& args ) {
    for(int i=0; i<mPlayers.size(); ++i) {
        if(mPlayers[i].connectionReference == &args.conn) {
            cout << mPlayers[i].playerId << " idle"<< endl;
            break;
        }
    }
}

bool isPrefix(string s, string prefix) {
    return s.compare(0, prefix.length(), prefix) == 0;
}

string cutPrefix(string s, string prefix) {
    int n = prefix.length();
    return s.substr(n, s.length() - n); 
}

void crPlayerCommManager::onMessage( ofxLibwebsockets::Event& args ) {

    crPlayerToken token;
    bool foundPlayer = false;
    for(int i=0; i<mPlayers.size(); ++i) {
        if(mPlayers[i].connectionReference == &args.conn) {
            token = mPlayers[i];
            foundPlayer = true;
            break;
        }
    }

    if(!foundPlayer)
        return;

    string msg = args.message;
    string loginPrefix = "login-user:";
    string cmdPrefix = "user-cmd:";
    string donePrefix = "user-done";
    
    if(isPrefix(msg, loginPrefix)) {
        string username = cutPrefix(msg, loginPrefix);
        if(mListener != NULL)
            mListener->onPlayerSentUsername(token.playerId, username);
    }
    else if (isPrefix(msg, cmdPrefix)) {
        string cmd = cutPrefix(msg, cmdPrefix);
        if(mListener != NULL)
            mListener->onPlayerSentInput(token.playerId, cmd);
    }
    else if (isPrefix(msg, donePrefix)) {
        if(mListener != NULL)
            mListener->onPlayerDone(token.playerId);
    }
}

void crPlayerCommManager::onBroadcast( ofxLibwebsockets::Event& args ) {
    //...
}