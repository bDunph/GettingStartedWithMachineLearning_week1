#pragma once

#include <vector>

#include "ofMain.h"
#include "ofxRapidLib.h"
#include "ofxMaxim.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void exit() override;
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	/* RapidLib */
	std::vector<rapidlib::trainingExample> data;
	rapidlib::classification knn;
	bool isRecording;
	bool isTrained;
	bool isRunning;
	bool isEmpty;
	double label;
	int estimatedLabel;

	/* Video as input data */
	ofVideoGrabber vidGrabber;
	ofPixels frameData;
	ofTexture data1, data2, data3;
	int vidWidth;
	int vidHeight;

	/* Audio as output */
	void audioOut(ofSoundBuffer& output) override;
	std::size_t bufferSize;
	std::size_t sampleRate;
	ofSoundStream soundStream;
	double outputs[2];
	maxiMix mymix;
	maxiOsc timer;
	int currentCount;
	int lastCount;
	int playHead;
	maxiSample bigRackTom, smallRackTom, floorTom, kick, rackTom, rim, rimClick;

};
