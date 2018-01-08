#pragma once

#include "ofxLibwebsockets.h"

class IPlayerConnectionListener {
    public:
        virtual void onPlayerLeft(int playerId) = 0;
        virtual void onPlayerJoined(int playerId) = 0;
        virtual void onPlayerSentUsername(int playerId, string username) = 0;
        virtual void onPlayerSentInput(int playerId, string input) = 0;
        virtual void onPlayerDone(int playerId) = 0;
};

class crPlayerCommManager {
	public:
        crPlayerCommManager();
        virtual ~crPlayerCommManager();

        bool initialize(IPlayerConnectionListener * listener);
        void sendInputCommand(int activePlayerId);
        void sendEndInput(int playerId);

        // websocket methods
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );

        string getIpAddress();

    private:

        typedef struct {
            int playerId;
            ofxLibwebsockets::Connection* connectionReference;
        } crPlayerToken;

        //ofxTCPClient mClient;
        IPlayerConnectionListener * mListener;
        ofxLibwebsockets::Server mServer;

        int mNextPlayerId;
        vector<crPlayerToken> mPlayers;
};
