#include "crJoinTag.h"
#include "QrCode.hpp"


using qrcodegen::QrCode;

crJoinTag::crJoinTag() {

}

crJoinTag::~crJoinTag() {

}

void crJoinTag::initialize(string ip) {

    string text = "www.yonderware.com/crushrooms/index.html?ip=" + ofToString(ip);
	const QrCode qr = QrCode::encodeText(text.c_str(), QrCode::Ecc::LOW);

    int margin = 2;
    int qrSize = qr.getSize();
    int imgSize = 2 * margin + qrSize;

    mImage.allocate(imgSize, imgSize, OF_IMAGE_COLOR);
    mImage.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    ofPixels & pixels = mImage.getPixels();

    cout << pixels.size() / (float)(imgSize * imgSize) << endl;

    for(int i=0; i<imgSize; ++i) {
        for(int j=0; j<imgSize; ++j) {
            int idx = i * imgSize + j;
            bool check = qr.getModule(j - margin, i - margin);
            for(int p=0; p<3; ++p)
                pixels[3 * idx + p] = check ? 0 : 255;
            //pixels[3 * idx + 3] = 255;
        }
    }

    mImage.setFromPixels(pixels);
}

void crJoinTag::draw(float x, float y, float w, float h) {
    ofSetColor(255);
    mImage.draw(x, y, w, h);
}
