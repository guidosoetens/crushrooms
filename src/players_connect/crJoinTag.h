#pragma once

#include "ofMain.h"

class crJoinTag {
	public:
        crJoinTag();
        virtual ~crJoinTag();

        void initialize(string ip);
        void draw(float x, float y, float w, float h);

    private:
        ofImage mImage;
};
