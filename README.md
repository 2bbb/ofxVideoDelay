# ofxVideoDelay

## How to use

```cpp

#include "ofxVideoDelay.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
	ofxVideoDelay videoDelay;
	void setup() {
		videoDelay.setup(2.0f, 1.5f); // buffering texture for 2.0sec, using default texture of 1.5sec delay
		...
	}

	void update() {
		videoDelay.update();
		videoDelay.addTexture(tex/* ofTexture */);
		...
	}

	void draw() {
		const auto &fbo = videoDelay.getFbo();
		fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
		...
	}
}

```
