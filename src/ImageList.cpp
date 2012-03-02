#include "ImageList.h"


void ImageList::create() {
	
	// get image paths => dir.getPath(i)
	dir.listDir("images/random_textures/");
	dir.sort();

	if( dir.size() ){
		images.assign(dir.size(), ofImage());
	}
}

ofImage ImageList::getRandom() {
	
	ofImage img;
	img.loadImage( dir.getPath( (int)ofRandom(dir.size()) ) );
	return img;
}