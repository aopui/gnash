#ifndef GNASH_ASOBJ_FLASH_MEDIA_H
#define GNASH_ASOBJ_FLASH_MEDIA_H 1
#include "sharedlib.h"
static gnash::SharedLib::initentry *as3mediaclasses[] = {
	gnash::id3info_class_init,
	gnash::microphone_class_init,
	gnash::camera_class_init,
	gnash::soundchannel_class_init,
	gnash::soundloadercontext_class_init,
	gnash::soundmixer_class_init,
	gnash::soundtransform_class_init,
	0
};
#endif // end of GNASH_ASOBJ_FLASH_MEDIA_H
