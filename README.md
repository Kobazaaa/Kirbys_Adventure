<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h1 align="center">KIRBY'S ADVENTURE</h1>

  <p align="center">
    Kirby's Adventure is a side-scrolling platform game developed in 1993 for the NES. The game has a wide variety of enemies with special abilities. When inhaling and swallowing an enemy as Kirby, you can take over their special powers. Kirby on his own also has some special moves. He can walk, sprint, jump, fly, inhale, slide, and more.    <br />
    <strong>Original game : </strong>
    <a href="https://en.wikipedia.org/wiki/Kirby%27s_Adventure"><strong>General info »</strong></a>
    ·
    <a href="https://www.youtube.com/watch?v=_WsaJLcBQCY/$"><strong>Youtube video »<strong></a>
    <br />
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#my-version">My version</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#how-to-play">How To Play</a></li>
    <li><a href="#class-structure">Class structure</a></li>
    <li><a href="#classes-deep-dive">Classes Deep Dive</a></li>
    <li><a href="#checklist">Checklist</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<img src="images/KirbysAdventure.png" alt="kirby and enemies" width=75%>

Here's why:
* "Kirby's Adventure" is an easy to understand and fun to play game, that's why it was one of the first games that I thought of when choosing a game.
* Kirby's cute and friendly looking appearance makes  him one of my favorite Nintendo characters.
* One of my best friends is the biggest Kirby fan I know, so I couldn't not choose a Kirby game.
<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version

This section gives a clear and detailed overview of which parts of the original game I planned to make.

### The minimum I will most certainly develop:
* Kirby's basic mechanics
* The first level of the game
* Different types of enemies, as found in level 1
* Power-Ups and Abilities for Kirby

### What I will probably make as well:
* Particle effects for all the different actions in the game
* Loading screens and menus
* The level-hub as seen in the original game

### What I plan to create if I have enough time left:
* The jump pad mini game, accessed when completing a level, used to gain bonus score or extra lives
* More levels
* More enemies and power-ups for Kirby
* The small 'boss-fight' found in level 2

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started
Detailed instructions on how to run the game project are in this section.

### Prerequisites

<!--This is an example of how to list things you need to use the software and how to install them. -->
* Visual Studio 2022

### How to run the project

* Please use the latest, most up-to-date version of the project for an optimal experience.
* Clone the repository via GitHub.
* Open the Solution file in MCVS and set 'GameProject' as the start-up project.
* Make sure the build dependencies for GameProject are also set to 'Engine'.
* Build.
* Play.
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- HOW TO PLAY -->
## How to play

Please refer to the screenshot included down below to have a better understanding about the controls in "Kirby's Adventure".
### Controls

><img src="images/Kirby's Adventure How2Play.png" alt="how2play" width=500>

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CLASS STRUCTURE -->
## Class structure 

### Overview 
> This is a general overview of the different classes in my game, as well as how they interact with each other.
> Please note that only the Object Composition and Inheritance relationships are including in this image.
>
><img src="images/Kirby's Adventure Class Structure.png" alt="inheritance image" width=750>


### Inheritance
>I created an overarching Entity class which all types of entities inherit from. This class has member functions such as Draw, Update, ApplyGravity and more that apply to all entities. Member variables include m_pTexture, m_Position, m_Velocity and other variables that all entities must have.
>Since the game has different types of enemies, each with different functionality, I made a class for each type. Each type then inherits from the Enemy class (which in turn inherits from the Entity class), which includes functionality shared across all Enemy types.
>
>Example:
>Both Kirby (the player) and the enemies should abide by the laws of gravity, hence why the ApplyGravity function is part of the Entity class. But only Kirby can be controlled by user-input, thus this functionality is part of the Kirby class. Furthermore, each Enemy can interact with Kirby, but only Hot Head can shoot fire. Therefore those functionalities are placed in the Enemy and HotHead class respectively.
>
>
> Apart for the aforementioned classes, my game also has base classes such as Projectile, Ability and BaseState.
> Projectile is a base class which all the different types of projectiles inherit from.
> Ability is a base class from which all the different abilities inherit.
> BaseState is an abstract base class from which all the states the game can be in inherit from. These different states are then used and manipulated by the StateMachine to display the correct things.
>
>
><img src="images/Kirby's Adventure Inheritence Tree.png" alt="inheritence image" width=750>


<!-- ###  -->
## Classes Deep Dive

### Texture- and SoundManager

