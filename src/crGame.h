#pragma once

#include "ofMain.h"
#include "crPlayerCommManager.h"
#include "crJoinTag.h"
#include "crPlayer.h"

class crGame : public IPlayerConnectionListener {
	public:
        crGame();
        virtual ~crGame();

        void setup();
		void update(float dt);
		void draw();

        void keyPressed(int key);
        void onMouseDown(ofVec2f pt);

        virtual void onPlayerLeft(int playerId);
        virtual void onPlayerJoined(int playerId);
        virtual void onPlayerSentUsername(int playerId, string username);
        virtual void onPlayerSentInput(int playerId, string input);
        virtual void onPlayerDone(int playerId);

    private:

        vector<crPlayer> mPlayers;
        int getPlayerIndex(int playerId);

        typedef enum {
            GAMESTATE_LOBBY = 0,
            GAMESTATE_INTRO,
            GAMESTATE_GATHER_COMMANDS,
            GAMESTATE_PLAY,
            GAMESTATE_OUTRO
        } crGameState;
        crGameState mGameState;
        float mGameStateParam;

        crPlayerCommManager mPlayersManager;
        crJoinTag mJoinTag;
        ofTrueTypeFont mFont;

        bool canStartGame();
        void startGame();
        void startRound();
        void printPlayer(crPlayer & player, int line);

};
