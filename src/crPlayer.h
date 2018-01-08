#pragma once
#include "ofMain.h"

typedef struct {
    int playerId;
    bool isDone;
    bool isActivePlayer;
    bool isRegisteredPlayer;
    string name;
    string command;
} crPlayer;