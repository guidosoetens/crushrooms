#pragma once

#include "crGame.h"

#define WAIT_INPUT_TIME 21.0

crGame::crGame()
:   mGameState(GAMESTATE_LOBBY)
,   mGameStateParam(0) 
{
    
}

crGame::~crGame() {

}

void crGame::setup() {
    mPlayersManager.initialize(this);
    mJoinTag.initialize(mPlayersManager.getIpAddress());
    mFont.loadFont("Courier Bold", 16);
}

int crGame::getPlayerIndex(int playerId) {
    for(int i=0; i<mPlayers.size(); ++i) {
        if(mPlayers[i].playerId == playerId) {
            return i;
        }
    }
    return -1;
}

void crGame::onPlayerLeft(int playerId) {
    int idx = getPlayerIndex(playerId);
    if(idx >= 0)
        mPlayers.erase(mPlayers.begin() + idx);
}

void crGame::onPlayerJoined(int playerId) {
    crPlayer player;
    player.playerId = playerId;
    player.isDone = false;
    player.isActivePlayer = false;
    player.isRegisteredPlayer = false;
    player.name = "";
    player.command = "";
    mPlayers.push_back(player);
}

void crGame::onPlayerSentUsername(int playerId, string username) {
    int idx = getPlayerIndex(playerId);
    if(idx >= 0) {
        mPlayers[idx].isRegisteredPlayer = true;
        mPlayers[idx].name = username;
    }
}

void crGame::onPlayerSentInput(int playerId, string input) {
    int idx = getPlayerIndex(playerId);
    if(idx >= 0) {
        mPlayers[idx].command = input;
    }
}

void crGame::onPlayerDone(int playerId) {
    int idx = getPlayerIndex(playerId);
    if(idx >= 0) {
        mPlayers[idx].isDone = true;
    }
}

void crGame::update(float dt) {

    switch(mGameState) {
        case GAMESTATE_LOBBY: {
            break;
        }
        case GAMESTATE_INTRO: {
            mGameStateParam += dt;
            if(mGameStateParam > 1) {
                mGameStateParam = 0;
                startRound();
            }
            break;
        }
        case GAMESTATE_GATHER_COMMANDS: {
            mGameStateParam += dt / WAIT_INPUT_TIME;

            bool allDone = true;
            for(int i=0; i<mPlayers.size(); ++i) {
                if(mPlayers[i].isActivePlayer) {
                    allDone = allDone && mPlayers[i].isDone;
                }
            }  

            if(mGameStateParam > 1.0 || allDone) {
                //send 'late' message to players who have no provided input:
                for(int i=0; i<mPlayers.size(); ++i) {
                    mPlayersManager.sendEndInput(mPlayers[i].playerId);
                }  

                //go to next state:
                mGameState = GAMESTATE_PLAY;
                mGameStateParam = 0;
            }

            break;
        }

        case GAMESTATE_PLAY: {
            mGameStateParam += dt / 3.0;
            if(mGameStateParam > 1.0) {
                if(ofRandom(1.0) < .1) {
                    //some player won...
                    mGameState = GAMESTATE_OUTRO;
                    mGameStateParam = 0;

                }
                else {
                    //nobody won, go to next round
                    startRound();
                }
            }
            break;
        }
        case GAMESTATE_OUTRO: {
           mGameStateParam += dt;
            if(mGameStateParam > 1.0) {
                //go back to lobby
                mGameState = GAMESTATE_LOBBY;
                mGameStateParam = 0;
            }
            break;
        }
    }
}

void crGame::printPlayer(crPlayer & player, int line) {
    string s = "Player: ";
    if(player.isRegisteredPlayer)
        s += player.name;
    else
        s += "[" + ofToString(player.playerId) + "]";
    mFont.drawString(s, 250, 60 + line * 20);
}

void crGame::draw() {

    if(mGameState == GAMESTATE_LOBBY) {
        mJoinTag.draw(30,50,200,200);

        //first print registered players:
        int line = 0;
        for(int i=0; i<mPlayers.size(); ++i) {
            if(line < 4)
                ofSetColor(0,255,0);
            else
                ofSetColor(0,100,0);
            
            if(mPlayers[i].isRegisteredPlayer) {
                printPlayer(mPlayers[i], line++);
            }
        }

        ofSetColor(40,40,40);
        for(int i=0; i<mPlayers.size(); ++i) {
            if(!mPlayers[i].isRegisteredPlayer) {
                printPlayer(mPlayers[i], line++);
            }
        }
    }
    else {
        //draw board:

        ofSetColor(0);

        string txt = "Gamestate: ";
        if(mGameState == GAMESTATE_INTRO)
            txt += "Intro...";
        else if(mGameState == GAMESTATE_GATHER_COMMANDS) {
            int timeLeft = (int)fminf(WAIT_INPUT_TIME - 1, floorf(WAIT_INPUT_TIME * (1 - mGameStateParam)));
            txt += "Waiting for input, time left " + ofToString(timeLeft) + " seconds";
        }
        else if(mGameState == GAMESTATE_PLAY) {
            txt += "Processing input";
        }
        else if(mGameState == GAMESTATE_OUTRO) {
            txt += "Outro...";
        }

        mFont.drawString(txt, 250, 400);
    }
}

void crGame::keyPressed(int key) {
    const int enterKey = 13;
    if(key == enterKey) {
        startGame();
    }
}

void crGame::onMouseDown(ofVec2f pt) {
    //...
}

bool crGame::canStartGame() {
    if(mGameState == GAMESTATE_LOBBY) {
        int numRegistered = 0;
        for(int i=0; i<mPlayers.size(); ++i) {
            if(mPlayers[i].isRegisteredPlayer)
                numRegistered++;
        }
        return numRegistered > 1;
    }
    return false;
}

void crGame::startGame() {
    if(!canStartGame())
        return;

    mGameState = GAMESTATE_INTRO;
    mGameStateParam = 0;

    int numActivePlayers = 0;
    for(int i=0; i<mPlayers.size() && numActivePlayers < 4; ++i) {
        if(mPlayers[i].isRegisteredPlayer && numActivePlayers < 4) {
            mPlayers[i].isActivePlayer = true;
            numActivePlayers++;
        }
        else
            mPlayers[i].isActivePlayer = false;
    }
}

void crGame::startRound() {
    mGameState = GAMESTATE_GATHER_COMMANDS;
    mGameStateParam = 0;
    for(int i=0; i<mPlayers.size(); ++i) {
        if(mPlayers[i].isActivePlayer) {
            mPlayers[i].isDone = false;
            mPlayers[i].command = "";
            mPlayersManager.sendInputCommand(mPlayers[i].playerId);
        }
    }   
}