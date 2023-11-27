/*
 * entertainer.h
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */

#ifndef ENTERTAINER_H_
#define ENTERTAINER_H_

#include "open_interface.h"

/*
 * Set up the sound system for The Entertainer, the song played by the ice cream truck
 */
void entertainer_init();

//play the song
void entertainer_play(oi_t* oi);

//Set volume for the speakers. Have no idea if this is actually something you can do
void entertainer_volume(int volume);

//Pause/stop the song
void entertainer_pause();
#endif /* ENTERTAINER_H_ */
