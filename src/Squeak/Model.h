//
//  Model.h
//  Squeak
//
//  Created by Ethan Geller on 11/25/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef Squeak_Model_h
#define Squeak_Model_h

//#include <stdio>

#include "Room.h"
#include "Thing.h"

//name reserved for designated non-rooms on the map
string nullRoomName = "";
int minimumThingsInRoom = 1;
int maximumThingsInRoom = 4;

//Here are the things you can put in rooms:
//first argument: name, second argument: file path to audio source
vector<Thing> allTheThings = {
    Thing("Rusty Blender", "sound/singles/blender.wav",
          
          "Squeak switches on the large whirring death-bowl and smiles. \'Not today.\'"),
    
    
    Thing("Alchemist's lab equipment", "sound/singles/alchemist_crash.wav",
          
          "A large set of alchemy materials has a short argument with gravity. Gravity wins again. \'Your alchemy days are over, pal.\'"),
    
    
    Thing("Easily Agitated Arrow", "sound/singles/arrow.wav",
          
          "Squeak approches the arrow stuck in the wall. \'Maybe crumbs had a run in with some archers,\' deduces Squeak. Squeak throws the arrow across the room, hitting a bullseye on the opposite wall.  \'Too easy.\'"),
    
    Thing("Proud Bear", "sound/singles/bear.wav",
          
          "A very large mouse says \'hello\' politely. Squeak nods in return. No time for pleasantries today."),
    
    /*Thing("Blackest Sheep", "sound/singles/black_sheep.wav", "Fluffs asks what you\'re up to later. You give a non-commital response. You leave your options open."),*/
    Thing("Bell for Boxing", "sound/singles/boxing_bell.wav",
          
          "Three rings is a signal for the humans to kill each other. Good to remember."),
    
    Thing("Lubricious Bubble Wrap", "sound/singles/bubble_wrap.wav",
          
        "The lubricious bubble wrap gets a bit fresh with Squeak. Squeak makes an example of a few intact sections."),
    
    Thing("Biggest Bubble", "sound/singles/Bubble.wav",
          
          "The problem with bubbles, Squeak muses, is that they always pop before you learn to love them. Float on, bubble, and be at peace."),
    
    Thing("SellFone (tm)", "sound/singles/cell_phone.wav",
          
          "An obnoxious human device fires off unprovoked. However, Squeak finds the phone\'s noises strangely seductive, and begins to dance, all by himself."),
    
    Thing("Lusty Coal Miner", "sound/singles/coal_miner.wav",
          
          "\'Citezen! Did you see a mouse or group of mice moving through this room?\' Squeak says, his voice implying little to no nonsense. The lusty coal miner simply coos at you in return. You decide it is best to leave before things go any further."),
    
    Thing("Drill Man!!", "sound/singles/drill.wav",
          
          "\'Drill Man, did you see Crumbs here a few hours ago?\' Drill Man whirs ambiguously. \'You were never the collaborating type.\'"),
    
    Thing("Serenity Gong", "sound/singles/gong.wav",
          
          "A large human strikes a giant hanging plate. The question is not how, but why?"),
    
    Thing("Shooty Thing", "sound/singles/gunshot.wav",
          
          "The gentlest caress sets off the human shooty thing. A coward\'s weapon. Squeak never got in a jam that his pawfists couldn\'t get him out of."),
    
    /*Thing("Private Horse", "sound/singles/Horse1.wav",
          
          "Squeak asks private horse about Crumbs\'s whereabouts. \'I\'m just an old soldier following orders, Squeak. You know that. My years of heroics are long past me. But if you follow your heart, it will lead you to the people you love. Give love a chance, Squeak.\', he neighs. His kind horse noises affect you more than you thought they could. \'Not this old heart\' Squeak replies inwardly. Squeak steels himself for what is to come."),*/
    
    Thing("Gaggle of Kids", "sound/singles/kids.wav",
          
          "The human children cry out in what you assume is pain. You briefly reflect upon the cruelty of the world."),
    
    Thing("Loudest Keyboard", "sound/singles/keyboard.wav",
          
          "The keyboard does what keyboards do best, and loudly."),
    
    Thing("Rambo Knife", "sound/singles/knife.wav",
          
          "The knife makes a characteristically menacing knife-like sound. \'Hopefully crumbs wasn\'t here. He never had much luck with knives.\'"),
    
    Thing("Fifth Lion", "sound/singles/Lion5.wav",
          
          "Marco, the Fifth Lion of the Jade temple says \'hello\'. Squeak regrets not getting to know Marco better when he had the chance. \'Have you seen Crumbs, Marco?\' \'Hello!\' Marco cries louder. Squeak decides that it is best to leave him alone."),
    
    Thing("Very Fast Oscillator", "sound/singles/microwave.wav",
          
          "Squeak tries in vain to enter the oscillator while it is running. Surely this device has some other purpose. He presses the Popcorn button, hoping that it will lead him to Crumbs."),
    
    Thing("TellerFone (tm)", "sound/singles/modern_phone.wav",
          
          "Squeak almost calls out for your receptionist Terry to answer the phone. Squeak chuckles. What a silly thing to do."),
    
    Thing("Other Mouse In The Room", "sound/singles/mouse_footsteps.wav",
          
          "The other mouse in the room scampers away before Squeak gets a chance to question him."),
    
    Thing("Box, Full of Music", "sound/singles/music_box.wav",
          
          "Squeak is lost in thought. How can this box be real if our ears aren\'t real? Squeak snaps out of it."),
    
    Thing("Ringmaster 5000", "sound/singles/old_phone.wav",
          
          "The Ringmaster 5000 gives a ring with a rich top-end and a moderate . A big step up over the Ringmaster 4450."),
    
    Thing("Hollowball", "sound/singles/pingpong_ball.wav",
          
          "The Hollowball reminds Squeak of his carefree college years. Squeak\'s mind wanders to thoughts of Rick-- but that is a story for another game."),
    
    Thing("Barbershop", "sound/singles/quartet.wav",
          
          "A small team of human barbers performs a brief ritual. Squeak decides it is best not to disturb them."),
    
    Thing("Bell Jr. 2000", "sound/singles/service_bell.wav",
          
          "Finally, a mouse-appropriate instrument. Squeak pushes down with all your might."),
    
    Thing("Standing Wave", "sound/singles/Single_wave.wav",
          
          "This standing wave must be lost. \'You\'re not the only one.\'"),
    
    Thing("Board, Skater of Worlds", "sound/singles/skateboard.wav",
          
          "Squeak gazes in awe upon Board, Skater of Worlds as it glides effortlessly across the floor. It seems to whisper to Squeak as it passes: \'ssskate or dieeeee...\'. A general feeling of unworthiness washes over Squeak."),
    
    Thing("Brassy Hornoclast", "sound/singles/Trumpet.wav",
          
          "Squeak knows better than to tango with the Brassy Hornoclast."),
    
    Thing("Whitest Sheep", "sound/singles/white_sheep.wav",
          
          "Stuffs was a friend, once. He eyes Squeak warily. Don\'t you play coy, Stuffs, you big fat fuck.")
    
};

