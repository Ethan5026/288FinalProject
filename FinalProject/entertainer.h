/*
 * entertainer.h
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */

#ifndef ENTERTAINER_H_
#define ENTERTAINER_H_

//The volume of the speakers, don't know if it can be implemented
int volume;
/*
 * Set up the sound system for The Entertainer, the song played by the ice cream truck
 */
void entertainer_init();

//play the song
void entertainer_play();

//Set volume for the speakers. Have no idea if this is actually something you can do
void entertainer_volume(int volume);

//Pause/stop the song
void entertainer_pause();
#endif /* ENTERTAINER_H_ */
