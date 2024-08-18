///
/// The image label control
///

#pragma once

#include "GUIObject.h"

/// The image label which display a image on the GUI,
/// which support the alpha blending and resizing
class PImageLabel : public PGUIObject {
public:
	/// Create the image label from the image path, the image label
	/// will automatically resize into the size of the image, the
	/// lifetime of the inner image is considered by image label
	/// \param ImagePath The path of the image file
	PImageLabel(const PString &ImagePath);
	/// Create the image with the image pointer
	/// \param Image The image pointer referred to the image object,
	/// the lifetime of the image object is not considered by image label
	PImageLabel(PImage *Image);
	~PImageLabel() override;

public:
	void OnDraw() override;

private:
	PImage	*_image;
	PDevice *_device;
	bool	 _destructImage;
};