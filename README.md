The CMakeLists.txt in this repo is geared for running a GitHub workflow that compiles an SFML project on Mac, Windows, and Linux. I don't use it for my own local builds: you probably want to use your own IDE or CMake setup.

To add new resources (images, sounds...) to a project, copy or place the actual file in the proper subfolder of resources/. Then open resources/resources.txt, and follow the pattern used in the template: the first string of a line is the filename, the second is the key to access that resource. Thus if you have
```
@images
icon.png        icon
background.png  background
...
```
you can access that resource in your code with `gTexture("background")`, and similarly for `gFont(...)` and `gSound(...)`

  This template depends on some headers from my "reusable modules" repo: https://github.com/johnnywz00/SFML-shared-headers
* **jwz:**  C++ utility functions, #defines, shortcuts
* **jwzsfml:**  Like above, but SFML-specific; #includes SFML (sans Network)
* **zsprite:**  Wrapper class for SFML sprite with many extra methods, particularly related to collision checking
* **vsprite:**  Subclass of ZSprite that uses velocity
* **resourcemanager:**  Static class for accessing resource files globally
* **timedeventmanager:**  Manages fuses/daemons, delayed callbacks
* **animations:**  Animation system (works to some extent, but still under construction)