//Here are the rooms!:
//first argument: name, second argument: file path to audio source
vector<Room> allTheRooms = {
    Room("Porch", "sound/impulses/porch.wav",
         
         "Squeak steps onto a picturesque porch overlooking the ocean. \'This must be the place. Now to find crumbs.\' Squeak looks at his options. All he has to trace Crumbs is the tape from the wire Crumbs was wearing before he was made. PRESS [p] TO PLAY THE TAPE. PRESS [L] TO SHOW CONTROLS",
         
         "sound/ambient/beach.wav"),
    
    Room("Entrance", "sound/impulses/entryway.wav",
         
         "Squeak is at the entryway to the mansion. He takes a deep breath and prepares himself for this hell-hole."),
    
    Room("Hall of a Thousand Mirrors", "sound/impulses/hall_of_athousand_mirrors.wav",
         
         "The second he stumbles into the Hall of a Thousand Mirrors, Squeak is disoriented. What a pointless room. \'I hope you didn\'t die here, Crumbs....\'"),
    
    Room("Study", "sound/impulses/study.wav",
         
         "Squeak quietly and carefully enters the study. Best not to disturb anyone working here."),
    
    Room("All-Too-Bright Basement", "sound/impulses/basement.wav",
         
         "Squeak stumbles down some stairs into a kinda-gross and All-Too-Bright Basement. \'This can\'t be too far from hell itself\' Squeak mutters."),
    
    Room("Park of Cars", "sound/impulses/carpark.wav" ,
         
         "Squeak runs screaming into The Park of Cars, where the human car-masters go to rest. Good for them."),
    
    Room("Hall of the Anchients", "sound/impulses/ancient_lecture_hall.wav",
         
         "Squeak purposefully walk into the Hall of the Anchients. This room smells musty and learned, like a fine beard. This is a hall as old as time itself."),
    
    Room("Aviary", "sound/impulses/aviary.wav",
         
         "Squeak nervously shuffles into an Aviary. This is a very dangerous room for a mouse. Best to stay alert.",
         
         "sound/ambient/aviary.wav"),
    
    Room("Closet of Loathing", "sound/impulses/closet.wav",
         
         "Squeak tiptoes into The Closet of Loathing. Some say this is the place where the famed Admiral Cheesehunter was driven mad by the piles of random objects that tend to be placed here and forgotten."),
    
    Room("Salon of 18th Century Frenchmen", "sound/impulses/French_18th_Century_Salon.wav",
         
         "The moment Squeak walks into this Salon of 18th Century Frenchmen, the interior decorator in him evaluates the decor immediately. He begrudgingly admits that the choices are largely tasteful."),
    
    Room("Half Bathroom", "sound/impulses/half_bath.wav",
         
         "Squeak stumbles into the Half Bathroom. \'Good choice\' Squeak smirks. \'You should never go Full Bathroom.\'"),
    
    Room("Showers", "sound/impulses/showers.wav",
         
         "Squeak enters a room full of showers spraying endlessly. He is reminded of how much he hates being wet, as well as his inability to turn off these showers. He decides it is best not to spend to long here.",
         
         "sound/ambient/showers.wav"),
    
    Room("Sewers", "sound/impulses/sewer.wav",
         
         "Squeak sloshes along into a sewer pipe. He feels right at home.",
         
         "sound/ambient/sewer.wav"),
    
    Room("Old Library", "sound/impulses/library.wav",
         
         "Squeak walks into the library. He can\'t read so this room kind of sucks for him."),
    
    Room("Horrible Chapel", "sound/impulses/chapel.wav",
         
         "After his last marriage, Squeak swore he\'d never set foot in a church again. He sighs and proceed. \'This\'d better be worth it, Crumbs.\'",
         
         "sound/ambient/chapel.wav"),
    
    Room("Heaven", "sound/impulses/heaven.wav",
         
         "Squeak ascendeds into heaven. Checkmate, Atheists."),
    
    Room("Hell", "sound/impulses/hell.wav",
         
         "Squeak descends into the pits of Hell. He always knew he\'d end up here someday.",
         
         "sound/ambient/hell.wav"),
    
    Room("New Concert Hall", "sound/impulses/Musikvereinsaal.wav",
         
         "Squeak power slides onto the stage of the New Concert Hall. Someone must have really forked it over for this bad boy."),
    
    Room("Mysterious Grotto", "sound/impulses/mysterious_grotto.wav",
         
         "Squeak cautiously steps out into the mysterious grotto. \'This place reeks of Foul Play.\'"),
    
    Room("Obnoxious Echo Chamber", "sound/impulses/obnoxious_echo_chamber.wav",
         
         "Squeak begrudgingly strides into the Obnoxious Echo Chamber. What kind of idiot would build such a room? He decides to spend as little time here as possible."),
    
    Room("Opera Hall", "sound/impulses/Opera_hall.wav",
         
         "Squeak make a grand entrance onto the stage of the Opera Hall. Is this where Crumbs sang his last aria?"),
    
    Room("Grand Carpark", "sound/impulses/entryway.wav",
         
         "Squeak enters the Grand Carpark. This is a much larger Carhouse than Squeak could have ever imagined."),
    
    Room("Saloon", "sound/impulses/saloon.wav",
         
         "Squeak confidently strides up to the bar of an old Saloon. He orders a whiskey straight to prove himself to the bartender. It seems like the bartender doesn\'t really care. \'Did you see an average height mouse wearing a wire come through here?\' Squeak asks. \'Not my business,\' says the bartender",
         
         "sound/ambient/saloon.wav"),
    
    Room("Padded Cell", "sound/impulses/solitary_confinement.wav",
         
         "Squeak enters the Padded Cell. \'I could get used to this.\'"),
    
    Room("Stormy Tower", "sound/impulses/stormy_tower.wav",
         
         "Squeak struggles up the stairs to the stormy tower.",
         
         "sound/ambient/stormy_tower.wav"),
    
    Room("Lodge of Sweat", "sound/impulses/sweat_lodge.wav",
         
         "Squeak enters a very sweaty Lodge of Sweat. \'I wish I could open the pores of the Truth.\' Squeak sighs."),
    
    Room("Megalomaniac\'s Bunker", "sound/impulses/underground_bunker.wav",
         
         "Squeak waits patiently as an elevator carries him 20 stories down into the Megalomaniac\'s Bunker. Whoever built this place had the right idea."),
    
    Room("Warehouse of Cat Mannequins",  "sound/impulses/entryway.wav",
         
         "Squeak enters a Warehouse filled with mannequins of felines. \'Stay calm, Squeak...Stay calm\' Squeak repeats to himself in urgent whispers."),
    
    Room("Existential Horror", "sound/impulses/science_place_1.wav",
         
         "Squeak enters a void, filled with the worst things imaginable. \'Where the fuck am I?\' Squeak yells.")
};



#endif