The Texture- and SoundManager are static classes that manage certain objects. The TextureManager holds all the textures while the SoundManager holds all the sounds.
This is to avoid having duplicate sounds or textures (as these managers also prevent the same texture from being loaded twice).

### LevelManager

A LevelManager instance holds all the levels a LevelHub can have. This means that expanding the game to add more levels is simply adding a Level instance to the correct LevelHub.
The LevelManager can also load levels from a file, making it even easier to expand. Simply adding the correct information to the files and the level will automatically be added to the game.

### EnemyManager

An EnemyManager is specific to each Level and handles all the enemies of said level. It draws, updates, and handles every enemy.
Since the EnemyManagers are specific to each level, creating them is just defining and adding them to the level file, they will automatically be loaded and created.

### ViewFade

ViewFade is a static class that handles certain effects that must be applied to the ViewPort. This class is used for things such as darkening the screen or fading in and out.
This allows the game to have smooth transitions between different states.

### Collision

The Collision class is another static class that allows me to quickly detect and handle collisions between entities, projectiles, powerups and the world.
Calling one of its functions with 2 parameters (the 2 objects you want to check collision with), will return a boolean (wether or not the objects are colliding). In case of collision with the world, this class will also handle that.
Returning a boolean allows each object to still do extra things when a collision happens, that is not handled by the Collision class.

### ParticleSystem

The ParticleSystem class will HANDLE, UPDATE, DRAW and CREATE particles. Each particle is defined to have a position, velocity, lifetime and animation. I opted to make this class static, for the same reasons why I made the Texture- and SoundManager static. I wouldn't need more than one ParticleSystem instance, and the static class makes for ease of use. I can easily add particles from different sources in my game.
Since adding animated particles based on the position in a spritesheet, I premade some functions in this class to quickly add, for example, impact particles. This allows me to easily add particles in game (air bubbles, projectile hits wall, sprint, ... ).

### Animation(manager)

The Animation and AnimationManager class go hand in hand. The Animation class holds a single animation, defined as a vector of frames (which are in turn defined to have a source rectangle and duration). Each animation can draw their own frames if activated.
The AnimationManager is another class. It acts as an encapsulator for a group of Animations. For example, Kirby has his own AnimationManager, which contains a vector of all of Kirby's different animations. In the Kirby class all I have to do is specify to the AnimationManager which animation to play.
The AnimationManager also allows to load a set of animations from a file for ease of use. I specifically implented this because a lot of my animations have different sizes and durations frame per frame. Defining these characteristics in a file is way easier and more managable.

### StateMachine

The StateMachine manages the different states the game can be in (title screen, gameplay, pause screen, or game over). Whichever state is the current active one will be drawn and updated.
The different states are classes on their own, which inherit from an abstract base class, named BaseState. Each state class that inherits from BaseState  class must have an Enter, Exit, Update and Draw function. The Enter and Exit functions are called when the StateMachine enters of leaves said state, allowing from smoother transitions.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CHECKLIST -->
## Checklist

- [x] Accept / set up github project
- [x] week 01 topics applied
    - [x] const keyword applied proactively (variables, functions,..)
    - [x] static keyword applied proactively (class variables, static functions,..)
    - [x] object composition (optional)
- [x] week 02 topics applied
- [x] week 03 topics applied
- [x] week 04 topics applied
- [x] week 05 topics applied
- [x] week 06 topics applied
- [x] week 07 topics applied
- [x] week 08 topics applied
- [x] week 09 topics applied (optional)
- [x] week 10 topics applied (optional)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Kobe Dereyne - kobe.dereyne@student.howest.be

Project Link: [https://github.com/HowestDAE/dae14-KobeDereyne](https://github.com/HowestDAE/dae14-KobeDereyne)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. 

* [Kirby's Advenure Online](https://www.retrogames.cc/nes-games/kirby-s-adventure-usa.html): for allowing me to actually play 'Kirby's Adventure' online.
* [The Spriter Resource](https://www.spriters-resource.com/nes/kirbyadv/): for providing all the necessary spritesheets.
* [The Sounds Resource](https://www.sounds-resource.com/nes/kirbysadventure/sound/19653/): for providing all the necessary sound effects.
* [Kirby's Adventure Music](https://downloads.khinsider.com/game-soundtracks/album/kirby-s-adventure): for providing all the necessary music.
* [cpp reference](https://en.cppreference.com/): for enlightening me on some C++ matters, such as std::map.
* [Lucid](https://lucid.app/documents#/documents?folder_id=recent): for allowing me to make clean overviews of my class structure and inheritance.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

